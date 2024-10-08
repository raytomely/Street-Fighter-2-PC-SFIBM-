/****
   Plane Mode font kernel Program
   Borland C++ 2.0 compiler Only
   1992.9.17 By Jung y.d.

   �i�E���a ���a�� ͥ�a �b �a���a��
   1992.9.17 ���w��
****/

//#include <dos.h>
#include <string.h>
//#include <malloc.h>
#include <stdio.h>

#include "grker0.h"
#include "animate0.h"

#include "sf2_sdl.h"

/* ���� 16 by 16 dot ���a�� ���� */
unsigned short far _bm[16]     ; // temporary font bitmap buffer
/* �e�i ���� �i�a ���a�� �A���a ͡���� */
unsigned short far *_FCBITMAP  ; // hangul first  character bitmap buffer pointer
/* �e�i ���� �i�a ���a�� �A���a ͡���� */
unsigned short far *_MCBITMAP  ; // hangul middle character bitmap buffer pointer
/* �e�i ���� �i�a ���a�� �A���a ͡���� */
unsigned short far *_LCBITMAP  ; // hangul last   character bitmap buffer pointer
/* �w�� �i�a ���a�� �A���a ͡���� */
unsigned char far *_ECBITMAP ; // english bitmap buffer pointer

/* �e�i ���� �i�a ���� */
#define NOFC 19  // Number Of First  Hangul Character
/* �e�i ���� �i�a ���� */
#define NOMC 21  // Number Of Middle Hangul Character
/* �e�i ���� �i�a ���� */
#define NOLC 27  // Number Of Last   Hanugl Character

/***
   Hangul code to Bitmap Convertion
   input  f,m,l  : code of first,middle,last hangul character
   output bitmap : 16 x 16 bitmap of hangul character

   return value  : offset of horizontal position

   Hangul code is special format.
               is not conventional format ( KS, ... )

   �e�i ��,��,���� š�a�i 16 by 16 dot ���a�� ͥ�a�� �e��
    f : ���� š�a
    m : ���� š�a
    l : ���� š�a
    bitmap : �e�i �i�a ���a��
    �e�i ͡���e �b�� ͡���a���� ������ �Ŭ��w,���s�w���e �a�a�a.
***/
int HCode2Bitmap(unsigned char f,unsigned char m,unsigned char l,unsigned short bitmap[16])
{
   int f0,m0,l0,fo,mo,lo,i ;
   unsigned int fm,mm,lm ;
   static char fo2[22] = { 0,0,0,0,0,0,0,0,0,1,3,3,3,1,2,4,4,4,2,1,3,0 } ;
   static char lo2[22] = { 0,0,0,0,0,0,0,0,0,3,0,0,0,3,3,2,2,2,3,3,0,0 } ;

   if (f>19) f=0 ;
   if (m>21) m=0 ;
   if (l>27) l=0 ;

   fo = fo2[m] ; mo=1 ; lo = lo2[m] ;
   if (f==1||f==16) mo = 0 ;
   if (l>0) { fo += 5 ; mo += 2 ; }

   f0 = f+fo*NOFC-1 ; m0 = m+mo*NOMC-1 ; l0 = l+lo*NOLC-1 ;

   for ( i=0 ; i<16 ; i++ )
   bitmap[i]=0 ;

   fm = f ? 0xffff : 0 ;
   mm = m ? 0xffff : 0 ;
   lm = l ? 0xffff : 0 ;

   for ( i=0 ; i<16 ; i++ )
   bitmap[i]
     = (_FCBITMAP[f0*16+i]&fm) | (_MCBITMAP[m0*16+i]&mm) | (_LCBITMAP[l0*16+i]&lm) ;

   if (f==0&&m==0&&l==0) return(8) ;
   return(16) ;
}

/***
   Load Font File
   input fn : font file name
   return value  0 : normal
                -1 : font file not exist

   ͥ�a �����i ���e�a.
***/
int loadFont(char *fn)
{
   FILE *fp ;

   fp=fopen(getFilePath(fn),"rb") ;
   if (fp==NULL) return(-1) ;
   _FCBITMAP=malloc(190*32) ; if (_FCBITMAP!=NULL) fread(_FCBITMAP,190,32,fp) ;
   _MCBITMAP=malloc( 84*32) ; if (_MCBITMAP!=NULL) fread(_MCBITMAP, 84,32,fp) ;
   _LCBITMAP=malloc(108*32) ; if (_LCBITMAP!=NULL) fread(_LCBITMAP,108,32,fp) ;
   _ECBITMAP=malloc(1504)   ; if (_ECBITMAP!=NULL) fread(_ECBITMAP,1,1504,fp) ;
   fclose(fp) ;
   return(0) ;
}

/**
  fast plane mode put text bitmap
  input x,y  : locate of x,y
        str  : print string
        mode : not used
        fc   : character foreground color
        bc   : not used

  str is special format of Street Fighter ][

  $str : english
  #str : hangul

  ���� ���a�� �a�E���a �b ��˥ ( ���a�� �A���a�i�� 4���a�a �ᎁ�� �A�ᴡ �q )
  �a�� ���a�e 4�� �������ᴡ �e�a. ( �i�E���a �b���i �a���� ���w�a�� ���� )
**/
void fPText(unsigned int x,unsigned int y,char str[],unsigned char mode,unsigned char fc,unsigned char bc)
{
   unsigned int i,p ;
   unsigned char c,f,m,l,t ;

   t=0 ;

   for ( i=0 ; str[i]!='\0' ; i++ )
   {
     c=str[i] ;
     if (c=='$') { t=1 ; continue ; }
     if (c=='#') { t=0 ; continue ; }
     if (c==' '||c=='_') { x += 8 ; continue ; }
     if (t==0)
     {
       f=str[i++]-'@' ; m=str[i++]-'@' ; l=str[i]-'@' ;
       HCode2Bitmap(f,m,l,_bm) ;
       blit_bitmap_hangul(active_page, x, y, _bm, 16, 16, fc);
       x += 16 ;
     }
     if (t==1)
     {

       p=str[i]-'!' ; p <<= 4 ;
       blit_bitmap(active_page, x, y, _ECBITMAP+p, 8, 16, fc);
       x += 8 ;
     }
   }
}

void pCTrpSpr0(int x,int y,Sprite *spr,Image *img);

/**
  fast plane mode put Sprite from text bitmap
  input x,y   : locate of x,y
        ch    : ascii code
        FC,BC : foreground , background Sprite
        IMG   : Image Data Structure

  ���� �w�e ���a ���a�� �b ��˥ ( �� ���� �a�a�a���a�� �a���a. )
  �e�i�a�i �a���� �a�w�E�a.
**/
void fPLargeECh(unsigned int x,unsigned int y,unsigned int xt,unsigned int yt,unsigned char ch,Sprite *FC,Sprite *BC,Image *IMG)
{
   unsigned int i,p,j ;
   unsigned char f ;
   int dx[8] ;

   dx[0]=3 ; dx[1]=2 ; dx[2]=1 ; dx[3]=0 ;
   dx[4]=7 ; dx[5]=6 ; dx[6]=5 ; dx[7]=4 ;

   p=ch-'!' ; p <<= 4 ;
   for ( i=3 ; i<12 ; i++,y+=yt )
   {
     f = *(_ECBITMAP+i+p) ;
     for ( j=0 ; j<8 ; j++ )
     {
       if (j==4) continue ;
       if (f&(0x80>>j)) pCTrpSpr0(x+dx[j]*xt,y,FC,IMG) ;
                else    pCTrpSpr0(x+dx[j]*xt,y,BC,IMG) ;
     }
   }
}
