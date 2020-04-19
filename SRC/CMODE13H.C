#ifndef __DEFINES_H_
#include "defines.h"
#endif
#ifndef __MODE13H_H_
#include "mode13h.h"
#endif

#include <stdio.h>

byte M13_init(void)
{
 if (!M13_makelfb())
   {
    printf("Could not allocate memory for the LFB!\n");
    return 0;
   }
 M13_setactivepage(VGALFB);
 M13_setmcga();
 M13_clearlfb();
 return 1;
}

void M13_deinit(void)
{
 M13_setactivepage(VGA);
 M13_settext();
 M13_deletelfb();
}
byte M13_getpixel(int16 x,int8 y)
{
 return (M13_getpixel_(curpage,x,y));
}
void M13_putpixel(int16 x,int8 y,byte c)
{
 M13_putpixel_(curpage,x,y,c);
 }
void M13_fillb(byte v,int32 s)
{
 M13_fillb_(curpage,v,s);
}
void M13_fillw(word v,int32 s)
{
M13_fillw_(curpage,v,s);
}
void M13_filld(dword v,int32 s)
{
 M13_filld_(curpage,v,s);
}
void M13_hline(int8 y,int16 x1,int16 x2,byte c)
{
 M13_hline_(curpage,y,x1,x2,c);
}
void M13_vline(int16 x,int8 y1,int8 y2,byte c)
{
M13_vline_(curpage,x,y1,y2,c);
}
void M13_line(int16 x1,int8 y1,int16 x2,int8 y2,byte c)
{
 M13_line_(curpage,x1,y1,x2,y2,c);
 }
void M13_showlfb(void)
{
M13_showbuffer(VGALFB,VGA,_64K);
}
void M13_clearlfb(void)
{
 M13_clearbuffer(VGALFB,_64K);
}
void M13_clearvga(void)
{
 M13_clearbuffer(VGA,_64K);

}

void M13_setactivepage(byte *dest)
{
 curpage = dest;
}

byte M13_makelfb(void)
{
 VGALFB = (byte *) malloc(64000);
 if (VGALFB == NULL) return 0;
 else  return 1;
}

void M13_deletelfb(void)
{
 free(VGALFB);
}

void M13_getcolor(byte cnum,byte *rval,byte *gval,byte *bval)
{
 outp(0x3c7,cnum);
 *rval = inp(0x3c9);
 *gval = inp(0x3c9);
 *bval = inp(0x3c9);
};

void M13_setcolor(byte cnum,byte rval,byte gval,byte bval)
{
 outp(0x3c8,cnum);
 outp(0x3c9,rval);
 outp(0x3c9,gval);
 outp(0x3c9,bval);
};


void M13_preservepalette(void)
{
 int i;
 byte red,green,blue;

 for (i=0;i<256;i++) {
   M13_getcolor(i,&red,&green,&blue);
   pal[i][R] = red;
   pal[i][G] = green;
   pal[i][B] = blue;
 };
};

void M13_setpalette(void)
{
 int i;
 for (i=0;i<256;i++)
	M13_setcolor(i,pal[i][R],pal[i][G],pal[i][B]);
};


void M13_fadein(void)
{
 word i,j;
 byte red,green,blue;

 for (i=0;i<64;i++) {
	M13_waitvretrace();
	for(j=0;j<256;j++) {
		M13_getcolor(i,&red,&green,&blue);
		if (red   < pal[i][R]) red++;
		if (green < pal[i][G]) green++;
		if (blue  < pal[i][B]) blue++;
		M13_setcolor(j,red,green,blue);
	};
 };
};

void M13_blackout(void)
{
 word i,j;
 byte red,green,blue;

 for (i=0;i<64;i++) {
	M13_waitvretrace();
	for(j=0;j<256;j++) {
		M13_getcolor(i,&red,&green,&blue);
		if (red   > 0) red=0;
		if (green > 0) green=0;
		if (blue  > 0) blue=0;
		M13_setcolor(j,red,green,blue);
	};
 };
};

void M13_fadeoutblack(void)
{
 word i,j;
 byte red,green,blue;

 for (i=0;i<64;i++)
    {
	M13_waitvretrace();
	for(j=0;j<256;j++)
	 {
		M13_getcolor(i,&red,&green,&blue);
		if (red   > 0) red--;
		if (green > 0) green--;
		if (blue  > 0) blue--;
		M13_setcolor(j,red,green,blue);
	 };
    };
};

void M13_fadeoutwhite(void)
{
 word i,j;
 byte red,green,blue;

 for (i=0;i<64;i++)
    {
	M13_waitvretrace();
	for(j=0;j<256;j++)
	 {
		M13_getcolor(i,&red,&green,&blue);
		if (red   < 63) red++;
		if (green < 63) green++;
		if (blue  < 63) blue++;
		M13_setcolor(j,red,green,blue);
	 };
    };
};

void M13_fadeoutred(void)
{
 word i,j;
 byte red,green,blue;

 for (i=0;i<64;i++)
    {
	M13_waitvretrace();
	for(j=0;j<256;j++)
	 {
		M13_getcolor(i,&red,&green,&blue);
		if (red   < 63) red++;
		if (green > 0) green--;
		if (blue  > 0) blue--;
		M13_setcolor(j,red,green,blue);
	 };
    };
};

void M13_fadeoutgreen(void)
{
 word i,j;
 byte red,green,blue;

 for (i=0;i<64;i++)
    {
	M13_waitvretrace();
	for(j=0;j<256;j++)
	 {
		M13_getcolor(i,&red,&green,&blue);
		if (red   > 0) red--;
		if (green < 63) green++;
		if (blue  > 0) blue--;
		M13_setcolor(j,red,green,blue);
	};
    };
};

void M13_fadeoutblue(void)
{
 word i,j;
 byte red,green,blue;

 for (i=0;i<64;i++)
    {
	M13_waitvretrace();
	for(j=0;j<256;j++)
	 {
		M13_getcolor(i,&red,&green,&blue);
		if (red   > 0) red--;
		if (green > 0) green--;
		if (blue  < 63) blue++;
		M13_setcolor(j,red,green,blue);
	  };
     };
};

void M13_fadeoutyellow(void)
{
 word i,j;
 byte red,green,blue;

 for (i=0;i<64;i++)
    {
	M13_waitvretrace();
	for(j=0;j<256;j++)
	 {
		M13_getcolor(i,&red,&green,&blue);
		if (red   < 63) red++;
		if (green < 63) green++;
		if (blue  > 0) blue--;
		M13_setcolor(j,red,green,blue);
	  };
     };
};

void M13_fadeoutpurple(void)
{
 word i,j;
 byte red,green,blue;

 for (i=0;i<64;i++)
    {
	M13_waitvretrace();
	for(j=0;j<256;j++)
	 {
		M13_getcolor(i,&red,&green,&blue);
		if (red   < 63) red++;
		if (green > 0) green--;
		if (blue  < 63) blue++;
		M13_setcolor(j,red,green,blue);
	};
    };
};

void M13_fadeoutcyan(void)
{
 word i,j;
 byte red,green,blue;

 for (i=0;i<64;i++)
    {
	M13_waitvretrace();
	for(j=0;j<256;j++)
	 {
		M13_getcolor(i,&red,&green,&blue);
		if (red   > 0) red--;
		if (green < 63) green++;
		if (blue  < 63) blue++;
		M13_setcolor(j,red,green,blue);
	  };
     };
};



void M13_fadetocolor(byte rval,byte gval,byte bval)
{
 word i,j;
 byte red,green,blue;

 for (i=0;i<64;i++)
    {
	M13_waitvretrace();
	for(j=0;j<256;j++)
	 {
		M13_getcolor(i,&red,&green,&blue);
		red   != rval ? (  red > rval ?   red-- :   red++) :   red+=0;
		green != gval ? (green > gval ? green-- : green++) : green+=0;
		blue  != bval ? ( blue > bval ?  blue-- :  blue++) :  blue+=0;
		M13_setcolor(j,red,green,blue);
	 };
    };
};

uint32 time=0;

void __interrupt __far timer(void);
void (__interrupt __far *old_timer_handler)();

void __interrupt __far timer(void)
{
 time++;
}

void TIMER_start(void)
{
 old_timer_handler = _dos_getvect(0x1C);
 _dos_setvect(0x1C,timer);
}

void TIMER_reset(void)
{
 time=0;
}

double TIMER_stop(void)
{
 return (time / 18.2);
}

void TIMER_unload(void)
{
 _dos_setvect(0x1C,old_timer_handler);
}
