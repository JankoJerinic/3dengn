#include "affine.h"
#include "camera.h"
#include <string.h>
#include <stdlib.h>


void CAMERA_defaultinit(void)
{
 defaultcam = (camera *) malloc (sizeof (camera));
 defaultcam->worldcamera = (xform *) malloc (sizeof (xform));
 defaultcam->cameraworld = (xform *) malloc (sizeof (xform));
 defaultcam->name = "DefaultCamera";
 defaultcam->aspect = D2R(1.6);
 defaultcam->FOV = 45;
 defaultcam->maxwdth = 320;
 defaultcam->maxhght = 200;
 defaultcam->kx = I2R(-386);
 defaultcam->ky = I2R(-386);
 VEC_init(defaultcam->x,I2R(1),0,0);
 VEC_init(defaultcam->y,0,I2R(1),0);
 VEC_init(defaultcam->z,0,0,I2R(1));
 MAT_identity(defaultcam->worldcamera->m);
 VEC_init(defaultcam->worldcamera->v,0,0,0);
 MAT_identity(defaultcam->cameraworld->m);
 VEC_init(defaultcam->cameraworld->v,0,0,0);
}

void CAMERA_defaultcalckx(real FOV,real MAXWDTH)
{
 real FOVcot = rdiv(COS(FOV/2),SIN(FOV/2));

 defaultcam->FOV = FOV;
 defaultcam->maxwdth = MAXWDTH;
 defaultcam->kx = -mul(I2R(MAXWDTH/2),FOVcot);
}

void CAMERA_defaultcalcky(real FOV,real MAXHGHT,real aspect)
{
 real FOVcot = rdiv(COS(FOV/2),SIN(FOV/2));

 defaultcam->FOV = FOV;
 defaultcam->maxhght = MAXHGHT;
 defaultcam->aspect = aspect;
 defaultcam->ky = -mul(mul(I2R(MAXHGHT/2),FOVcot),aspect);
}

void CAMERA_setdefault(camera *cam)
{
 memcpy(&olddefaultcam,&defaultcam,sizeof(camera));
 memcpy(&defaultcam,&cam,sizeof(camera));
}

void CAMERA_alignvertex(vertex *v)
{
 XFORM_transform(v->aligned,v->position,defaultcam->cameraworld);
}
void CAMERA_projectvertex(vertex *v)
{
 v->x = 160 + R2I(mul(defaultcam->kx,rdiv(v->aligned[0],v->aligned[2])));
 v->y = 100 - R2I(mul(defaultcam->ky,rdiv(v->aligned[1],v->aligned[2])));
}

void CAMERA_alignobject(object *o)
{
 uint32 i;
 
 for (i=0;i<o->verts->vcount;i++)
  CAMERA_alignvertex(&o->verts->verts[i]);
}

void CAMERA_projectobject(object *o)
{
 uint32 i;
 
 for (i=0;i<o->verts->vcount;i++)
  CAMERA_projectvertex(&o->verts->verts[i]);
}

void CAMERA_translatex(real dx)
{
 vector v;
 
 VEC_translatex(defaultcam->worldcamera->v,dx);
 VEC_copy(v,defaultcam->worldcamera->v);
// VEC_neg(v);
 VEC_mulmatrix(defaultcam->cameraworld->v,v,defaultcam->cameraworld->m);
}
void CAMERA_translatey(real dy)
{
 vector v;
 
 VEC_translatey(defaultcam->worldcamera->v,dy);
 VEC_copy(v,defaultcam->worldcamera->v);
// VEC_neg(v);
 VEC_mulmatrix(defaultcam->cameraworld->v,v,defaultcam->cameraworld->m);
}
void CAMERA_translatez(real dz)
{
 vector v;

 VEC_translatez(defaultcam->worldcamera->v,dz);
 VEC_copy(v,defaultcam->worldcamera->v);
// VEC_neg(v);
 VEC_mulmatrix(defaultcam->cameraworld->v,v,defaultcam->cameraworld->m);
}
void CAMERA_translate(real dx,real dy,real dz)
{
 vector v;
 
 VEC_translate(defaultcam->worldcamera->v,dx,dy,dz);
 VEC_copy(v,defaultcam->worldcamera->v);
// VEC_neg(v);
 VEC_mulmatrix(defaultcam->cameraworld->v,v,defaultcam->cameraworld->m);
}

void CAMERA_vtranslate(vector v)
{
 vector u;

 VEC_vtranslate(defaultcam->worldcamera->v,v);
 VEC_copy(v,defaultcam->worldcamera->v);
 VEC_neg(v);
 VEC_mulmatrix(defaultcam->cameraworld->v,v,defaultcam->cameraworld->m);
}

void CAMERA_rotatex(real angle)
{
 matrix rotx,newm;

 VEC_rotatex(defaultcam->x,angle);
 VEC_rotatex(defaultcam->y,angle);
 VEC_rotatex(defaultcam->z,angle);
 MAT_initrotx(rotx,angle);
 MAT_mul(newm,rotx,defaultcam->worldcamera->m);
 MAT_copy(defaultcam->worldcamera->m,newm);
 XFORM_inverse(defaultcam->cameraworld,defaultcam->worldcamera);
}
void CAMERA_rotatey(real angle)
{
 matrix roty,newm;

 VEC_rotatey(defaultcam->x,angle);
 VEC_rotatey(defaultcam->y,angle);
 VEC_rotatey(defaultcam->z,angle);
 MAT_initroty(roty,angle);
 MAT_mul(newm,roty,defaultcam->worldcamera->m);
 MAT_copy(defaultcam->worldcamera->m,newm);
 XFORM_inverse(defaultcam->cameraworld,defaultcam->worldcamera);

}
void CAMERA_rotatez(real angle)
{
 matrix rotz,newm;

 VEC_rotatez(defaultcam->x,angle);
 VEC_rotatez(defaultcam->y,angle);
 VEC_rotatez(defaultcam->z,angle);
 MAT_initrotz(rotz,angle);
 MAT_mul(newm,rotz,defaultcam->worldcamera->m);
 MAT_copy(defaultcam->worldcamera->m,newm);
 XFORM_inverse(defaultcam->cameraworld,defaultcam->worldcamera);
}

void CAMERA_rotate(real anx,real any,real anz)
{
  matrix rot,newm;
  VEC_rotate(defaultcam->x,anx,any,anz);
  VEC_rotate(defaultcam->y,anx,any,anz);
  VEC_rotate(defaultcam->z,anx,any,anz);
  MAT_initrot(rot,anx,any,anz);
  MAT_mul(newm,rot,defaultcam->worldcamera->m);
  MAT_copy(defaultcam->worldcamera->m,newm);
  XFORM_inverse(defaultcam->cameraworld,defaultcam->worldcamera);
}

void CAMERA_xform(xform *t)
{
 xform transform;

 XFORM_combine(&transform,t,defaultcam->worldcamera);
 XFORM_copy(defaultcam->worldcamera,&transform);
 XFORM_inverse(defaultcam->cameraworld,defaultcam->worldcamera);
}


