/****
    Street Fighter ][ Header File
    Original Game By CapCom
    IBM PC Game By Jung Young Dug ( KETEL ID : WD40 )
    Program Skill level : 1990.1 (0.7)
    Source Code Bolarnd C++ 2.0

    �a�a2 �A�� ����
    ���b CAPCOM
    IBM PC ��� ���w��
    �a���a�� ���� ���� : 1990�e 1�� ����
    ���aš�a ���e�a C++ 2.0
****/

#include <SDL_mixer.h>

typedef struct
{
    char name[20];
    int ID;
    Mix_Chunk *sound;
}CHAR_SND;

#define MAXIMG 120    // Maximum image Size
#define MAXSPR 120    // Maximum Sprite Size

/**
   define Warrior ID Number
   ā����i ���A ���a
**/

#define RYU     0
#define HONDA   1
#define BLANK   2
#define GUILE   3
#define KEN     4
#define CHUNLI  5
#define ZANGIEF 6
#define DALSIM  7

/* �i�A���� ���A */
#define HUMAN  0  // Player is Human  �a�q�� ����
#define COMPU  1  // Player is Computer  ���A��a ����
#define REMOTE 2  // Player connected Remote Modem  ���Q�a�� �e�i

/* ��˥ �A�� ��ѡ ���� */
#define EXIT      1  // Keyboard Signal is EXIT ( ESC key pressed )
#define BREAK     2  // Keyboard Signal is BREAK ( F10 Key pressed )
#define CONTINUE  3  // Keyboard Signal is CONTINUE
#define NEWPLAYER 4  // New Player Come on Signal

/* �i�A���� ���� ���w�� ����A�� �g�v�a. */
#define NOTSELECT (-1)     // Player is not Selected ( use Select Procedure )

/**
   Game Environment Structure
   �A�� �ŉw �e�� ����
**/

typedef struct
{
  int t ; // match time  �e�e�w ���� ���e
  int n,mn,tr ; // match number �e�� �����U��, �� �����U��
  unsigned char Bk    ; // Back Ground : RYU=0,HONDA=1, ...
  unsigned char loser[12] ; // �A���A�� �� �A���� �w�e
} GameENV ;

#define MAXASEQ  2000  // maximum Actor Sequence �A�� ���b �a�q��
#define MAXACT   128   // maximum Action Size �A�� ���b ��
#define MAXACTOR 400   // �A�� ���b ���e ��

/**
   Action Structure ���b ����
   Action is time Sequence of Actor ���b�e Actor�� ���e�� �a�q�a�� ����
   Action is Animation ���b�e animation�� �{�a.
**/
typedef struct
{
  unsigned char n[MAXACT]     ; // �e���b�� actor��
  int As[MAXASEQ],Ap[MAXACT]  ; // ���b �a�q ����, ���b �a�q ͡����
} ACTION ;

/**
   Actor Structure   Actor ����
   Actor is transition state of Sprite or Scene
   Actor�e �a�a�a���a�a �a���� �e���e�i �a�a���a, �b�� �����a ͡�q�E �wȁ
**/
typedef struct
{
   unsigned char sp ;  // sprite pointer  �a�a�a���a ��ѡ
   char dx,dy ;        // relative move position of X,Y  �w�� ���a ������
   unsigned int i ;    // Actor Information ���b�wȁ
} ACTOR ;

/*
  ���s ���b ���� ( ��� �a�� �e�� �wȁ�� ���s�A ���e ���b )

*/
typedef struct
{
   unsigned char lc,kc,ann,dann,mc,vfn[13] ;
} COMBRULE ;

/*
  ���a ���b ���� ( ǡ���a�i ���a��a�� �����i�� �a���e ���b )
*/
typedef struct
{
   char s[10],vfn[13],code ;
}  KEYRULE ;

#define MAXCR 20  /* �A�� ���s���b ���� */
#define MAXKR 14  /* �A�� ���a���b ���� */

/**
   Player Structure   �i�A���� ����
**/

typedef struct
{
   int ctrl,pl,end ; // Player state & control & player type ( RYU,KEN .. )
   int power,resis,energe,ve ;  // Player power & resistance & energe
   int x,y,ann,ani,ar,mirr ;    // Player Current Action #, Action index, Actor
   int x2,y2,ann2,ani2,ar2,mirr2 ;   // temporary action
   unsigned int i,i2 ;      // player Actor Information 1,2
   int nr,win,scr,chicken ;   // Player Near Distance
   char hit,san,can ;
   char far *key2asc ; // Player key state
   KEYRULE far *KR ;  // Spectial Attack Key Pattern
   KEYBUFF far *KB ;  // RingBuffer for Keyboard
   COMBRULE far *CR ; // Combination Attack RULE
   CTRLKEY far *K  ;  // Player Control Key
   ACTION  far *AN ;  // Player Action
   ACTOR   far *AR ;  // Player Actor
   Sprite  far *Spr ; // Player Sprite
   Image   far *Img ; // Player Image
   CHAR_SND *KR_sounds;
   CHAR_SND *CR_sounds;
} PLAYER ;


