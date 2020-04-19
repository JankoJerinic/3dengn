#include "object3d.h"

#include "affine.h"

vert_array *VERT_alloc(uint32 vcount)
{
 vert_array *v;
 v = (vert_array *)malloc(sizeof(vert_array));
 if(!v)
       return 0;

 v->verts = (vertex *)malloc(sizeof(vertex)*vcount);
 if (!v->verts)
 {
    VERT_free(v);
    return 0;
    };
 v->vcount = vcount;
 return v;
}

void VERT_free(vert_array *vertices)
{
 if (vertices)
   {
     if (vertices->verts)
     free(vertices->verts);
     vertices->verts = 0;
     free(vertices);
    }
}

face_array *FACE_alloc(uint32 fcount)
{
  face_array *f;
  long i;

  f=(face_array *)malloc(sizeof(face_array));
  if (!f)
	return 0;
  f->faces = (face *)malloc(sizeof(face)*fcount);
  if (!f->faces)
     {
      FACE_free(f);
      return 0;
     };

 for (i=0;i< fcount;i++)
	f->faces[i].points = 0;

  f->fcount = fcount;
  return f;

}

void FACE_free(face_array *faces)
{
 if (faces)
    {
     if (faces->faces)
      {
        free (faces->faces);
	faces->faces =0;
      }
       free(faces);
    }
}

object *OBJ_alloc(uint32 vcount,uint32 fcount)
{
 object *obj;

 obj = (object *)malloc(sizeof(object));

 if(!obj) return 0;

 obj->verts = VERT_alloc(vcount);
 obj->faces = FACE_alloc(fcount);

 if ((!obj->verts)||(!obj->faces))
    {
     OBJ_free(obj);
     return 0;
    };
 return obj;
}

void OBJ_free(object *obj)
{
 if (obj)
    {
     VERT_free(obj->verts);
     obj->verts = 0;
     FACE_free(obj->faces);
     obj->faces = 0;
    }
}
real compute_2area(face f, vertex *v, int axis1, int axis2)
{
 long a,b,c;
 real area;

 b=f.points[2];
 area=0;
 for(a=0;a<3;a++)
  {
   c=f.points[a];
   area+=mul(v[c].position[axis2]+v[b].position[axis2],
             v[c].position[axis1]-v[b].position[axis1]);
   b=c;
  }
  return area;
}

void OBJ_initnormals(object *obj)
{
 vertex *v = obj->verts->verts;
 face   *f = obj->faces->faces;
 vector a,b,norm;
 uint32 i,k;

 k = D2R(1/256.0);
 OBJ_lscale(obj,k,k,k);
 for (i=0;i < obj->faces->fcount ; i++)
      {
 	VEC_sub(a,v[f[i].points[2]].position,v[f[i].points[1]].position);
	VEC_sub(b,v[f[i].points[0]].position,v[f[i].points[1]].position);
	VEC_crossp(norm,a,b);
 	VEC_normalize(norm);
	VEC_copy(f[i].normal,norm);
     }
 k = rdiv(I2R(1),k);
 OBJ_lscale(obj,k,k,k);
}

void VERT_initnormals(object *obj)
{
 vertex *v = obj->verts->verts;
 face *f = obj->faces->faces;
 uint32 i,j;
 vector norm;

 for (i=0;i<obj->verts->vcount;i++)
  {
   VEC_init( norm,0,0,0);
   for (j=0;j<obj->faces->fcount;j++)
   {
    if ( (f[j].points[0] == i) || (f[j].points[1] == i) || (f[j].points[2] == i))
	 VEC_add( norm, norm, f[j].normal);
   }
   VEC_normalize(norm);
   VEC_copy( v[i].normal, norm);
   }
}

void OBJ_centroidspecify(object *obj,vector v)
{
 VEC_copy(obj->centroid,v);
}

void OBJ_centroidalign(object *obj)
{
 vector centroid;
 uint32 i;

 VEC_copy(centroid,obj->centroid);
 VEC_neg(centroid);

 for (i=0;i<obj->verts->vcount;i++)
 {
  VEC_copy(obj->verts->verts[i].local,obj->verts->verts[i].position);
  VEC_vtranslate(obj->verts->verts[i].local,centroid);
 }
}


void OBJ_translate(object *obj,real dx,real dy,real dz)
{
 uint32 i;

 for (i=0;i<obj->verts->vcount;i++)
   VEC_translate(obj->verts->verts[i].position,dx,dy,dz);
 VEC_translate(obj->centroid,dx,dy,dz);
}

void OBJ_translatex(object *obj,real dx)
{
 uint32 i;

 for (i=0;i<obj->verts->vcount;i++)
   VEC_translatex(obj->verts->verts[i].position,dx);
 VEC_translatex(obj->centroid,dx);
}

void OBJ_translatey(object *obj,real dy)
{
 uint32 i;

 for (i=0;i<obj->verts->vcount;i++)
   VEC_translatey(obj->verts->verts[i].position,dy);
  VEC_translatey(obj->centroid,dy);
}

void OBJ_translatez(object *obj,real dz)
{
 uint32 i;

 for (i=0;i<obj->verts->vcount;i++)
   VEC_translatez(obj->verts->verts[i].position,dz);
 VEC_translatez(obj->centroid,dz);
}

void OBJ_vtranslate(object *obj,vector v)
{
 uint32 i;

 for (i=0;i<obj->verts->vcount;i++)
     VEC_vtranslate(obj->verts->verts[i].position,v);
  VEC_vtranslate(obj->centroid,v);
}

void OBJ_rotate(object *obj,real ax,real ay,real az)
{
 uint32 i;

 VEC_rotate(obj->centroid,ax,ay,az);
 if (obj->shading >= SHADING_GOURAUD)
 {
   for (i=0;i<obj->verts->vcount;i++)
   {
    VEC_rotate(obj->verts->verts[i].position,ax,ay,az);
    VEC_rotate(obj->verts->verts[i].normal,ax,ay,az);
   }
 }
 else if (obj->shading == SHADING_FLAT)
    {
     for (i=0;i<obj->verts->vcount;i++)
         VEC_rotate(obj->verts->verts[i].position,ax,ay,az);
     for (i=0;i<obj->faces->fcount;i++)
        VEC_rotate(obj->faces->faces[i].normal,ax,ay,az);
    }
 else
   for (i=0;i<obj->verts->vcount;i++)
    VEC_rotate(obj->verts->verts[i].position,ax,ay,az);
 OBJ_centroidalign(obj);
}

void OBJ_rotatex(object *obj,real ax)
{
 uint32 i;

 VEC_rotatex(obj->centroid,ax);
 if (obj->shading >= SHADING_GOURAUD)
   for (i=0;i<obj->verts->vcount;i++)
   {
    VEC_rotatex(obj->verts->verts[i].position,ax);
    VEC_rotatex(obj->verts->verts[i].normal,ax);
   }
 else
   if (obj->shading == SHADING_FLAT)
    {
     for (i=0;i<obj->verts->vcount;i++)
         VEC_rotatex(obj->verts->verts[i].position,ax);
     for (i=0;i<obj->faces->fcount;i++)
        VEC_rotatex(obj->faces->faces[i].normal,ax);
    }
 else
   for (i=0;i<obj->verts->vcount;i++)
    VEC_rotatex(obj->verts->verts[i].position,ax);
 OBJ_centroidalign(obj);

}

void OBJ_rotatey(object *obj,real ay)
{
 uint32 i;

 VEC_rotatey(obj->centroid,ay);
 if (obj->shading >= SHADING_GOURAUD)
   for (i=0;i<obj->verts->vcount;i++)
   {
    VEC_rotatey(obj->verts->verts[i].position,ay);
    VEC_rotatey(obj->verts->verts[i].normal,ay);
   }
 else
   if (obj->shading == SHADING_FLAT)
    {
     for (i=0;i<obj->verts->vcount;i++)
         VEC_rotatey(obj->verts->verts[i].position,ay);
     for (i=0;i<obj->faces->fcount;i++)
        VEC_rotatey(obj->faces->faces[i].normal,ay);
    }
 else
   for (i=0;i<obj->verts->vcount;i++)
    VEC_rotatey(obj->verts->verts[i].position,ay);
 OBJ_centroidalign(obj);

}

void OBJ_rotatez(object *obj,real az)
{
 uint32 i;

 VEC_rotatez(obj->centroid,az);
 if (obj->shading >= SHADING_GOURAUD)
   for (i=0;i<obj->verts->vcount;i++)
   {
    VEC_rotatez(obj->verts->verts[i].position,az);
    VEC_rotatez(obj->verts->verts[i].normal,az);
   }
 else
   if (obj->shading == SHADING_FLAT)
    {
     for (i=0;i<obj->verts->vcount;i++)
         VEC_rotatez(obj->verts->verts[i].position,az);
     for (i=0;i<obj->faces->fcount;i++)
        VEC_rotatez(obj->faces->faces[i].normal,az);
    }
 else
   for (i=0;i<obj->verts->vcount;i++)
    VEC_rotatez(obj->verts->verts[i].position,az);
 OBJ_centroidalign(obj);

}


void OBJ_xform(object *obj,xform *t)
{
 vector v;
 uint32 i;

 XFORM_transform(v,obj->centroid,t);
 VEC_copy(obj->centroid,v);
 if (obj->shading >= SHADING_GOURAUD)
  for (i=0;i<obj->verts->vcount;i++)
   {
    XFORM_transform(v,obj->verts->verts[i].position,t);
    VEC_copy(obj->verts->verts[i].position,v);
    VEC_mulmatrix(v,obj->verts->verts[i].normal,t->m);
    VEC_copy(obj->verts->verts[i].normal,v);
   }
 else if (obj->shading == SHADING_FLAT)
   {
    for (i=0;i<obj->verts->vcount;i++)
    {
     XFORM_transform(v,obj->verts->verts[i].position,t);
     VEC_copy(obj->verts->verts[i].position,v);
    }
    for (i=0;i<obj->faces->fcount;i++)
     {
      VEC_mulmatrix(v,obj->faces->faces[i].normal,t->m);
      VEC_copy(obj->faces->faces[i].normal,v);
     }
   }
 else
     for (i=0;i<obj->verts->vcount;i++)
         {
          XFORM_transform(v,obj->verts->verts[i].position,t);
          VEC_copy(obj->verts->verts[i].position,v);
         }
 OBJ_centroidalign(obj);
}

void OBJ_scale(object *obj,real sx,real sy,real sz)
{
 uint32 i;

 for (i=0;i<obj->verts->vcount;i++)
   VEC_scale(obj->verts->verts[i].position,sx,sy,sz);
 VEC_scale(obj->centroid,sx,sy,sz);
 OBJ_centroidalign(obj);
}

void OBJ_scalex(object *obj,real sx)
{
 uint32 i;

 for (i=0;i<obj->verts->vcount;i++)
   VEC_scalex(obj->verts->verts[i].position,sx);
 VEC_scalex(obj->centroid,sx);
 OBJ_centroidalign(obj);
}

void OBJ_scaley(object *obj,real sy)
{
 uint32 i;

 for (i=0;i<obj->verts->vcount;i++)
   VEC_scaley(obj->verts->verts[i].position,sy);
 VEC_scaley(obj->centroid,sy);
 OBJ_centroidalign(obj);
}

void OBJ_scalez(object *obj,real sz)
{
 uint32 i;

 for (i=0;i<obj->verts->vcount;i++)
   VEC_scalez(obj->verts->verts[i].position,sz);
 VEC_scalez(obj->centroid,sz);
 OBJ_centroidalign(obj);
}

void OBJ_lrotate(object *obj,real ax,real ay,real az)
{
 uint32 i;

 if (obj->shading >= SHADING_GOURAUD)
 {
   for (i=0;i<obj->verts->vcount;i++)
    {
     VEC_rotate(obj->verts->verts[i].local,ax,ay,az);
     VEC_copy(obj->verts->verts[i].position,obj->verts->verts[i].local);
     VEC_vtranslate(obj->verts->verts[i].position,obj->centroid);
     VEC_rotate(obj->verts->verts[i].normal,ax,ay,az);
   }
   for (i=0;i<obj->faces->fcount;i++) VEC_rotate(obj->faces->faces[i].normal,ax,ay,az);
 }
 else if (obj->shading == SHADING_FLAT)
   {
    for (i=0;i<obj->verts->vcount;i++)
     {
      VEC_rotate(obj->verts->verts[i].local,ax,ay,az);
      VEC_copy(obj->verts->verts[i].position,obj->verts->verts[i].local);
      VEC_vtranslate(obj->verts->verts[i].position,obj->centroid);
      VEC_rotate(obj->verts->verts[i].normal,ax,ay,az);
     }
      for (i=0;i<obj->faces->fcount;i++)
        VEC_rotate(obj->faces->faces[i].normal,ax,ay,az);
   }
 else
     for (i=0;i<obj->verts->vcount;i++)
         {
          VEC_rotate(obj->verts->verts[i].local,ax,ay,az);
          VEC_copy(obj->verts->verts[i].position,obj->verts->verts[i].local);
          VEC_vtranslate(obj->verts->verts[i].position,obj->centroid);
         }

}

void OBJ_lrotatex(object *obj,real ax)
{
 uint32 i;

 if (obj->shading >= SHADING_GOURAUD)
   for (i=0;i<obj->verts->vcount;i++)
    {
     VEC_rotatex(obj->verts->verts[i].local,ax);
     VEC_copy(obj->verts->verts[i].position,obj->verts->verts[i].local);
     VEC_vtranslate(obj->verts->verts[i].position,obj->centroid);
     VEC_rotatex(obj->verts->verts[i].normal,ax);
   }
 else
  if (obj->shading == SHADING_FLAT)
  {
   for (i=0;i<obj->verts->vcount;i++) 
    {
     VEC_rotatex(obj->verts->verts[i].local,ax);
     VEC_copy(obj->verts->verts[i].position,obj->verts->verts[i].local);
     VEC_vtranslate(obj->verts->verts[i].position,obj->centroid);
    }
     for (i=0;i<obj->faces->fcount;i++)
      {
        VEC_rotatex(obj->faces->faces[i].centroid.local,ax);
        VEC_copy(obj->faces->faces[i].centroid.position,obj->faces->faces[i].centroid.local);
        VEC_vtranslate(obj->faces->faces[i].centroid.position,obj->centroid);
	VEC_rotatex(obj->faces->faces[i].normal,ax);
	VEC_rotatex(obj->faces->faces[i].normal1,ax);
	VEC_rotatex(obj->faces->faces[i].normal2,ax);
      }
   }
 else
  for (i=0;i<obj->verts->vcount;i++) {
     VEC_rotatex(obj->verts->verts[i].local,ax);
     VEC_copy(obj->verts->verts[i].position,obj->verts->verts[i].local);
     VEC_vtranslate(obj->verts->verts[i].position,obj->centroid);
   }

}

void OBJ_lrotatey(object *obj,real ay)
{
 uint32 i;

 if (obj->shading >= SHADING_GOURAUD)
   for (i=0;i<obj->verts->vcount;i++)
    {
     VEC_rotatey(obj->verts->verts[i].local,ay);
     VEC_copy(obj->verts->verts[i].position,obj->verts->verts[i].local);
     VEC_vtranslate(obj->verts->verts[i].position,obj->centroid);
     VEC_rotatey(obj->faces->faces[i].normal,ay);
     VEC_rotatey(obj->faces->faces[i].normal1,ay);
     VEC_rotatey(obj->faces->faces[i].normal2,ay);
   }
 else if (obj->shading == SHADING_FLAT)
 {
  for (i=0;i<obj->verts->vcount;i++)
   {
    VEC_rotatey(obj->verts->verts[i].local,ay);
    VEC_copy(obj->verts->verts[i].position,obj->verts->verts[i].local);
    VEC_vtranslate(obj->verts->verts[i].position,obj->centroid);
   }
    for (i=0;i<obj->faces->fcount;i++)
    {
     VEC_rotatey(obj->faces->faces[i].centroid.local,ay);
     VEC_copy(obj->faces->faces[i].centroid.position,obj->faces->faces[i].centroid.local);
     VEC_vtranslate(obj->faces->faces[i].centroid.position,obj->centroid);
     VEC_rotatey(obj->faces->faces[i].normal,ay);
     VEC_rotatey(obj->faces->faces[i].normal1,ay);
     VEC_rotatey(obj->faces->faces[i].normal2,ay);
    }
   }
 else 
  {
   for (i=0;i<obj->verts->vcount;i++)
   {
    VEC_rotatey(obj->verts->verts[i].local,ay);
    VEC_copy(obj->verts->verts[i].position,obj->verts->verts[i].local);
    VEC_vtranslate(obj->verts->verts[i].position,obj->centroid);
   }
  for (i=0;i<obj->faces->fcount;i++)
  {
    VEC_rotatey(obj->faces->faces[i].centroid.local,ay);
    VEC_copy(obj->faces->faces[i].centroid.position,obj->faces->faces[i].centroid.local);
    VEC_vtranslate(obj->faces->faces[i].centroid.position,obj->centroid);
  }
 }
}

void OBJ_lrotatez(object *obj,real az)
{
 uint32 i;

 if (obj->shading >= SHADING_GOURAUD)
   for (i=0;i<obj->verts->vcount;i++)
    {
     VEC_rotatez(obj->verts->verts[i].local,az);
     VEC_copy(obj->verts->verts[i].position,obj->verts->verts[i].local);
     VEC_vtranslate(obj->verts->verts[i].position,obj->centroid);
     VEC_rotatez(obj->verts->verts[i].normal,az);
   }
 else if (obj->shading == SHADING_FLAT)
   {
    for (i=0;i<obj->verts->vcount;i++)
    {
     VEC_rotatez(obj->verts->verts[i].local,az);
     VEC_copy(obj->verts->verts[i].position,obj->verts->verts[i].local);
     VEC_vtranslate(obj->verts->verts[i].position,obj->centroid);
    }
    for (i=0;i<obj->faces->fcount;i++)
       VEC_rotatez(obj->faces->faces[i].normal,az);
   }
 else
  for (i=0;i<obj->verts->vcount;i++)
   {
    VEC_rotatez(obj->verts->verts[i].local,az);
    VEC_copy(obj->verts->verts[i].position,obj->verts->verts[i].local);
    VEC_vtranslate(obj->verts->verts[i].position,obj->centroid);
   }
}

void OBJ_lscale(object *obj,real sx,real sy,real sz)
{
 uint32 i;

 for (i=0;i<obj->verts->vcount;i++)
  {
   VEC_scale(obj->verts->verts[i].local,sx,sy,sz);
   VEC_copy(obj->verts->verts[i].position,obj->verts->verts[i].local);
   VEC_vtranslate(obj->verts->verts[i].position,obj->centroid);
  }
}

void OBJ_lscalex(object *obj,real sx)
{
 uint32 i;

 for (i=0;i<obj->verts->vcount;i++)
  {
   VEC_scalex(obj->verts->verts[i].local,sx);
   VEC_copy(obj->verts->verts[i].position,obj->verts->verts[i].local);
   VEC_vtranslate(obj->verts->verts[i].position,obj->centroid);
  }
}

void OBJ_lscaley(object *obj,real sy)
{
 uint32 i;

 for (i=0;i<obj->verts->vcount;i++)
  {
   VEC_scaley(obj->verts->verts[i].local,sy);
   VEC_copy(obj->verts->verts[i].position,obj->verts->verts[i].local);
   VEC_vtranslate(obj->verts->verts[i].position,obj->centroid);
  }
}

void OBJ_lscalez(object *obj,real sz)
{
 uint32 i;

 for (i=0;i<obj->verts->vcount;i++)
  {
   VEC_scalez(obj->verts->verts[i].local,sz);
   VEC_copy(obj->verts->verts[i].position,obj->verts->verts[i].local);
   VEC_vtranslate(obj->verts->verts[i].position,obj->centroid);
  }
}

