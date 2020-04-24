# 3dengn
This is probably my first serious 3D "engine" (software rasterizer) from 1997, when I was in my last year of elementary school. At some point, a year before, the geometry behind projecting 3D points onto a 2D screen had just made sense to me, based on similarity of triangles, and I drew the basic projection equation, before working out a simple renderer in Turbo Pascal, and MCGA mode in MS-DOS. Being constrained by the low limit of memory I had at my disposal, and relative difficulty of venturing into a true 32-bit mode, I abandoned Pascal and went for Watcom C, with which I could easily harness the power of [DOS4GW](https://en.wikipedia.org/wiki/DOS/4G).

The renderer supported several shading modes - Flag, Gouraud and Phong, included some rudimentary and inefficient motion blurring, implemented Sutherland-Hodgeman clipping, Bressenham's algorithm for lines, both the Painter's algorithm and a Z-Buffer, used integers for floating point arithmetic, trigonometric look up tables, and many other simple hacks, most of which have been coded in Assembly, for performance reasons. Alas, there was no texture mapping, at least in this attempt at a 3D renderer. This came at a time where 3D hardware and OpenGL were available, but there was an element of pleasure in implementing things from scratch. 

It came with a small library for affine transformations (on vectors, not quaternions), a library of routines for DOS mode 13h coded in Assembly, of course, and a bunch of other cool stuff.

The code style is super-weird. Indentation is screwed up everywhere, nomenclature shows a certion obsessive compulsion, magic numbers are all over the place (for instance, 320 and 200), but the code is still pretty cool - at least to me - as a time capsule into certain barely-teenager's programming beginnings.

Models were created in 3D Studio 4 (for MS-DOS) and exported in a simple ASCII format (.ASC). We have some simple geometric shapes, a few tori, a duck and a teapot - popular 3D Studio models at the time. 

I'll try to find the original compiler, fix the codebase up a little bit, and recompile it with a bit more options, because why not?

## Screenshots

### Flat Shading:
![Flat shading](https://i.imgur.com/wsiu1sh.png)

### Gouraud Shading:

![Gouraud](https://i.imgur.com/ZiARRzv.png)

### Phong Shading:
![Phong Shading](https://i.imgur.com/F40C0U4.png)

## Motion Blur
![Motion Blur](https://i.imgur.com/BOCrVHU.png)
