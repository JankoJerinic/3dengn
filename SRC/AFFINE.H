#ifndef __AFFINE_H_
#define __AFFINE_H_

#ifndef __DEFINES_H_
#include "defines.h"
#endif

typedef real vector[3];
typedef vector matrix[3];

typedef struct {
	matrix m;
 	vector v;
	} xform;


void VEC_init(vector v,real i,real j,real k);
void VEC_copy(vector v,vector u);
void VEC_add(vector w,vector u,vector v);
void VEC_sub(vector w,vector u,vector v);
void VEC_neg(vector w);
real VEC_dotp(vector u,vector v);
void VEC_crossp(vector w,vector u,vector v);
void VEC_normalize(vector w);
void VEC_mulmatrix(vector w,vector u,matrix m);
void VEC_matrixmul(vector w,matrix m,vector u);
void VEC_rotatex(vector v,real angle);
void VEC_rotatey(vector v,real angle);
void VEC_rotatez(vector v,real angle);
void VEC_rotate(vector v,real anx,real any,real anz);
void VEC_translate(vector v,real dx,real dy,real dz);
void VEC_vtranslate(vector v,vector u);
void VEC_translatex(vector v,real dx);
void VEC_translatey(vector v,real dy);
void VEC_translatez(vector v,real dz);
void VEC_vtranslatex(vector v,vector u);
void VEC_vtranslatey(vector v,vector u);
void VEC_vtranslatez(vector v,vector u);
void VEC_scale(vector v,real x,real y,real z);
void VEC_scalex(vector v,real x);
void VEC_scaley(vector v,real x);
void VEC_scalez(vector v,real x);
void VEC_printf(vector v);


void MAT_zero(matrix m);
void MAT_identity(matrix m);
void MAT_transpose(matrix m);
real MAT_det(matrix m);
void MAT_adj(matrix n,matrix m);
int MAT_inv(matrix n,matrix m);
void MAT_init(matrix m,real a11,real a12,real a13,real a21,real a22,real a23,real a31,real a32,real a33);
void MAT_initv(matrix m,vector u,vector v,vector w);
void MAT_copy(matrix n,matrix m);
void MAT_add(matrix p,matrix m,matrix n);
void MAT_sub(matrix p,matrix m,matrix n);
void MAT_mul(matrix p,matrix m,matrix n);
void MAT_rotatex(matrix m,real angle);
void MAT_rotatey(matrix m,real angle);
void MAT_rotatez(matrix m,real angle);
void MAT_rotate(matrix m,real xan,real yan,real zan);
void MAT_translate(matrix m,real dx,real dy,real dz);
void MAT_translatex(matrix m,real dx);
void MAT_translatey(matrix m,real dy);
void MAT_translatez(matrix m,real dz);
void MAT_scale(matrix m,real x);
void MAT_scalex(matrix m,real x);
void MAT_scaley(matrix m,real x);
void MAT_scalez(matrix m,real x);
void MAT_printf(matrix m);

void XFORM_transform(vector v,vector u,xform *m);
int  XFORM_inverse(xform *n,xform *m);
void XFORM_combine(xform *p,xform *n,xform *m);
void XFORM_copy(xform *p,xform *m);

#define MAT_initrotx(m,_xan) MAT_init(m,65536,0,0,0,COS(_xan),-SIN(_xan),0,SIN(_xan),COS(_xan))
#define MAT_initroty(m,_yan) MAT_init(m,COS(_yan),0,-SIN(_yan),0,65536,0,SIN(_yan),0,COS(_yan))
#define MAT_initrotz(m,_zan) MAT_init(m,COS(_zan),-SIN(_zan),0,SIN(_zan),COS(_zan),0,0,0,65536)
void MAT_initrot(matrix m,real xan,real yan,real zan);	
#endif
