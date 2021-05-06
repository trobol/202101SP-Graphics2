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

typedef struct a3_UI_Quad {
	a3vec2 pos;
	a3vec2 scale;
	a3_UI_Atlas_Coords coords;
	a3vec3 color;
} a3_UI_Quad;


void a3_UI_load(a3_DemoState* demoState);


void a3demo_drawText(const a3_DemoState* demoState, a3real x, a3real y, a3real font_scale, const char* text);



void a3demo_loadUIVertexArray(a3_DemoState* demoState);




typedef struct a3_UI_Element_Textbox {
	const char* text;
	a3boolean editable;
} a3_UI_Element_Textbox;

typedef struct a3_UI_Element_Checkbox {
	a3boolean checked;
} a3_UI_Element_Checkbox;

typedef struct a3_UI_Dynamic_Text {
	const char* text;
	a3real size;
	a3ui32 x, y;
} a3_UI_Dynamic_Text;

typedef struct a3_UI_Static_Text_Description {
	a3ui32 x, y;
	a3real size;
	const char* text;
	const a3_UI_Char* font_chars;
} a3_UI_Static_Text_Description;



typedef struct a3_UI_Button_Description {
	a3ui32 x, y;
	a3ui32 width, height;
	a3_UI_Atlas_Coords coords;
	a3vec3 color_hover;
	a3vec3 color_click;
	a3vec3 color;
} a3_UI_Button_Description;

typedef struct a3_UI_Button {
	a3ui32 x, y;
	a3ui32 width, height;
	a3_UI_Quad* quad;
	a3boolean pressed;
	a3boolean held;
	a3vec3 color_hover;
	a3vec3 color_click;
	a3vec3 color;
} a3_UI_Button;

typedef struct a3_UI_Checkbox_Descriptor {
	a3ui32 x, y;
	a3ui32 size;
	a3_UI_Atlas_Coords box_coords;
} a3_UI_Checkbox_Descriptor;

typedef struct a3_UI_Checkbox {
	a3_UI_Button* button;
	a3_UI_Quad* checkmark;
	a3boolean checked;
} a3_UI_Checkbox;


#define A3_UI_LAYOUT_MAX_QUADS 2048
#define A3_UI_LAYOUT_MAX_ITEMS 100

typedef struct a3_UI_Layout {
	a3ui32 x, y;

	a3ui32 quad_count;
	a3_UI_Quad quads[A3_UI_LAYOUT_MAX_QUADS];

	a3ui32 text_quad_count;
	a3_UI_Quad text_quads[A3_UI_LAYOUT_MAX_QUADS];

	a3ui32 dynamic_text_count;
	a3_UI_Dynamic_Text dynamic_text[A3_UI_LAYOUT_MAX_ITEMS];

	a3ui32 button_count;
	a3_UI_Button buttons[A3_UI_LAYOUT_MAX_ITEMS];

	a3ui32 checkbox_count;
	a3_UI_Checkbox checkboxes[A3_UI_LAYOUT_MAX_ITEMS];
} a3_UI_Layout;

void a3_UI_layoutAddButton(a3_UI_Layout* layout, a3_UI_Button** out, a3_UI_Button_Description description);
void a3_UI_layoutAddStaticText(a3_UI_Layout* layout, a3_UI_Static_Text_Description description);
void a3_UI_layoutAddQuad(a3_UI_Layout* layout, a3_UI_Quad** out, a3_UI_Quad quad);
void a3_UI_layoutAddCharQuad(a3_UI_Layout* layout, a3_UI_Quad quad);
void a3_UI_layoutAddCheckbox(a3_UI_Layout* layout, a3_UI_Checkbox** out, a3_UI_Checkbox_Descriptor descr);

void a3_UI_drawLayout(a3_DemoState* demoState, a3_UI_Layout* layout);
void a3_UI_updateLayout(a3_DemoState* demoState, a3_UI_Layout* layout);

#endif