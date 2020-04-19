#ifndef __CAMERA_H_
#define __CAMERA_H_

#include "affine.h"
#include "object3d.h"

typedef struct
  {
   char *name;
   vector position,x,y,z;
   real FOV;
   real maxwdth,maxhght,aspect;
   real kx,ky;
   xform *worldcamera,*cameraworld;
  } camera;

camera *defaultcam,*olddefaultcam;

void CAMERA_defaultinit(void);
void CAMERA_defaultcalckx(real FOV,real MAXWDTH);
void CAMERA_defaultcalcky(real FOV,real MAXHGHT,real aspect);
void CAMERA_setdefault(camera *cam);

void CAMERA_alignvertex(vertex *v);
void CAMERA_projectvertex(vertex *v);
void CAMERA_alignobject(object *o);
void CAMERA_projectobject(object *o);
void CAMERA_translatex(real dx);
void CAMERA_translatey(real dy);
void CAMERA_translatez(real dz);
void CAMERA_translate(real dx,real dy,real dz);
void CAMERA_vtranslate(vector v);
void CAMERA_rotatex(real angle);
void CAMERA_rotatey(real angle);
void CAMERA_rotatez(real angle);
void CAMERA_rotate(real anx,real any,real anz);
void CAMERA_xform(xform *t);




#endif
