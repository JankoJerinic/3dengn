#ifndef __POLYGON_H_
#define __POLYGON_H_

#include "affine.h"
#include "defines.h"
#include "mode13h.h"

real z_edge[200][2];
real a_edge[200][2];
short edge[200][2];
real light[200][2];
vector normals[200][2];
int *ZBuffer;

void CreateZBuffer(void);
void DeleteZBuffer(void);
void ClearZBuffer(void);
void ClearEBuffer(void);

void ZFlatLine(int32 y,int32 x0,int32 x1,int32 z0,int32 z1,uint8 color);
void ZFillEdgeB(int32 x0,int32 y0,int32 x1,int32 y1,int32 z0,int32 z1);
void ZFlatPoly(int32 x0,int32 y0,int32 x1,int32 y1,int32 x2,int32 y2,int32 z0,int32 z1,int32 z2,uint8 color);

void ZGouraudLine(int8 y,int16 x0,int16 x1,int32 z0,int32 z1,real c0,real c1);
void ZGouraudFillEdgeB(int32 x0,int32 y0,int32 x1,int32 y1,int32 z0,int32 z1,real c0,real c1);
void ZGouraudPoly(int32 x0,int32 y0,int32 x1,int32 y1,int32 x2,int32 y2,int32 z0,int32 z1,int32 z2,real c0,real c1,real c2);

void ZPhongLine(int8 y,int16 x0,int16 x1,int32 z0,int32 z1,real a0,real a1);
void ZPhongFillEdgeB(int32 x0,int32 y0,int32 x1,int32 y1,int32 z0,int32 z1,real a0,real a1);
void ZPhongPoly(int32 x0,int32 y0,int32 x1,int32 y1,int32 x2,int32 y2,int32 z0,int32 z1,int32 z2,real a0,real a1,real a2);

#endif


