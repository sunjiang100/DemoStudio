/*----------------------------------------------------------------------------
 *      RL-ARM - USB
 *----------------------------------------------------------------------------
 *      Name:    usb_config.c
 *      Purpose: System Configuration
 *      Rev.:    V4.20
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------

/*----------------------------------------------------------------------------
 *      USB Host Configuration
 *---------------------------------------------------------------------------*/

// <e> USB Host
//   <i> Enable the USB Host functionality
#define USBH_ENABLE                 1

//   <e> Open Host Controller Interface (OHCI)
//   <i> Enable or disable Open Host Controller Interface
//
//     <h> Root Hub ports used by OHCI
//     <i> These are the ports that OHCI will use.
//     <o1.0>  Port 1
//     <o1.1>  Port 2
//     <o1.2>  Port 3
//     <o1.3>  Port 4
//     <o1.4>  Port 5
//     <o1.5>  Port 6
//     <o1.6>  Port 7
//     <o1.7>  Port 8
//     <o1.8>  Port 9
//     <o1.9>  Port 10
//     <o1.10> Port 11
//     <o1.11> Port 12
//     <o1.12> Port 13
//     <o1.13> Port 14
//     <o1.14> Port 15
//     </h>
//
//     <o2> Start address of memory used by OHCI
//     <i> This is a start address of memory that OHCI will use for descriptors 
//     <i> and USB communication data.
//
//     <o3> Size of memory used by OHCI <1-1048576>
//     <i> This is a size of memory (in bytes) that OHCI will use for descriptors 
//     <i> and USB communication data.
//
//     <o4> Maximum number of Endpoint Descriptors used by OHCI <1-64>
//     <i> This is a maximum number of Endpoints that OHCI will use.
//
//     <o5> Maximum number of Transfer Descriptors used by OHCI <1-64>
//     <i> This is a maximum number of Transfers that OHCI will use.
//
//     <o6> Maximum number of Isochronous Transfer Descriptors used by OHCI <0-64>
//     <i> This is a maximum number of Isochronous Transfers that OHCI will use.
//   </e>
#define USBH_OHCI_ENABLE            1
#define USBH_OHCI_PORTS             0x00000001
#define USBH_OHCI_MEM_ADDR          0x20080000
#define USBH_OHCI_MEM_SIZE          2048
#define USBH_OHCI_ED_MAX_NUM        3
#define USBH_OHCI_TD_MAX_NUM        10
#define USBH_OHCI_ITD_MAX_NUM       0

//   <e> STM32F105/107 USB Host Controller
//   <i> Enable or disable USB Host Controller on ST STM32F105/107 Devices
//
//     <o1> Start address of memory used by STM32F105/107 USB Host Controller
//     <i> This is a start address of memory that STM32F105/107 USB Host Controller 
//     <i> will use for USB communication data.
//
//     <o2> Size of memory used by STM32F105/107 USB Host Controller <1-1048576>
//     <i> This is a size of memory (in bytes) that STM32F105/107 USB Host Controller 
//     <i> will use for USB communication data.
//   </e>
#define USBH_STM32_ENABLE           0
#define USBH_STM32_MPOOL_MEM_POS    0x2000E800
#define USBH_STM32_MPOOL_MEM_SZ     0x00000800

//   <h> Class Configuration

//     <e> Mass Storage Device Class (MSC)
//     <i> Enable or disable Mass Storage Device Class handling
//
//       <o1> Timeout for transaction requests <1-2000000000>
//       <i> This value represents timeout in approximately 5 to 8 CPU clock cycles
//       <i> Default:   1000000 (approximately 80 milliseconds at CPU clock 100 MHz)
//
//       <o2> Timeout for block read <1-2000000000>
//       <i> This value represents timeout in approximately 5 to 8 CPU clock cycles
//       <i> Default:   10000000 (approximately 0.8 seconds at CPU clock 100 MHz)
//
//       <o3> Timeout for block write <1-2000000000>
//       <i> This value represents timeout in approximately 5 to 8 CPU clock cycles
//       <i> Default:  100000000 (approximately 8 seconds at CPU clock 100 MHz)
//
//       <o4> Number of block read retries <1-100>
//
//       <o5> Number of block write retries <1-100>
//     </e>
#define USBH_MSC_ENABLE             1
#define USBH_MSC_TR_TOUT            1000000
#define USBH_MSC_RD_BLK_TOUT        10000000
#define USBH_MSC_WR_BLK_TOUT        100000000
#define USBH_MSC_RD_RETRIES         3
#define USBH_MSC_WR_RETRIES         3

//     <e> Human Interface Device Class (HID)
//     <i> Enable or disable Human Interface Device Class handling
//
//       <o1> Timeout for transaction requests <1-2000000000>
//       <i> This value represents timeout in approximately 5 to 8 CPU clock cycles
//       <i> Default:   100000 (approximately 8 milliseconds at CPU clock 100 MHz)
//     </e>
//   </h>
#define USBH_HID_ENABLE             0
#define USBH_HID_TR_TOUT            100000

// </e>


/*----------------------------------------------------------------------------
 *      USB Device Configuration
 *---------------------------------------------------------------------------*/

// <e> USB Device
//   <i> Enable the USB Device functionality
#define USBD_ENABLE                 0

//   <o0.0> High-speed
//     <i> Enable high-speed functionality (if device supports it)
#define USBD_HS_ENABLE              0

//   <h> Device Settings
//     <i> These settings affect Device Descriptor
//     <o0> Power
//       <i> Default Power Setting
//       <0=> Bus-powered
//       <1=> Self-powered
//     <o1> Max Endpoint 0 Packet Size
//       <i> Maximum packet size for endpoint zero (bMaxPacketSize0)
//       <8=> 8 Bytes <16=> 16 Bytes <32=> 32 Bytes <64=> 64 Bytes
//     <o2.0..15> Vendor ID <0x0000-0xFFFF>
//       <i> Vendor ID assigned by the USB-IF (idVendor)
//     <o3.0..15> Product ID <0x0000-0xFFFF>
//       <i> Product ID assigned by the manufacturer (idProduct)
//     <o4.0..15> Device Release Number <0x0000-0xFFFF>
//       <i> Device release number in binary-coded decimal (bcdDevice)
//   </h>
#define USBD_POWER                  0
#define USBD_MAX_PACKET0            8
#define USBD_DEVDESC_IDVENDOR       0xC251
#define USBD_DEVDESC_IDPRODUCT      0x2203
#define USBD_DEVDESC_BCDDEVICE      0x0100

//   <h> Configuration Settings
//     <i> These settings affect Configuration Descriptor
//     <o0.5> Remote Wakeup
//       <i> Configuration support for remote wakeup (D5: of bmAttributes)
//     <o1.0..7> Maximum Power Consumption (in mA) <0-510><#/2>
//       <i> Maximum power consumption of the USB device 
//       <i> from the bus in this specific configuration 
//       <i> when the device is fully operational (bMaxPower)
//   </h>
#define USBD_CFGDESC_BMATTRIBUTES   0x80
#define USBD_CFGDESC_BMAXPOWER      0x32

//   <h> String Settings
//     <i> These settings affect String Descriptor
//     <o0.0..15> Language ID <0x0000-0xFCFF>
//       <i> English (United States) = 0x0409
//     <s0.126> Manufacturer String
//       <i> String descriptor describing manufacturer
//     <s1.126> Product String
//       <i> String descriptor describing product
//     <e1.0> Serial Number
//       <i> Enable serial number string
//       <i> If disabled serial number string will not be assigned to the USB Device
//       <s2.126> Serial Number String
//         <i> String descriptor describing device's serial number
//     </e>
//   </h>
#define USBD_STRDESC_LANGID         0x0409
#define USBD_STRDESC_MAN            L"Keil Software"
#define USBD_STRDESC_PROD           L"Keil USB Device"
#define USBD_STRDESC_SER_ENABLE     1
#define USBD_STRDESC_SER            L"0001A0000000"

//   <e0> Class Support
//     <i> Enables USB Device Class specific Requests
#define USBD_CLASS_ENABLE           1

//     <e0.0> Human Interface Device (HID)
//       <i> Enable class support for Human Interface Device (HID)
//       <h> Interrupt Endpoint Settings
//         <o1.0..4> Interrupt In Endpoint Number                  <1=>   1 <2=>   2 <3=>   3
//                                                 <4=>   4        <5=>   5 <6=>   6 <7=>   7
//                                                 <8=>   8        <9=>   9 <10=> 10 <11=> 11
//                                                 <12=>  12       <13=> 13 <14=> 14 <15=> 15
//         <o2.0..4> Interrupt Out Endpoint Number <0=>   Not used <1=>   1 <2=>   2 <3=>   3
//                                                 <4=>   4        <5=>   5 <6=>   6 <7=>   7
//                                                 <8=>   8        <9=>   9 <10=> 10 <11=> 11
//                                                 <12=>  12       <13=> 13 <14=> 14 <15=> 15
//           <i> If interrupt out endpoint is not used select "Not used"
//         <h> Endpoint Settings
//           <o3.0..10> Maximum Endpoint Packet Size (in bytes) <0-64>
//           <o4.0..10> Endpoint polling Interval (in ms) <0-255>
//           <e5> High-speed
//             <i> If high-speed is enabled set endpoint settings for it
//             <o6.0..10> Maximum Endpoint Packet Size (in bytes) <0-1024>
//             <o6.11..12> Additional transactions per microframe <0=> None <1=> 1 additional <2=> 2 additional
//             <o7.0..10> Endpoint polling Interval (in ms) <1=>      1 <2=>      2 <3=>      4 <4=>      8
//                                                          <5=>     16 <6=>     32 <7=>     64 <8=>    128
//                                                          <9=>    256 <10=>   512 <11=>  1024 <12=>  2048
//                                                          <13=>  4096 <14=>  8192 <15=> 16384 <16=> 32768
//           </e>
//         </h>
//       </h>
//       <h> Human Interface Device Settings
//         <i> Device specific settings
//         <s0.126> HID Interface String
//         <o8.0..7> Number of Reports <1-255>
//         <o9.0..6> Input Report Size (in bytes) <1-64>
//         <o10.0..6> Output Report Size (in bytes) <1-64>
//         <o11.0..6> Feature Report Size (in bytes) <1-64>
//       </h>
//     </e>
#define USBD_HID_ENABLE             0
#define USBD_HID_EP_INTIN           1
#define USBD_HID_EP_INTOUT          1
#define USBD_HID_WMAXPACKETSIZE     4
#define USBD_HID_BINTERVAL          32
#define USBD_HID_HS_ENABLE          0
#define USBD_HID_HS_WMAXPACKETSIZE  4
#define USBD_HID_HS_BINTERVAL       6
#define USBD_HID_STRDESC            L"USB_HID"
#define USBD_HID_CFG_REPORTNUM      1
#define USBD_HID_INREPORT_BYTES     1
#define USBD_HID_OUTREPORT_BYTES    1
#define USBD_HID_FEATREPORT_BYTES   1

//     <e0.0> Mass Storage Device (MSC)
//       <i> Enable class support for Mass Storage Device (MSC)
//       <h> Bulk Endpoint Settings
//         <o1.0..4> Bulk In Endpoint Number                  <1=>   1 <2=>   2 <3=>   3
//                                            <4=>   4        <5=>   5 <6=>   6 <7=>   7
//                                            <8=>   8        <9=>   9 <10=> 10 <11=> 11
//                                            <12=>  12       <13=> 13 <14=> 14 <15=> 15
//         <o2.0..4> Bulk Out Endpoint Number                 <1=>   1 <2=>   2 <3=>   3
//                                            <4=>   4        <5=>   5 <6=>   6 <7=>   7
//                                            <8=>   8        <9=>   9 <10=> 10 <11=> 11
//                                            <12=>  12       <13=> 13 <14=> 14 <15=> 15
//         <h> Endpoint Settings
//           <o3> Maximum Packet Size <1-1024>
//           <e4> High-speed
//             <i> If high-speed is enabled set endpoint settings for it
//             <o5> Maximum Packet Size <1-1024>
//             <o6> Maximum NAK Rate <0-255>
//           </e>
//         </h>
//       </h>
//       <h> Mass Storage Device Settings
//         <i> Device specific settings
//         <s0.126> MSC Interface String
//         <h> Inquiry Data
//           <s1.8>  Vendor Identification
//           <s2.16> Product Identification
//           <s3.4>  Product Revision Level
//         </h>
//       </h>
//     </e>
#define USBD_MSC_ENABLE             1
#define USBD_MSC_EP_BULKIN          2
#define USBD_MSC_EP_BULKOUT         2
#define USBD_MSC_WMAXPACKETSIZE     64
#define USBD_MSC_HS_ENABLE          0
#define USBD_MSC_HS_WMAXPACKETSIZE  512
#define USBD_MSC_HS_BINTERVAL       0
#define USBD_MSC_STRDESC            L"USB_MSC"
#define USBD_MSC_INQUIRY_DATA       "Keil    "         \
                                    "LPC17xx Disk    " \
                                    "1.0 "

//     <e0.0> Audio Device (ADC)
//       <i> Enable class support for Audio Device (ADC)
//       <h> Isochronous Endpoint Settings
//         <o1.0..4> Isochronous Out Endpoint Number                 <1=>   1 <2=>   2 <3=>   3
//                                                   <4=>   4        <5=>   5 <6=>   6 <7=>   7
//                                                   <8=>   8        <9=>   9 <10=> 10 <11=> 11
//                                                   <12=>  12       <13=> 13 <14=> 14 <15=> 15
//         <h> Endpoint Settings
//           <o2.0..10> Maximum Endpoint Packet Size (in bytes) <0-1024>
//           <o3.0..10> Endpoint polling Interval (in ms) <1=>      1 <2=>      2 <3=>      4 <4=>      8
//                                                        <5=>     16 <6=>     32 <7=>     64 <8=>    128
//                                                        <9=>    256 <10=>   512 <11=>  1024 <12=>  2048
//                                                        <13=>  4096 <14=>  8192 <15=> 16384 <16=> 32768
//           <e4> High-speed
//             <i> If high-speed is enabled set endpoint settings for it
//             <o5.0..10> Maximum Endpoint Packet Size (in bytes) <0-1024>
//             <o5.11..12> Additional transactions per microframe <0=> None <1=> 1 additional <2=> 2 additional
//           </e>
//         </h>
//       </h>
//       <h> Audio Device Settings
//         <i> Device specific settings
//         <s0.126> Audio Control Interface String
//         <s1.126> Audio Streaming (Zero Bandwidth) Interface String
//         <s2.126> Audio Streaming (Operational) Interface String
//         <o6.0..7> Audio Subframe Size (in bytes) <0-255>
//         <o7.0..7> Sample Resolution (in bits) <0-255>
//         <o8.0..23> Sample Frequency (in Hz) <0-16777215>
//         <o9> Packet Size (in bytes) <1-256>
//         <o10> Packet Count <1-16>
//       </h>
//     </e>
#define USBD_ADC_ENABLE             0
#define USBD_ADC_EP_ISOOUT          3
#define USBD_ADC_WMAXPACKETSIZE     64
#define USBD_ADC_BINTERVAL          1
#define USBD_ADC_HS_ENABLE          0
#define USBD_ADC_HS_WMAXPACKETSIZE  64
#define USBD_ADC_CIF_STRDESC        L"USB_ADC"
#define USBD_ADC_SIF1_STRDESC       L"USB_ADC1"
#define USBD_ADC_SIF2_STRDESC       L"USB_ADC2"
#define USBD_ADC_BSUBFRAMESIZE      2
#define USBD_ADC_BBITRESOLUTION     16
#define USBD_ADC_TSAMFREQ           32000
#define USBD_ADC_CFG_P_S            32
#define USBD_ADC_CFG_P_C            1

//     <e0> Communication Device (CDC)
//       <i> Enable class support for Communication Device (CDC)
//       <h> Interrupt Endpoint Settings
//         <o1.0..4> Interrupt In Endpoint Number                 <1=>   1 <2=>   2 <3=>   3
//                                                <4=>   4        <5=>   5 <6=>   6 <7=>   7
//                                                <8=>   8        <9=>   9 <10=> 10 <11=> 11
//                                                <12=>  12       <13=> 13 <14=> 14 <15=> 15
//         <h> Endpoint Settings
//           <o2.0..10> Maximum Endpoint Packet Size (in bytes) <0-1024>
//           <o3.0..10> Endpoint polling Interval (in ms) <0-255>
//           <e4> High-speed
//             <i> If high-speed is enabled set endpoint settings for it
//             <o5.0..10> Maximum Endpoint Packet Size (in bytes) <0-1024>
//             <o5.11..12> Additional transactions per microframe <0=> None <1=> 1 additional <2=> 2 additional
//             <o6.0..10> Endpoint polling Interval (in ms) <1=>      1 <2=>      2 <3=>      4 <4=>      8
//                                                          <5=>     16 <6=>     32 <7=>     64 <8=>    128
//                                                          <9=>    256 <10=>   512 <11=>  1024 <12=>  2048
//                                                          <13=>  4096 <14=>  8192 <15=> 16384 <16=> 32768
//           </e4>
//         </h>
//       </h>
//       <h> Bulk Endpoint Settings
//         <o7.0..4> Bulk In Endpoint Number                  <1=>   1 <2=>   2 <3=>   3
//                                            <4=>   4        <5=>   5 <6=>   6 <7=>   7
//                                            <8=>   8        <9=>   9 <10=> 10 <11=> 11
//                                            <12=>  12       <13=> 13 <14=> 14 <15=> 15
//         <o8.0..4> Bulk Out Endpoint Number                 <1=>   1 <2=>   2 <3=>   3
//                                            <4=>   4        <5=>   5 <6=>   6 <7=>   7
//                                            <8=>   8        <9=>   9 <10=> 10 <11=> 11
//                                            <12=>  12       <13=> 13 <14=> 14 <15=> 15
//         <h> Endpoint Settings
//           <o9> Maximum Packet Size <1-1024>
//           <e10> High-speed
//             <i> If high-speed is enabled set endpoint settings for it
//             <o11> Maximum Packet Size <1-1024>
//             <o12> Maximum NAK Rate <0-255>
//           </e10>
//         </h>
//       </h>
//       <h> Communication Device Settings
//         <i> Device specific settings
//         <s0.126> Communication Class Interface String
//         <s1.126> Data Class Interface String
//         <o13> Maximum Communication Device Buffer Size
//            <8=> 8 Bytes <16=> 16 Bytes <32=> 32 Bytes <64=> 64 Bytes
//         <o14> Maximum Communication Device Output Buffer Size
//            <8=> 8 Bytes <16=> 16 Bytes <32=> 32 Bytes <64=> 64 Bytes <128=> 128 Bytes 
//       </h>
//     </e>
#define USBD_CDC_ENABLE             0
#define USBD_CDC_EP_INTIN           1
#define USBD_CDC_WMAXPACKETSIZE     16
#define USBD_CDC_BINTERVAL          2
#define USBD_CDC_HS_ENABLE          0
#define USBD_CDC_HS_WMAXPACKETSIZE  16
#define USBD_CDC_HS_BINTERVAL       2
#define USBD_CDC_EP_BULKIN          2
#define USBD_CDC_EP_BULKOUT         2
#define USBD_CDC_WMAXPACKETSIZE1    64
#define USBD_CDC_HS_ENABLE1         0
#define USBD_CDC_HS_WMAXPACKETSIZE1 64
#define USBD_CDC_HS_BINTERVAL1      0
#define USBD_CDC_CIF_STRDESC        L"USB_CDC"
#define USBD_CDC_DIF_STRDESC        L"USB_CDC1"
#define USBD_CDC_BUFSIZE            64
#define USBD_CDC_OUTBUFSIZE         128

//     <e0> Vendor Specific Device
//       <i> Enables USB Vendor specific Requests
//     </e>
#define USBD_VENDOR_ENABLE          0

//   </e>
// </e>


/* USB Device Calculations --------------------------------------------------*/

#define USBD_IF_NUM                (USBD_HID_ENABLE+USBD_MSC_ENABLE+(USBD_ADC_ENABLE*2)+(USBD_CDC_ENABLE*2))
#define USBD_MULTI_IF              (USBD_CDC_ENABLE*(USBD_HID_ENABLE|USBD_MSC_ENABLE|USBD_ADC_ENABLE))
#define MAX(x, y)                (((x) < (y)) ? (y) : (x))
#define USBD_EP_NUM_CALC0           MAX((USBD_HID_ENABLE*(USBD_HID_EP_INTIN)),  (USBD_HID_ENABLE*(USBD_HID_EP_INTOUT!=0)*(USBD_HID_EP_INTOUT)))
#define USBD_EP_NUM_CALC1           MAX((USBD_MSC_ENABLE*(USBD_MSC_EP_BULKIN)), (USBD_MSC_ENABLE*(USBD_MSC_EP_BULKOUT)))
#define USBD_EP_NUM_CALC2           MAX((USBD_ADC_ENABLE*(USBD_ADC_EP_ISOOUT)), (USBD_CDC_ENABLE*(USBD_CDC_EP_INTIN)))
#define USBD_EP_NUM_CALC3           MAX((USBD_CDC_ENABLE*(USBD_CDC_EP_BULKIN)), (USBD_CDC_ENABLE*(USBD_CDC_EP_BULKOUT)))
#define USBD_EP_NUM_CALC4           MAX(USBD_EP_NUM_CALC0, USBD_EP_NUM_CALC1)
#define USBD_EP_NUM_CALC5           MAX(USBD_EP_NUM_CALC2, USBD_EP_NUM_CALC3)
#define USBD_EP_NUM_CALC6           MAX(USBD_EP_NUM_CALC4, USBD_EP_NUM_CALC5)
#define USBD_EP_NUM                (USBD_EP_NUM_CALC6)

#if    (USBD_HID_ENABLE)
#if    (USBD_MSC_ENABLE)
#if ((((USBD_HID_EP_INTIN   == USBD_MSC_EP_BULKIN)  || \
       (USBD_HID_EP_INTIN   == USBD_MSC_EP_BULKIN)))|| \
      ((USBD_HID_EP_INTOUT  != 0)                   && \
       (USBD_HID_EP_INTOUT  == USBD_MSC_EP_BULKIN)  || \
       (USBD_HID_EP_INTOUT  == USBD_MSC_EP_BULKOUT)))
#error "HID and Mass Storage Device Interface can not use same Endpoints!"
#endif
#endif
#if    (USBD_ADC_ENABLE)
#if   ((USBD_HID_EP_INTIN   == USBD_ADC_EP_ISOOUT)  || \
      ((USBD_HID_EP_INTOUT  != 0)                   && \
       (USBD_HID_EP_INTOUT  == USBD_ADC_EP_ISOOUT)))
#error "HID and Audio Device Interface can not use same Endpoints!"
#endif
#endif
#if    (USBD_CDC_ENABLE)
#if  (((USBD_HID_EP_INTIN   == USBD_CDC_EP_INTIN)   || \
       (USBD_HID_EP_INTIN   == USBD_CDC_EP_BULKIN)  || \
       (USBD_HID_EP_INTIN   == USBD_CDC_EP_BULKOUT))|| \
      ((USBD_HID_EP_INTOUT  != 0)                   && \
      ((USBD_HID_EP_INTOUT  == USBD_CDC_EP_INTIN)   || \
       (USBD_HID_EP_INTOUT  == USBD_CDC_EP_BULKIN)  || \
       (USBD_HID_EP_INTOUT  == USBD_CDC_EP_BULKOUT))))
#error "HID and Communication Device Interface can not use same Endpoints!"
#endif
#endif
#endif

#if    (USBD_MSC_ENABLE)
#if    (USBD_ADC_ENABLE)
#if   ((USBD_MSC_EP_BULKIN  == USBD_ADC_EP_ISOOUT)  || \
       (USBD_MSC_EP_BULKOUT == USBD_ADC_EP_ISOOUT))
#error "Mass Storage Device and Audio Device Interface can not use same Endpoints!"
#endif
#endif
#if    (USBD_CDC_ENABLE)
#if   ((USBD_MSC_EP_BULKIN  == USBD_CDC_EP_INTIN)   || \
       (USBD_MSC_EP_BULKIN  == USBD_CDC_EP_BULKIN)  || \
       (USBD_MSC_EP_BULKIN  == USBD_CDC_EP_BULKOUT) || \
       (USBD_MSC_EP_BULKOUT == USBD_CDC_EP_INTIN)   || \
       (USBD_MSC_EP_BULKOUT == USBD_CDC_EP_BULKIN)  || \
       (USBD_MSC_EP_BULKOUT == USBD_CDC_EP_BULKOUT))
#error "Mass Storage Device and Communication Device Interface can not use same Endpoints!"
#endif
#endif
#endif

#if    (USBD_ADC_ENABLE)
#if    (USBD_CDC_ENABLE)
#if   ((USBD_ADC_EP_ISOOUT  == USBD_CDC_EP_INTIN)   || \
       (USBD_ADC_EP_ISOOUT  == USBD_CDC_EP_BULKIN)  || \
       (USBD_ADC_EP_ISOOUT  == USBD_CDC_EP_BULKOUT))
#error "Audio Device and Communication Device Interface can not use same Endpoints!"
#endif
#endif
#endif

#define USBD_ADC_CIF_NUM           (0)
#define USBD_ADC_SIF1_NUM          (1)
#define USBD_ADC_SIF2_NUM          (2)
#define USBD_CDC_CIF_NUM           (USBD_ADC_ENABLE*2+0)
#define USBD_CDC_DIF_NUM           (USBD_ADC_ENABLE*2+1)
#define USBD_HID_IF_NUM            (USBD_ADC_ENABLE*2+USBD_CDC_ENABLE*2)
#define USBD_MSC_IF_NUM            (USBD_ADC_ENABLE*2+USBD_CDC_ENABLE*2+USBD_HID_ENABLE)

#define USBD_ADC_CIF_STR_NUM       (3+USBD_STRDESC_SER_ENABLE+0)
#define USBD_ADC_SIF1_STR_NUM      (3+USBD_STRDESC_SER_ENABLE+1)
#define USBD_ADC_SIF2_STR_NUM      (3+USBD_STRDESC_SER_ENABLE+2)
#define USBD_CDC_CIF_STR_NUM       (3+USBD_STRDESC_SER_ENABLE+USBD_ADC_ENABLE*3+0)
#define USBD_CDC_DIF_STR_NUM       (3+USBD_STRDESC_SER_ENABLE+USBD_ADC_ENABLE*3+1)
#define USBD_HID_IF_STR_NUM        (3+USBD_STRDESC_SER_ENABLE+USBD_ADC_ENABLE*3+USBD_CDC_ENABLE*2)
#define USBD_MSC_IF_STR_NUM        (3+USBD_STRDESC_SER_ENABLE+USBD_ADC_ENABLE*3+USBD_CDC_ENABLE*2+USBD_HID_ENABLE)

#if    (USBD_HID_ENABLE)
#if    (USBD_HID_HS_ENABLE)
#define USBD_HID_MAX_PACKET       ((USBD_HID_HS_WMAXPACKETSIZE > USBD_HID_WMAXPACKETSIZE) ? USBD_HID_HS_WMAXPACKETSIZE : USBD_HID_WMAXPACKETSIZE)
#else
#define USBD_HID_MAX_PACKET        (USBD_HID_WMAXPACKETSIZE)
#endif
#else
#define USBD_HID_MAX_PACKET        (0)
#endif
#if    (USBD_MSC_ENABLE)
#if    (USBD_MSC_HS_ENABLE)
#define USBD_MSC_MAX_PACKET       ((USBD_MSC_HS_WMAXPACKETSIZE > USBD_MSC_WMAXPACKETSIZE) ? USBD_MSC_HS_WMAXPACKETSIZE : USBD_MSC_WMAXPACKETSIZE)
#else
#define USBD_MSC_MAX_PACKET        (USBD_MSC_WMAXPACKETSIZE)
#endif
#else
#define USBD_MSC_MAX_PACKET        (0)
#endif
#if    (USBD_ADC_ENABLE)
#if    (USBD_ADC_HS_ENABLE)
#define USBD_ADC_MAX_PACKET       ((USBD_ADC_HS_WMAXPACKETSIZE > USBD_ADC_WMAXPACKETSIZE) ? USBD_ADC_HS_WMAXPACKETSIZE : USBD_ADC_WMAXPACKETSIZE)
#else
#define USBD_ADC_MAX_PACKET        (USBD_ADC_WMAXPACKETSIZE)
#endif
#else
#define USBD_ADC_MAX_PACKET        (0)
#endif
#if    (USBD_CDC_ENABLE)
#if    (USBD_CDC_HS_ENABLE)
#define USBD_CDC_MAX_PACKET       ((USBD_CDC_HS_WMAXPACKETSIZE > USBD_CDC_WMAXPACKETSIZE) ? USBD_CDC_HS_WMAXPACKETSIZE : USBD_CDC_WMAXPACKETSIZE)
#else
#define USBD_CDC_MAX_PACKET        (USBD_CDC_WMAXPACKETSIZE)
#endif
#if    (USBD_CDC_HS_ENABLE1)
#define USBD_CDC_MAX_PACKET1      ((USBD_CDC_HS_WMAXPACKETSIZE1 > USBD_CDC_WMAXPACKETSIZE1) ? USBD_CDC_HS_WMAXPACKETSIZE1 : USBD_CDC_WMAXPACKETSIZE1)
#else
#define USBD_CDC_MAX_PACKET1       (USBD_CDC_WMAXPACKETSIZE1)
#endif
#else
#define USBD_CDC_MAX_PACKET        (0)
#define USBD_CDC_MAX_PACKET1       (0)
#endif
#define USBD_MAX_PACKET_CALC0     ((USBD_HID_MAX_PACKET   > USBD_HID_MAX_PACKET  ) ? (USBD_HID_MAX_PACKET  ) : (USBD_HID_MAX_PACKET  ))
#define USBD_MAX_PACKET_CALC1     ((USBD_ADC_MAX_PACKET   > USBD_CDC_MAX_PACKET  ) ? (USBD_ADC_MAX_PACKET  ) : (USBD_CDC_MAX_PACKET  ))
#define USBD_MAX_PACKET_CALC2     ((USBD_MAX_PACKET_CALC0 > USBD_MAX_PACKET_CALC1) ? (USBD_MAX_PACKET_CALC0) : (USBD_MAX_PACKET_CALC1))
#define USBD_MAX_PACKET           ((USBD_MAX_PACKET_CALC2 > USBD_CDC_MAX_PACKET1 ) ? (USBD_MAX_PACKET_CALC2) : (USBD_CDC_MAX_PACKET1 ))


/*----------------------------------------------------------------------------
 *      USB Config Functions
 *---------------------------------------------------------------------------*/

#ifndef  __USB_CONFIG___
#define  __USB_CONFIG__

#ifndef  __NO_USB_LIB_C
#include <usb_lib.c>
#endif

#endif  /* __USB_CONFIG__ */
