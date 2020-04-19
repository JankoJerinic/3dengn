#ifndef __OBJECT3D_H_
#define __OBJECT3D_H_

#ifndef __AFFINE_H_
#include "affine.h"
#endif
#ifndef __DEFINES_H_
#include "defines.h"
#endif

#define SHADING_NONE            0
#define SHADING_FLAT            1
#define SHADING_GOURAUD         2
#define SHADING_PHONG           3

struct vertex_s
       {
	vector position;
	vector local;
	vector normal;
        vector aligned;
	int16 x;	   // screen coordinates
	int16 y;	   //
       };

typedef struct vertex_s vertex;

struct vert_array_s
       {
        vertex *verts;
        uint32 vcount;
       };

typedef struct vert_array_s vert_array;

struct face_s
       {
        long *points;
        uint32 vcount;
        vector normal;
	vector normal1;
	vector normal2;
	vertex centroid;
       };

typedef struct face_s face;

struct face_array_s
       {
        face *faces;
        uint32 fcount;
       };

typedef struct face_array_s face_array;

struct object_s
       {
   	vector centroid;
        face_array *faces;
        vert_array *verts;
        uint8 shading;
       };

typedef struct object_s object;

vert_array  *VERT_alloc(uint32 vcount);
void         VERT_free(vert_array *vertices);
face_array  *FACE_alloc(uint32 fcount);
void         FACE_free(face_array *faces);
object      *OBJ_alloc(uint32 vcount,uint32 fcount);
void         OBJ_free(object *obj);
void         OBJ_initnormals(object *obj);
void         OBJ_initnormals1(object *obj);
void 	     OBJ_initnormals2(object *obj);
void 	     VERT_initnormals(object *obj);

void         OBJ_centroidspecify(object *obj,vector v);
void         OBJ_centroidalign(object *obj);
void 	     OBJ_translate(object *obj,real dx,real dy,real dz);
void 	     OBJ_translatex(object *obj,real dx);
void 	     OBJ_translatey(object *obj,real dy);
void 	     OBJ_translatez(object *obj,real dz);
void         OBJ_vtranslate(object *obj,vector v);
void 	     OBJ_rotate(object *obj,real ax,real ay,real az);
void 	     OBJ_rotatex(object *obj,real ax);
void 	     OBJ_rotatey(object *obj,real ay);
void 	     OBJ_rotatez(object *obj,real az);
void 	     OBJ_scale(object *obj,real sx,real sy,real sz);
void 	     OBJ_scalex(object *obj,real sx);
void 	     OBJ_scaley(object *obj,real sy);
void 	     OBJ_scalez(object *obj,real sz);
void 	     OBJ_xform(object *obj,xform *t);

void 	     OBJ_lrotate(object *obj,real ax,real ay,real az);
void 	     OBJ_lrotatex(object *obj,real ax);
void 	     OBJ_lrotatey(object *obj,real ay);
void 	     OBJ_lrotatez(object *obj,real az);
void 	     OBJ_lscale(object *obj,real sx,real sy,real sz);
void 	     OBJ_lscalex(object *obj,real sx);
void 	     OBJ_lscaley(object *obj,real sy);
void 	     OBJ_lscalez(object *obj,real sz);

void 	     OBJ_atranslate(object *obj,real dx,real dy,real dz);
void 	     OBJ_atranslatex(object *obj,real dx);
void 	     OBJ_atranslatey(object *obj,real dy);
void 	     OBJ_atranslatez(object *obj,real dz);
void         OBJ_avtranslate(object *obj,vector v);
void 	     OBJ_arotate(object *obj,real ax,real ay,real az);
void 	     OBJ_arotatex(object *obj,real ax);
void 	     OBJ_arotatey(object *obj,real ay);
void 	     OBJ_arotatez(object *obj,real az);
void 	     OBJ_ascale(object *obj,real sx,real sy,real sz);
void 	     OBJ_ascalex(object *obj,real sx);
void 	     OBJ_ascaley(object *obj,real sy);
void 	     OBJ_ascalez(object *obj,real sz);

#endif
