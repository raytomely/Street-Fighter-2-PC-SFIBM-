/****

   AT �¸w 101 ǡ�����a �����A�I �A�� ��˥

   MSC Version 1991.4.30 
   Borland C++ 2.0 Version Created 1992.8.23
****/

#include <dos.h>
#include <stdio.h>

/** ǡ�����a �a�Eš�a�� �a�A ���w�a�e ǡ�����a ���q ���� */
#define _ESC 1          /* ESC key                    */

#define _1 2            /* 1 key                      */
#define _2 3            /* 2                          */
#define _3 4            /* 3                          */
#define _4 5            /* 4                          */
#define _5 6            /* 5                          */
#define _6 7            /* 6                          */
#define _7 8            /* 7                          */
#define _8 9            /* 8                          */
#define _9 10           /* 9                          */
#define _0 11           /* 0                          */
#define _MINUS 12       /* -                          */
#define _EQUAL 13       /* =                          */
#define _BACKSP 14      /* back space                 */
#define _Q 16           /*                            */
#define _W 17           /*                            */
#define _E 18           /*                            */
#define _R 19           /*                            */
#define _T 20           /*                            */
#define _Y 21           /*                            */
#define _U 22           /*                            */
#define _I 23           /*                            */
#define _O 24           /*                            */
#define _P 25           /*                            */
#define _LPAREN 26      /* [                          */
#define _RPAREN 27      /* ]                          */
#define _A 30           /*                            */
#define _S 31           /*                            */
#define _D 32           /*                            */
#define _F 33           /*                            */
#define _G 34           /*                            */
#define _H 35           /*                            */
#define _J 36           /*                            */
#define _K 37           /*                            */
#define _L 38           /*                            */
#define _SEMI 39        /*  ;                         */
#define _QUATO 40       /*  '                         */
#define _RSLASH 43      /*  \ key                     */
#define _Z 44           /*                            */
#define _X 45           /*                            */
#define _C 46           /*                            */
#define _V 47           /*                            */
#define _B 48           /*                            */
#define _N 49           /*                            */
#define _M 50           /*                            */
#define _COMMA 51       /*  ,                         */
#define _DOT 52         /*  .                         */
#define _SLASH 53       /*  / key                     */
#define _SPACE 57       /*  space bar key             */

#define _F1  59         /*  Function 1 key            */
#define _F2  60         /*                            */
#define _F3  61         /*                            */
#define _F4  62         /*                            */
#define _F5  63         /*                            */
#define _F6  64         /*                            */
#define _F7  65         /*                            */
#define _F8  66         /*                            */
#define _F9  67         /*                            */
#define _F10 68         /*                            */

#define _RET 28         /* Return or Enter key        */
#define _TAB 15         /* Tab key                    */

#define _LSHIFT 42      /* Left Shift Key             */
#define _RSHIFT 54      /* Right Shift Key            */

#define _LCTRL 29       /* Left Control key           */
#define _RCTRL 29       /* Right Control key          */

#define _LALT 56        /* Left Alternate key         */
#define _RALT 56        /* Right Alternate key        */

#define _NHOME 71       /* XT 83 Keyborad Home key    */
#define _NPGUP 73       /*                Page Up     */
#define _NEND  79       /*                End         */
#define _NPGDN 81       /*                Page Down   */
#define _NINS  82       /*                Insert      */
#define _NDEL  83       /*                Delete      */

#define _NUMLOCK 69     /* Number Lock                /*
#define _CAPLOCK 58     /* Caps lock                  /*
#define _SCRLOCK 70     /* scroll lock                */

#define _UPARR 72       /* �� ���i�a                  */
#define _DWARR 80       /* �a�� ���i�a                */
#define _LFARR 75       /* �E�� ���i�a                */
#define _RGARR 77       /* ���e�� ���i�a              */

#define KBDINTVECT 0x9  /* ǡ�����a ������a ��ѡ     */

/** 
  ǡ�����a �a�Eš�a �����i �q�e ���i 
  ��) �����i�a ǡ���a ���ụ �wȁ���e �a�Eš�a�a 77 ���a�� _KS[77]�� 1�� �E�a.
                      ��ụ �wȁ���e                      _KS[77]�� 0�� �E�a.
**/    
unsigned char _KS[128] =
{
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
} ;

/**
  ǡ�����a �a�Eš�a �A�� �����i �q�e ���i
  �a���� �A�� �����e, ǡ���a ���ụ ���e�i �����s���a. �����e ���e �A�����a
  ���a �A���A�� ���i �����e ���ụ �wȁ�A�� ���� �i�i�A�e �e�A�a�� ���ụ 
  ���e�A �i�a�A�� ���a�a �a�w�S���a.
**/
unsigned char _KE[128] =
{
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
} ;

/** 
  �a��ǡ���a ���ụ �wȁ�e �a�� ���ụ ǡ���� �a�Eš�a�t�� �����S���a. 
  Hit Any key to start�w�A �a�w
**/
unsigned char _KBHIT=0 ;

/** �a��ǡ���a ���ứ�� �a�� �����e�a. ***/
#define KBDWAIT  for ( _KBHIT=0 ; !_KBHIT ; ) ; _KBHIT=0 ; 

/** ������ ǡ���a ������a ��˥ �q�� ͡���� **/
void interrupt far (*oldKBD)(void) ;

/** ������ ǡ���a ������a ��˥ **/
void interrupt keyStatusCheck(void)
{
   unsigned char k,scancode ;

   asm cli ;  /* ������a �q�� */

   k=inportb(0x60) ;  /* ǡ���t�i ���ᵥ�a. */

   scancode = k&(0x7f) ;  /* �a�Eš�a �e�i �e�a. */

   if (k<128 && _KS[scancode]==0)  /* ǡ���a ���ụ ���e */ 
   {
     _KS[scancode]=1 ; _KBHIT=scancode ; _KE[scancode]=1 ;
   }

   /** ǡ���a ��ụ ���e **/
   if (k>127) _KS[scancode]=0 ;
   /**
   (*oldKBD)() ;
    �� ��˥�i �a�e ������ ǡ���a ��˥�� ��З�e�a. getch() �q���w�i �i�����a.
    �����i��e ǡ���i �������a�� ���a�e �� �����a �e�a.
    -> �a�����aũ�� �����i ������ ���a.
   **/
   outportb(0x20,0x20) ;  /* ������a �A���A �{�v�q�i �i�a�� */
                          /* (*oldKBD)()�i ��� ���a�e �w���A�e �� ��˥��
                             ͡�q�A�� ���a�a�� ���ᕡ �E�a */
                         
   asm sti ;  /* ������a �q�� Ё�A */
}

/** ǡ�����a ������a ��˥�i ���������a�� �a���a. **/
setNewKBDInt()
{
   oldKBD   = getvect(KBDINTVECT) ;
   setvect(KBDINTVECT,keyStatusCheck) ;
   return(0) ;
}

/** ǡ�����a ������a ��˥�i �������� �a���a. **/
setOldKBDInt()
{
   setvect(KBDINTVECT,oldKBD) ;
   return(0) ;
}

main()
{
   int x,y,shoot ;

   setNewKBDInt() ; /* ǡ�����a ������a ��˥�i �a���a */

   printf(" Hit Any Key to Test Start !!\n") ;
   KBDWAIT ; /* �a��ǡ�a ���ứ�� �a�� ����  */

   x=0 ; y=0 ; shoot=0 ;

   /** �wз ���i�aǡ�i ������ ǡ���� ���
        �a�A���a ǡ���i �����e ǡ���� ��� **/

   for ( ; ; )
   {
     if (_KS[_LFARR]) x-- ;  /* ���b ���i�aǡ���a ����v�a�e �wз �q�� */
     if (_KS[_RGARR]) x++ ;  /* ���b ���i�aǡ���a ����v�a�e �wз �w�a */
     if (_KS[_UPARR]) y-- ;  /*  �� ���i�aǡ���a ����v�a�e �wз �q��  */
     if (_KS[_DWARR]) y++ ;  /* �a�� ���i�aǡ���a ����v�a�e �wз �w�a */
     /* �����e ǡ��, ����v�i���e �w�a */
     if (_KE[_SPACE]) { _KE[_SPACE]=0 ; shoot++ ; }
     if (_KE[_ESC])   break ; /*  ESC  ǡ���a ���ỡ�e �{���a . */
     printf("x=%5d y=%5d  shoot=%5d\n",x,y,shoot) ;
   }

   setOldKBDInt() ;
}
  


