
/*本文件为16×16点阵中文字库文件,字的横向8点构成一字节,左边点在字节的高位,字符点阵四角按左上角→右上角→左下角→右下角取字*/
/*如你想在以后继续用HZDotReader打开文件,追加汉字点阵数据,请不要修改或增删所有注释*/

#define  CB9E3  0x0000  /*广  */
#define  CD6DD  CB9E3+1  /*州  */
#define  CB9FA  CD6DD+1  /*国  */
#define  CD2B5  CB9FA+1  /*业  */
#define  CB5E7  CD2B5+1  /*电  */
#define  CD7D3  CB5E7+1  /*子  */
#define  CBFC6  CD7D3+1  /*科  */
#define  CBCBC  CBFC6+1  /*技  */
#define  CD3D0  CBCBC+1  /*有  */
#define  CCFDE  CD3D0+1  /*限  */
#define  CB9AB  CCFDE+1  /*公  */
#define  CCBBE  CB9AB+1  /*司  */
#define  CA3AC  CCBBE+1  /*，  */
#define  CB4F2  CA3AC+1  /*打  */
#define  CD3A1  CB4F2+1  /*印  */
#define  CB2E2  CD3A1+1  /*测  */
#define  CCAD4  CB2E2+1  /*试  */
#define  CA1A3  CCAD4+1  /*。  */
#define  CBBB0  CA1A3+1  /*话  */
#define  CA3BA  CBBB0+1  /*：  */
#define  CA1A2  CA3BA+1  /*、  */

unsigned char TestGY_Code[672] = 
{
/*广   CB9E3 */
0x01,0x00,0x00,0x80,0x3F,0xFE,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,
0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x40,0x00,0x40,0x00,0x80,0x00,0x00,0x00,

/*州   CD6DD */
0x08,0x04,0x08,0x84,0x08,0x84,0x08,0x84,0x08,0xC4,0x4C,0xA4,0x4A,0xA4,0x4A,0x94,
0x88,0x94,0x08,0x84,0x08,0x84,0x10,0x84,0x10,0x84,0x20,0x84,0x20,0x84,0x40,0x04,

/*国   CB9FA */
0x00,0x00,0x7F,0xFC,0x40,0x04,0x5F,0xF4,0x41,0x04,0x41,0x04,0x41,0x04,0x4F,0xE4,
0x41,0x44,0x41,0x24,0x41,0x24,0x5F,0xF4,0x40,0x04,0x40,0x04,0x7F,0xFC,0x40,0x04,

/*业   CD2B5 */
0x04,0x40,0x04,0x40,0x04,0x40,0x04,0x44,0x44,0x46,0x24,0x4C,0x24,0x48,0x14,0x50,
0x1C,0x50,0x14,0x60,0x04,0x40,0x04,0x40,0x04,0x44,0xFF,0xFE,0x00,0x00,0x00,0x00,

/*电   CB5E7 */
0x01,0x00,0x01,0x00,0x01,0x00,0x3F,0xF8,0x21,0x08,0x21,0x08,0x3F,0xF8,0x21,0x08,
0x21,0x08,0x21,0x08,0x3F,0xF8,0x21,0x08,0x01,0x02,0x01,0x02,0x00,0xFE,0x00,0x00,

/*子   CD7D3 */
0x00,0x00,0x3F,0xF0,0x00,0x20,0x00,0x40,0x00,0x80,0x01,0x00,0x01,0x00,0x01,0x04,
0xFF,0xFE,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x05,0x00,0x02,0x00,

/*科   CBFC6 */
0x06,0x08,0x78,0x88,0x08,0x48,0x08,0x48,0xFE,0x08,0x18,0x88,0x1C,0x48,0x2A,0x48,
0x28,0x0E,0x48,0x78,0x8B,0x88,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,

/*技   CBCBC */
0x10,0x20,0x10,0x20,0x10,0x20,0xFD,0xFE,0x10,0x20,0x14,0x20,0x19,0xFC,0x31,0x08,
0xD0,0x88,0x10,0x90,0x10,0x60,0x10,0x60,0x10,0x90,0x11,0x0E,0x56,0x04,0x20,0x00,

/*有   CD3D0 */
0x01,0x00,0x01,0x00,0x7F,0xFE,0x02,0x00,0x04,0x00,0x0F,0xF0,0x18,0x10,0x28,0x10,
0x4F,0xF0,0x88,0x10,0x08,0x10,0x0F,0xF0,0x08,0x10,0x08,0x90,0x08,0x70,0x08,0x20,

/*限   CCFDE */
0x00,0x00,0xFB,0xF8,0x92,0x08,0x93,0xF8,0xA2,0x08,0xA2,0x08,0x93,0xF8,0x8A,0x80,
0x8A,0x48,0xAA,0x50,0x92,0x20,0x82,0x20,0x82,0x10,0x82,0x8E,0x83,0x04,0x82,0x00,

/*公   CB9AB */
0x00,0x00,0x04,0x40,0x06,0x40,0x04,0x40,0x08,0x20,0x08,0x10,0x10,0x18,0x22,0x0E,
0x43,0x04,0x02,0x00,0x04,0x20,0x08,0x10,0x11,0xF8,0x3F,0x18,0x10,0x10,0x00,0x00,

/*司   CCBBE */
0x00,0x00,0x3F,0xFC,0x00,0x04,0x00,0x04,0x7F,0xE4,0x00,0x04,0x00,0x04,0x3F,0xC4,
0x20,0x44,0x20,0x44,0x3F,0xC4,0x20,0x44,0x20,0x04,0x00,0x14,0x00,0x08,0x00,0x00,

/*，   CA3AC */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x10,0x00,0x20,0x00,0x00,0x00,

/*打   CB4F2 */
0x10,0x00,0x10,0x00,0x13,0xFE,0x10,0x20,0xFE,0x20,0x10,0x20,0x14,0x20,0x18,0x20,
0x30,0x20,0xD0,0x20,0x10,0x20,0x10,0x20,0x10,0x20,0x10,0x20,0x50,0xA0,0x20,0x40,

/*印   CD3A1 */
0x00,0x00,0x0E,0xFC,0x70,0x84,0x40,0x84,0x40,0x84,0x40,0x84,0x7E,0x84,0x40,0x84,
0x40,0x84,0x44,0x84,0x58,0x94,0xE0,0x88,0x40,0x80,0x00,0x80,0x00,0x80,0x00,0x80,

/*测   CB2E2 */
0x40,0x02,0x27,0xC2,0x24,0x42,0x84,0x52,0x45,0x52,0x55,0x52,0x15,0x52,0x25,0x52,
0x25,0x52,0x25,0x52,0xC5,0x52,0x41,0x02,0x42,0x82,0x42,0x42,0x44,0x4A,0x48,0x04,

/*试   CCAD4 */
0x00,0x20,0x40,0x28,0x20,0x24,0x30,0x24,0x27,0xFE,0x00,0x20,0xE0,0x20,0x27,0xE0,
0x21,0x20,0x21,0x10,0x21,0x10,0x21,0x0A,0x29,0xCA,0x36,0x06,0x20,0x02,0x00,0x00,

/*。   CA1A3 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x30,0x00,0x48,0x00,0x48,0x00,0x30,0x00,0x00,0x00,0x00,0x00,

/*话   CBBB0 */
0x40,0x00,0x20,0x1C,0x33,0xE0,0x20,0x20,0x00,0x20,0x07,0xFE,0xF0,0x20,0x10,0x20,
0x10,0x20,0x11,0xFC,0x11,0x04,0x11,0x04,0x15,0x04,0x19,0xFC,0x11,0x04,0x00,0x00,

/*：   CA3BA */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x30,0x00,0x30,0x00,0x00,0x00,0x30,0x00,0x30,0x00,0x00,0x00,0x00,0x00,

/*、   CA1A2 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x30,0x00,0x10,0x00,0x00,0x00,0x00,0x00

};
