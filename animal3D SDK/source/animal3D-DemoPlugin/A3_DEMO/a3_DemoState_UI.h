#ifndef __ANIMAL3D_DEMOSTATE_UI_H
#define __ANIMAL3D_DEMOSTATE_UI_H

#include "animal3D/animal3D.h"
#include "animal3D-A3DM/a3math/a3vector.h"
#include "animal3D-A3DG/a3graphics/a3_Framebuffer.h"

/*
	grid based ui
	this means elements fit within grid cells and will be rendered as such
*/

#define A3_UI_CHAR_START 33
#define A3_UI_CHAR_END 127
#define A3_UI_CHAR_COUNT (A3_UI_CHAR_END - A3_UI_CHAR_START)

typedef struct a3_DemoState a3_DemoState;

typedef enum a3_UI_SpriteID {
	A3_UI_SPRITE_CHECK,
	A3_UI_SPRITE_SQUARE,
	A3_UI_SPRITE_CIRCLE,
	A3_UI_SPRITE_ROUND_SQR,
	A3_UI_SPRITE_
} a3_UI_SpriteID;





// where a sprite is in an atlas
typedef struct a3_UI_Atlas_Coords {
	a3vec2 pos;
	a3vec2 size;
} a3_UI_Atlas_Coords;


typedef struct a3_UI_Char {
	a3_UI_Atlas_Coords coords;
	a3ui32 width, height;
	a3ui32 left, top;
	a3ui32 advance;
} a3_UI_Char;


typedef struct a3_Screen_Rect {
	a3ui32 width, height;
	a3ui32 x, y;
	a3_Framebuffer buffer;
} a3_Screen_Rect;

typedef struct a3_UI_Draw_Rect {
	a3vec2 pos;
	a3vec2 scale;
	a3_UI_Atlas_Coords coords;
	a3vec3 color;
} a3_UI_Draw_Rect;



void a3_UI_update(a3_DemoState* demoState);
void a3_UI_render(a3_DemoState* demoState);

void a3_UI_load(a3_DemoState* demoState);

a3_Screen_Rect a3demo_createScreenRect(a3ui32 width, a3ui32 height, a3ui32 x, a3ui32 y);

void a3demo_drawText(const a3_DemoState* demoState, a3real x, a3real y, a3real font_scale, const char* text);


/*
	trying to do signed distance field fonts
	https://steamcdn-a.akamaihd.net/apps/valve/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf

*/

void a3demo_loadUIVertexArray(a3_DemoState* demoState);


/*
	elements are all the stuff drawn that is not text
	elements are added one by one in the order they will apear,
	the only way to remove elements is to clear all the elements
*/

#define A3_UI_MAX_ELEM_COUNT 512
#define A3_UI_MAX_ELEM_TYPES  32
#define A3_UI_MAX_ELEM_DATA_SIZE  32

typedef struct a3_UI_Element a3_UI_Element;

typedef void (*a3_UI_Element_VTable_Entry)(a3_DemoState* demoState, a3_UI_Element* elem);

typedef enum a3_UI_Element_VTable_Index {
	A3_UI_ELEMENT_VTABLE_UPDATE,
	A3_UI_ELEMENT_VTABLE_RENDER,

	A3_UI_ELEMENT_VTABLE_COUNT
} a3_UI_Element_VTable_Index;

typedef struct a3_UI_Element_VTable {
	a3_UI_Element_VTable_Entry list[A3_UI_ELEMENT_VTABLE_COUNT];

} a3_UI_Element_VTable;

typedef struct a3_UI_Element_Type {
	const char* name;
	a3ui32 data_size;
	a3_UI_Element_VTable vtable;
} a3_UI_Element_Type;

// create a new element type,
void a3_UI_createElementType(a3_UI_Element_Type* dst, const char* name, a3ui32 dataSize, a3_UI_Element_VTable vtable);

typedef struct a3_UI_Element {
	a3ui32 x, y;
	a3ui32 width, height;
	a3vec3 color;
	a3_UI_Atlas_Coords coords;
	const a3_UI_Element* parent;
	const a3_UI_Element_Type* type;


	a3ubyte data[A3_UI_MAX_ELEM_DATA_SIZE];

} a3_UI_Element;


typedef struct a3_UI_Element_Manager {
	a3ui32 count;
	a3_UI_Element elements[A3_UI_MAX_ELEM_COUNT];

} a3_UI_Element_Manager;


a3_UI_Element* a3_UI_addElement(a3_DemoState* demoState, const a3_UI_Element_Type* type, const a3_UI_Element* parent, void* data);
//void a3_UI_resizeElement(a3_UI_Element* elem, a3ui32 x, a3ui32 y, a3ui32 width, a3ui32 height);

typedef struct a3_UI_Element_Textbox a3_UI_Element_Textbox;
typedef struct a3_UI_Element_Checkbox a3_UI_Element_Checkbox;

void a3_UI_Element_update(a3_DemoState* demoState, a3_UI_Element* elem);
void a3_UI_Element_render(a3_DemoState* demoState, a3_UI_Element* elem);

typedef struct a3_UI_Element_Textbox {
	const char* text;
	a3boolean editable;
} a3_UI_Element_Textbox;

typedef struct a3_UI_Element_Checkbox {
	a3boolean checked;
} a3_UI_Element_Checkbox;

#endif