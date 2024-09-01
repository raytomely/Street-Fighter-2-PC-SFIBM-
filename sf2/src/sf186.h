// from sf186.c
int systemCheck(void);
int isVga256K(void);
int mem520KFree(void);
int textLogo(void);
int systemWakeUp(void);
int loadFileConfig(char fn[]);
int setInitialValue(GameENV *ENV,PLAYER *LP,PLAYER *RP);
int viewWarn(void);
int demo(GameENV *GENV,PLAYER *LP,PLAYER *RP);
int loadDemoResource(char info[][80]);
int demo1(void);
int meetChar(Sprite *Spr,char info[][80]);
int gameSelect(GameENV *ENV,PLAYER *LP,PLAYER *RP);
int setOption(GameENV *ENV,PLAYER *LP,PLAYER *RP);
int macroEnv(unsigned char L,unsigned char R, int *v,int l,int u,int i);
int drawOpt(void);
int select(GameENV *GENV,PLAYER *LP,PLAYER *RP);
int loadSelectResource(GameENV *ENV);
int selectMode(GameENV *ENV,PLAYER *LP,PLAYER *RP);
int selectCtrl(GameENV *ENV,PLAYER *PL,int *x,int *y);
int moveAirplane(int x1,int y1,int x2,int y2,int p);
int vsScreen(PLAYER *LP,PLAYER *RP);
int game(GameENV *GENV,PLAYER *LP,PLAYER *RP);
int drawWinner(GameENV *ENV,PLAYER *LP,PLAYER *RP);
int drawLoser(int pl);
int loadGameResource(GameENV *ENV,PLAYER *LP,PLAYER *RP);
int convertCharacterColor(Image Img[],int n);
int ConvertImageColor(unsigned char *s,int l,unsigned char lookup[]);
int loadBkimg(char *fn,unsigned int xs,unsigned int ys2);
int playGame(GameENV *ENV,PLAYER *LP,PLAYER *RP);
int newPlayerComeIn(PLAYER *PL);
int startScene(PLAYER *LP,PLAYER *RP,GameENV *ENV,int off,int moff,int bks,int t);
int endScene(PLAYER *LP,PLAYER *RP,int off,int moff,int bks,int t,int p);
int viewInfo(PLAYER *LP,PLAYER *RP);
int initPlayer(PLAYER *P,int x,int y,int mirr);
int setPostition(PLAYER *LP,PLAYER *RP,int *off,int moff);
int creatTempAction(PLAYER *RP);
int pullControl(int *xl,int *xr,int off,int moff);
int setWinnerOrLoserAction(PLAYER *PL,PLAYER *PR);
int setTempPosition(PLAYER *LP);
int is2568Action(PLAYER *LP,PLAYER *RP);
int drawBackground(int scr,int off,int xs);
int drawActor(PLAYER *LP,PLAYER *RP,int scr);
int drawTempActor(PLAYER *PL);
int drawScoreBoard(PLAYER *LP,PLAYER *RP,int t);
int setPlayerActor(PLAYER *PL,int ann,int ani,int *ar,unsigned int *i);
int setNextAction(PLAYER *MP,PLAYER *EP);
int damageCheck(PLAYER *MP,PLAYER *EP);
int waveDamageCheck(PLAYER *MP,PLAYER *EP);
int setDamageAction(PLAYER *MP,PLAYER *EP,int da,int power,int flag);
int isOverLap(PLAYER *MP,int sL,int xl0,int yl0,int lm,PLAYER *EP,int sR,int xr0,int yr0,int rm);
int specialAttack(PLAYER *PL);
//int getSpecialCode(KEYRULE *KR,KEYBUFF *KB,int ctrl,char san);
int getSpecialCode(PLAYER *PL);
int patternMatch(char *str1,char *str2);
int combAttack(PLAYER *MP,PLAYER *EP);
int normalAttack(PLAYER *MP,PLAYER *EP);
int replaceAction(PLAYER *PL,int ac,int ann,int ani,int mask);
int getIndex(PLAYER *PL);
int getAttackCode(CTRLKEY *K,int ctrl);
int movePlayer(PLAYER *PL,PLAYER *EP);
int setAction(PLAYER *PL,int act);
int getMoveCode(CTRLKEY *K,int mirr,int ctrl);
int loadAttackRule(char fn[],int *nr,KEYRULE *PKR,COMBRULE *PCR,PLAYER *PL);
int loadSeq(char fn[],ACTION *AN,ACTOR *AR);
int field(char ch,char str[],unsigned int *f);
int fadeIn(void);
int fadeOut(void);
int cError(char ms1[],char ms2[],int m);
int systemDown(void);

// from psrker0.c
void initImg(Image img[], int n);
void freeImg(Image img[],int n);
unsigned int loadImgSpr(char IdFn[],Image img[],Sprite spr[],int flag);
void pPutRLESprMode(int x,int y,Sprite *spr,Image *img,int m);
void pPutSprMode(int x,int y,Sprite *spr,Image *img,int m);
void pCTrpSpr0(int x,int y,Sprite *spr,Image *img);
void pCTrpSpr1(int x,int y,Sprite *spr,Image *img);
void pCTrpSpr2(int x,int y,Sprite *spr,Image *img);
void pCTrpSpr3(int x,int y,Sprite *spr,Image *img);

// from sf2kbd0.c
int setNewSystemInt(void);
int loadConfig(char fn[]);
int flushKey(CTRLKEY *K);
void waitSec(int t);
int setOldSystemInt(void);
void keyStatusCheck(void);
void sync(void);

#define processKeyboardInput() keyStatusCheck(); sync()

// from pfont0.c
int loadFont(char *fn);
void fPText(unsigned int x,unsigned int y,char str[],unsigned char mode,unsigned char fc,unsigned char bc);
void fPLargeECh(unsigned int x,unsigned int y,unsigned int xt,unsigned int yt,unsigned char ch,Sprite *FC,Sprite *BC,Image *IMG);

// from sf2_sdl.c
void load_char_sounds(PLAYER *P1, PLAYER *P2);
void say_char_voice(int voice, PLAYER *PL, int type);

#include "sf2_sdl.h"
#include "pgker0.h"




