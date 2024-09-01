#include <SDL/SDL.h>
#include <SDL_mixer.h>

void convert_surface(SDL_Surface **surface)
{
    SDL_Surface *temp_surf = &(**surface);
    *surface = SDL_DisplayFormat (temp_surf);
    SDL_FreeSurface(temp_surf);
}

void convert_surface_alpha(SDL_Surface **surface)
{
    SDL_Surface *temp_surf = &(**surface);
    *surface = SDL_DisplayFormatAlpha(temp_surf);
    SDL_FreeSurface(temp_surf);
}

SDL_Surface *create_surface(int width, int height)
{
    SDL_Surface *surface;
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, rmask, gmask, bmask, amask);
    convert_surface(&surface);
    return surface;
}

SDL_Surface *copy_surface(SDL_Surface *surface)
{
    return SDL_ConvertSurface(surface, surface->format, SDL_SWSURFACE);
}

int get_file_size(FILE* file)
{
    fseek (file, 0, SEEK_END);
    int file_size = ftell(file);
    fseek (file, 0, SEEK_SET);
    return file_size;
}

int main_loop = 1;
SDL_Event event;
SDL_Surface *screen = NULL;
SDL_Surface *page[4] = {NULL};
SDL_Surface *active_page = NULL;
SDL_Color SF2Palette[256];
SDL_Color SF2FadePalette[256];
unsigned char background[1000*168];
Uint8 alpha_color = 0;
Mix_Chunk *game_sounds[20];
Mix_Music *game_music;
char data_folder[512];
char file_path[1024];
int sound_on;

void setSF2Palette(void)
{
   int i,r,g,b ; float scale = 4.2;

   for ( r=0,i=0 ; r<8 ; r++ )
     for ( g=0 ; g<8 ; g++ )
      for ( b=0 ; b<4 ; b++,i++ )
      {
          SF2Palette[i].r=(r*8+4)*scale;
          SF2Palette[i].g=(g*8+4)*scale;
          SF2Palette[i].b=(b*16+8)*scale;
          //printf("%d %d %d %d \n",i,SF2Palette[i].r,SF2Palette[i].g,SF2Palette[i].b);
      }
   SDL_SetPalette(screen, SDL_LOGPAL|SDL_PHYSPAL, SF2Palette, 0, 256);
}

void setBrightness(int factor)
{
   int i ;
   for ( i=0 ; i<256 ; i++ )
   {
     SF2FadePalette[i].r = (SF2Palette[i].r * factor) / 100 ;
     SF2FadePalette[i].g = (SF2Palette[i].g * factor) / 100 ;
     SF2FadePalette[i].b = (SF2Palette[i].b * factor) / 100 ;
   }
}

void blit_raw(SDL_Surface *dst, int x_pos, int y_pos, unsigned char *p, int xcs, int xs, int ys)
{
    int x, y;
    // compute starting address in dst surface
    Uint8  *dst_buffer = dst->pixels + y_pos * dst->pitch + x_pos;
    Uint8  *src_bitmap = p;

    for(y = 0; y < ys; y++)
    {
        for(x = 0; x < xs; x++)
        {
            if(src_bitmap[x] != alpha_color)
                dst_buffer[x] = src_bitmap[x];
        }
        dst_buffer += dst->w;
        src_bitmap += xcs;
    }
}

#define X_FLIP 1
#define Y_FLIP 2

void blit_raw_flipped(SDL_Surface *dst, int x_pos, int y_pos, unsigned char *p, int xcs, int xs, int ys, int flip)
{
    int x, y, x2;
    int xstart = 0, xinc = 1;
    // compute starting address in dst surface
    Uint8  *dst_buffer = dst->pixels + y_pos * dst->pitch + x_pos;
    Uint8  *src_bitmap = p;
    if(flip & X_FLIP)
    {
        xstart  = xs-1; xinc = -1;
    }
    if(flip & Y_FLIP)
    {
        src_bitmap = p+xcs*(ys-1);
        xcs = -xcs;
    }

    for(y = 0; y < ys; y++)
    {
        //src_bitmap = p+xcs*(ys-y-1);
        for(x = 0, x2 = xstart; x < xs; x++, x2+=xinc)
        {
                if(src_bitmap[x2] != alpha_color)
                    dst_buffer[x]=src_bitmap[x2];
        }
        dst_buffer += dst->w;
        src_bitmap += xcs;
    }
}

void blit_scaled_X2(SDL_Surface *dst, int x_pos, int y_pos, unsigned char *p, int xcs, int xs, int ys)
{
    int x, y, x2;
    unsigned char pixel;
    // compute starting address in dst surface
    Uint8  *dst_buffer = dst->pixels + y_pos * dst->pitch + x_pos;
    Uint8  *src_bitmap = (Uint8*) p;

    for(y = 0; y < ys; y++)
    {
        for(x = 0, x2= 0; x < xs; x++, x2 += 2)
        {
            pixel = src_bitmap[x];
            dst_buffer[x2] = pixel; dst_buffer[x2+1] = pixel;
            (dst_buffer + dst->w)[x2] = pixel; (dst_buffer + dst->w)[x2+1] = pixel;
        }
        dst_buffer += dst->w << 1;
        src_bitmap += xcs;
    }
}

#define WORD(b1, b2) ((b2 << 8) | (b1))

void blit_RLE(SDL_Surface *dst, unsigned char *src,unsigned int length, int x_pos, int y_pos, int xs, int ys)
{
    int i,j,n,x;
    // compute starting address in dst surface
    Uint8  *dst_buffer = dst->pixels + y_pos * dst->pitch + x_pos;
    Uint8  *src_bitmap = src;

    for(i = 0, x = 0; i < length; i += WORD(src[i+1], src[i+2])+3)
    {
        x+=src[i];
        if(x >= xs)
        {
            dst_buffer += dst->w;
            x = x - xs;
        }
        n = i+3+WORD(src[i+1], src[i+2]);
        for(j = i+3; j < n; j++)
        {
            dst_buffer[x] = src_bitmap[j];
            x++;
            if(x >= xs)
            {
                dst_buffer += dst->w;
                x = x - xs;
            }
        }
    }
}

void RLE_decoding(unsigned char *src,unsigned char *dst,unsigned int length, int sz)
{
    int i,j,k,n;
    for(i = 0, j = 0; i < length; i += WORD(src[i+1], src[i+2])+3)
    {
        for(n = 0; n < src[i]; n++)
            dst[j+n] = 0;
        j+=src[i];
        n = i+3+WORD(src[i+1], src[i+2]);
        for(k = i+3; k < n; k++,j++)
            dst[j] = src[k];
    }
    n = sz - j; j++;
    for(i = 0; i < n; i++, j++)
        dst[j] = 0;
}

#define SWAP_NIBBLE(b) ((b<<3&0x8888)|(b<<1&0x4444)|(b>>1&0x2222)|(b>>3&0x1111))
#define SWAP_BYTE(b) ((b<<8&0XFF00)|(b>>8&0X00FF))

void print_scanline(unsigned char scanline)
{
   int i, bitmask = 0x80;
   for(i=0; i<8; i++)
   {
       if(scanline & bitmask)printf("%d",1); else printf("%d",0);bitmask>>=1;
   }
   printf("\n");
}

void print_bitmap(unsigned char *bitmap)
{
    int i;
    for(i = 0; i < 16; i++)
    {
        print_scanline(SWAP_NIBBLE(bitmap[i]));
    }
    printf("\n");
}

void print_font(unsigned char *_ECBITMAP)
{
    int i;
    for(i = 0; i < 1504; i+=16)
    {
        printf("\n\n");
        printf("i=%d c=%c \n",i/16,(i/16+33));
        print_bitmap(&_ECBITMAP[i]);
    }
}

void blit_bitmap(SDL_Surface *dst, int x_pos, int y_pos, unsigned char *p, int xs, int ys, unsigned char color)
{
    int x, y;
    unsigned char c, bitmask = 0x80 ;
    // compute starting address in dst surface
    Uint8  *dst_buffer = dst->pixels + y_pos * dst->pitch + x_pos;

    for(y = 0; y < ys; y++)
    {
        c = SWAP_NIBBLE(p[y]);
        bitmask = 0x80;
        for(x = 0; x < xs; x++)
        {
            if(c & bitmask)
                dst_buffer[x] = color;
            //else
                //dst_buffer[x] = 0;
            bitmask >>= 1;
        }
        dst_buffer += dst->w;
        bitmask = 0x80;
    }
}

void blit_bitmap_hangul(SDL_Surface *dst, int x_pos, int y_pos, unsigned short *p, int xs, int ys, unsigned char color)
{
    int x, y;
    unsigned short c, bitmask = 0x8000 ;
    // compute starting address in dst surface
    Uint8  *dst_buffer = dst->pixels + y_pos * dst->pitch + x_pos;

    for(y = 0; y < ys; y++)
    {
        c = SWAP_NIBBLE(p[y]);
        c = SWAP_BYTE(c);
        bitmask = 0x8000;
        for(x = 0; x < xs; x++)
        {
            if(c & bitmask)
                dst_buffer[x] = color;
            //else
                //dst_buffer[x] = 0;
            bitmask >>= 1;
        }
        dst_buffer += dst->w;
    }
}

char *getFilePath(char *filename)
{
    int len = strlen(data_folder);
    memset(file_path, 0, sizeof(file_path));
    strcpy(file_path, data_folder);
    file_path[len] = '/'; len++;
    strcpy(&file_path[len], filename);
    return file_path;
}

void set_data_folder(void)
{
    //atexit(SF2_SDL_quit);
    FILE *fp ;
    fp=fopen("data_folder","rb") ;
    if(fp==NULL)
    {
        fprintf(stderr, "Couldn't open data_folder file \n");
        //SF2_SDL_quit();
        exit(1);
    }
    memset(data_folder, 0, sizeof(data_folder));
    fread(data_folder, sizeof(char), get_file_size(fp), fp) ;
    fclose(fp) ;
}

void load_config(void)
{
    char c, s[10];
    FILE *fp ;
    fp=fopen("sf2config.cfg","rb") ;
    if(fp==NULL)
    {
        fprintf(stderr, "Couldn't open sf2config.cfg file \n");
        exit(1);
    }
    memset(data_folder, 0, sizeof(data_folder));
    fscanf(fp,"%s %c", s, &c);
    c=getc(fp);
    while(isspace(c) && c != EOF){c=getc(fp);} ungetc(c, fp);
    fgets(data_folder, 512, fp);
    int i = strlen(data_folder)-1;
    while(isspace(data_folder[i]) && i > 0){data_folder[i] = 0; i--;}
    //fscanf(fp,"%s %c %s", s, &c, data_folder);
    fscanf(fp,"%s %c %d", s, &c, &sound_on);
    fclose(fp) ;
}

void print_logo(void)
{
    int x, y, i;
    FILE *fp ;
    unsigned char logo[50*80] ;

    fp=fopen(getFilePath("tlogo.sf2"),"rb") ;
    fread(logo, 80, 50, fp) ;
    fclose(fp) ;
    x=0;y=0;
    for(i = 0; i < 80*50; i+=2)
    {
        if(logo[i] == 176 )printf(" ");
        else
            if (logo[i] < 128)
                printf("%c ",logo[i]);
            else
                printf("%d ",(logo[i+1]));
        x+=1;
        if(x>=80){printf("\n\n");x=0;y++;}
    }
    printf("x=%d y=%d \n",x,y);
}

extern unsigned char _KBHIT;
void keyStatusCheck(void);
void sync(void);
void sleep(void);
#define processKeyboardInput() keyStatusCheck(); sync()

void blit_logo(unsigned char *_ECBITMAP)
{
    int x, y, i;
    FILE *fp ;
    unsigned char logo[50*80], c = 0;

    SDL_Rect rect = {0,  0,  8, 16};
    SDL_Color colors[9]={{255, 255, 0}, {0, 255, 0}, {0, 128, 0}, {0, 128, 128}, {0, 0, 255}, {0, 0, 0}, {255, 255, 255}, {0, 0, 128}, {0, 0, 170}};
    SDL_SetColors(screen, colors, 0, 9);
    SDL_FillRect(screen, NULL, 7);
    fp=fopen(getFilePath("tlogo.sf2"),"rb") ;
    fread(logo, 80, 50, fp) ;
    fclose(fp) ;
    x=0;y=0;
    for(i = 0; i < 80*50; i+=2)
    {
        if(logo[i] != 176 )
        {
            if (logo[i] < 128)
            {
                rect.x = x*8; rect.y = y*16;
                SDL_FillRect(screen, &rect, 8);
                blit_bitmap(screen, x*8, y*16, _ECBITMAP+((logo[i]-'!') << 4) , 8, 16, 6);
            }
            else
            {
                switch(logo[i+1])
                {
                    case 14: c = 0; break; case 16: c = 5; break; case 10: c = 1; break;
                    case 2: c = 2; break; case 3: c = 3; break; case 9: c = 4; break;
                }
                rect.x = x*8; rect.y = y*16;
                SDL_FillRect(screen, &rect, c);
            }
        }
        x+=1;
        if(x>=80){x=0;y+=1;}
    }
    SDL_Flip(screen);
    for ( _KBHIT=0 ; !_KBHIT ; ) {processKeyboardInput(); sleep();}
    SDL_SetPalette(screen, SDL_LOGPAL|SDL_PHYSPAL, SF2Palette, 0, 256);
}

void image_fill(SDL_Surface *dst, int x, int y, int xs, int ys, int color)
{
    static SDL_Rect rect;
    rect.x = x; rect.y = y; rect.w = xs; rect.h = ys;
    SDL_FillRect(dst, &rect, color);
}

void viewPage_offset(int x, int y, int p)
{
    unsigned char *op = page[p]->pixels + y * page[p]->pitch + x;
    SDL_FillRect(screen, NULL, 0);
    blit_scaled_X2(screen, 0, 0, op, 320, 320-x, 200-y);
    SDL_Flip(screen);
    SDL_Delay(3);
}

#include "sf2kbd0.h"

int sf2_keys[SDLK_LAST];

void intit_sf2_keys(void)
{
    memset(sf2_keys, 0, sizeof(sf2_keys));
    sf2_keys[SDLK_ESCAPE] = _ESC ;          // ESC key

    sf2_keys[SDLK_1] = _1 ;            // 1 key
    sf2_keys[SDLK_2] = _2 ;            // 2
    sf2_keys[SDLK_3] = _3 ;            // 3
    sf2_keys[SDLK_4] = _4 ;            // 4
    sf2_keys[SDLK_5] = _5 ;            // 5
    sf2_keys[SDLK_6] = _6 ;            // 6
    sf2_keys[SDLK_7] = _7 ;            // 7
    sf2_keys[SDLK_8] = _8 ;            // 8
    sf2_keys[SDLK_9] = _9 ;           // 9
    sf2_keys[SDLK_0] = _0 ;           // 0
    sf2_keys[SDLK_MINUS] = _MINUS ;       // -
    sf2_keys[SDLK_EQUALS] = _EQUAL ;       // =
    sf2_keys[SDLK_BACKSPACE] = _BACKSP ;      // back space
    sf2_keys[SDLK_q] = _Q ;           //
    sf2_keys[SDLK_w] = _W ;           //
    sf2_keys[SDLK_e] = _E ;           //
    sf2_keys[SDLK_r] = _R ,           //
    sf2_keys[SDLK_t] = _T ;           //
    sf2_keys[SDLK_y] = _Y ;           //
    sf2_keys[SDLK_u] = _U ;           //
    sf2_keys[SDLK_i] = _I ;           //
    sf2_keys[SDLK_o] = _O ;           //
    sf2_keys[SDLK_p] = _P ;           //
    sf2_keys[SDLK_LEFTPAREN] = _LPAREN ;      // [
    sf2_keys[SDLK_RIGHTPAREN] = _RPAREN ;      // ]
    sf2_keys[SDLK_a] = _A ;           //
    sf2_keys[SDLK_s] = _S ;           //
    sf2_keys[SDLK_d] = _D ;           //
    sf2_keys[SDLK_f] = _F ;           //
    sf2_keys[SDLK_g] = _G ;           //
    sf2_keys[SDLK_h] = _H ;           //
    sf2_keys[SDLK_j] = _J ;           //
    sf2_keys[SDLK_k] = _K ;           //
    sf2_keys[SDLK_l] = _L ;           //
    sf2_keys[SDLK_SEMICOLON] = _SEMI ;        //  ;
    sf2_keys[SDLK_QUOTE] = _QUATO ;       //  '
    sf2_keys[SDLK_BACKSLASH] = _RSLASH ;      //  \ key
    sf2_keys[SDLK_z] = _Z ;           //
    sf2_keys[SDLK_x] = _X ;           //
    sf2_keys[SDLK_c] = _C ;           //
    sf2_keys[SDLK_v] = _V ;           //
    sf2_keys[SDLK_b] = _B ;           //
    sf2_keys[SDLK_n] = _N ;           //
    sf2_keys[SDLK_m] = _M ;           //
    sf2_keys[SDLK_COMMA] = _COMMA ;       //  ,
    sf2_keys[SDLK_PERIOD] = _DOT ;         //  .
    sf2_keys[SDLK_SLASH] = _SLASH ;       //  / key
    sf2_keys[SDLK_SPACE] = __SPACE ;       //  space bar key

    sf2_keys[SDLK_F1] = _F1  ;         //  Function 1 key
    sf2_keys[SDLK_F2] = _F2  ;         //
    sf2_keys[SDLK_F3] = _F3  ;         //
    sf2_keys[SDLK_F4] = _F4  ;         //
    sf2_keys[SDLK_F5] = _F5  ;         //
    sf2_keys[SDLK_F6] = _F6  ;         //
    sf2_keys[SDLK_F7] = _F7  ;         //
    sf2_keys[SDLK_F8] = _F8  ;         //
    sf2_keys[SDLK_F9] = _F9  ;         //
    sf2_keys[SDLK_F10] = _F10 ;         //

    sf2_keys[SDLK_RETURN] = _RET ;         // Return or Enter key
    sf2_keys[SDLK_TAB] = _TAB ;         // Tab key

    sf2_keys[SDLK_LSHIFT] = _LSHIFT ;      // Left Shift Key
    sf2_keys[SDLK_RSHIFT] = _RSHIFT ;      // Right Shift Key

    sf2_keys[SDLK_LCTRL] = _LCTRL ;       // Left Control key
    sf2_keys[SDLK_RCTRL] = _RCTRL ;       // Right Control key

    sf2_keys[SDLK_LALT] = _LALT ;        // Left Alternate key
    sf2_keys[SDLK_RALT] = _RALT ;        // Right Alternate key

    sf2_keys[SDLK_HOME] = _NHOME ;       // XT 83 Keyborad Home key
    sf2_keys[SDLK_PAGEUP] = _NPGUP ;       //                Page Up
    sf2_keys[SDLK_END] = _NEND  ;       //                End
    sf2_keys[SDLK_PAGEDOWN] = _NPGDN ;       //                Page Down
    sf2_keys[SDLK_INSERT] = _NINS  ;       //                Insert
    sf2_keys[SDLK_DELETE] = _NDEL  ;       //                Delete

    sf2_keys[SDLK_NUMLOCK] = _NUMLOCK ;     // Number Lock
    sf2_keys[SDLK_CAPSLOCK] = _CAPLOCK ;     // Caps lock
    sf2_keys[SDLK_SCROLLOCK] = _SCRLOCK ;     // scroll lock

    sf2_keys[SDLK_UP] = _UPARR ;
    sf2_keys[SDLK_DOWN] = _DWARR ;
    sf2_keys[SDLK_LEFT] = _LFARR ;
    sf2_keys[SDLK_RIGHT] = _RGARR ;
}

int systemDown(void);

int read_keyboard(void)
{
    if(SDL_PollEvent(&event) == 1)
    {
        switch(event.type)
        {
            case SDL_QUIT:
                systemDown();
                break;
            case SDL_KEYDOWN:
                //printf("k=%d\n",sf2_keys[event.key.keysym.sym]);
                return sf2_keys[event.key.keysym.sym];
                 /*switch(event.key.keysym.sym)
                {
                    case SDLK_1:
                        printf("1\n");
                        break;

                }*/
            case SDL_KEYUP:
                return sf2_keys[event.key.keysym.sym]+128;
        }
    }
    return 0;
}

int fileExist(char *filename)
{
    FILE *output = fopen(getFilePath(filename), "r");
    if(output != NULL)
    {
        fclose(output);
        return 1;
    }
    fclose(output);
    return 0;
}

/*typedef struct
{
    char name[20];
    int ID;
    Mix_Chunk *sound;
}CHAR_SND;*/

#include "animate0.h"
#include "sf20.h"

#define MAX_CHAR_SOUND 20
#define SOUND_ID 0XFF00
#define SOUND_INDEX 0XFF
#define getSoundIndex(sounds, i) ((sounds[i].ID & SOUND_ID) == SOUND_ID ? sounds[i].ID & SOUND_INDEX : i)

CHAR_SND char1_KR_sounds[MAX_CHAR_SOUND];
CHAR_SND char2_KR_sounds[MAX_CHAR_SOUND];
CHAR_SND char1_CR_sounds[MAX_CHAR_SOUND];
CHAR_SND char2_CR_sounds[MAX_CHAR_SOUND];

int isSoundLoaded(char snd[], CHAR_SND char_snd[])
{
    int i;
    for(i = 0; i < 20; i++)
    {
        if(strcmp(snd, char_snd[i].name) == 0)
            return i;
    }
    return -1;
}

void load_char_KR_sounds(PLAYER *PL, CHAR_SND char_KR_snd[])
{
    int i, id;
    for(i = 0; (i < PL->san) && (i < MAX_CHAR_SOUND); i++)
    {
        id = isSoundLoaded(PL->KR[i].vfn ,char_KR_snd);
        if(id >= 0)
        {
            char_KR_snd[i].ID = id | SOUND_ID;
            strcpy(char_KR_snd[i].name, char_KR_snd[id].name);
        }
        else
        {
            if(fileExist(PL->KR[i].vfn))
                char_KR_snd[i].sound = Mix_LoadWAV(getFilePath(PL->KR[i].vfn));
            char_KR_snd[i].ID = i;
            strcpy(char_KR_snd[i].name, PL->KR[i].vfn);
        }
    }
}

void load_char_CR_sounds(PLAYER *PL, CHAR_SND char_CR_snd[])
{
    int i, id;
    for(i=0 ; (PL->CR[i].lc<100)  && (i < MAX_CHAR_SOUND); i++)
    {
        id = isSoundLoaded((char*)PL->CR[i].vfn ,char_CR_snd);
        if(id >= 0)
        {
            char_CR_snd[i].ID = id | SOUND_ID;
            strcpy(char_CR_snd[i].name, char_CR_snd[id].name);
        }
        else
        {
            if(fileExist((char*)PL->CR[i].vfn))
                char_CR_snd[i].sound = Mix_LoadWAV(getFilePath((char*)PL->CR[i].vfn));
            char_CR_snd[i].ID = i;
            strcpy(char_CR_snd[i].name, (char*)PL->CR[i].vfn);
        }
    }
}

void free_char_sounds(void)
{
    int i;
    for(i = 0; i < MAX_CHAR_SOUND; i++)
    {
        Mix_FreeChunk(char1_KR_sounds[i].sound);
        Mix_FreeChunk(char2_KR_sounds[i].sound);
        Mix_FreeChunk(char1_CR_sounds[i].sound);
        Mix_FreeChunk(char2_CR_sounds[i].sound);
    }
    memset(char1_KR_sounds, 0, sizeof(char1_KR_sounds));
    memset(char2_KR_sounds, 0, sizeof(char2_KR_sounds));
    memset(char1_CR_sounds, 0, sizeof(char1_KR_sounds));
    memset(char2_CR_sounds, 0, sizeof(char2_CR_sounds));
}

void load_char_sounds(PLAYER *P1, PLAYER *P2)
{
    free_char_sounds();
    if(sound_on)
    {
        load_char_KR_sounds(P1, char1_KR_sounds);
        load_char_KR_sounds(P2, char2_KR_sounds);
        load_char_CR_sounds(P1, char1_CR_sounds);
        load_char_CR_sounds(P2, char2_CR_sounds);
    }
    P1->KR_sounds = char1_KR_sounds; P1->CR_sounds = char1_CR_sounds;
    P2->KR_sounds = char2_KR_sounds; P2->CR_sounds = char2_CR_sounds;
}

void load_sounds(void)
{
    game_sounds[0] = Mix_LoadWAV(getFilePath("plane.voc"));
    game_sounds[1] = Mix_LoadWAV(getFilePath("start.voc"));
    game_sounds[2] = Mix_LoadWAV(getFilePath("defence.voc"));
    game_sounds[3] = Mix_LoadWAV(getFilePath("waveFx.voc"));
    game_sounds[4] = Mix_LoadWAV(getFilePath("ko.voc"));
    char tmp[7]="d#.voc" ;
    int i;
    for(i = 1; i < 7; i++)
    {
        tmp[1]=i+'0';
        game_sounds[i+4] = Mix_LoadWAV(getFilePath(tmp));
    }
    tmp[0] = 'c';
    for(i = 11; i < 19; i++)
    {
        tmp[1]=i-10+'0';
        game_sounds[i] = Mix_LoadWAV(getFilePath(tmp));
    }
}

void free_sounds(void)
{
    int i;
    for(i = 0; i < 20; i++)
        Mix_FreeChunk(game_sounds[i]);
    free_char_sounds();
}

void say_voice(int voice)
{
    Mix_PlayChannel(-1, game_sounds[voice], 0);
}

void say_char_voice(int voice, PLAYER *PL, int type)
{
    if(type == KR_voice)
        Mix_PlayChannel(-1, PL->KR_sounds[getSoundIndex(PL->KR_sounds, voice)].sound, 0);
    else if(type == CR_voice)
        Mix_PlayChannel(-1, PL->CR_sounds[getSoundIndex(PL->CR_sounds, voice)].sound, 0);
}

void silent_voice(void)
{
    Mix_HaltChannel(-1);
}

int sound_playing(void)
{
    return Mix_Playing(-1);
}

void play_music(char *filename)
{
    if (game_music != NULL)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(game_music);
    }
    if(sound_on)
        game_music = Mix_LoadMUS(getFilePath(filename));
    Mix_PlayMusic(game_music, -1);
}

void off_music(void)
{
    Mix_HaltMusic();
    Mix_FreeMusic(game_music);
}

int music_playing(void)
{
    return Mix_PlayingMusic();
}

void SF2_SDL_init(void)
{
   SDL_Init(SDL_INIT_VIDEO);
   SDL_WM_SetCaption("SF2",NULL);
   screen = SDL_SetVideoMode(640, 400, 8, SDL_HWPALETTE|SDL_HWSURFACE|SDL_DOUBLEBUF);
   int i;
   for(i = 0; i < 4; i++)
      page[i] = create_surface(320, 200);
   setSF2Palette();
   intit_sf2_keys();
   Mix_Init(0);
   Mix_OpenAudio(8000, MIX_DEFAULT_FORMAT, 2, 1024);
   load_sounds();
}

extern unsigned char *_EBF , *_ECBITMAP;
extern unsigned short *_FCBITMAP, *_MCBITMAP, *_LCBITMAP;
#define MAXIMG 120    // Maximum image Size
extern Image  ImgL[MAXIMG],ImgR[MAXIMG] ;
void freeImg(Image img[],int n);

void SF2_SDL_quit(void)
{
   freeImg(ImgR,MAXIMG) ; freeImg(ImgL,MAXIMG) ;
   free(_EBF); free(_FCBITMAP); free(_MCBITMAP); free(_LCBITMAP); free(_ECBITMAP);
   int i;
   for(i = 0; i < 4; i++)
      SDL_FreeSurface(page[i]);
   if (game_music != NULL)
   {
        Mix_HaltMusic();
        Mix_FreeMusic(game_music);
   }
   free_sounds();
   Mix_CloseAudio();
   Mix_Quit();
   SDL_Quit();
}

void processInput(void)
{
    if(SDL_PollEvent(&event) == 1)
    {
        switch(event.type)
        {
            case SDL_QUIT:
                main_loop = 0;
                break;
            /*case SDL_KEYDOWN:
                 switch(event.key.keysym.sym)
                {
                    case SDLK_1:
                        printf("1\n");
                        break;
                }*/
        }
    }
}

void sleep(void)
{
    static int old_time = 0,  actual_time = 0;
    actual_time = SDL_GetTicks();
    if (actual_time - old_time < 16) // if less than 16 ms has passed
    {
        SDL_Delay(16 - (actual_time - old_time));
        old_time = SDL_GetTicks();
    }
    else
    {
        old_time = actual_time;
    }
}

void sleep_ms(int ms)
{
    static int old_time = 0,  actual_time = 0;
    actual_time = SDL_GetTicks();
    if (actual_time - old_time < ms) // if less than ms has passed
    {
        SDL_Delay(ms - (actual_time - old_time));
        old_time = SDL_GetTicks();
    }
    else
    {
        old_time = actual_time;
    }
}

