/**
  �a���A���� ����
**/

#define far

typedef struct
{
   int xs,ys,ls ; /* �a���� �a��,�A���a�� . �s�E�w�� �s�E �a�� */
   unsigned char far *p ; /* �a�� �A���a ͡���� */
} Image ;

/*
   �a�a�a���a ����
*/
typedef struct
{
   int xo[2],yo[2],i[2],n,xs,ys,xfs ; /* �a���i �a�� x,y ����, ����
                                         �a�a�a���a x,y �a���a */
   char xh1,xh2,yh1,yh2 ; /* ���b ��� �� */
} Sprite ;
