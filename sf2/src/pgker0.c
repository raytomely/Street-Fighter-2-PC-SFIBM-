/*********

     VGA 320 * 200 , 256 ��� �w�e���a �a��Ϣ �a���a�១

     ���aš�a : Borland C++ 2.0
     �A�b���a : 1992.9.17
     �A�b�a   : �� �w ��

     �w�e���a : 4 �A���� �a�w�a�w ( 256K �a�� �a�a�A�� )
     packed mode : 1 �A���� �a�w�a�w ( ��ɷ�� �a�����aũ )
*********/

//#include <alloc.h>
#include <stdio.h>
#include <stdlib.h>
//#include <dos.h>

#include "sf2_sdl.h"

#define max(a,b)	((a) > (b) ? (a) : (b))
#define min(a,b)	((a) < (b) ? (a) : (b))

#include "pgker0.h"

/*void setStAdd() ;        // �������� �a�E ���b ��a�A�a �A˷
void actPage() ;         // �b�� �A���� �i�w
void planeMode() ;       // �y���� �A���a��i ���b, �a�E���a�� �a���a
void writeMode() ;       // �a�a�� �a�� ���a �e�w
void pBox2() ;           // �w�e���a�A�� �锁���a�� �w�a�a����
void pBoxL() ;           // �w�e���a�A�� �锁���a�� ������ �w�a�a����
void pPutImgMode() ;     // �w�e���a�A�� �a�a�a���a�i �a�� �wȁ�� �a����
void pCTrpRImg0() ;
void pCTrpRImg1() ;
void pCTrpRImg2() ;
void pCTrpRImg3() ;
void pImgCpLU() ;
void imgHFill() ;
void pSprCp() ;
void pSprCpUD() ;
void pSprCpMir() ;
void pSprCpMirUD() ;
void pImgVFill() ;
void pBytePut() ;
void pImgHFill() ;
void pImgFill() ;
void pBitImgCp() ;
void fPBitImgCp() ;
void get1DACR() ;
void getNDACR() ;
void set1DACR() ;
void setNDACR() ;
void brightCtrl() ;
void greyCtrl() ;
void setRGBPalette() ;
void asmDecode() ;*/

/*#define pLineHL(X,Y,L,C)     pImgHFill(_PSEG[(Y)],(X),(X)+(L)-1,(C))
#define pLineH2(X1,Y,X2,C)   pImgHFill(_PSEG[(Y)],(X1),(X2),(C))
#define pLineVL(X,Y,L,C)     pImgVFill(_PSEG[(Y)],(X),80,(L),(C))
#define pLineV2(X,Y1,Y2,C)   pImgVFill(_PSEG[(Y1)],(X),80,(Y2)-(Y1)+1,(C))*/

#include "grker0.h"

#define far

unsigned char far *_ADD[200]  ; // Packed Mode�A�� y ���a�A Ё�w�a�e �������� ��a�A�a
unsigned int  far _SEG[200]   ; // Packed Mode�A�� y ���a�A Ё�w�a�e �������� �A�a���a
unsigned int  far _PSE[4][200]; // �w�e���a�A�� y ���a�A Ё�w�a�e �������� �A�a���a
                                // [0][y] : �A���� 0 , [1][y] : �A���� 1 , ...
unsigned int  far *_PSEG ;      // �e�� �b���A������ ��a�A�a ͡����
                                // _PSEG[y] �t�� �e�� �b�� �A�����A�� y ���a�A
                                // Ё�w�a�e �������� �A�a���a��

unsigned char *_EBF ; // �e�� �s�E ���i Ή ���� ����

int _XL,_YL,_XR,_YR ;           // �����e �w�i�� ���� ���a�t

// (_XL,_YL) *------------
//           !           !
//           !           !
//           !-----------* (_XR,_YR)

/*
   vga320Mode()�e VGA�i 320*200 256 ��ᡡ�a�� �A˷�e�a.
     �a��a�� �e���i�� ������
   ���� ! �a�a �A�a�i �a���g�e�a.
*/
int vga320Mode(void)
{
   unsigned int i ;
   // �a�����a ũ�� �A˷�e�a.
   /*asm (
     mov ax,013h
     int 10h
   }*/

   // y ���a�A Ё�w�a�e �������� ��a�A�a ͡����t�i �A˷�e�a.
   // �a�a�� ���b ��a�A�a�e A000:0000 ���a.

   for ( i=0 ; i<200 ; i++ )
   {
     _ADD[i]=(unsigned char far *)(0xa0000000 + i*320) ;
     _SEG[i]=0xa000+i*20 ;
     _PSE[0][i]=0xa000 + i*5 ;
     _PSE[1][i]=0xa000 + i*5 + 0x400 ;
     _PSE[2][i]=0xa000 + i*5 + 0x800 ;
     _PSE[3][i]=0xa000 + i*5 + 0xc00 ;
   }

   _XL=_YL=0 ; _XR=319 ; _YR=199 ; // �w�i�t�i ����e�a.
   _PSEG = _PSE[0] ; // �b���A�����i 0 �A������ ����e�a.
   _EBF = malloc(13000) ; // �sЁ�A ����i �i�w�e�a.
   return(0) ;
}

/*
   vgaTextMode()�e �a�a�i 80*25 ��� �B�a�a ���a�� �A˷�e�a.
   ���� ! �a�a�A�a�i �a���g�e�a.
*/
int vgaTextMode(void)
{
   /*asm {
     mov ax,03h
     int 10h
   }*/
   return(0) ;
}

/*
   setStAdd()�e �a�a �a������ �a�E ��a�A�a�i ����e�a.

   �w�e ���a�A��e 4 �A�����a �a�w�a�a
      �A���� 0 �e setStadd(0x0000)
      �A���� 1 �e setStadd(0x4000)
      �A���� 2 �e setStadd(0x8000)
      �A���� 3 �e setStadd(0xc000)

   �� ��˥�e �a�a���� �w�a ���� �a�a�����a �a�� �A���� �����A���e�A �a���a.

   ���� ! ���� �a�a���e �A�� �e��a 4 Ϣ�I���a, �e�� 1 Ϣ�I�� �a�� ���a�e
          �a�a�� �a�e �����A���a��i �a���ᴡ�e�a.
*/
void setStAdd(unsigned int st)
{
   /*asm mov dx,03dah
lp:
   asm {
     in  al,dx
     test al,08h
     jz lp

     mov bx,st

     mov dx,03d4h
     mov al,0dh
     mov ah,bl
     out dx,ax

     mov dx,03d4h
     mov al,0ch
     mov ah,bh
     out dx,ax
   }*/
}

/*
   actPage() �e �e�� �b�� �A�����i �A˷�e�a.
   ���� ! ����i �A�����a �a�w�e�� ��a�i �a���g�e�a.
           �a�� �a�a�e 0-3 �a�� 4 �A�����a �a�w�a�a.
*/
void actPage(int p)
{
   active_page = page[p];
   _PSEG = _PSE[p] ;
}

/*
   planeMode() �e 1 �A���� �a�a packed���a�i 4 �A���� �w�e���a�� �a���a.
   �a���A���� ����i �a�w�a�a�e ϩ����a�� �w�e ���a�� ���Ё���e�a.
*/
void planeMode(void)
{
   //outport(0x3c4,0x0604) ;
   //outport(0x3d4,0x0014) ;
   //outport(0x3d4,0xe317) ;
}

/*
   writeMode(mode) �e �a�a �a�� ���a�i ����e�a.

   ���b �e�� 0 : ��ɷ�� �a�����a���� host memory�A�� ���������A �孷
             1 : ���� ���������a���� �a�����a���� 4�� �a�a�a.
                 ���a�����a�i ���w, ���� ���������A ���w���a, �a�a�a���a�i
                 ��w�a�a, ���� �a�a�a���a �១�i �e�a.
             2,3 �e �a�a �A�A��i ������
*/
void writeMode(unsigned char mode)
{
   /*unsigned char mask ;

   outportb(0x3ce,0x5) ;
   mask=inportb(0x3cf) ;
   outportb(0x3cf,(mask&0xfc)|mode) ;*/
}

/**
   pBox2(para,..) �e �w�e���a�A�� �w�a�i �a���a.

   input parameter
       x1,y1 �e ���b�w�e�� ���a���a.
       x2,y2 �e ���b�w�e�� ���a���a.
       c �e �a�� �����a.
**/
void pBox2(int x1,int y1,int x2,int y2,int c)
{
   pLineH2(x1,y1,x2,c) ;
   pLineH2(x1,y2,x2,c) ;
   pLineV2(x1,y1,y2,c) ;
   pLineV2(x2,y1,y2,c) ;
}

/**
   pBoxL(para,..) �e �w�e���a�A�� �w�a�i �a���a.

   input parameter
       x,y   ���b �w�e�� ���a���a.
       xl,yl �b�a�� ���w,���� �wз�a���� ����
       c �a�� ��
**/
void pBoxL(int x,int y,int xl,int yl,int c)
{
   pLineHL(x,y,xl,c) ;
   pLineHL(x,yl+y-1,xl,c) ;
   pLineVL(x,y,yl,c) ;
   pLineVL(xl+x-1,y,yl,c) ;
}

/*
   �i�E ���a�A�� �a�� ���e �wȁ

   ------      0        ------
   !#$  !               !#$  !  �a���� �a���a.
   !    !  -----------> !    !
   ------               ------
   ------      1        ------
   !#$  !               !  $#!  �ᶉ�A ��å�� ��� �a���a.
   !    !  -----------> !    !
   ------               ------
   ------      2        ------
   !#$  !               !    !  ��a���i �ỳ��� �a���a.
   !    !  -----------> !#$  !
   ------               ------
   ------      3        ------
   !#$  !               !    !  �ᶉ�A ��á��, �ỳ��� �a���a.
   !    !  -----------> !  $#!
   ------               ------
*/

/**
  pPutImgMode(para,,) �a�a�a���a�i �a�� ���e �wȁ�A �a�a �a���a.
  �a�a�a���a�e 2 �a�� unsigned char�wȁ�� ���i���� �a �t�� 0 ���w���A�e
  �a���� �g�a�� ���w�i ��ѡ�e�a.
  ���b �e��
      �a�a�a���a�i �a�� ���a (x,y)
      �a�a�a���a�a ��w�A�᷶�e ���i ͡���� p
      �a�a�a���a�� �a��,�A�� �a�� xs,ys
      �a�� ���e �wȁ 0 : �a�� �a���a.
                     1 : �ᶉ�A ��å �wȁ�� �a���a.
                     2 : �ỳ�� �a���a.
                     3 : �ᶉ�A ��å�wȁ�i �ỳ�� �a���a.

  ���� ! �w�e���a�A�� �a�wЁ���e�a. �i��Ϸ �A�a�e�a.
**/
void pPutImgMode(int x,int y,unsigned char *p,int xs,int ys,int m)
{
   switch (m)
   {
     case 1 : pCTrpRImg1(x,y,p,xs,ys) ; break ;
     case 2 : pCTrpRImg2(x,y,p,xs,ys) ; break ;
     case 3 : pCTrpRImg3(x,y,p,xs,ys) ; break ;
     default: pCTrpRImg0(x,y,p,xs,ys) ; break ;
   }
}

/**
  pCTrpRImg0(para,,)
  transparent means Sprite Put

  Input parameter
      display point (x,y)
      image data pointer p
      image x,y size xs,ys

  Warnning !! this procedure use plane Mode
  �w�e���a�A�� �a�a�a���a�i �a���a.
**/
void pCTrpRImg0(int x,int y,unsigned char *p,int xs,int ys)
{
   int x1,y1,xs1,ys1 ;

   x1 = max(x,_XL) ; xs1 = min(x+xs-1,_XR) - x1 + 1 ;
   y1 = max(y,_YL) ; ys1 = min(y+ys-1,_YR) - y1 + 1 ;

   if (xs1>0 && ys1>0)
   blit_raw(active_page, x1, y1, p+xs*(y1-y)+x1-x, xs, xs1, ys1);
   //pSprCp(FP_SEG(p),FP_OFF(p)+xs*(y1-y)+x1-x,xs,xs1,ys1,_PSEG[y1],x1,80) ;
}

/**
  pCTrpRImg1(para,,) Clipping Transparent Rectangular Image Mirror Put Procedure
  transparent means Sprite Put

  Input parameter
      display point (x,y)
      image data pointer p
      image x,y size xs,ys

  Warnning !! this procedure use plane Mode
  �w�e���a�A�� �a�a�a���a�i �ᶉ�A ��å �wȁ�� �a���a.
**/
void pCTrpRImg1(int x,int y,unsigned char *p,int xs,int ys)
{
   int x1,y1,x2,ys1 ;

   x1 = max(x,_XL) ; x2 = min(x+xs-1,_XR) ;
   y1 = max(y,_YL) ; ys1 = min(y+ys-1,_YR) - y1 + 1 ;

   if (x2>=x1 && ys1>0)
   blit_raw_flipped(active_page, x1, y1, p+xs*(y1-y)+xs+x-1-x2, xs, x2-x1+1, ys1, 1);
   //pSprCpMir(FP_SEG(p),FP_OFF(p)+xs*(y1-y)+xs+x-1-x2,xs,x2-x1+1,ys1,_PSEG[y1],x1,80) ;
}

/**
  pCTrpRImg2(para,,) Clipping Transparent Rectangular Image UD Put Procedure
  transparent means Sprite Put

  Input parameter
      display point (x,y)
      image data pointer p
      image x,y size xs,ys

  Warnning !! this procedure use plane Mode
  �w�e���a�A�� �a�a�a���a�i �ỳ�� �a���a.
**/
void pCTrpRImg2(int x,int y,unsigned char *p,int xs,int ys)
{
   int x1,y1,xs1,y2 ;

   x1 = max(x,_XL) ; xs1 = min(x+xs-1,_XR) - x1 + 1 ;
   y1 = max(y,_YL) ; y2 = min(y+ys-1,_YR)  ;

   if (xs1>0 && y2>=y1)
   blit_raw_flipped(active_page, x1, y1, p+xs*(ys+y-y2-1)+x1-x, xs, xs1, y2-y1+1, 2);
   //pSprCpUD(FP_SEG(p),FP_OFF(p)+xs*(ys+y-y2-1)+x1-x,xs,xs1,y2-y1+1,_PSEG[y1],x1,80) ;
}

/**
  pCTrpRImg3(para,,) Clipping Transparent Rectangular Image Mir UD Put Procedure
  transparent means Sprite Put

  Input parameter
      display point (x,y)
      image data pointer p
      image x,y size xs,ys

  Warnning !! this procedure use plane Mode
  �w�e���a�A�� �a�a�a���a�i �ᶉ�A ��å�wȁ�i �ỳ�� �a���a.
**/
void pCTrpRImg3(int x,int y,unsigned char *p,int xs,int ys)
{
   int x1,y1,x2,y2 ;

   x1 = max(x,_XL) ; x2 = min(x+xs-1,_XR) ;
   y1 = max(y,_YL) ; y2 = min(y+ys-1,_YR) ;

   if (x2>=x1 && y2>=y1)
   blit_raw_flipped(active_page, x1, y1, p+xs*(ys+y-1-y2)+xs+x-1-x2, xs, x2-x1+1, y2-y1+1, 3);
   //pSprCpMirUD(FP_SEG(p),FP_OFF(p)+xs*(ys+y-1-y2)+xs+x-1-x2,xs,x2-x1+1,y2-y1+1,_PSEG[y1],x1,80) ;
}


/**
   Image to Image Copy procedure
   image is 2D structure ( column size , row size )
   computer image is virtually mapping of 1D memory
   simply C language 2D array

   source image             destination image
  +------------+              +------------+
  !            !              ! seg,off    !
  ! seg,off    !   partial    !  +###      !
  !   +###  ===================> ####      !
  !   #### ys  !     copy     !  ####      !
  !   ####     !              !            !
  !    xs      !              !            !
  +------------+              +------------+
       scs                          dcs

  sseg,soff : source image start address ( segment , offset )
  scs       : source image column size
  xs,ys     : source image x,y size for copy

  dseg,doff : destination image start address ( segment , offset )
  dcs       : destination image column size

  �a���e�� ������ �A���a �孷 ���� �wȁ���a.
  �a���� �a�����e 2�a�� ���i�i �i�a�a, �a�a�� ��������,���a�Q �A�����w
  �a�w��a�� ����E 1 �a�� �A�����i �i�e�a.

  ���i �i�e �a�a�� 320*200 ���a�e �a���a���a 320, �A���a���a 200�� 2 �a��
  ���i �A���a�� �����a�A ��s�i �� ���a.

  �孷 ���a�a ���a�Q �A�����e �a�a�a���a�i �孷�a�e ���� �{��, �孷 ���a�a
  �a�a �����������e �a���i ��� ���e���� �{�a.

   ** �w�e ���a�A�� ҁ�a�e �����a�� �g�e�a.
**/

/**
  pImgCpLU()   source      image : host memory or VGA video RAM
               destination image : host memory or VGA video RAM

  Use Write Mode 0,1,2,3
  specially Write Mode 1 -> high speed scrolling or panning

  packed ���a�A�� �a���e�� �孷�i �e�a.
  �w�e���a�A�� �a�a �a�����a 1�� ����eҁ ���w���e ���� �孷�a�� �a�w�E�a.
**/
typedef unsigned int ui;
void pImgCpLU(ui sseg,ui soff,ui scs,ui xs,ui ys,ui dseg,ui doff, ui dcs)
{
   /*asm {
     push ds
     push si

     mov ax,sseg
     mov ds,ax
     mov ax,dseg
     mov es,ax

     cld

     mov si,soff
     mov di,doff

     mov bx,scs
     xor ax,ax
     mov dx,xs
   }

lp0:              // for ( y=0 to ys )
   asm {
     mov cx,dx
     rep movsb

     inc ax
     sub si,dx
     sub di,dx
     add si,bx
     add di,dcs
     cmp ax,ys
     jne lp0

     pop si
     pop ds
   }*/
}

/**
   pSprCp is Plane Mode Sprite Normal Put Procedure
      source      image : host Memory Only
      destination image : VGA video RAM Only
   �w�e���a�A�� �a�a�a���a�i �a���a.
**/
void pSprCp(ui sseg,ui soff,ui scs,ui xs,ui ys,ui dseg,ui doff, ui dcs)
{
   /*unsigned int sis,dis ;

   sis = ys*scs ; dis = ys*dcs ;

   asm {
     push ds
     push si
     mov ax,sseg
     mov ds,ax
     mov ax,dseg
     mov es,ax

     mov si,soff

     mov dx,3c4h
     mov al,2
     out dx,al
     mov al,11h
     mov cx,doff
     mov di,cx
     shr di,1
     shr di,1
     and cx,3
     rol al,cl
     mov dx,3c5h
     out dx,al

     mov bx,xs
   }
lp1:
   asm mov cx,ys
lp0:
   asm {
     mov ah,ds:[si]
     cmp ah,0
     je skip
     mov es:[di],ah
   }
skip:
   asm {
     add si,scs
     add di,dcs
     loop lp0
     rol al,1
     out dx,al
     adc di,0
     sub si,sis
     sub di,dis
     inc si
     dec bx
     jg lp1

     pop si
     pop ds
   }

   asm mov dx,03c4h
   asm mov ax,0f02h
   asm out dx,ax*/

}

/**
   pSprCpUD is Plane Mode Sprite Upside down Put Procedure
      source      image : host Memory Only
      destination image : VGA video RAM Only
   �w�e���a�A�� �a�a�a���a�i �ỳ�� �a���a.
**/
void pSprCpUD(ui sseg,ui soff,ui scs,ui xs,ui ys,ui dseg,ui doff, ui dcs)
{
   /*unsigned int sis,dis ;

   sis = ys*scs ; dis = ys*dcs ;
   soff += (scs*ys-scs) ;

   asm {
     push ds
     push si
     mov ax,sseg
     mov ds,ax
     mov ax,dseg
     mov es,ax

     mov si,soff

     mov dx,3c4h
     mov al,2
     out dx,al
     mov al,11h
     mov cx,doff
     mov di,cx
     shr di,1
     shr di,1
     and cx,3
     rol al,cl
     mov dx,3c5h
     out dx,al

     mov bx,xs
   }
lp1:
   asm mov cx,ys
lp0:
   asm {
     mov ah,ds:[si]
     cmp ah,0
     je skip
     mov es:[di],ah
   }
skip:
   asm {
     sub si,scs
     add di,dcs
     loop lp0
     rol al,1
     out dx,al
     adc di,0
     add si,sis
     sub di,dis
     inc si
     dec bx
     jg lp1

     pop si
     pop ds
   }

   asm mov dx,03c4h
   asm mov ax,0f02h
   asm out dx,ax*/
}

/**
   pSprCpMir is Plane Mode Sprite Mirror Put Procedure
      source      image : host Memory Only
      destination image : VGA video RAM Only
   �w�e���a�A�� �a�a�a���a�i �ᶉ�A ��å �wȁ�� �a���a.
**/
void pSprCpMir(ui sseg,ui soff,ui scs,ui xs,ui ys,ui dseg,ui doff, ui dcs)
{
   /*unsigned int sis,dis ;

   sis = ys*scs ; dis = ys*dcs ;
   soff += (xs-1) ;

   asm {
     push ds
     push si
     mov ax,sseg
     mov ds,ax
     mov ax,dseg
     mov es,ax

     mov si,soff

     mov dx,3c4h
     mov al,2
     out dx,al
     mov al,11h
     mov cx,doff
     mov di,cx
     shr di,1
     shr di,1
     and cx,3
     rol al,cl
     mov dx,3c5h
     out dx,al

     mov bx,xs
   }
lp1:
   asm mov cx,ys
lp0:
   asm {
     mov ah,ds:[si]
     cmp ah,0
     je skip
     mov es:[di],ah
   }
skip:
   asm {
     add si,scs
     add di,dcs
     loop lp0
     rol al,1
     out dx,al
     adc di,0
     sub si,sis
     sub di,dis
     dec si
     dec bx
     jg lp1

     pop si
     pop ds
   }
   asm mov dx,03c4h
   asm mov ax,0f02h
   asm out dx,ax*/
}

/**
   pSprCpMirUD is Plane Mode Sprite Mirror Upside down Put Procedure
      source      image : host Memory Only
      destination image : VGA video RAM Only
   �w�e���a�A�� �a�a�a���a�i �ᶉ�A ��å �wȁ�� �ỳ�� �a���a.
**/
void pSprCpMirUD(ui sseg,ui soff,ui scs,ui xs,ui ys,ui dseg,ui doff, ui dcs)
{
   /*unsigned int sis,dis ;

   sis = ys*scs ; dis = ys*dcs ;
   soff += (xs+scs*ys-scs-1) ;

   asm {
     push ds
     push si
     mov ax,sseg
     mov ds,ax
     mov ax,dseg
     mov es,ax

     mov si,soff

     mov dx,3c4h
     mov al,2
     out dx,al
     mov al,11h
     mov cx,doff
     mov di,cx
     shr di,1
     shr di,1
     and cx,3
     rol al,cl
     mov dx,3c5h
     out dx,al

     mov bx,xs
   }
lp1:
   asm mov cx,ys
lp0:
   asm {
     mov ah,ds:[si]
     cmp ah,0
     je skip
     mov es:[di],ah
   }
skip:
   asm {
     sub si,scs
     add di,dcs
     loop lp0
     rol al,1
     out dx,al
     adc di,0
     add si,sis
     sub di,dis
     dec si
     dec bx
     jg lp1

     pop si
     pop ds
   }
   asm mov dx,03c4h
   asm mov ax,0f02h
   asm out dx,ax*/
}


/**
   imgHFill(para,..) is image Horizontal Fill with color

   !--------------!
   ! seg,off      !
   !   +#######   !
   !      xs      !
   !              !
   !              !
   !              !
   !--------------!

   input parameter
       seg,off : segment,offset of image
       xs      : X size
       color   : color

   this procedure use Draw horizontal Line for packed mode

   Warnning !! this procedure used packed mode
   specially high speed Clear Screen Procedure for Plane Mode

   packed���a�A�� ���w��i �a���a.
   �w�e���a�A��e ���� ���e ��é��˥�a�� �a�w�I �� ���a.
**/
void imgHFill(ui seg,ui off,ui xs,unsigned char color)
{
   /*asm {
     push ds
     push si

     mov ax,seg
     mov es,ax
     mov di,off

     cld

     mov al,color
     mov ah,al

     mov cx,xs

     shr cx,1
     jnb lp1
     stosb
   }
lp1:
   asm {
     rep stosw
     pop si
     pop ds
   }*/
}

/**
   plane Mode Image Veritcal Fill Procedure

   input parameter
       seg,off : segment,offset of image
       scs     : image column size
       ys      : Y size
       color   : color

   this procedure use Draw vertical Line for plane Mode
   �i�E ���a�A�� ������i �a���a.
**/
void pImgVFill(ui seg,ui off,ui scs,ui ys,unsigned char color)
{
   /*asm {
     push ds
     push si

     mov dx,3c4h
     mov al,2
     out dx,al

     mov ax,seg
     mov es,ax

     mov cx,off
     mov di,cx
     shr di,1
     shr di,1
     and cx,3
     mov al,1
     shl al,cl
     mov dx,3c5h
     out dx,al
     mov dx,scs
     mov ah,color
     mov cx,ys
   }
lp0:
   asm {
     mov es:[di],ah
     add di,dx
     loop lp0

     pop si
     pop ds
   }
   asm mov dx,03c4h
   asm mov ax,0f02h
   asm out dx,ax*/
}

/**
   plane Mode put 1 pixel

   �i�E ���a�A�� �e ��i ���e�a.
**/
void pBytePut(ui seg,ui off,unsigned char color)
{
   /*asm {
     push ds
     push si

     mov dx,3c4h
     mov al,2
     out dx,al

     mov ax,seg
     mov es,ax

     mov cx,off
     mov di,cx
     shr di,1
     shr di,1
     and cx,3
     mov al,1
     shl al,cl
     mov dx,3c5h
     out dx,al
     mov ah,color
     mov es:[di],ah

     pop si
     pop ds
   }
   asm mov dx,03c4h
   asm mov ax,0f02h
   asm out dx,ax*/
}

/**
   plane Mode Image Horizontal Fill procedure

   !--------------!
   ! seg          !
   !   +######+   !
   !  x1     x2   !
   !              !
   !              !
   !              !
   !--------------!

   input parameter
       seg     : segment of image
       x1,x2   : horizontal fill left,right point
       color   : color

   this procedure use Draw horizontal Line for plane mode
   �w�e���a�A�� ���w��i �a���a.
**/
void pImgHFill(ui seg,ui x1,ui x2,ui color)
{
   /*unsigned int off2 ;

   asm {
     push ds
     push si

     mov ax,seg
     mov es,ax
     mov dx,3c4h
     mov al,2
     out dx,al

     mov cx,x1
     mov bx,cx
     and cx,3
     mov al,0fh
     shl al,cl

     mov cx,x2
     mov off2,cx
     and cx,3
     mov ah,0e1h
     rol ah,cl

     mov cl,2
     shr bx,cl
     shr word ptr off2,cl
     mov dx,03c5h

     cmp bx,off2
     je skip

     mov di,bx
     out dx,al
     mov al,ah
     mov ah,color
     mov es:[di],ah
     mov di,off2
     out dx,al
     mov es:[di],ah

     mov al,0fh
     out dx,al

     mov di,bx
     mov cx,off2
   }
lp0:
   asm {
     inc di
     cmp di,cx
     je end
     mov es:[di],ah
     jmp lp0
   }
skip:
   asm {
     and al,ah
     out dx,al
     mov di,bx
     mov ah,color
     mov es:[di],ah
  }
end:
   asm pop si
   asm pop ds
   asm mov dx,03c4h
   asm mov ax,0f02h
   asm out dx,ax*/
}


/**
   planeMode Image Fill procedure

   !--------------!
   ! seg,x1   x2  !
   !   ########   !
   !   ########ys !
   !   ########   !
   !              !
   !              !
   !--------------!
         scs

   input parameter
       seg     : segment of image
       x1,x2   : left,right value of x point
       scs     : image column size
       ys      : fill y size
       color   : fill color

   this procedure use Draw bar for plane mode
   �i�E ���a�A�� �a�b�w �����i �����e ���a�� é�e�a.
**/
void pImgFill(ui seg,ui x1,ui scs,ui x2,ui ys,ui color)
{
   /*unsigned int off2 ;

   asm {
     push ds
     push si

     mov ax,seg
     mov es,ax
     mov dx,3c4h
     mov al,2
     out dx,al

     mov cx,x1
     mov bx,cx
     and cx,3
     mov al,0fh
     shl al,cl

     mov cx,x2
     mov off2,cx
     and cx,3
     mov ah,0e1h
     rol ah,cl

     mov cl,2
     shr bx,cl
     shr word ptr off2,cl
     mov dx,03c5h

     cmp bx,off2
     je skip

     mov di,bx
     out dx,al
     mov al,ah
     mov ah,color
     mov cx,ys
   }
lp0:
   asm {
     mov es:[di],ah
     add di,scs
     loop lp0

     out dx,al
     mov di,off2
     mov cx,ys
  }
lp1:
   asm {
     mov es:[di],ah
     add di,scs
     loop lp1

     mov al,0fh
     out dx,al

     mov dx,off2
     inc bx
     sub dx,bx
     mov cx,ys
     mov di,bx
   }
lp3: asm mov bx,0
lp2: asm cmp bx,dx
   asm {
    je next0
    mov es:[di],ah
    inc di
    inc bx
    jmp lp2
  }
next0:
   asm {
     sub di,dx
     add di,scs
     loop lp3
     jmp end
   }
skip:
   asm {
     and al,ah
     out dx,al
     mov di,bx
     mov cx,ys
     mov ah,color
   }
lp5:
   asm {
     mov es:[di],ah
     add di,scs
     loop lp5
   }
end:
   asm pop si
   asm pop ds
   asm mov dx,03c4h
   asm mov ax,0f02h
   asm out dx,ax*/
}

/**
   plane Mode Bitmapped image copy

   input parameter see previous information
   note !! scs is source column size -> byte order not bit order
   example ) 16 x 16 bitmap font : source column size is 2 , not 16

   �i�E ���a�A�� ���a���i �a���a.
**/
void pBitImgCp(ui sseg,ui soff,ui scs,ui ys,ui dseg,ui doff,ui dcs,unsigned char color)
{
   /*unsigned int sis,dis ;

   sis = ys*scs ; dis = ys*dcs ;

   asm {
     push ds
     push si
     mov ax,sseg
     mov ds,ax
     mov ax,dseg
     mov es,ax

     mov si,soff

     mov dx,3c4h
     mov al,2
     out dx,al
     mov al,11h
     mov cx,doff
     mov di,cx
     shr di,1
     shr di,1
     and cx,3
     rol al,cl
     mov dx,3c5h
     out dx,al

     mov bl,color
     mov bh,scs
   }
lp2:
   asm mov cl,1
lp1:
   asm mov ch,ys
lp0:
   asm {
     mov ah,ds:[si]
     shl ah,cl
     jnb skip
     mov es:[di],bl
   }
skip:
   asm {
     add si,scs
     add di,dcs
     dec ch
     jg lp0
     rol al,1
     out dx,al
     adc di,0
     sub si,sis
     sub di,dis
     inc cl
     cmp cl,9
     jne lp1
     inc si
     dec bh
     jg lp2

     pop si
     pop ds
   }

   asm mov dx,03c4h
   asm mov ax,0f02h
   asm out dx,ax*/
}

/**
   fast plane Mode bitmapped image Copy

   input parameter see previous information
   note !! scs is source column size -> byte order, not bit order
   example ) 16 x 16 bitmap font : source column size is 2 , not 16

   warnnig !!
     because this procedure direct controlled write mode register with bitmap,
     bitmap data 4 bit nibble is swapped.

   �i�E ���a�A�� �����a�� ���a���i �a���a.
   ���a���a�� �i�� mask register�i �����a�a�� ���a�� �A���a�a 4bit�� �ỳ�ụ
   �wȁ�a�� �e�a.
**/
void fPBitImgCp(ui sseg,ui soff,unsigned char scs,ui ys,ui dseg,ui doff,ui dcs,unsigned char color)
{
   /*asm {
     push ds
     push si

     mov ax,sseg
     mov ds,ax
     mov ax,dseg
     mov es,ax

     mov si,soff
     mov di,doff

     mov dx,3c4h
     mov al,2
     out dx,al
     mov dx,3c5h
     mov cl,04
     mov ch,color
    }

    asm mov bh,ys
lp2:
    asm mov bl,scs
lp1:
   asm {
     mov ah,ds:[si]
     mov al,ah
     and al,0f0h
     shr al,cl
     out dx,al
     mov es:[di],ch
     inc di
     mov al,ah
     and al,0fh
     out dx,al
     mov es:[di],ch
     inc di
     inc si
     dec bl
     jg lp1
     add di,dcs
     sub di,scs
     sub di,scs
     dec bh
     jg lp2

     pop si
     pop ds
   }

   asm mov dx,03c4h
   asm mov ax,0f02h
   asm out dx,ax*/
}

/*
  get one Digital Analog Conver Register value
  Digital Analog Convert Register means VGA 256 R,G,B palette

  input  i : index value of palette register
  output r,g,b : r,g,b value of palette registers

  �i�A�a �A���a�� �t�i�i ���ᵥ�a.
**/
void get1DACR(unsigned char i,unsigned char *r,unsigned char *g,unsigned char *b)
{
   /*outportb(0x3c7,i) ;
   *r = inportb(0x3c9)   ;
   *g = inportb(0x3c9)   ;
   *b = inportb(0x3c9)   ;*/
}

/*
  get N Digital Analog Conver Register value
  Digital Analog Convert Register means VGA 256 R,G,B palette

  input  s : start index value of palette register
         n : number of index
  output r[],g[],b[] : r,g,b array value of palette registers

  N���� �i�A�a �A����t�i�i ���ᵥ�a.
**/
void getNDACR(unsigned char s,int n,unsigned char *r,unsigned char *g,unsigned char *b)
{
   /*int i ;

   outportb(0x3c7,s) ;

   for ( i=0 ; i<n ; i++ )
   {
     r[i] = inportb(0x3c9)   ;
     g[i] = inportb(0x3c9)   ;
     b[i] = inportb(0x3c9)   ;
   }*/
}

/*
  set one Digital Analog Conver Register value
  Digital Analog Convert Register means VGA 256 R,G,B palette

  input      i : index value of palette register
         r,g,b : r,g,b value of palette registers
  �e���� �i�A�a�A �t�i ����e�a.
**/
void set1DACR(unsigned char i,unsigned char r,unsigned char g,unsigned char b)
{
   /*outportb(0x3c8,i) ;
   outportb(0x3c9,r) ;
   outportb(0x3c9,g) ;
   outportb(0x3c9,b) ;*/
}

/*
  set N Digital Analog Conver Register value
  Digital Analog Convert Register means VGA 256 R,G,B palette

  input  s : start index value of palette register
         n : number of index
         r[],g[],b[] : r,g,b array value of palette registers
  N���� �i�A�a �t�i ����e�a.
**/
void setNDACR(unsigned char s,int n,unsigned char *r,unsigned char *g,unsigned char *b)
{
   /*int i ;

   outportb(0x3c8,s) ;

   for ( i=0 ; i<n ; i++ )
   {
     outportb(0x3c9,r[i]) ;
     outportb(0x3c9,g[i]) ;
     outportb(0x3c9,b[i]) ;
   }*/
}

/*
  bright control of VGA graphic screen
  input s : start index of palette register
        n : number of index
        r[],g[],b[] : r,g,b value of palette
        factor : birghtness percent(%)
  ���A�� �j���i % �� �����e�a.
*/
void brightCtrl(unsigned char s,int n,unsigned char *r,unsigned char *g,unsigned char *b,int factor)
{
   /*int i ;
   unsigned char r1,g1,b1 ;

   outportb(0x3c8,s) ;

   for ( i=0 ; i<n ; i++ )
   {
     r1 = (r[i] * factor)/100 ; outportb(0x3c9,r1) ;
     g1 = (g[i] * factor)/100 ; outportb(0x3c9,g1) ;
     b1 = (b[i] * factor)/100 ; outportb(0x3c9,b1) ;
   }*/
}

/*
  grey control of VGA graphic screen
  input s : start index of palette register
        n : number of index
        r[],g[],b[] : r,g,b value of palette
        factor : grey level percent(%)
  ���A�� �w�q�i %�� �����e�a
*/
void greyCtrl(unsigned char s,int n,unsigned char *r,unsigned char *g,unsigned char *b,int factor)
{
   /*int i ;
   unsigned char grey ;

   outportb(0x3c8,s) ;

   for ( i=0 ; i<n ; i++ )
   {
     grey = ((r[i]*3 + g[i]*5 + b[i]*2)/10)*factor/100  ;
     outportb(0x3c9,grey) ;
     outportb(0x3c9,grey) ;
     outportb(0x3c9,grey) ;
   }*/
}

/*
  set RGB palette

  index is 8 bit

  +--+--+--+--+--+--+--+--+
  |R2|R1|R0|G2|G1|G0|B1|B0|
  +--+--+--+--+--+--+--+--+
     RED      GREEN  BLUE

  �a�a2 ��w �i�A�a ���
*/
void setRGBPalette(void)
{
   int i,r,g,b ;

   for ( r=0,i=0 ; r<8 ; r++ )
     for ( g=0 ; g<8 ; g++ )
      for ( b=0 ; b<4 ; b++,i++ )
      set1DACR(i,r*8+4,g*8+4,b*16+8) ;
}

/*
 fast decoding of Modified Run length encoding

 input  s : source encoding data buffer pointer
        l : source encoding data size
 output d : destination decoding data buffer pointer

 Modified Run Length Encoding Format

 d1 d2 d3 d4 d5 d6 .....  dn

 first  data d1 : length of 0
 second data d2,d3 : length of data ( integer )
 d4,d5,d6 .. dk : data value ( length is d2 + d3*256 )
      .
      .
      .

 example) source encoding data is
            4 6 0 1 2 3 7 9 3 4 2 0 2 3

          decoding data is
            0 0 0 0 1 2 3 7 9 3 0 0 0 0 2 3
            !-----! !---------! !-----! !-!
               4         6         4     2

  �e�� Run length �s�i �����a�� ΅�a
*/

void asmDecode(unsigned char *s,unsigned char *d,unsigned int l)
{
   /*asm {

     push ds
     push si

     lds si,s
     les di,d
     mov bx,l
     mov dx,di

     cld
   }
lp0:
   asm {
     xor ch,ch
     mov cl,ds:[si]
     inc si
     dec bx
     jl  end
     xor ax,ax
     shr cx,1
     jnb lp1
     stosb
   }
lp1: asm rep stosw
   asm {
     mov cx,ds:[si]
     inc si
     inc si
     dec bx
     dec bx
     jl  end
     sub bx,cx
     shr cx,1
     jnb lp2
     movsb
   }
lp2: asm rep movsw
   asm jmp lp0
end:
   asm {
     mov cx,100
     xor ax,ax
     rep stosw
     mov ax,di
     sub ax,dx

     pop si
     pop ds
   }*/
}


