#ifndef __24L01_H__
#define __24L01_H__
//***************************************************************//

#define BYTE unsigned char
#define uchar unsigned char

//***************************************************************//
#define TX_ADR_WIDTH    5   // 5 bytes TX(RX) address width
#define TX_PLOAD_WIDTH  5  // 	5 bytes TX payload


//***************************************************************//
//                   FUNCTION's PROTOTYPES  //
/****************************************************************/
 extern BYTE SPI_RW(BYTE byte);                                // Single SPI read/write
 extern BYTE SPI_Read(BYTE reg);                               // Read one byte from nRF24L01
 extern BYTE SPI_RW_Reg(BYTE reg, BYTE byte);                  // Write one byte to register 'reg'
 extern BYTE SPI_Write_Buf(BYTE reg, BYTE *pBuf, BYTE bytes);  // Writes multiply bytes to one register
 extern BYTE SPI_Read_Buf(BYTE reg, BYTE *pBuf, BYTE bytes);   // Read multiply bytes from one register
 extern void TX_Mode(void);
 extern void RX_Mode(void);

 extern void init_24l01_io(void);

//*****************************************************************/
#endif   /* _BYTE_DEF_ */