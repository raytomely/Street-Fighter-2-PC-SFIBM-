int vga320Mode(void);
int vgaTextMode(void);
void setStAdd(unsigned int st);
void actPage(int p);
void planeMode(void);
void writeMode(unsigned char mode);
void pBox2(int x1,int y1,int x2,int y2,int c);
void pBoxL(int x,int y,int xl,int yl,int c);
void pCTrpRImg0(int x,int y,unsigned char *p,int xs,int ys);
void pCTrpRImg1(int x,int y,unsigned char *p,int xs,int ys);
void pCTrpRImg2(int x,int y,unsigned char *p,int xs,int ys);
void pCTrpRImg3(int x,int y,unsigned char *p,int xs,int ys);
typedef unsigned int ui;
void pImgCpLU(ui sseg,ui soff,ui scs,ui xs,ui ys,ui dseg,ui doff, ui dcs);
void pSprCp(ui sseg,ui soff,ui scs,ui xs,ui ys,ui dseg,ui doff, ui dcs);
void pSprCpUD(ui sseg,ui soff,ui scs,ui xs,ui ys,ui dseg,ui doff, ui dcs);
void pSprCpMir(ui sseg,ui soff,ui scs,ui xs,ui ys,ui dseg,ui doff, ui dcs);
void pSprCpMirUD(ui sseg,ui soff,ui scs,ui xs,ui ys,ui dseg,ui doff, ui dcs);
void imgHFill(ui seg,ui off,ui xs,unsigned char color);
void pImgVFill(ui seg,ui off,ui scs,ui ys,unsigned char color);
void pBytePut(ui seg,ui off,unsigned char color);
void pImgHFill(ui seg,ui x1,ui x2,ui color);
void pImgFill(ui seg,ui x1,ui scs,ui x2,ui ys,ui color);
void pBitImgCp(ui sseg,ui soff,ui scs,ui ys,ui dseg,ui doff,ui dcs,unsigned char color);
void fPBitImgCp(ui sseg,ui soff,unsigned char scs,ui ys,ui dseg,ui doff,ui dcs,unsigned char color);
void get1DACR(unsigned char i,unsigned char *r,unsigned char *g,unsigned char *b);
void getNDACR(unsigned char s,int n,unsigned char *r,unsigned char *g,unsigned char *b);
void set1DACR(unsigned char i,unsigned char r,unsigned char g,unsigned char b);
void setNDACR(unsigned char s,int n,unsigned char *r,unsigned char *g,unsigned char *b);
void brightCtrl(unsigned char s,int n,unsigned char *r,unsigned char *g,unsigned char *b,int factor);
void greyCtrl(unsigned char s,int n,unsigned char *r,unsigned char *g,unsigned char *b,int factor);
void setRGBPalette(void);
void asmDecode(unsigned char *s,unsigned char *d,unsigned int l);





