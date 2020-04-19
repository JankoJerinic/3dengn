#ifndef __MODE13H_H
#define __MODE13H_H

#include "defines.h"

#define _WIDTH		320
#define	_HEIGHT		200
#define _HALFWIDTH    	160
#define _HALFHEIGHT	100
#define _MAXX		_WIDTH
#define _MAXY		_HEIGHT
#define _MAXC		255
#define R		0
#define G		1
#define B		2
#define rndx		random(319)
#define rndy		random(199)
#define rndc		random(255)

byte *VGA = 0xA0000;
byte *VGALFB;
byte *curpage;
byte pal[256][3];

void M13_setactivepage(byte *dest);
byte M13_makelfb(void);
void M13_deletelfb(void);
void M13_getcolor(byte cnum,byte *rval,byte *gval,byte *bval);
void M13_setcolor(byte cnum,byte rval,byte gval,byte bval);
void M13_preservepalette(void);
void M13_setpalette(void);
void M13_fadein(void);
void M13_blackout(void);
void M13_fadeoutblack(void);
void M13_fadeoutwhite(void);
void M13_fadeoutred(void);
void M13_fadeoutgreen(void);
void M13_fadeoutblue(void);
void M13_fadeoutyellow(void);
void M13_fadeoutpurple(void);
void M13_fadeoutcyan(void);
void M13_fadetocolor(byte rval,byte gval,byte bval);

void M13_setvmode(int8 mode);
#pragma aux M13_setvmode = parm [AX] modify [AX];

void M13_setmcga(void);
#pragma aux M13_setmcga modify [AX];

void M13_settext(void);
#pragma aux M13_settext modify [AX];

void M13_clrscr(void);
#pragma aux M13_ClrScr modify [EAX ECX EDI];

void M13_fillb_(void *dest,byte value,int32 size);
#pragma aux M13_fillb_ parm [EDI][EAX][ECX] modify [ECX];

void M13_fillw_(void *dest,word value,int32 size);
#pragma aux M13_fillw_ parm [EDI][EAX][ECX] modify [ECX];

void M13_filld_(void *dest,dword value,int32 size);
#pragma aux M13_filld_ parm [EDI][EAX][ECX] modify [ECX];

void M13_putpixel_(void *dest,int16 x,int8 y,byte color);
#pragma aux M13_putpixel_ parm [EDI][EAX][EBX][EDX];

byte M13_getpixel_(void *dest,int16 x,int8 y);
#pragma aux M13_getpixel_ parm [EDI][EAX][EBX] value [DL];

void M13_waitvretrace(void);
#pragma aux M13_waitvretrace modify [DX AL];

void M13_hline_(void *dest,int8 y,int16 x1,int16 x2,byte color);
#pragma aux M13_hline_ parm [EDI][EBX][EDX][ECX][EAX] modify [EAX EBX ECX EDX EDI];

void M13_vline_(void *dest,int16 x,int8 y1,int8 y2,byte color);
#pragma aux M13_vline_ parm [EDI][EAX][EBX][ECX][EDX] modify [EAX EBX ECX EDX EDI];

void M13_line_(void *dest,int16 x1,int16 y1,int16 x2,int16 y2,byte color);
#pragma aux M13_line_ parm [EDI][EAX][ECX][EBX][ESI][EDX] modify [EAX EBX ECX EDX];

void M13_clearbuffer(void *dest,dword size);
#pragma aux M13_clearbuffer parm [EDI][ECX] modify [EAX];

void M13_showbuffer(void *source,void *dest,dword size);
#pragma aux M13_showbuffer parm [ESI][EDI][ECX] modify [ECX ESI EDI];

byte M13_getesc(void);
#pragma aux M13_getesc modify [AL] value[AL];

byte M13_keypressed(void);
#pragma aux M13_keypressed modify [AX DI] value [AL];

byte M13_getpixel(int16 x,int8 y);
void M13_putpixel(int16 x,int8 y,byte c);
void M13_fillb(byte v,int32 s);
void M13_fillw(word v,int32 s);
void M13_filld(dword v,int32 s);
void M13_hline(int8 y,int16 x1,int16 x2,byte c);
void M13_vline(int16 x,int8 y1,int8 y2,byte c);
void M13_line(int16 x1,int8 y1,int16 x2,int8 y2,byte c);
void M13_showlfb(void);
void M13_clearlfb(void);
void M13_clearvga(void);
byte M13_init(void);
void M13_deinit(void);

void   TIMER_start(void);
void   TIMER_reset(void);
double TIMER_stop(void);
void   TIMER_unload(void);

#endif
