/****
   STREET FIGHTER ][ KEYBOARD INPUT ROUTINE
   Borland C++ 2.0 Version Created 1992.8.23  By Jung Y.D.

   �a�a2�w ǡ���a ���b ��˥
****/

#include "sf2_sdl.h"

/* ǡ���a �a�E š�a ���� */

#define _ESC 1          // ESC key

#define _1 2            // 1 key
#define _2 3            // 2
#define _3 4            // 3
#define _4 5            // 4
#define _5 6            // 5
#define _6 7            // 6
#define _7 8            // 7
#define _8 9            // 8
#define _9 10           // 9
#define _0 11           // 0
#define _MINUS 12       // -
#define _EQUAL 13       // =
#define _BACKSP 14      // back space
#define _Q 16           //
#define _W 17           //
#define _E 18           //
#define _R 19           //
#define _T 20           //
#define _Y 21           //
#define _U 22           //
#define _I 23           //
#define _O 24           //
#define _P 25           //
#define _LPAREN 26      // [
#define _RPAREN 27      // ]
#define _A 30           //
#define _S 31           //
#define _D 32           //
#define _F 33           //
#define _G 34           //
#define _H 35           //
#define _J 36           //
#define _K 37           //
#define _L 38           //
#define _SEMI 39        //  ;
#define _QUATO 40       //  '
#define _RSLASH 43      //  \ key
#define _Z 44           //
#define _X 45           //
#define _C 46           //
#define _V 47           //
#define _B 48           //
#define _N 49           //
#define _M 50           //
#define _COMMA 51       //  ,
#define _DOT 52         //  .
#define _SLASH 53       //  / key
#define __SPACE 57       //  space bar key

#define _F1  59         //  Function 1 key
#define _F2  60         //
#define _F3  61         //
#define _F4  62         //
#define _F5  63         //
#define _F6  64         //
#define _F7  65         //
#define _F8  66         //
#define _F9  67         //
#define _F10 68         //

#define _RET 28         // Return or Enter key
#define _TAB 15         // Tab key

#define _LSHIFT 42      // Left Shift Key
#define _RSHIFT 54      // Right Shift Key

#define _LCTRL 29       // Left Control key
#define _RCTRL 29       // Right Control key

#define _LALT 56        // Left Alternate key
#define _RALT 56        // Right Alternate key

#define _NHOME 71       // XT 83 Keyborad Home key
#define _NPGUP 73       //                Page Up
#define _NEND  79       //                End
#define _NPGDN 81       //                Page Down
#define _NINS  82       //                Insert
#define _NDEL  83       //                Delete

#define _NUMLOCK 69     // Number Lock
#define _CAPLOCK 58     // Caps lock
#define _SCRLOCK 70     // scroll lock

#define _UPARR 72
#define _DWARR 80
#define _LFARR 75
#define _RGARR 77

#define TIMERINTVECT 0x1c  /* �a���� ������a �B�� */
#define KBDINTVECT 0x9     /* ǡ�����a ������a �B�� */

/** ǡ�� ���e �wȁ �e�� **/
unsigned char _KS[128] =
{
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
} ;

/** ǡ�� �A�� �wȁ �e�� **/
unsigned char _KE[128] =
{
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
} ;

/**
  _KBHIT   ǡ���a�a ����v�a.
  _LKBHIT  ���b �i�A����A �i�w�E ǡ���a�a ����v�a
  _RKBHIT  ���b �i�A����A �i�w�E ǡ���a�a ����v�a.
  scanHashTbl �����b �i�A����a ���e ǡ�� �a�Eš�a hash table
**/
unsigned char _KBHIT=0,_LKBHIT,_RKBHIT,scanHashTbl[128] ;
char *scan2Asc="UDLR456123",Timer ;

/* ǡ���a�a ���ứ�� �a�� ���a���a. */
#define KBDWAIT  for ( _KBHIT=0 ; !_KBHIT ; ) ;

/* ǡ���a ���b�i ���e ���� */
typedef struct
{
   unsigned char b[17],p ;
}  KEYBUFF ;

/*
  �i�A���� ����ǡ �a�Eš�a
   U:���a  L:��  R:��   D:�f�q
   a,b,c:�i���b ��,��,�w, x,y,z:�����b ��,��,�w
*/
typedef struct
{
   unsigned int U,L,R,D,a,b,c,x,y,z ;
}  CTRLKEY ;

/* ��,���b �i�A����A�A  ǡ���a ���� �i�w */
KEYBUFF  LKB,RKB ;
/* ��ͩ�a ���b �i�A���� ���� ǡ (�a�E š�a�t) */
CTRLKEY  LK = { 17,30,31,44,19,20,21,33,34,35 } ;
/* ��ͩ�a ���b �i�A���� ���� ǡ (�a�E š�a�t) */
CTRLKEY  RK = { 72,75,77,80,82,71,73,83,79,81 } ;

#define LEFTMASK 0x40
#define RGHTMASK 0x20

char *_tmp = "abcdefghij" ;
char Rkey2asc[10] = "ulrd123456" ;
char Lkey2asc[10] = "ulrd123456" ;

/*
   �a�a2 �ŉw �e��
   WAITCONST : ���A�� �����A �a�e ���e ���e �e��
   COMTYPE   : ���A�� ���A ( ������ �a�a blinking timing �i�� )
   SOUND_DEV : �w�b�E �a���a �a�a ���A
*/
int WAITCONST,COMTYPE,SOUND_DEV ;

/* ��,���b �i�A���� ǡ���i�� �wȁ�e�� */
unsigned char LKBDN,RKBDN ;

#define interrupt
#include <ctype.h>

void interrupt (*oldKBD)(void) ;   /* ���� ǡ���a ������a ��˥ */
void interrupt (*oldTimer)(void) ; /* ���� �a���� ������a ��˥ */

/* ǡ���a �wȁ ���a ������a ��˥ */
void interrupt keyStatusCheck(void)
{
   unsigned char k,scancode,push,t ; k=scancode=push=0;
   unsigned char c,cu ;
   int i,r ;

   //asm cli ;

   //k=inportb(0x60) ;
   k=read_keyboard() ;
   if (!k) return;

   scancode = k&(0x7f) ;

/*
   ���a��a ���e ǡ���a �����b �i�A���� ���� ǡ���a�i ��a�a��
   �b �i�A����A �i�w�E ����A �����e�a.
*/

   if (k<128 && _KS[scancode]==0)
   {
     _KS[scancode]=1 ; _KBHIT=scancode ; _KE[scancode]=1 ;
     t=scanHashTbl[scancode] ;
     if (t&LEFTMASK)
     {
       LKBDN=0 ; c=Lkey2asc[t&0xf] ;
       LKB.b[LKB.p]=c ; LKB.p ++ ; if (LKB.p>15) LKB.p=0 ;
       _LKBHIT=c ;
     }
     if (t&RGHTMASK)
     {
       RKBDN=0 ; c=Rkey2asc[t&0xf] ;
       RKB.b[RKB.p]=c ; RKB.p ++ ; if (RKB.p>15) RKB.p=0 ;
       _RKBHIT=c ;
     }
   }

   /* ǡ���a�� ǡ���i �����a */
   if (k>127)
   {
     t=scanHashTbl[scancode] ;
     if (t&LEFTMASK)
     {
       LKBDN=1 ; c=cu=Lkey2asc[t&0xf] ;  r=LKB.p ;
       if (_KS[scancode]>20)  cu=toupper(c) ;
       for ( i=0 ; i<16 ; i++ )
       {
         r-- ;  if (r<0) r=15 ;
         if (LKB.b[r]==c) { LKB.b[r]=cu ; break ; }
       }
     }
     if (t&RGHTMASK)
     {
       RKBDN=1 ; c=cu=Rkey2asc[t&0xf] ; r=RKB.p ;
       if (_KS[scancode]>20) cu=toupper(c) ;
       for ( i=0 ; i<16 ; i++ )
       {
         r-- ;  if (r<0) r=15 ;
         if (RKB.b[r]==c) { RKB.b[r]=cu ; break ; }
       }
     }
     _KS[scancode]=0 ;
   }
   //outportb(0x20,0x20) ;
   //asm sti ;
}

#define MAXGAB 6

/** ǡ���a�a ���ụ ���e�i ���a. (1/18.2 ���e�b�a��) */
void interrupt sync(void)
{
   int i ;

   Timer++ ;

   /* ǡ���a�a ����v�a�e ���ụ ���e �w�a */
   for ( i=0 ; i<127 ; i++ )
   if (_KS[i]>0 && _KS[i]<255) _KS[i]++ ;

   if (LKBDN)
   {
     LKBDN++ ;
     if (LKBDN>MAXGAB)
     {  LKB.b[LKB.p]='?' ; LKB.p ++ ; LKBDN=0 ; if (LKB.p>15) LKB.p=0 ; }
   }
   if (RKBDN)
   {
     RKBDN++ ;
     if (RKBDN>MAXGAB)
     {  RKB.b[RKB.p]='?' ; RKB.p ++ ; RKBDN=0 ; if (RKB.p>15) RKB.p=0 ; }
   }

   //(*oldTimer)() ;
}

/* ������a ��˥�i �����A �i�w�e�a */
int setNewSystemInt(void)
{
   /*oldKBD   = getvect(KBDINTVECT) ;
   oldTimer = getvect(TIMERINTVECT) ;
   setvect(KBDINTVECT,keyStatusCheck) ;
   setvect(TIMERINTVECT,sync) ;*/
   LKB.p = RKB.p = LKB.b[16] = RKB.b[16] = LKBDN = RKBDN = 0 ;
   return(0) ;
}

/* ������ ������a ��˥�a�� �A˷�e�a */
int setOldSystemInt(void)
{
   /*setvect(KBDINTVECT,oldKBD) ;
   setvect(TIMERINTVECT,oldTimer) ;*/
   return(0) ;
}

#include <stdio.h>
#define far

/* �a�a2 �ŉw ��� �����i ���e�a */
int loadConfig(char fn[])
{
   FILE *fp ;
   int i,lp ;
   unsigned int far *p ;

   lp=1000 ; COMTYPE=0 ; SOUND_DEV=0 ;
   fp=fopen(getFilePath(fn),"r") ;
   if (fp!=NULL)
   {
     fscanf(fp,"%u %u %u %u %u %u %u %u %u %u",&LK.U,&LK.L,&LK.R,&LK.D,
                &LK.a,&LK.b,&LK.c,&LK.x,&LK.y,&LK.z) ;
     fscanf(fp,"%u %u %u %u %u %u %u %u %u %u",&RK.U,&RK.L,&RK.R,&RK.D,
                &RK.a,&RK.b,&RK.c,&RK.x,&RK.y,&RK.z) ;
     fscanf(fp,"%d %d %d",&lp,&COMTYPE,&SOUND_DEV) ;
     fclose(fp) ;
   }

   WAITCONST = (50.0 - 2800.0*lp/3275.0) ;
   p = (unsigned int far *)&LK ;
   for ( i=0 ; i<10  ; i++ ) scanHashTbl[p[i]] = (LEFTMASK|i) ;

   p = (unsigned int far *)&RK ;
   for ( i=0 ; i<10  ; i++ ) scanHashTbl[p[i]] |= (RGHTMASK|i) ;
   return(0) ;
}

/* �i�A����A �i�w�E ǡ�A�� ����i �����a. */
int flushKey(CTRLKEY *K)
{
   unsigned int far *p ;
   int i ;

   p = (unsigned int far *)K ;
   for ( i=0 ; i<10 ; i++ ) _KE[p[i]]=0 ;
   return(0) ;
}

/** ���� ���e ���e ���a���a **/
void waitSec(int t)
{
    SDL_Delay(t*60);
   //Timer=0 ;
   //for ( ; Timer!=t ; ) ;
}

