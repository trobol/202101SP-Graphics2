#ifndef __ANIMAL3D_DEMOSTATE_UI_H
#define __ANIMAL3D_DEMOSTATE_UI_H

#include "animal3D/animal3D.h"
#include "animal3D-A3DM/a3math/a3vector.h"
#include "animal3D-A3DG/a3graphics/a3_Framebuffer.h"

/*
	grid based ui
	this means elements fit within grid cells and will be rendered as such

	no overlap for the time being
*/
typedef struct a3_UI_Controller {
	int a;
} a3_UI_Controller;

typedef struct	a3_DemoState					a3_DemoState;

typedef void (*a3_UI_Callback)(a3_DemoState* demoState);

typedef struct a3_UI_VTable {
	a3_UI_Callback mousedown;
	a3_UI_Callback mouseup;
	a3_UI_Callback mouseover;

} a3_UI_VTable;


typedef struct a3_UI_Rect {
	int a;
} a3_UI_Rect;


typedef struct a3_Screen_Rect {
	a3ui32 width, height;
	a3ui32 x, y;
	a3_Framebuffer buffer;
} a3_Screen_Rect;

typedef struct a3_Rect {
	a3vec2 pos;
	a3vec2 scale;
	a3vec2 tex_coord;
	a3vec2 tex_scale;
} a3_Rect;

a3_Screen_Rect a3demo_createScreenRect(a3ui32 width, a3ui32 height, a3ui32 x, a3ui32 y);
void a3demo_drawScreenRects(a3_DemoState* demoState, a3_Screen_Rect rects[], a3ui32 count);

void a3demo_resizeRect(a3ui32 x, a3ui32 y);

/*
	trying to do signed distance field fonts
	https://steamcdn-a.akamaihd.net/apps/valve/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf

*/

typedef struct a3_UI_TextRenderer {
	int a;
} a3_UI_TextRenderer;

void a3demo_loadUIBuffers(a3_DemoState* demoState);

#endif