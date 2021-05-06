#include "../a3_DemoState_UI.h"
#include "../a3_DemoState.h"
#include "animal3D-A3DG/a3graphics/a3_VertexBuffer.h"
#include "animal3D-A3DG/a3graphics/a3_VertexDescriptors.h"
#include "animal3D-A3DG/animal3D-A3DG.h"

#include <stdio.h>
#include <stdlib.h>

// OpenGL
#ifdef _WIN32
#include <gl/glew.h>
#include <Windows.h>
#include <GL/GL.h>
#else	// !_WIN32
#include <OpenGL/gl3.h>
#endif	// _WIN32

#define A3_DEMO_RES_DIR	"../../../../resource/"

void a3_UI_loadElementTypes(a3_DemoState* demoState);
void a3demo_loadUIVertexArray(a3_DemoState* demoState);
void a3_UI_loadFont(a3_DemoState* demoState);
void a3_UI_loadSpriteCoords(a3_DemoState* demoState);


void a3_UI_bufferDrawRects(a3_DemoState const* demoState, a3_UI_Draw_Rect* rects, a3ui32 count);
void a3_UI_drawBufferedRects(a3_DemoState const* demoState, a3ui32 count, const a3_DemoStateShaderProgram* program, const a3_Texture* tex);

a3_Screen_Rect a3demo_createScreenRect(a3ui32 width, a3ui32 height, a3ui32 x, a3ui32 y) {
	a3_Framebuffer fbo;
	a3framebufferCreate(&fbo, "fbo:c16x4:d24s8", 4, a3fbo_colorRGBA16, a3fbo_depthDisable, width, height);

	return (a3_Screen_Rect) { .width = width, .height = height, .x = x, .y = y, .buffer = fbo };
}

void a3_UI_vtableCall(a3_DemoState* demoState, const a3_UI_Element_VTable_Index index) {
	a3_UI_Element_Manager* manager = &demoState->ui_elem_manager;
	a3_UI_Element* elem = elem = manager->elements;
	a3_UI_Element* end = end = elem + manager->count;



	for (; elem < end; elem++) {
		if (elem->type->vtable.list[index])
			elem->type->vtable.list[index](demoState, elem);
	}
}


void a3_UI_createElementType(a3_UI_Element_Type* dst, const char* name, a3ui32 dataSize, a3_UI_Element_VTable vtable) {
	*dst = (a3_UI_Element_Type){ .name = name, .data_size = dataSize, .vtable = vtable };
}

a3_UI_Element* a3_UI_addElement(a3_DemoState* demoState, const a3_UI_Element_Type* type, const a3_UI_Element* parent, void* data) {
	a3_UI_Element_Manager* manager = &demoState->ui_elem_manager;
	a3_UI_Element* elem = manager->elements + manager->count;
	manager->count++;

	*elem = (a3_UI_Element){ .type = type, .parent = parent };

	memcpy(elem->data, data, type->data_size);

	return elem;
}


void a3_UI_update(a3_DemoState* demoState) {
	a3_UI_vtableCall(demoState, A3_UI_ELEMENT_VTABLE_UPDATE);
}

void a3_UI_render(a3_DemoState* demoState) {
	//a3_UI_vtableCall(demoState, A3_UI_ELEMENT_VTABLE_RENDER);

	const a3_UI_Element_Manager* manager = &demoState->ui_elem_manager;

	a3_UI_Draw_Rect* rect_buffer = malloc(sizeof(a3_UI_Draw_Rect) * manager->count);
	if (rect_buffer == 0) return;
	a3_UI_Draw_Rect* rect = rect_buffer;

	const a3_UI_Element* elem = manager->elements;
	const a3_UI_Element* end = elem + manager->count;

	for (; elem < end; elem++, rect++) {
		*rect = (a3_UI_Draw_Rect){
			.pos = (a3vec2){(a3real)elem->x, (a3real)elem->y},
			.scale = (a3vec2){(a3real)elem->width, (a3real)elem->height},
			.coords = elem->coords,
			.color = elem->color
		};
	}

	a3_UI_bufferDrawRects(demoState, rect_buffer, manager->count);
	a3_UI_drawBufferedRects(demoState, manager->count, demoState->prog_drawRect, demoState->tex_ui_sprites);
}

void a3_UI_Element_Hoverbox_update(a3_DemoState* demoState, a3_UI_Element* elem) {
	a3ret x = a3mouseGetX(demoState->mouse);
	a3ret y = a3mouseGetX(demoState->mouse);

	a3ui32 cmp_x = x - elem->x;
	a3ui32 cmp_y = y - elem->y;

	if (cmp_x < elem->width && cmp_y < elem->height)
		elem->color = (a3vec3){ 1, 1, 1 };
	else
		elem->color = (a3vec3){ 0, 0, 0 };



	printf("%d, %d   %d, %d %f\n", x, y, elem->width, elem->height, elem->color.x);
}

void a3_UI_Element_update(a3_DemoState* demoState, a3_UI_Element* elem) {
	printf("%s update\n", elem->type->name);
}
void a3_UI_Element_render(a3_DemoState* demoState, a3_UI_Element* elem) {
	printf("%s render\n", elem->type->name);
}


void a3_UI_load(a3_DemoState* demoState) {

	a3_UI_loadFont(demoState);

	a3_UI_loadSpriteCoords(demoState);

	a3_UI_loadElementTypes(demoState);

	a3demo_loadUIVertexArray(demoState);


	a3_UI_Element* elem = a3_UI_addElement(demoState, demoState->ui_hoverbox, 0, 0);
	elem->x = 100;
	elem->y = 100;
	elem->width = 500;
	elem->height = 500;

	elem->coords = demoState->ui_atlas_box_round;
}

void a3_UI_loadElementTypes(a3_DemoState* demoState) {

	a3_UI_createElementType(demoState->ui_checkbox, "checkbox", 0, (a3_UI_Element_VTable) { a3_UI_Element_update, a3_UI_Element_render });
	a3_UI_createElementType(demoState->ui_textbox, "textbox", 0, (a3_UI_Element_VTable) { a3_UI_Element_update, a3_UI_Element_render });
	a3_UI_createElementType(demoState->ui_hoverbox, "hoverbox", 0, (a3_UI_Element_VTable) { a3_UI_Element_Hoverbox_update, a3_UI_Element_render });

}

void a3_UI_loadSpriteCoords(a3_DemoState* demoState) {

	a3_UI_Atlas_Coords* coords = demoState->ui_atlas_coords;
	a3ui32 tex_px = 512;
	a3ui32 sprite_px = 128;
	a3real size = (a3real)sprite_px / (a3real)tex_px;

	for (a3ui32 y = 0; y < tex_px; y += sprite_px, coords++)
		for (a3ui32 x = 0; x < tex_px; x += sprite_px, coords++) {
			*coords = (a3_UI_Atlas_Coords){
				.pos = {(a3real)x / (a3real)tex_px, (a3real)y / (a3real)tex_px},
				.size = {size, size}
			};
		}
}

void a3_UI_loadFont(a3_DemoState* demoState) {



	a3ui32 tex_width = demoState->tex_font->width;
	a3ui32 tex_height = demoState->tex_font->height;

	FILE* fp = fopen(A3_DEMO_RES_DIR"font_data.txt", "r");
	if (!fp) {
		printf("failed to open font data\n");
		return;
	}

	a3ui32 i, char_index;

	for (i = 0, char_index = A3_UI_CHAR_START; i < A3_UI_CHAR_COUNT; i++, char_index++) {
		a3ui32 n;
		a3ui32 x, y;
		a3ui32 width, height;
		a3ui32 left, top;
		a3ui32 advance;

		int ret = fscanf(fp, "%i %i %i %i %i %i %i %i\n",
			&n, &x, &y, &width, &height, &left, &top, &advance);
		if (ret != 8) {
			printf("failed to parse font data: read %i ints\n", i);
			return;
		}
		if (n != char_index) {
			printf("failed to parse font data: tried to read %i but got %i\n", char_index, n);
			return;
		}

		// convert coordinates to opengl tex coords
		a3real norm_x = (float)x / (float)tex_width;
		a3real norm_y = (float)y / (float)tex_height;
		a3real norm_width = (float)width / (float)tex_width;
		a3real norm_height = (float)height / (float)tex_height;

		a3_UI_Atlas_Coords coords = (a3_UI_Atlas_Coords){
			.pos = (a3vec2) { norm_x, 1.0f - (norm_y + norm_height)},
			.size = (a3vec2){ norm_width, norm_height }
		};


		demoState->ui_characters[i] = (a3_UI_Char){
			.coords = coords,
			.width = width,
			.height = height,
			.left = left,
			.top = top,
			.advance = advance
		};

	}

	fclose(fp);
}

void a3demo_loadUIVertexArray(a3_DemoState* demoState) {
	a3vec2 vertices[4] = {
		{  0.0f,   0.0f },
		{  1.0f,   0.0f },
		{  0.0f,   1.0f },
		{  1.0f,   1.0f }
	};
	a3bufferCreate(demoState->vbo_ui_quad, "vbo:ui_quad", a3buffer_vertex, sizeof(vertices), vertices);
	a3bufferCreate(demoState->vbo_ui_instances, "vbo:ui_instances", a3buffer_vertex, 200 * sizeof(a3_UI_Draw_Rect), NULL);

	a3_VertexFormatDescriptor format[1] = { 0 };
	a3_VertexAttributeDescriptor attrib[16] = { 0 };

	a3vertexAttribCreateDescriptor(attrib, a3attrib_position, a3attrib_vec2);
	a3vertexFormatCreateDescriptor(format, attrib, 1);

	a3vertexArrayCreateDescriptor(demoState->vao_ui_quads, "vao:ui_quads", demoState->vbo_ui_quad, format, 0);

	glBindVertexArray(demoState->vao_ui_quads->handle->handle);

	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);

	a3bufferActivate(demoState->vbo_ui_instances);

	const a3ui32 step = (4 * sizeof(a3vec2)) + (sizeof(a3vec3));

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, step, (void*)0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, step, (void*)(1 * sizeof(a3vec2)));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, step, (void*)(2 * sizeof(a3vec2)));
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, step, (void*)(3 * sizeof(a3vec2)));
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, step, (void*)(4 * sizeof(a3vec2)));

	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	glBindVertexArray(0);

}

void a3_UI_bufferDrawRects(a3_DemoState const* demoState, a3_UI_Draw_Rect* rects, a3ui32 count) {
	glBindBuffer(GL_ARRAY_BUFFER, demoState->vbo_ui_instances->handle->handle);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(a3_UI_Draw_Rect), rects, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void a3_UI_drawBufferedRects(a3_DemoState const* demoState, a3ui32 count, const a3_DemoStateShaderProgram* program, const a3_Texture* tex) {

	glDisable(GL_CULL_FACE);

	a3mat4 proj, invProj;
	a3real right = (a3real)demoState->windowWidth;
	a3real bottom = (a3real)demoState->windowHeight;
	a3real border = (a3real)demoState->frameBorder;
	a3real4x4MakeOrthographicProjectionPlanes(&proj.mm, &invProj.mm, right, 0, 0, bottom, 0, 10);

	a3shaderProgramActivate(program->program);

	a3shaderUniformSendFloatMat(a3unif_mat4, false, program->uP, 1, proj.mm);

	a3textureActivate(tex, a3tex_unit00);

	glBindVertexArray(demoState->vao_ui_quads->handle->handle);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);
	glBindVertexArray(0);
}


void a3demo_drawText(const a3_DemoState* demoState, a3real base_x, a3real base_y, a3real font_scale, const char* text) {

	a3ui32 text_len = min(((a3ui32)strlen(text)), 200);

	a3_UI_Draw_Rect rects[200];

	a3ui32 space_width = 40;

	a3real x_pos = base_x;
	a3real y_pos = base_y;

	a3ui32 i, rect_index;

	for (i = 0, rect_index = 0; i < text_len; i++) {

		a3ui8 c = text[i];

		if (c == 32) { // space 
			x_pos += space_width;
			continue;
		}
		if (c < A3_UI_CHAR_START && c > A3_UI_CHAR_END) { // make any characters we cant handle a ?
			c = '?';
		}
		// TODO: might want to handle newlines and stuff

		a3ui8 index = c - A3_UI_CHAR_START;
		if (index >= A3_UI_CHAR_COUNT) continue;
		a3_UI_Char ui_char = demoState->ui_characters[index];


		a3vec2 pos = (a3vec2){
			x_pos + (float)ui_char.left,
			y_pos - (float)ui_char.top
		};
		a3vec2 scale = (a3vec2){
			(float)ui_char.width,
			(float)ui_char.height
		};

		a3real2MulS(scale.v, font_scale);
		a3real2MulS(pos.v, font_scale);

		rects[rect_index] = (a3_UI_Draw_Rect){
			.coords = ui_char.coords,
			.pos = pos,
			.scale = scale,
		};
		rect_index++;

		x_pos += (a3real)ui_char.advance;
	}


	a3_UI_bufferDrawRects(demoState, rects, rect_index);

	a3_UI_drawBufferedRects(demoState, rect_index, demoState->prog_drawText, demoState->tex_font);
}