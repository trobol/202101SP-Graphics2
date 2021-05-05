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

#define A3_UI_CHAR_START 33
#define A3_UI_CHAR_END 127
#define A3_UI_CHAR_COUNT (A3_UI_CHAR_END - A3_UI_CHAR_START)

typedef struct	a3_DemoState					a3_DemoState;

typedef void (*a3_UI_Callback)(a3_DemoState* demoState);

typedef struct a3_UI_VTable {
	a3_UI_Callback mousedown;
	a3_UI_Callback mouseup;
	a3_UI_Callback mouseover;

} a3_UI_VTable;


typedef struct a3_UI_Char {
	a3vec2 tex_coords;
	a3vec2 tex_scale;
	a3ui32 width, height;
	a3ui32 left, top;
	a3ui32 advance;
} a3_UI_Char;


typedef struct a3_Screen_Rect {
	a3ui32 width, height;
	a3ui32 x, y;
	a3_Framebuffer buffer;
} a3_Screen_Rect;

typedef struct a3_UI_Rect {
	a3vec2 pos;
	a3vec2 scale;
	a3vec2 tex_coords;
	a3vec2 tex_scale;
	a3vec3 color;
} a3_UI_Rect;

a3_Screen_Rect a3demo_createScreenRect(a3ui32 width, a3ui32 height, a3ui32 x, a3ui32 y);
void a3demo_render_UI_rects(const a3_DemoState* demoState, const a3_UI_Rect* rects, a3ui32 count);
void a3demo_drawText(const a3_DemoState* demoState, a3real x, a3real y, const char* text);

void a3demo_loadFontData(a3_DemoState* demoState);
void a3demo_load_UI(a3_DemoState* demoState);

/*
	trying to do signed distance field fonts
	https://steamcdn-a.akamaihd.net/apps/valve/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf

*/

typedef struct a3_UI_TextRenderer {
	int a;
} a3_UI_TextRenderer;

void a3demo_loadUIVertexArray(a3_DemoState* demoState);
//void a3demo_init_UI(a3_UI_Controller* contr);

typedef struct a3_UI_Text {
	a3ui32 x, y;
} a3_UI_Text;


#endif