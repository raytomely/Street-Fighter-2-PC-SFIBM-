/**
   VGA 320*200, 256 Color Graphic Macro
   �a�a 320*200 256 �i�a �a��Ϣ ����

   1992. 9.17 �� �w ��
**/

#include "sf2_sdl.h"

#define far

extern  unsigned char far *_ADD[200] ; /* packed ���a ��a�A�a ͡���� */
extern  unsigned int  far  _SEG[200],*_PSEG ; /* packed ���a ��a�A�a �A�a���a
                                                 plane ���a �b���A���� ͡���� */
extern  unsigned int  far  _PSE[][200] ; /* plane ���a 4 page ��a�A�a �A�a���a */

/**
   Packed Mode Graphic Macro
   packed ���a �a��Ϣ �a�a��
**/

/* x,y ���a�A �� c �� �a���a. */
#define setP(X,Y,C)          (*(_ADD[(Y)]+(X)) = (C))
/* x,y �A�� xl,yl ������ bar�i �� c�� �a���a. */
#define barL(X,Y,XL,YL,C)    imgFill(_SEG[(Y)],(X),320,(XL),(YL),(C))
/* x1,y1 �A�� x2,y2 �a�� �� c�� bar�i �a���a. */
#define bar2(X1,Y1,X2,Y2,C)  imgFill(_SEG[(Y1)],(X1),320,(X2)-(X1)+1,(Y2)-(Y1)+1,(C))
/* �e���e�i �� c�� é�e�a. */
#define ClrScr(C)            imgHFill(0xa000,0,0xfa00,(C))
/* x,y�A�� ���w���� l�� ���w��i �� c�� �a���a. */
#define lineHL(X,Y,L,C)      imgHFill(_SEG[(Y)],(X),(L),(C))
/* x,y�A�� x2,y�a�� ���w��i �� c�� �a���a. */
#define lineH2(X1,Y,X2,C)    imgHFill(_SEG[(Y)],(X),(X2)-(X1)+1,(C))
/* x,y�A�� �������� l�� ������i �� c�� �a���a. */
#define lineVL(X,Y,L,C)      imgVFill(_SEG[(Y)],(X),320,(L),(C))
/* x,y�A�� x,y2�a�� ������i �� c�� �a���a. */
#define lineV2(X,Y1,Y2,C)    imgVFill(_SEG[(Y1)],(X),320,(Y2)-(Y1)+1,(C))
/* ���a x,y�A �a���a�a C,R���� �a�a ͡����a P�� �a�a�a���a�i �a���a. */
#define putSpr0(X,Y,P,C,R)   sprCp(FP_SEG(P),FP_OFF(P),C,C,R,_SEG[Y],X,320)
/* ���a x,y�A �a���a�a C,R���� �a�a ͡����a P�� �a�a�a���a�i �ᶉ�w�w�a�� �a���a. */
#define putSpr1(X,Y,P,C,R)   sprCpMir(FP_SEG(P),FP_OFF(P),C,C,R,_SEG[Y],X,320)
/* ���a x,y�A �a���a�a C,R���� �a�a ͡����a P�� �a�a�a���a�i �ỳ�� �a���a. */
#define putSpr2(X,Y,P,C,R)   sprCpUD(FP_SEG(P),FP_OFF(P),C,C,R,_SEG[Y],X,320)
/* ���a x,y�A �a���a�a C,R���� �a�a ͡����a P�� �a�a�a���a�i �ỳ�� �ᶉ�w�w�a�� �a���a. */
#define putSpr3(X,Y,P,C,R)   sprCpMirUD(FP_SEG(P),FP_OFF(P),C,C,R,_SEG[Y],X,320)

/**
   Plane Mode Graphic Marco
   4 �A���� �w�e ���a �a��Ϣ �a�a��
**/

/* x,y ���a�A �� c �� �a���a. */
#define psetP(X,Y,C)         pBytePut(_PSEG[(Y)],(X),(C))
/* x,y �A�� xl,yl ������ bar�i �� c�� �a���a. */
//#define pBarL(X,Y,XL,YL,C)   pImgFill(_PSEG[(Y)],(X),80,(XL)+(X)-1,(YL),(C))
#define pBarL(X,Y,XL,YL,C)   image_fill(active_page, X, Y, XL, YL, C)
/* x1,y1 �A�� x2,y2 �a�� �� c�� bar�i �a���a. */
//#define pBar2(X1,Y1,X2,Y2,C) pImgFill(_PSEG[(Y1)],(X1),80,(X2),(Y2)-(Y1)+1,(C))
#define pBar2(X1,Y1,X2,Y2,C) image_fill(active_page, X1, Y1, (X2)-(X1)+1, (Y2)-(Y1)+1, C)
/* x,y�A�� ���w���� l�� ���w��i �� c�� �a���a. */
//#define pLineHL(X,Y,L,C)     pImgHFill(_PSEG[(Y)],(X),(X)+(L)-1,(C))
#define pLineHL(X,Y,L,C)     image_fill(active_page, X, Y, L, 1, C)
/* x,y�A�� x2,y�a�� ���w��i �� c�� �a���a. */
//#define pLineH2(X1,Y,X2,C)   pImgHFill(_PSEG[(Y)],(X1),(X2),(C))
#define pLineH2(X1,Y,X2,C)   image_fill(active_page, X1, Y, (X2)-(X1)+1, 1, C)
/* x,y�A�� �������� l�� ������i �� c�� �a���a. */
//#define pLineVL(X,Y,L,C)     pImgVFill(_PSEG[(Y)],(X),80,(L),(C))
#define pLineVL(X,Y,L,C)     image_fill(active_page, X, Y, 1, L, C)
/* x,y�A�� x,y2�a�� ������i �� c�� �a���a. */
//#define pLineV2(X,Y1,Y2,C)   pImgVFill(_PSEG[(Y1)],(X),80,(Y2)-(Y1)+1,(C))
#define pLineV2(X,Y1,Y2,C)   image_fill(active_page, X, Y1, 1, (Y2)-(Y1)+1, C)
/* �e���e�i �� c�� é�e�a. */
//#define pClrScr(C)           imgHFill(_PSEG[0],0,0x4000,(C)) ;
#define pClrScr(C)           SDL_FillRect(active_page, NULL, C) ;
/* �A���� s�A�� �A���� d�� ���� �孷�e�a */
//#define page2PageCp(S,D)     writeMode(1) ; pImgCpLU(_PSE[(S)][0],0,80,80,200,_PSE[(D)][0],0,80) ;
//#define page2PageCp(S,D)     memmove(page[D]->pixels, page[S]->pixels, 64000);
#define page2PageCp(S,D)     SDL_BlitSurface(page[S], NULL, page[D], NULL);
/* ���a x,y�A �a���a�a C,R���� �a�a ͡����a P�� �a�a�a���a�i �a���a. */
#define pPutSpr0(X,Y,P,C,R)  psprCp(FP_SEG(P),FP_OFF(P),C,C,R,_PSEG[Y],X,80)
/* ���a x,y�A �a���a�a C,R���� �a�a ͡����a P�� �a�a�a���a�i �ᶉ�w�w�a�� �a���a. */
#define pPutSpr1(X,Y,P,C,R)  psprCpMir(FP_SEG(P),FP_OFF(P),C,C,R,_PSEG[Y],X,80)
/* ���a x,y�A �a���a�a C,R���� �a�a ͡����a P�� �a�a�a���a�i �ỳ�� �a���a. */
#define pPutSpr2(X,Y,P,C,R)  psprCpUD(FP_SEG(P),FP_OFF(P),C,C,R,_PSEG[Y],X,80)
/* ���a x,y�A �a���a�a C,R���� �a�a ͡����a P�� �a�a�a���a�i �ỳ�� �ᶉ�w�w�a�� �a���a. */
#define pPutSpr3(X,Y,P,C,R)  psprCpMirUD(FP_SEG(P),FP_OFF(P),C,C,R,_PSEG[Y],X,80)
/* ���a���e �A�����i ����e�a */
//#define viewPage(P)          setStAdd((P)*0x4000)
//#define viewPage(P)          SDL_BlitSurface(page[P], NULL, screen, NULL); SDL_Flip(screen); sleep()
#define viewPage(P)          blit_scaled_X2(screen, 0, 0, page[P]->pixels, 320, 320, 200); SDL_Flip(screen); sleep()
/* �w�e �a�� ���a 1 */
#define planeWrite1          planeMode() ; writeMode(1) ;
/* �w�e �a�� ���a 0 */
#define planeWrite0          planeMode() ; writeMode(0) ;
/* packed �a�� ���a 1 */
#define packWrite1           packMode()  ; writeMode(1) ;
/* packed �a�� ���a 0 */
#define packWrite0           packMode()  ; writeMode(0) ;

/* ��ѡ i�A red,green,blue�t�a�� �i�A�a ����e�a */
#define setRGB(I,R,G,B)      set1DACR((I),(R),(G),(B))

#define _R7 0xe0
#define _R6 0xc0
#define _R5 0xa0
#define _R4 0x80
#define _R3 0x60
#define _R2 0x40
#define _R1 0x20

#define _G7 0x1c
#define _G6 0x18
#define _G5 0x14
#define _G4 0x10
#define _G3 0x0c
#define _G2 0x08
#define _G1 0x04

#define _B3 0x03
#define _B2 0x02
#define _B1 0x01


