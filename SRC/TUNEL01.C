#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <time.h>

#include "mydefs.h"

WORD lut[64000];
BYTE *VGAA=0xa0000;

BYTE scbuff[320*200];

BYTE texture[256*256];


/*
  Pravi se tekstura koja ce biti nalepljena na tunel
*/
void maketexture(void)
{
 int i,j,x,y,p;

 outp(0x3c8,0);
 for (i=0; i<256; i++)
   {
    outp(0x3c9,i/4);
    outp(0x3c9,i/4);
    outp(0x3c9,i/4);
   }

 for (i=0; i<65536; i++)
   texture[i]=0;
/*
 for (i=0; i<65536; i++)
   {
    x=i/256;
    y=i%256;

    if ((x%8)==0) texture[i]=255;
    if ((y%8)==0) texture[i]=255;
   }
*/

 srand(clock());
 for (j=0; j<5; j++)
   {
    x=random(256); y=random(256);

    for (i=0; i<200000; i++)
      {
       p=random(4);
       switch (p)
         {
          case 0: x--; if (x<0) x=255;
                  break;
          case 1: y--; if (y<0) y=255;
                  break;
          case 2: x++; if (x>255) x=0;
                  break;
          case 3: y++; if (y>255) y=0;
                  break;
         }
       texture[256*y+x]+=8;
      }
   }

  /* blurovanje teksture */
  for ( j=0; j<5; j++ )
    for ( x=0; x<256*256; x+=256 )
      for ( y=0; y<256; y++ )
      {
  	      texture[x+y]=(texture[((x+256)&0xFF00)+y]+
  	                    texture[x+((y+1)&0xFF)]    +
		                 texture[((x-256)&0xFF00)+y]+
		                 texture[x+((y-1)&0xFF)]     ) >> 2;
      }
}

/*

 izracunavanje lookup tabele za tunel.
 za svaku tacku na ekranu racuna se njena adresa u texturi. na koju foru??

                        A
                        |y
                        |
                        |
                        +oko
                  \     |     /
                   \    |    /K
                    \   |   /
                     \  |  /
                      \ | /
                       \|/       x
             --------------------->
                       O(0,0)

Oko se nalazi na y osi i gleda ka tacki O. Zamislita sada K zavrti oko y ose
i formira konus. Koordinata na nekom y preseku konusa ce biti x koordinata
u teksturi, a y koordinata ce biti y na teksturi.

*/

#define SMALL               1e-2
#define not_small(a)        ( ( (a)<-SMALL ) || (SMALL <(a)) )
#define M_SQRT2             1.41421356237309504880

//     xr = ((x/320.0) - 0.5 )*2.0;      // xr=-1..1
//     yr = ((y/200.0) - 0.5 )*2.0;      // yr=-1..1

void CalcTunel1(void )
 {

  int x,y;


  for( y=0; y<200; y++ )
   for( x=0; x<320; x++ )
    {
     double r,f;
     double xr, yr;
     WORD index;
     WORD ix, iy;

     xr=((x-160)/100.0);
     yr=((y-100)/100.0);

     if( not_small( xr ) || not_small( yr ) )
       f = (atan2( yr, xr )/3.141592653589+1.0)/2.0;
     else
       f=0.0;

     r = sqrt( xr*xr + yr*yr)/M_SQRT2;
     if (r>0.02)
      {
        r = 0.5/(r+0.001);
//        r = r/(r+0.1);
        ix =r * 64;
        iy =f * 256;

        index = iy*256 + ix;
      }
     else index=0;

     if( index < 65536 )
       lut[ y * 320 +  x ]  = index;
    }
 }

/*

glavna petlja tunela. Postu se u nizu lut nalazi adresa svake tacke
ekrana u texturi, jednostavno se prvo procita vrednost u lut tabeli
i taj broj je index u texturi. tada se iz teksture procita boja
i pukne u ekransku bafer. Kada se sve zavrsi, ekranski bafer se
kopira u video memoriju.

*/

void anim (int speed)
{
 int i;
 WORD *tt=lut;


 outp(0x3c8,0); outp(0x3c9,63);outp(0x3c9,0);outp(0x3c9,0); // rastermetar
 i=63999;
 while (i>=0) *(scbuff+i--)=*(texture+ ((*tt++)+=speed) );
/* ovo je malko citkija varijanta
   {
     *(scbuff+i)=*(texture+*tt);
     (*tt)+=speed;  tt++;
     i--;
   }
*/
 outp(0x3c8,0); outp(0x3c9,0);outp(0x3c9,0);outp(0x3c9,0); // rastermetar

 waitvr();
 memcpy(VGAA, scbuff, 64000);
}

void main (void)
{
 int speed=2,c;

 vmode(0x13);

 maketexture();
 CalcTunel1();

l1:

 while (!kbhit()) anim(speed);
 c=getch();
 switch(c)
  {
    case '+': speed++; break;
    case '-': speed--; break;
    case 27: goto kraj;
  }
 goto l1;

kraj:
 vmode(0x03);
}

