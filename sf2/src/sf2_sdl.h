#ifndef SF2_SDL_H
#define SF2_SDL_H

#include <SDL/SDL.h>
#include <SDL_mixer.h>

extern SDL_Surface *screen;
extern SDL_Surface *page[4];
extern SDL_Surface *active_page;
extern SDL_Color SF2Palette[256];
extern SDL_Color SF2FadePalette[256];
extern unsigned char background[1000*168];

void setSF2Palette(void);
void setBrightness(int factor);
SDL_Surface *create_surface(int width, int height);
void blit_raw(SDL_Surface *dst, int x_pos, int y_pos, unsigned char *p, int xcs, int xs, int ys);
void blit_raw_flipped(SDL_Surface *dst, int x_pos, int y_pos, unsigned char *p, int xcs, int xs, int ys, int flip);
void blit_scaled_X2(SDL_Surface *dst, int x_pos, int y_pos, unsigned char *p, int xcs, int xs, int ys);
void blit_RLE(SDL_Surface *dst, unsigned char *src,unsigned int length, int x_pos, int y_pos, int xs, int ys);
void blit_bitmap(SDL_Surface *dst, int x_pos, int y_pos, unsigned char *p, int xs, int ys, unsigned char color);
void blit_bitmap_hangul(SDL_Surface *dst, int x_pos, int y_pos, unsigned short *p, int xs, int ys, unsigned char color);
extern unsigned char *_ECBITMAP ;
void print_font(unsigned char *_ECBITMAP);
void print_logo(void);
void blit_logo(unsigned char *_ECBITMAP);
void viewPage_offset(int x, int y, int p);
void image_fill(SDL_Surface *dst, int x, int y, int xs, int ys, int color);
void RLE_decoding(unsigned char *src,unsigned char *dst,unsigned int length, int sz);
void intit_sf2_keys(void);
int read_keyboard(void);
void load_sounds(void);
void free_sounds(void);
void say_voice(int voice);
void silent_voice(void);
int sound_playing(void);
void play_music(char *filename);
void off_music(void);
int music_playing(void);
char *getFilePath(char *filename);
void set_data_folder(void);
void load_config(void);
void SF2_SDL_init(void);
void SF2_SDL_quit(void);
void processInput(void);
void sleep(void);
void sleep_ms(int ms);

#define plane_voc 0
#define start_voc 1
#define defence_voc 2
#define waveFx_voc 3
#define ko_voc 4
#define dmg_voc 5
#define country_voc 11
#define KR_voice 0
#define CR_voice 1

#endif // SF2_SDL_H
