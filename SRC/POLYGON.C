#include "affine.h"
#include "defines.h"
#include "polygon.h"
#include "mode13h.h"

void CreateZBuffer(void)
{
  ZBuffer = (int *) malloc(256000);
}

void DeleteZBuffer(void)
{
  free(ZBuffer);
}

void ClearZBuffer(void)
{
 M13_filld_(ZBuffer,0xF0000000,_256K);
}

void ClearEBuffer(void)
{
 M13_filld_(edge,0x00000140,800);
}

void ZFlatLine(int32 y,int32 x0,int32 x1,real z0,real z1,uint8 color)
{
  real loop,zvalue,zadd = 0,ypos;

  zvalue = z0;
  loop = (x1 - x0);
  if (loop != 0) { zadd = ((z1-z0) / loop); }
  if (x1 > 320) x1 = 320;
  if (x0 < 0) {
    zvalue -= (zadd * x0);
    x0 = 0;
  }
  ypos = (y << 8) + (y << 6) + x0;
  for (loop = x0; loop < x1; loop++)
  {
//    if (zvalue > 0)
      if (ZBuffer[ypos] < zvalue)
    	{
      	 ZBuffer[ypos] = zvalue;
      	 VGALFB[ypos] = color;
    	}
    ypos++;
    zvalue += zadd;
  }
}

void ZFillEdgeB(int32 x0,int32 y0,int32 x1,int32 y1,real z0,real z1)
{
  real loop;
  real dx, dinc, px;
  real temp;
  real zadd;
  real zvalue;

  if (y0 != y1)
  {
    if (y1 < y0)
    {
      temp = y1; y1 = y0; y0 = temp;
      temp = x1; x1 = x0; x0 = temp;
    }

    temp = (y1-y0);
    zadd = (z1-z0)  / temp;
    dx = ((x1-x0) << 16) / temp;
    zvalue  = z0;
    dinc = x0 << 16;
    for (loop=y0; loop < y1; loop++)
    {
      if ((loop >= 0) && (loop < 200))
        {
          px = dinc >> 16;

          if (px < edge[loop][0]) {
            edge[loop][0] = px;
            z_edge[loop][0] = zvalue;
          }
          if (px > edge[loop][1]) {
            edge[loop][1] = px;
            z_edge[loop][1] = zvalue; 
          }
        }
      dinc += dx;
      zvalue += zadd;
    }
  }
}

void ZFlatPoly(int32 x0,int32 y0,int32 x1,int32 y1,int32 x2,int32 y2,real z0,real z1,real z2,uint8 color)
{
  int Ymin, Ymax;
  int c;

  Ymin = y0; Ymax =y0;
  if (y1 < Ymin) Ymin = y1;
  if (y1 > Ymax) Ymax = y1;
  if (y2 < Ymin) Ymin = y2;
  if (y2 > Ymax) Ymax = y2;

  if (Ymin < 0) Ymin = 0;
  if (Ymax > 200) Ymax = 200;
  if (Ymax == Ymin) return;

  if ((Ymin < 200) && (Ymax > 0))
  {
    ZFillEdgeB(x0,y0,x1,y1,z0,z1);
    ZFillEdgeB(x1,y1,x2,y2,z1,z2);
    ZFillEdgeB(x2,y2,x0,y0,z2,z0);

    for (c=Ymin; c < Ymax; c++)
      ZFlatLine(c,edge[c][0],edge[c][1],z_edge[c][0],z_edge[c][1], color);
  }
}

void ZGouraudLine(int8 y,int16 x0,int16 x1,int32 z0,int32 z1,real c0,real c1)
{
  real loop,zvalue,zadd,ypos;
  uint32 color,cadd;

  color = c0;
  zvalue = z0;

  loop = (x1 - x0);
  if (loop != 0)
     {
        zadd = ((z1-z0) / loop);
        cadd = ((c1-c0) / loop); 

  if (x1 > 320) x1 = 320;
  if (x0 < 0)
  {
    zvalue -= (zadd * x0);
    color  -= (cadd * x0);
    x0 = 0;
  }

  ypos = (y << 8) + (y << 6) + x0;

  for (loop = x1 - x0; loop > 0; loop--)
  {
    if (ZBuffer[ypos] < zvalue )
    {
      ZBuffer[ypos] = zvalue;
      VGALFB[ypos] = color >> 16;
    }
    ypos++;
    zvalue += zadd;
    color += cadd;
  }
 }
}

void ZGouraudFillEdgeB(int32 x0,int32 y0,int32 x1,int32 y1,int32 z0,int32 z1,real c0,real c1)
{
  real loop;
  real dx, dinc, px;
  real temp;
  real zadd;
  real zvalue;
  real color,cadd;

  if (y0 != y1)
  {
    if (y1 < y0)
    {
      temp = y1; y1 = y0; y0 = temp;
      temp = x1; x1 = x0; x0 = temp;
      temp = z1; z1 = z0; z0 = temp;
      temp = c1; c1 = c0; c0 = temp;
    }

    temp = (y1-y0);
    zadd = (z1-z0) / temp;
    cadd = (c1-c0) / temp;
    dx = ((x1-x0) << 16) / temp;
    zvalue  = z0;
    color = c0;
    dinc = x0 << 16;
    for (loop=y0; loop < y1; loop++)
    {
      if ((loop >= 0) && (loop < 200))
        {
          px = dinc >> 16;
          if (px < edge[loop][0]) {
            edge[loop][0] = px;
            z_edge[loop][0] = zvalue;
            light[loop][0] = color;
          }
          if (px > edge[loop][1]) 
	  {
            edge[loop][1] = px;
            z_edge[loop][1] = zvalue;
            light[loop][1] = color;
          }
        }
      dinc += dx;
      zvalue += zadd;
      color += cadd;
    }
  }
}


void ZGouraudPoly(int32 x0,int32 y0,int32 x1,int32 y1,int32 x2,int32 y2,int32 z0,int32 z1,int32 z2,real c0,real c1,real c2)
{
  int Ymin, Ymax;
  int c;

  Ymin = y0; Ymax =y0;
  if (y1 < Ymin) Ymin = y1;
  if (y1 > Ymax) Ymax = y1;
  if (y2 < Ymin) Ymin = y2;
  if (y2 > Ymax) Ymax = y2;

  if (Ymin < 0) Ymin = 0;
  if (Ymax > 200) Ymax = 200;
  if (Ymax == Ymin) return;

  if ((Ymin < 200) && (Ymax > 0))
  {
    ZGouraudFillEdgeB(x0,y0,x1,y1,z0,z1,c0,c1);
    ZGouraudFillEdgeB(x1,y1,x2,y2,z1,z2,c1,c2);
    ZGouraudFillEdgeB(x2,y2,x0,y0,z2,z0,c2,c0);

    for (c=Ymin; c < Ymax; c++)
      ZGouraudLine(c,edge[c][0],edge[c][1],z_edge[c][0],z_edge[c][1],light[c][0],light[c][1]);
  }
}


void ZPhongLine(int8 y,int16 x0,int16 x1,int32 z0,int32 z1,real a0,real a1)
{
  real loop,zvalue,zadd,ypos;
  real angle,anadd;
  int color;

  angle = a0; // Pochetna vrednost ugla Phong senchenja u FIXP32
  zvalue = z0;

  loop = (x1 - x0);
  if (loop != 0)
     {
        zadd = ((z1-z0) / loop);
        anadd = ((a1-a0) / loop); // inkrement ugla u FIXP32

  if (x1 > 320) x1 = 320;
  if (x0 < 0)
  {
    zvalue -= (zadd * x0);
    angle  -= (anadd * x0);
    x0 = 0;
  }

  ypos = (y << 8) + (y << 6) + x0;

  for (loop = x1 - x0; loop > 0; loop--)
  {
    if (ZBuffer[ypos] < zvalue )
    {
      ZBuffer[ypos] = zvalue;
      color = R2I(mul( COS( R2I(angle) ),16711680));
      if (color < 0) color = 0;
      VGALFB[ypos] = color;
    }
    ypos++;
    zvalue += zadd;
    angle += anadd;
  }
 }
}

void ZPhongFillEdgeB(int32 x0,int32 y0,int32 x1,int32 y1,int32 z0,int32 z1,real a0,real a1)
{
  real loop;
  real dx, dinc, px;
  real temp;
  real zadd;
  real zvalue;
  real angle,anadd;

  if (y0 != y1)
  {
    if (y1 < y0)
    {
      temp = y1; y1 = y0; y0 = temp;
      temp = x1; x1 = x0; x0 = temp;
      temp = z1; z1 = z0; z0 = temp;
      temp = a1; a1 = a0; a0 = temp;
    }

    temp = (y1-y0);
    zadd = (z1-z0)  / temp;
    anadd = (a1-a0) / temp; // Inkrement boje Gouraud senchenja.
    dx = ((x1-x0) << 16) / temp;
    zvalue  = z0;
    angle = a0;
    dinc = x0 << 16;
    for (loop=y0; loop < y1; loop++)
    {
      if ((loop >= 0) && (loop < 200))
        {
          px = dinc >> 16;
          if (px < edge[loop][0]) {
            edge[loop][0] = px;
            z_edge[loop][0] = zvalue;
            a_edge[loop][0] = angle;
          }
          if (px > edge[loop][1])
	  {
            edge[loop][1] = px;
            z_edge[loop][1] = zvalue;
            a_edge[loop][1] = angle;
          }
        }
      dinc += dx;
      zvalue += zadd;
      angle += anadd;
    }
  }
}

void ZPhongPoly(int32 x0,int32 y0,int32 x1,int32 y1,int32 x2,int32 y2,int32 z0,int32 z1,int32 z2,real a0,real a1,real a2)
{
  int Ymin, Ymax;
  int c;

  Ymin = y0; Ymax =y0;
  if (y1 < Ymin) Ymin = y1;
  if (y1 > Ymax) Ymax = y1;
  if (y2 < Ymin) Ymin = y2;
  if (y2 > Ymax) Ymax = y2;

  if (Ymin < 0) Ymin = 0;
  if (Ymax > 200) Ymax = 200;
  if (Ymax == Ymin) return;

  if ((Ymin < 200) && (Ymax > 0))
  {
    ZPhongFillEdgeB(x0,y0,x1,y1,z0,z1,a0,a1);
    ZPhongFillEdgeB(x1,y1,x2,y2,z1,z2,a1,a2);
    ZPhongFillEdgeB(x2,y2,x0,y0,z2,z0,a2,a0);

    for (c=Ymin; c < Ymax; c++)
      ZPhongLine(c,edge[c][0],edge[c][1],z_edge[c][0],z_edge[c][1],a_edge[c][0],a_edge[c][1]);
  }
}

// shininess i sh_strength su 1 => plastika ili metal (metal kad je N blizhe 0)
// za shininess je 1 maksimum,smanjenje istiche specular.
// kad je shininess = 0 ,jedina boja je boja speculara.
// kad je sh_strength < 1 ,boja tamni,specular postaje slabije izrazhen.
// Obratno,kada je sh_strength > 1,objekat je sve sjajniji.
// Za objekte koji nisu plastichni se sjaj smanjuje,pa treba smanjiti
// shininess ili boju specular osvetljenja,shto ce istaci difuznu,tj. prirodnu
// boju objekta pri bilo kom shininess/sh_strength.
