#include "affine.h"
#include "mode13h.h"
#include "object3d.h"
#include "3ds.h"
#include "camera.h"
#include "polygon.h"

void MakeGourPal(void);
void MakePhongPal( double Ra, double Rd, double Rs,
                   double Ga, double Gd, double Gs,
                   double Ba, double Bd, double Bs,
                   uint8 N);

void main(int argc,char *argv[])
{
  object *o;
  uint32 i,frames=0;
  real c0,c1,c2;
  vertex *v;
  face *f;
  vector light;
  int16 rx=0,ry=0,rz=0;
  real ax,ay,bx,by;
  byte *Buffer = (byte *) malloc(64000),blurring = 0;
  double t,fps;
 

  M13_filld_(Buffer,0,_64K);
 
  if (argc < 2)
  {
   printf("\nSHOWASC / 3D Flat/Gouraud/Phong Shader made by Jjerry/2000.\n");
   printf("USAGE:  showasc.exe [filename.asc] -shading -distance\n");
   printf("Shading : 1 - Flat / 2 - Gouraud / 3 - Phong.\n");
   return;
  }
 
  if (access(argv[1],0))
  {
   printf("\nFile does not exist!\n");
   return;
  }
   
  printf("\nReading file : %s ... \n",argv[1]);
  o = OBJ_readasc(argv[1]);
  v = o->verts->verts;
  f = o->faces->faces;

  o->shading = atoi(argv[2]);
  if (o->shading > 3) o->shading = 3;
  if (o->shading < 1) o->shading = 1;

  CAMERA_defaultinit();
  CAMERA_translatez(D2R(-atoi(argv[3])));

  VEC_copy(light,defaultcam->z);

  printf("\n CONTROLS: ");
  printf("\n 1,2,3 - Change shading to Flat,Gouraud,Phong,respectively.\n");
  printf(" 'Q' and 'W' rotate the object around the X-axis.\n");
  printf(" 'A' and 'S' rotate the object around the Y-axis.\n");
  printf(" 'Z' and 'X' rotate the object around the Z-axis.\n");
  printf(" '[' and ']' translate the camera's Z-coordinate.\n"); 
  printf(" 'B' toggles motion blurring.\n");
  printf(" Spacebar ends all rotations.\n");
  printf(" ESC key exits the program.\n");
  printf(" Press any key...\n");
  while (!kbhit());

  M13_init();
  CreateZBuffer();
  ClearZBuffer();
  ClearEBuffer();

  M13_preservepalette();
 
  if (o->shading == SHADING_PHONG)
 	MakePhongPal(0,0,0,21,31,31,31,31,31,100);
  else 
   MakeGourPal();
   
	
  OBJ_lrotate(o,-90,0,90);
  TIMER_start();
  while (!M13_getesc())
  {
   M13_clearlfb();
  CAMERA_alignobject(o);
  CAMERA_projectobject(o);

  for (i=0;i<o->faces->fcount;i++)
   {
    ax = v[f[i].points[1]].x - v[f[i].points[0]].x;
    ay = v[f[i].points[1]].y - v[f[i].points[0]].y;
    bx = v[f[i].points[2]].x - v[f[i].points[0]].x;
    by = v[f[i].points[2]].y - v[f[i].points[0]].y;
    if (ax*by - ay*bx < 0)
    {
     if (o->shading > SHADING_FLAT) 
      {
       c0 = mul(I2R(254),VEC_dotp(v[f[i].points[0]].normal,light));
       c1 = mul(I2R(254),VEC_dotp(v[f[i].points[1]].normal,light));
       c2 = mul(I2R(254),VEC_dotp(v[f[i].points[2]].normal,light));
       if (c0<0) c0 = 0;
       if (c1<0) c1 = 0;
       if (c2<0) c2 = 0;
       ZGouraudPoly(v[f[i].points[0]].x,v[f[i].points[0]].y,
               v[f[i].points[1]].x,v[f[i].points[1]].y,
               v[f[i].points[2]].x,v[f[i].points[2]].y,
               v[f[i].points[0]].position[2],
               v[f[i].points[1]].position[2],
               v[f[i].points[2]].position[2],
               c0+65536,c1+65536,c2+65536);
       ClearEBuffer();
     }
    else if (o->shading == SHADING_FLAT)
            {
	     c0 = R2I(mul(I2R(254),VEC_dotp(f[i].normal,light)));
	     if (c0<0) c0 = 0;
  	     ZFlatPoly(v[f[i].points[0]].x,v[f[i].points[0]].y,
               v[f[i].points[1]].x,v[f[i].points[1]].y,
               v[f[i].points[2]].x,v[f[i].points[2]].y,
               v[f[i].points[0]].position[2],
               v[f[i].points[1]].position[2],
               v[f[i].points[2]].position[2],
               c0+1);
       	     ClearEBuffer();
    	    }	     
    }		
   }
  ClearZBuffer();
  OBJ_lrotate(o,rx,ry,rz);

  if (blurring) 
   {
    for (i=0;i<_64K;i++)
     {
      if (VGALFB[i] > Buffer[i]) Buffer[i] = VGALFB[i];
      if (Buffer[i] < 10) Buffer[i] = 0;
	else Buffer[i] -= 10;
     }
  M13_waitvretrace();
  M13_showbuffer(Buffer,VGA,_64K);
 } 
  else    
    {   
      M13_waitvretrace();
      M13_showlfb();
    }
  frames++;

  if (kbhit())
  {
   i = getch();
   switch(toupper(i))
   {
    case '1' : o->shading = 1; MakeGourPal(); break; 
    case '2' : o->shading = 2; MakeGourPal(); break;
    case '3' : o->shading = 3; MakePhongPal(0,0,0,21,31,31,63,63,63,100); break;
    case 'B' : blurring = ~blurring;break;
    case 'Q' : rx ++; break;
    case 'W' : rx --; break;
    case 'A' : ry ++; break;
    case 'S' : ry --; break;
    case 'Z' : rz ++; break;
    case 'X' : rz --; break;
    case '[' : CAMERA_translatez(I2R(30)); break;
    case ']' : CAMERA_translatez(I2R(-30)); break;
    case ' ' : rx=0;ry=0;rz=0; break;
   }
  }
 }
  t = TIMER_stop();
  fps = frames / t;
  TIMER_unload();
  M13_setpalette();
  M13_deinit();
  OBJ_free(o);
  printf("Frames drawn	  : %d\n",frames);
  printf("Time taken	  : %fs\n",t);
  printf("Average FPS rate  : %f\n",fps);
}

void MakePhongPal( double Ra, double Rd, double Rs,
                   double Ga, double Gd, double Gs,
                   double Ba, double Bd, double Bs,
                   unsigned int N)
{
 double diffuse, specular;
 int red, green, blue,i;
 double angle = 3.14159265 / 2.0;
 double angle_step = (3.14159265 / 510.0);
 for (i = 1; i < 256; i++)
   {
    diffuse = Rd * cos(angle);
    specular = Rs * pow(cos(angle), N);
    red = Ra + diffuse + specular;

    diffuse = Gd * cos(angle);
    specular = Gs * pow(cos(angle), N);
    green = Ga + diffuse + specular;

    diffuse = Bd * cos(angle);
    specular = Bs * pow(cos(angle), N);
    blue = Ba + diffuse + specular;

    if (red > 63) red = 63;
    if (green > 63) green = 63;
    if (blue > 63) blue = 63;

    M13_setcolor(i,red,green,blue);
    angle -= angle_step;
   }
  M13_setcolor(0,0,0,0);
}

void MakeGourPal(void)
{
 uint32 i;

  M13_setcolor(0,0,0,0);
  for (i=1;i<256;i++) 
  M13_setcolor(i,16+(i>>3),32+(i>>3),32+(i>>3));
}
