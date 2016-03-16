/*----------------------------------------------------------------------------
 *      RL-ARM - USB
 *----------------------------------------------------------------------------
 *      Name:    usbh_ohci_lpc17xx.c
 *      Purpose: OHCI Hardware Specific Layer Driver for the 
 *               NXP LPC17xx Device Series
 *      Rev.:    V4.20
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include <rl_usb.h>
#include <LPC17xx.H>

#define  oHcRhStatus       0x50         /* HcRhStatus register offset         */
#define  HcRhStatus_LPSC  (1 << 16)     /* HcRhStatus LPSC bit                */
#define  HcRhStatus_LPS   (1 <<  0)     /* HcRhStatus LPS  bit                */

#define			USB_OTC_SELECT	{ LPC_GPIO2->FIODIR |= (1UL<<5);LPC_GPIO2->FIOCLR |= (1UL<<5); }
#define		   USB_HOST_SELECT	USB_OTC_SELECT	   { LPC_GPIO2->FIOSET |= (1UL<<5); } 

extern   void USBH_OHCI_IRQHandler (void);

/************************* OHCI Hardware Configuration ************************/

// *** <<< Use Configuration Wizard in Context Menu >>> ***

// <o> USB Host OHCI Controller Address
#define USBH_OHCI_ADDR   0x5000C000

// *** <<< End of Configuration section             >>> ***


/************************** Module Functions **********************************/

/*------------------------- usbh_ohci_hw_delay --------------------------------
 *
 *  Delay of about 100 us at 100 MHz CPU clock.
 *
 *  Parameter:  delay:      Delay
 *  Return:
 *----------------------------------------------------------------------------*/

void usbh_ohci_hw_delay (U32 delay) {
  delay <<= 10;
  while (delay--) {
    __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop();
  }
}


/*------------------------- usbh_ohci_hw_reg_wr -------------------------------
 *
 *  Write hardware register.
 *
 *  Parameter:  reg:        Register offset
 *              val:        Register value 
 *  Return:
 *----------------------------------------------------------------------------*/

void usbh_ohci_hw_reg_wr (U32 reg_ofs, U32 val) {
  *((U32 *)(USBH_OHCI_ADDR + reg_ofs)) = val;
}


/*------------------------- usbh_ohci_hw_reg_rd -------------------------------
 *
 *  Read hardware register.
 *
 *  Parameter:  reg:        Register offset
 *  Return:                 Register value 
 *----------------------------------------------------------------------------*/

U32 usbh_ohci_hw_reg_rd (U32 reg_ofs) {
  return (*((U32 *)(USBH_OHCI_ADDR + reg_ofs)));
}


/*------------------------- usbh_ohci_hw_power --------------------------------
 *
 *  Turn port power on or off.
 *
 *  Parameter:  on:         Requested power state
 *  Return:                 Error code
 *----------------------------------------------------------------------------*/

BOOL usbh_ohci_hw_power (U32 on) {
  if (on) {
    usbh_ohci_hw_reg_wr(oHcRhStatus, usbh_ohci_hw_reg_rd(oHcRhStatus) | HcRhStatus_LPSC);
  } else {
    usbh_ohci_hw_reg_wr(oHcRhStatus, usbh_ohci_hw_reg_rd(oHcRhStatus) | HcRhStatus_LPS );
  }

  return (__TRUE);
}


/*------------------------- usbh_ohci_hw_init ---------------------------------
 *
 *  Initialize USB Host OHCI Controller hardware.
 *
 *  Parameter:
 *  Return:                 Error code
 *----------------------------------------------------------------------------*/

BOOL usbh_ohci_hw_init (void) {
  U32 tout;

  //USB_OTC_SELECT;
  USB_HOST_SELECT;
  LPC_SC->PCONP       |=  (1UL << 31);  /* Enable USB interface power/clock   */
  LPC_USB->OTGClkCtrl |=  0x19;         /* Enable Host, OTG and AHB master clk*/

  for (tout = 100; ; tout--) {
    if ((LPC_USB->OTGClkSt & 0x19) == 0x19)   /* Wait for clocks enabled      */
      break;
    if (!tout) 
      return (__FALSE);
  }

  LPC_USB->OTGStCtrl  |=  0x03;         /* Select port function               */

  /* Set pin functions                                                        */
  LPC_PINCON->PINSEL1 &= ~((3 << 26) | (3 << 28));
  LPC_PINCON->PINSEL1 |=  ((1 << 26) |  /* P0.29 -  USB_D+     (b01)          */
                           (1 << 28));  /* P0.30 -  USB_D-     (b01)          */

//  LPC_PINCON->PINSEL3 &= ~((3 <<  4) | (3 <<  6) | (3 << 12) | (3 << 22));
//  LPC_PINCON->PINSEL3 |=  ((1 <<  4) |  /* P1.18 -  USB_UP_LED (b01)          */
//                           (2 <<  6) |  /* P1.19 - !USB_PPWR   (b10)          */
//                           (2 << 12) |  /* P1.22 -  USB_PWRD   (b10)          */
//                           (2 << 22));  /* P1.27 - !USB_OVRCR  (b10)          */

  NVIC_SetPriority (USB_IRQn, 0);       /* Set USB interrupt highest priority */

  return (__TRUE);
}


/*------------------------- usbh_ohci_hw_uninit -------------------------------
 *
 *  Uninitialize USB Host OHCI Controller hardware.
 *
 *  Parameter:
 *  Return:                 Error code
 *----------------------------------------------------------------------------*/

BOOL usbh_ohci_hw_uninit (void) {
  U32 tout;

  LPC_USB->OTGStCtrl  &= ~0x03;         /* Deselect port function             */

  /* Reset pin functions                                                      */
  //LPC_PINCON->PINSEL3 &= ~((3 <<  4) | (3 <<  6) | (3 << 12) | (3 << 22));
  LPC_PINCON->PINSEL1 &= ~((3 << 26) | (3 << 28));

  LPC_USB->OTGClkCtrl &= ~0x19;         /* Disable Host,OTG and AHB master clk*/

  for (tout = 100; ; tout--) {
    if ((LPC_USB->OTGClkSt & 0x19) == 0)  /* Wait for clocks disabled         */
      break;
    if (!tout) 
      return (__FALSE);
  }

  LPC_SC->PCONP       &= ~(1UL << 31);  /* Disable USB interface power/clock  */

  return (__TRUE);
}


/*------------------------- usbh_ohci_hw_irq_en -------------------------------
 *
 *  USB Host OHCI Controller interrupt enable.
 *
 *  Parameter:
 *  Return:                 Error code
 *----------------------------------------------------------------------------*/

BOOL usbh_ohci_hw_irq_en (void) {
  NVIC_EnableIRQ  (USB_IRQn);           /* Enable USB interrupt               */

  return (__TRUE);
}


/*------------------------- usbh_ohci_hw_irq_dis ------------------------------
 *
 *  USB Host OHCI Controller interrupt disable.
 *
 *  Parameter:
 *  Return:                 Error code
 *----------------------------------------------------------------------------*/

BOOL usbh_ohci_hw_irq_dis (void) {
  NVIC_DisableIRQ (USB_IRQn);           /* Disable USB interrupt              */

  return (__TRUE);
}


/*------------------------- USB_IRQHandler ------------------------------------
 *
 *  Hardware USB Interrupt Handler Routine.
 *
 *  Parameter:
 *  Return:
 *----------------------------------------------------------------------------*/

void USB_IRQHandler (void) {
  USBH_OHCI_IRQHandler();
}
