#ifndef __DEFINES_H_
#include "defines.h"
#endif
#ifndef __AFFINE_H_
#include "affine.h"
#endif

#include <math.h>
#include <stdio.h>

void VEC_init(vector v,real i,real j,real k)
{
 v[0] = i;v[1] = j;v[2] = k;
}
void VEC_copy(vector v,vector u)
{
 v[0]=u[0];v[1]=u[1];v[2]=u[2];
}
void VEC_add(vector w,vector u,vector v)
{
 w[0]=u[0]+v[0];w[1]=u[1]+v[1];w[2]=u[2]+v[2];
}
void VEC_sub(vector w,vector u,vector v)
{
 w[0]=u[0]-v[0];w[1]=u[1]-v[1];w[2]=u[2]-v[2];
}
void VEC_neg(vector w)
{
 w[0]=-w[0];w[1]=-w[1];w[2]=-w[2];
}
real VEC_dotp(vector u,vector v)
{
 return (mul(u[0],v[0])+mul(u[1],v[1])+mul(u[2],v[2]));
}
void VEC_crossp(vector w,vector u,vector v)
{
 w[0] = sub(mul(u[1],v[2]),mul(u[2],v[1]));
 w[1] = sub(mul(u[2],v[0]),mul(u[0],v[2]));
 w[2] = sub(mul(u[0],v[1]),mul(u[1],v[0]));
}

void VEC_normalize(vector w)
{
 double x,y,z;
 double len;

 x = R2D(w[0]);
 y = R2D(w[1]);
 z = R2D(w[2]);
 len = sqrt(x*x+y*y+z*z);
 w[0] = D2R(x/len);
 w[1] = D2R(y/len);
 w[2] = D2R(z/len);
};

void VEC_mulmatrix(vector w,vector u,matrix m) // Matrix on the left
{
 w[0] = mul(m[0][0],u[0])+mul(m[0][1],u[1])+mul(m[0][2],u[2]);
 w[1] = mul(m[1][0],u[0])+mul(m[1][1],u[1])+mul(m[1][2],u[2]);
 w[2] = mul(m[2][0],u[0])+mul(m[2][1],u[1])+mul(m[2][2],u[2]);
 
}
void VEC_matrixmul(vector w,vector u,matrix m) // Matrix on the right
{
 w[0] = mul(m[0][0],u[0])+mul(m[1][0],u[1])+mul(m[2][0],u[2]);
 w[1] = mul(m[0][1],u[0])+mul(m[1][1],u[1])+mul(m[2][1],u[2]);
 w[2] = mul(m[0][2],u[0])+mul(m[1][2],u[1])+mul(m[2][2],u[2]);
}

void VEC_rotatex(vector v,real angle)
{
 real tmp;
 tmp = mul(v[1],COS(angle))-mul(v[2],SIN(angle));
 v[2] = mul(v[1],SIN(angle))+mul(v[2],COS(angle));
 v[1] = tmp;
}

void VEC_rotatey(vector v,real angle)
{
 real tmp;
 tmp = mul(v[0],COS(angle))-mul(v[2],SIN(angle));
 v[2] = mul(v[0],SIN(angle))+mul(v[2],COS(angle));
 v[0] = tmp;
}
void VEC_rotatez(vector v,real angle)
{
 real tmp;
 tmp = mul(v[0],COS(angle))-mul(v[1],SIN(angle));
 v[1] = mul(v[0],SIN(angle))+mul(v[1],COS(angle));
 v[0] = tmp;

}

void VEC_rotate(vector v,real xan,real yan,real zan)
{
 real tmp;
 tmp = mul(v[0],COS(zan))-mul(v[1],SIN(zan));
 v[1] = mul(v[0],SIN(zan))+mul(v[1],COS(zan));
 v[0] = tmp;
 tmp = mul(v[1],COS(xan))-mul(v[2],SIN(xan));
 v[2] = mul(v[1],SIN(xan))+mul(v[2],COS(xan));
 v[1] = tmp;
 tmp = mul(v[0],COS(yan))-mul(v[2],SIN(yan));
 v[2] = mul(v[0],SIN(yan))+mul(v[2],COS(yan));
 v[0] = tmp;
}

void VEC_translate(vector v,real dx,real dy,real dz)
{
 v[0] += dx;v[1] += dy;v[2] += dz;
}

void VEC_vtranslate(vector v,vector u)
{
 v[0] += u[0];v[1] += u[1];v[2] += u[2];
}


void VEC_translatex(vector v,real dx)
{
 v[0] += dx;
}
void VEC_translatey(vector v,real dy)
{
 v[1] += dy;
}
void VEC_translatez(vector v,real dz)
{
 v[2] += dz;
}
void VEC_vtranslatex(vector v,vector u)
{
 v[0]+=u[0];
}
void VEC_vtranslatey(vector v,vector u)
{
 v[1]+=u[1];
}
void VEC_vtranslatez(vector v,vector u)
{
 v[2]+=u[2];
}
void VEC_scale(vector v,real x,real y,real z)
{
 v[0] = mul(v[0],x);v[1] = mul(v[1],y);v[2] = mul(v[2],z);
}
void VEC_scalex(vector v,real x)
{
 v[0] = mul(v[0],x);
}
void VEC_scaley(vector v,real x)
{
 v[1] = mul(v[1],x);
}
void VEC_scalez(vector v,real x)
{
 v[2] = mul(v[2],x);
}
void VEC_printf(vector v)
{
 printf("%10.5f  %10.5f  %10.5f\n",R2D(v[0]),R2D(v[1]),R2D(v[2]));
}

void MAT_zero(matrix m)
{
 int i,j;

 for (i=0;i<3;i++)
  for (j=0;j<3;j++)
   m[i][j] = 0;
}

void MAT_identity(matrix m)
{
 m[0][0] = 65536;m[0][1] = 0;m[0][2] = 0;
 m[1][0] = 0;m[1][1] = 65536;m[1][2] = 0;
 m[2][0] = 0;m[2][1] = 0;m[2][2] = 65536;
}

void MAT_transpose(matrix m)
{
 real t;

 t = m[0][1]; m[0][1] = m[1][0]; m[1][0] = t;
 t = m[0][2]; m[0][2] = m[2][0]; m[2][0] = t;
 t = m[1][2]; m[1][2] = m[2][1]; m[2][1] = t;
}

real MAT_det(matrix m)
{
 real det = 0;

 det += mul(mul(m[0][0],m[1][1]),m[2][2]);
 det += mul(mul(m[0][1],m[1][2]),m[2][0]);
 det += mul(mul(m[0][2],m[1][0]),m[2][1]);
 det -= mul(mul(m[2][0],m[1][1]),m[1][2]);
 det -= mul(mul(m[2][1],m[1][2]),m[0][0]);
 det -= mul(mul(m[2][2],m[1][0]),m[0][1]);
 return det;
}

void MAT_adj(matrix n,matrix m)
{
 n[0][0] = mul(m[1][1],m[2][2])-mul(m[2][1],m[1][2]);
 n[0][1] = -(mul(m[0][1],m[2][2])-mul(m[2][1],m[0][2]));
 n[0][2] = mul(m[0][1],m[1][2])-mul(m[1][1],m[0][2]);
 n[1][0] = -(mul(m[1][0],m[2][2])-mul(m[2][0],m[1][2]));
 n[1][1] = mul(m[0][0],m[2][2])-mul(m[2][0],m[0][2]);
 n[1][2] = -(mul(m[0][0],m[1][2])-mul(m[1][0],m[0][2]));
 n[2][0] = mul(m[1][0],m[2][1])-mul(m[2][0],m[1][1]);
 n[2][1] = -(mul(m[0][0],m[2][1])-mul(m[2][0],m[0][1]));
 n[2][2] = mul(m[0][0],m[1][1])-mul(m[1][0],m[0][1]);
}

int MAT_inv(matrix n,matrix m)
{
 real k,d;

 d = MAT_det(m);
 if (!d)
   return -1;
 k = rdiv(I2R(1),d);
 MAT_adj(n,m);
 MAT_scale(n,k);
 return 0;
}

void MAT_init(matrix m,real a00,real a01,real a02,
                       real a10,real a11,real a12,
                       real a20,real a21,real a22)
{
 m[0][0] = a00; m[0][1] = a01; m[0][2] = a02;
 m[1][0] = a10; m[1][1] = a11; m[1][2] = a12;
 m[2][0] = a20; m[2][1] = a21; m[2][2] = a22;
}

void MAT_copy(matrix n,matrix m)
{
 n[0][0] = m[0][0]; n[0][1] = m[0][1]; n[0][2] = m[0][2];
 n[1][0] = m[1][0]; n[1][1] = m[1][1]; n[1][2] = m[1][2];
 n[2][0] = m[2][0]; n[2][1] = m[2][1]; n[2][2] = m[2][2];
}
void MAT_initv(matrix m,vector u,vector v,vector w)
{
 m[0][0] = u[0];m[0][1] = u[1];m[0][2] = u[2];
 m[1][0] = v[0];m[1][1] = v[1];m[1][2] = v[2];
 m[2][0] = w[0];m[2][1] = w[1];m[2][2] = w[2];
}

void MAT_add(matrix p,matrix m,matrix n)
{
  p[0][0] = m[0][0] + n[0][0];
  p[0][1] = m[0][1] + n[0][1];
  p[0][2] = m[0][2] + n[0][2];
  p[1][0] = m[1][0] + n[1][0];
  p[1][1] = m[1][1] + n[1][1];
  p[1][2] = m[1][2] + n[1][2];
  p[2][0] = m[2][0] + n[2][0];
  p[2][1] = m[2][1] + n[2][1];
  p[2][2] = m[2][2] + n[2][2];
}

void MAT_sub(matrix p,matrix m,matrix n)
{
  p[0][0] = m[0][0] - n[0][0];
  p[0][1] = m[0][1] - n[0][1];
  p[0][2] = m[0][2] - n[0][2];
  p[1][0] = m[1][0] - n[1][0];
  p[1][1] = m[1][1] - n[1][1];
  p[1][2] = m[1][2] - n[1][2];
  p[2][0] = m[2][0] - n[2][0];
  p[2][1] = m[2][1] - n[2][1];
  p[2][2] = m[2][2] - n[2][2];
}

void MAT_mul(matrix p,matrix m,matrix n)
{
  p[0][0] = mul(m[0][0],n[0][0])+mul(m[0][1],n[1][0])+mul(m[0][2],n[2][0]);
  p[0][1] = mul(m[0][0],n[0][1])+mul(m[0][1],n[1][1])+mul(m[0][2],n[2][1]);
  p[0][2] = mul(m[0][0],n[0][2])+mul(m[0][1],n[1][2])+mul(m[0][2],n[2][2]);
  p[1][0] = mul(m[1][0],n[0][0])+mul(m[1][1],n[1][0])+mul(m[1][2],n[2][0]);
  p[1][1] = mul(m[1][0],n[0][1])+mul(m[1][1],n[1][1])+mul(m[1][2],n[2][1]);
  p[1][2] = mul(m[1][0],n[0][2])+mul(m[1][1],n[1][2])+mul(m[1][2],n[2][2]);
  p[2][0] = mul(m[2][0],n[0][0])+mul(m[2][1],n[1][0])+mul(m[2][2],n[2][0]);
  p[2][1] = mul(m[2][0],n[0][1])+mul(m[2][1],n[1][1])+mul(m[2][2],n[2][1]);
  p[2][2] = mul(m[2][0],n[0][2])+mul(m[2][1],n[1][2])+mul(m[2][2],n[2][2]);
}

void MAT_scale(matrix m,real x)
{
  m[0][0] = mul(m[0][0],x);
  m[0][1] = mul(m[0][1],x);
  m[0][2] = mul(m[0][2],x);
  m[1][0] = mul(m[1][0],x);
  m[1][1] = mul(m[1][1],x);
  m[1][2] = mul(m[1][2],x);
  m[2][0] = mul(m[2][0],x);
  m[2][1] = mul(m[2][1],x);
  m[2][2] = mul(m[2][2],x);
}

void MAT_scalex(matrix m,real x)
{
 m[0][0] = mul(m[0][0],x);
 m[1][0] = mul(m[1][0],x);
 m[2][0] = mul(m[2][0],x);
}
void MAT_scaley(matrix m,real x)
{
 m[0][1] = mul(m[0][1],x);
 m[1][1] = mul(m[1][1],x);
 m[2][1] = mul(m[2][1],x);
}
void MAT_scalez(matrix m,real x)
{
 m[0][2] = mul(m[0][2],x);
 m[1][2] = mul(m[1][2],x);
 m[2][2] = mul(m[2][2],x);
}
void MAT_rotatex(matrix m,real angle)
{
 real tmp;
 tmp = mul(m[0][1],COS(angle))-mul(m[0][2],SIN(angle));
 m[0][2] = mul(m[0][1],SIN(angle))+mul(m[0][2],COS(angle));
 m[0][1] = tmp;
 tmp = mul(m[1][1],COS(angle))-mul(m[1][2],SIN(angle));
 m[1][2] = mul(m[1][1],SIN(angle))+mul(m[1][2],COS(angle));
 m[1][1] = tmp;
 tmp = mul(m[2][1],COS(angle))-mul(m[2][2],SIN(angle));
 m[2][2] = mul(m[2][1],SIN(angle))+mul(m[2][2],COS(angle));
 m[2][1] = tmp;
}
void MAT_rotatey(matrix m,real angle)
{
 real tmp;
 tmp = mul(m[0][0],COS(angle))-mul(m[0][2],SIN(angle));
 m[0][2] = mul(m[0][0],SIN(angle))+mul(m[0][2],COS(angle));
 m[0][0] = tmp;
 tmp = mul(m[1][0],COS(angle))-mul(m[1][2],SIN(angle));
 m[1][2] = mul(m[1][0],SIN(angle))+mul(m[1][2],COS(angle));
 m[1][0] = tmp;
 tmp = mul(m[2][0],COS(angle))-mul(m[2][2],SIN(angle));
 m[2][2] = mul(m[2][0],SIN(angle))+mul(m[2][2],COS(angle));
 m[2][0] = tmp;
}

void MAT_rotatez(matrix m,real angle)
{
 real tmp;
 tmp = mul(m[0][0],COS(angle))-mul(m[0][1],SIN(angle));
 m[0][1] = mul(m[0][0],SIN(angle))+mul(m[0][1],COS(angle));
 m[0][0] = tmp;
 tmp = mul(m[1][0],COS(angle))-mul(m[1][1],SIN(angle));
 m[1][1] = mul(m[1][0],SIN(angle))+mul(m[1][1],COS(angle));
 m[1][0] = tmp;
 tmp = mul(m[2][0],COS(angle))-mul(m[2][1],SIN(angle));
 m[2][1] = mul(m[2][0],SIN(angle))+mul(m[2][1],COS(angle));
 m[2][0] = tmp;
}

void MAT_rotate(matrix m,real xan,real yan,real zan)
{
 real tmp;

 tmp = mul(m[0][0],COS(zan))-mul(m[0][1],SIN(zan));
 m[0][1] = mul(m[0][0],SIN(zan))+mul(m[0][1],COS(zan));
 m[0][0] = tmp;
 tmp = mul(m[1][0],COS(zan))-mul(m[1][1],SIN(zan));
 m[1][1] = mul(m[1][0],SIN(zan))+mul(m[1][1],COS(zan));
 m[1][0] = tmp;
 tmp = mul(m[2][0],COS(zan))-mul(m[2][1],SIN(zan));
 m[2][1] = mul(m[2][0],SIN(zan))+mul(m[2][1],COS(zan));
 m[2][0] = tmp;

 tmp = mul(m[0][1],COS(xan))-mul(m[0][2],SIN(xan));
 m[0][2] = mul(m[0][1],SIN(xan))+mul(m[0][2],COS(xan));
 m[0][1] = tmp;
 tmp = mul(m[1][1],COS(xan))-mul(m[1][2],SIN(xan));
 m[1][2] = mul(m[1][1],SIN(xan))+mul(m[1][2],COS(xan));
 m[1][1] = tmp;
 tmp = mul(m[2][1],COS(xan))-mul(m[2][2],SIN(xan));
 m[2][2] = mul(m[2][1],SIN(xan))+mul(m[2][2],COS(xan));
 m[2][1] = tmp;

 tmp = mul(m[0][0],COS(yan))-mul(m[0][2],SIN(yan));
 m[0][2] = mul(m[0][0],SIN(yan))+mul(m[0][2],COS(yan));
 m[0][0] = tmp;
 tmp = mul(m[1][0],COS(yan))-mul(m[1][2],SIN(yan));
 m[1][2] = mul(m[1][0],SIN(yan))+mul(m[1][2],COS(yan));
 m[1][0] = tmp;
 tmp = mul(m[2][0],COS(yan))-mul(m[2][2],SIN(yan));
 m[2][2] = mul(m[2][0],SIN(yan))+mul(m[2][2],COS(yan));
 m[2][0] = tmp;
}

void MAT_translate(matrix m,real dx,real dy,real dz)
{
 m[0][0] += dx; m[0][1] += dy; m[0][2] += dz;
 m[1][0] += dx; m[1][1] += dy; m[1][2] += dz;
 m[2][0] += dx; m[2][1] += dy; m[2][2] += dz;
}
void MAT_translatex(matrix m,real dx)
{
 m[0][0] += dx;m[1][0] += dx;m[2][0] +=dx;
}
void MAT_translatey(matrix m,real dy)
{
 m[0][1] += dy;m[1][1] += dy;m[2][1] +=dy;
}
void MAT_translatez(matrix m,real dz)
{
 m[0][2] += dz;m[1][2] += dz;m[2][2] +=dz;
}
void MAT_printf(matrix m)
{
 int i;

 for (i=0;i<3;i++) 
  printf("\n %10.5f  %10.5f  %10.5f",R2D(m[i][0]),R2D(m[i][1]),R2D(m[i][2]));
 printf("\n");
}

void XFORM_transform(vector v,vector u,xform *m)
{
 VEC_mulmatrix(v,u,m->m);
 VEC_vtranslate(v,m->v);
}
int XFORM_inverse(xform *n,xform *m)
{
 if (MAT_inv(n->m,m->m)) return -1;

 VEC_mulmatrix(n->v,m->v,n->m);
 VEC_neg(n->v);
 return 0;
}
void XFORM_combine(xform *p,xform *n,xform *m)
{
 MAT_mul(p->m,n->m,m->m);
 VEC_mulmatrix(p->v,m->v,n->m);
 VEC_vtranslate(p->v,n->v);
}

void XFORM_copy(xform *p,xform *m)
{
 MAT_copy(p->m,m->m);
 VEC_copy(p->v,m->v);
}

void MAT_initrot(matrix m,real xan,real yan,real zan)
{
 real c1 = COS(xan), s1 = SIN(xan),
      c2 = COS(yan), s2 = SIN(yan),
      c3 = COS(zan), s3 = SIN(zan);
 
 MAT_init(m,mul(c2,c3)-mul(mul(s1,s2),s3),-mul(c2,s3)-mul(mul(s1,s2),c3),-mul(s2,c1),
	    mul(c1,s3),mul(c1,c3),-s1,
	    mul(s2,c3)+mul(c2,mul(s1,s3)),mul(mul(c2,s1),c3)-mul(s2,c3),mul(c1,c2));
}