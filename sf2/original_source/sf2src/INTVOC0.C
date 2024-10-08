/*******************************************************
   IBM PC �aϡ�� �q�� �b �a���a�� ���A

   Borland C++ 2.0
********************************************************/

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

#define SPEAKER_ON  (outp(97,(inp(97)|0x2)))   /* �aϡ��i ǥ�a */
#define SPEAKER_OFF (outp(97,(inp(97)&0xfc)))  /* �aϡ��i �e�a */
#define FREQ_IN(X) (outp(67,182),outp(66,((X)&0xff)),outp(66,((X)>>8)))
/** �a���� �a���� 0 �A �t�i ����e�a **/
#define Timer0_SET(X) (outp(0x43,0x34),outp(0x40,((X)&0xff)),outp(0x40,((X)>>8)))

#define BUSY 1  /** �q���� �a���e �����a. **/
#define IDLE 0  /** �q���� �b�� �{�v�a. **/

unsigned char *VoiceData,VoiceFlag=IDLE ;
unsigned int VoiceDataSize,VoiceDataBytePoint,TimerTick ;

#define TIMERINTVECT0 0x8  /* �a���� ������a �B�� **/
void interrupt (*TimerInt0x8)(void) ; /* �a���� ������a �q�� ͡���� */

void interrupt onePulsGen(void) /* ������a�� �e��a ���e ��˥ */
{
   disable() ; /* ������a �q�� */
   if (VoiceData[VoiceDataBytePoint]>0x7f) SPEAKER_ON  ;
                else                       SPEAKER_OFF ;

   VoiceDataBytePoint++ ; /* �q�� �A���a ͡����i �a�a �w�a */

   if (TimerTick==0) (*TimerInt0x8)() ; /* �����i �w�a��ǡ����Ё 
                                           ������ ������a��˥ ��З */ 
   /* �a���� �����a �������a 512�� �i�a�v�a�a�� 512�� �a�a �e�峢 ��З */    
   TimerTick++ ; if (TimerTick>511) TimerTick=0 ;

   /* �q�� �a�a�a �a �{�v�a�e �q�� �b ���� ��˥�i ���e�a. */
   if (VoiceDataBytePoint>=VoiceDataSize) silentVoice() ;
   outp(0x20,0x20) ; /* ������a�a �{�v�q�i �i�a���a. */
   enable() ; /* ������a �a�w */
}

/* �q������ fn�i �b�a�e ��˥ */
sayVoice(fn)
unsigned char fn[] ;
{
   FILE *fp ;

  
   if (VoiceFlag!=IDLE) return(0) ; /* �q���� �a�����e�����e ��З�e�q */

   /* �q�������i �i��, �A���� �i�w�h��, ���ᵥ�a */
   fp=fopen(fn,"rb") ; if (fp==NULL) return(0) ;
   VoiceDataSize=filelength(fileno(fp)) ;
   VoiceData=malloc(VoiceDataSize) ;
   if (VoiceData==NULL) { fclose(fp) ; return(0) ; }
   fread(VoiceData,1,VoiceDataSize,fp) ; fclose(fp) ;

   VoiceDataBytePoint=0 ; /* �q���A���a ͡����i ��q�a�� �i�w */

   disable() ; /* ������a �q�� */
   Timer0_SET(0x80) ; /* �a���� ������a�i 9.318Khz�� �A˷ */
   TimerInt0x8 = getvect(TIMERINTVECT0) ; /* ������ ������a ��˥�i ��ᵱ */
   setvect(TIMERINTVECT0,onePulsGen) ; /* ������ ������a ��˥�a�� �i�w */
   enable() ; /* ������a �a�w */
   TimerTick=0 ; /* ���� �e�� */
   VoiceFlag=BUSY ; /* �q�� �b�� */
}

silentVoice()
{
   if (VoiceFlag==IDLE) return(0) ; /* �q���� �e������ �a���e �{�� */
   disable() ;
   Timer0_SET(0x00) ;  /* �a���� �����i �������� 18.2Hz�� �e�e�a. */
   setvect(TIMERINTVECT0,TimerInt0x8) ; /* �a���� ������a�i �������� �e�q */
   enable() ;
   /* �q���A �i�w�E �A���� �e�� */
   if (VoiceData!=NULL) { free(VoiceData) ; VoiceData=NULL ; }
   VoiceFlag=IDLE ; /* �q�� �b�i �e�e�a. */
}

main(argc,argv)
int argc ;
char **argv ;
{
   sayVoice(argv[1]) ;
   for ( ; VoiceFlag==BUSY ; ) ;
}

