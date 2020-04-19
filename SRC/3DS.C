#include "object3d.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int readln(char *s, FILE *f);

int readln(char *s, FILE *f)
	{
	if(fgets(s,250,f)==0)
		return -1;

	s[strlen(s)-1]=0;
	return 0;
	}


object *OBJ_readasc(char *filename)
{
 FILE *f;
 char s[256];
 object *o;
 long a,b,c,x,y;
 double k,l,m,cx=0,cy=0,cz=0;
 face *fa;
 vertex *v;
 long *index;
 vector centroid,fc;
 real d;


 if (!(f=fopen(filename,"rt"))) return 0;

 do
   {
    if(readln(s,f)) return 0;
    }
 while(memcmp(s,"Tri-mesh",8));

 for(a=0;s[a]!=':';a++) {}
 a+=2;
 x=atol(s+a);

 for(;s[a]!=':';a++) {}
 a+=2;
 y=atol(s+a);

 	index=malloc(sizeof(long)*y*3);
	if(!index)
		return 0;

 	o = OBJ_alloc(x,y);
	if(readln(s,f))
		{
		OBJ_free(o);
		return 0;
		}
	for(a=0;a<x;a++)
		{
		if(readln(s,f))
			{
			OBJ_free(o);
			return 0;
			}

		if(memcmp(s,"Vertex",6))
			{
			a--;
			continue;
			}
		b=atol(s+7);

		if(a!=b)
			{
			OBJ_free(o);
			return 0;
			}
		for(c=0;s[c]!='X';c++) {}
		k=strtod(s+c+2,0);
		for(;s[c]!='Y';c++) {}
		l=strtod(s+c+2,0);
		for(;s[c]!='Z';c++) {}
		m=strtod(s+c+2,0);
		
   		cx += rdiv(D2R(k),I2R(o->verts->vcount));
		cy += rdiv(D2R(l),I2R(o->verts->vcount));
		cz += rdiv(D2R(m),I2R(o->verts->vcount));

		o->verts->verts[b].position[0]=D2R(k);
		o->verts->verts[b].position[1]=D2R(l);
		o->verts->verts[b].position[2]=D2R(m);
		o->verts->verts[b].local[0]=D2R(k);
		o->verts->verts[b].local[1]=D2R(l);
		o->verts->verts[b].local[2]=D2R(m);
	}

// In 3D Studio the Z-axis points upwards and the Y-axis is perpendicular 
// to the screen. Thus we change places of the coordinates and,in addition
// change the sign of out Z-component,since in 3D Studio the Y-axis points
// into the screen,making larger absolute Y farther away.

	
	VEC_init(o->centroid,cx,cy,cz);
 	VEC_copy(centroid,o->centroid);
	VEC_neg(centroid);

	for (a=0;a<x;a++) 
		VEC_vtranslate(o->verts->verts[a].local,centroid);

	fa = o->faces->faces;

	if(readln(s,f))
		{
		OBJ_free(o);
		return 0;
		}


	for(a=0;a<y;a++)
		{
		if(readln(s,f))
			{
			OBJ_free(o);
			return 0;
			}
		if(memcmp(s,"Face",4))
			{
			a--;
			continue;
			}

		b=atol(s+5);
		if(b!=a)
			{
			OBJ_free(o);
                        return 0;
			}
		fa->points=index;

		for(c=5;s[c]!='A';c++) {}
		c+=2;
		fa->points[0]=atol(s+c);
		for(;s[c]!='B';c++) {}
		c+=2;
		fa->points[1]=atol(s+c);
		for(;s[c]!='C';c++) {}
		c+=2;
		fa->points[2]=atol(s+c);

		fa->vcount=3;
		v = o->verts->verts;
		VEC_copy(fc,v[fa->points[0]].position);
		VEC_add(fc,fc,v[fa->points[1]].position);
		VEC_add(fc,fc,v[fa->points[2]].position);
		d = rdiv(I2R(1),I2R(3));
		VEC_scale(fc,d,d,d);
		VEC_copy(fa->centroid.position,fc);
		VEC_copy(fa->centroid.local,fc);
		VEC_vtranslate(fa->centroid.local,centroid);
		index+=3;
		fa++;

		}
	OBJ_initnormals(o);
	VERT_initnormals(o);
fclose(f);
return o;

};

