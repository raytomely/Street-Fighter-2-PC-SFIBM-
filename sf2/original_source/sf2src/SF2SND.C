/*
   �a�a2 �a���a,�����a �A�� ��˥

   ���a���� �a�i ��˥�e ���a�i ���� �s���e �� �����a.  
   
   ���a�� ���a ��˥�� �a�i �A�� ��˥�e ���a�a ���� library������ 
  
   obj �����e ���s���a. 
*/

/* ���a�� ���w�q�b �b ��˥ */
#include "sf2adl.c"

/** �q�� �b ��˥ **/
/***
   Motorola MC146818    CMOS Real Time Clock & RAM �A�� ó MC146818

   index port Address 0x70  ���B�a ͡�a ��a�A�a 
   data  port Address 0x71  �A���a ͡�a ��a�A�a

   inportance data address ���a�e �A���a ��a�A�a

   0xa Status Register A
       bit 7 : Update in process
               0 : read enable for time clock
               1 : updating time clock
       bit 6-4 : Stage Divider ( default Value 010 )
               000 : Timer frequency 4.194304 MHz
               001 : Timer frequency 1.048576 MHz
               010 : Timer frequency 32.768   KHz
               110 : Any
               111 : Any
       bit 3-0 : Rate Selection Bit
                      Timer freq 4.1 & 1.0 MHz   Timer freq 32.768 Khz
               0000 :          NONE                     NONE
               0001 :   32.768 KHz Interrupt       256 Hz  Interrupt
               0010 :   16.384 KHz Interrupt       128 Hz  Interrupt
               0011 :    8.192 KHz Interrupt      8192 Hz  Interrupt
               0100 :    4.096 KHz Interrupt      4096 Hz  Interrupt
               0101 :    2.048 KHz Interrupt      2048 Hz  Interrupt
               0110 :    1.024 KHz Interrupt      1024 Hz  Interrupt
               0111 :      512  Hz Interrupt       512 Hz  Interrupt
               1000 :      256  Hz Interrupt       256 Hz  Interrupt
               1001 :      128  Hz Interrupt       128 Hz  Interrupt
               1010 :       64  Hz Interrupt        64 Hz  Interrupt
               1011 :       32  Hz Interrupt        32 Hz  Interrupt
               1100 :       16  Hz Interrupt        16 Hz  Interrupt
               1101 :        8  Hz Interrupt         8 Hz  Interrupt
               1110 :        4  Hz Interrupt         4 Hz  Interrupt
               1111 :        2  Hz Interrupt         2 Hz  Interrupt

   0xb Status Register B
       bit 7 : timer data update enable
               0 : disable
               1 : enable
       bit 6 : periodic     interrupt enable ( interrupt 0x70 )
               0 : disable ( default value )
               1 : enable
       bit 5 : alram        interrupt enable ( interrupt 0x70 )
               0 : disable ( default value )
               1 : enable
       bit 4 : update ended interrupt enable ( interrupt 0x70 )
               0 : disable  ( default value )
               1 : enable
       bit 3 : Square Wave Enable
               0 : disable  ( default value )
               1 : enable
       bit 2 : Data numerical Base
               0 : BCD number   ( default )
               1 : binary number
       bit 1 : 24/12 select
               0 : 24 hour mode
               1 : 12 hour AM/PM mode
       bit 0 : Daylight Saving Enable
               0 : daylight saving disable ( default value )
               1 : daylight saving enable

   0xc Status Register C
   0xd Status Register D
***/
/* CMOS RAM�A �A���a�i �e�a */
#define CMOS_WRITE(I,X) (outportb(0x70,(I)) , outportb(0x71,(X)))
/* CMOS RAM�A�� �A���a�i ���e�a */
#define CMOS_READ(I)    (outportb(0x70,(I)) , inportb(0x71))

#define CMOSINTVECT 0x70 /* �����e �i�� ������a �B�� */
void interrupt (*oldRTC)(void) ; /* ������ �����e �i�� ������a �B�� */

#define BUSY 1 /* �q�� �b �� */
#define IDLE 0 /* �q�� �b�i �e�q */
#define MAX_VOICE_SIZE 20000 /* �q�� �a�a�� �A�� �a�� */

unsigned char *VoiceData,VoiceFlag=IDLE ; /* �q�� �A���a ͡���� , �q�� �wȁ �i�A�a */
int VoiceDataSize,VoiceDataBytePoint ; /* �q���A���a �a��, �e���q�� �a�a ��ѡ*/

#define SPEAKER_ON  (outp(97,(inp(97)|0x2)))  /* �aϡ�� �e�a */
#define SPEAKER_OFF (outp(97,(inp(97)&0xfc))) /* �aϡ�� �e�a */
#define FREQ_IN(X) (outp(67,182),outp(66,((X)&0xff)),outp(66,((X)>>8)))

extern int SOUND_DEV ; /* �a���a �a�a ���A */

#define NO_VOICE      0 /* �q�� �b�i �a�� �g�e�a */
#define PC_SPEAKER    1 /* ϡ�� �aϡ�ᝡ �q�� �b */
#define SOUND_BLASTER 2 /* �a���a �i���a�ᝡ �q�� �b */
#define COVOX1        3 /* š���a�a �a���� ͡�a 1�A �e�i */
#define COVOX2        4 /* š���a�a �a���� ͡�a 2�A �e�i */

#define COLOR_LPT1_PORT 0x378 /* ��� �ᔓ�� �a���� ͡�a 1 */
#define COLOR_LPT2_PORT 0x278 /* ��� �ᔓ�� �a���� ͡�a 2 */
#define MONO_LPT1_PORT  0x3bc /* ���I�A�a �ᔓ�� �a���� ͡�a 1 */

void interrupt onePulsGen(void)
{
   unsigned char data ;

   disable() ;

   data = VoiceData[VoiceDataBytePoint] ;
   switch(SOUND_DEV)
   {
     case PC_SPEAKER : if (data>127) SPEAKER_ON  ;
                              else   SPEAKER_OFF ;
                          break ;
     case SOUND_BLASTER : write_data(data) ; break ;
     case COVOX1        : outportb(COLOR_LPT1_PORT,data) ; break ;
     case COVOX2        : outportb(COLOR_LPT2_PORT,data) ; break ;
   }

   VoiceDataBytePoint++ ;

   if (VoiceDataBytePoint>=VoiceDataSize) silentVoice() ;

   CMOS_READ(0x0c) ; // reactive interrupt
   outportb(0xa0,0x20) ; outportb(0x20,0x20) ; // send EOI to 8259

   enable() ;
}

/* �q�� mdi ���� fn�� �����i ���a */
sayVoice(fn)
unsigned char fn[] ;
{
   FILE *fp ;

   if (SOUND_DEV==NO_VOICE) return(0) ;

   fp=fopen(fn,"rb") ; if (fp==NULL) return(0) ;
   if (VoiceFlag==BUSY) silentVoice() ;
   VoiceDataSize=filelength(fileno(fp)) ;
   if (VoiceDataSize>MAX_VOICE_SIZE) VoiceDataSize=MAX_VOICE_SIZE ;
   fread(VoiceData,1,VoiceDataSize,fp) ; fclose(fp) ;

   VoiceDataBytePoint=0 ;

   if (SOUND_DEV==SOUND_BLASTER) speaker_on() ;
   oldRTC = getvect(CMOSINTVECT) ;
   setvect(CMOSINTVECT, onePulsGen) ;
   outportb(0xa1,inportb(0xa1) & 0xfe) ;  // 2nd 8259A's IMR reset for RTC
   CMOS_WRITE(0xa,0xa3) ; // Set timer interrupt 8.192 KHz
   CMOS_WRITE(0xb,0x42) ; // enable timer interrupt
   VoiceFlag=BUSY ;
}

/* �q�� ���� �A���a �A���� �i�w */
allocVoiceMem()
{
   if ((VoiceData=malloc(MAX_VOICE_SIZE))==NULL) SOUND_DEV=NO_VOICE ;
}

/* �q���i �a��, ������ ������a ��˥�a�� ���� */
silentVoice()
{
   if (VoiceFlag==IDLE||SOUND_DEV==NO_VOICE) return(0) ;
   CMOS_WRITE(0xb,0x2) ; //disable RTC periodic int.
   CMOS_WRITE(0xa,0xa6) ; // restore timer frequency 1.024 KHz
   outportb(0xa1,inportb(0xa1) | 1) ;  // set mask for RTC
   setvect(CMOSINTVECT,oldRTC) ;
   if (SOUND_DEV==SOUND_BLASTER) speaker_off() ;
   VoiceFlag=IDLE ;
}
