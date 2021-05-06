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


void a3_UI_bufferDrawRects(a3_DemoState const* demoState, a3_UI_Quad* rects, a3ui32 count);
void a3_UI_drawBufferedRects(a3_DemoState const* demoState, a3ui32 count, const a3_DemoStateShaderProgram* program, const a3_Texture* tex);



void a3_UI_load(a3_DemoState* demoState) {

	a3_UI_loadFont(demoState);

	a3_UI_loadSpriteCoords(demoState);


	a3demo_loadUIVertexArray(demoState);

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
	a3bufferCreate(demoState->vbo_ui_instances, "vbo:ui_instances", a3buffer_vertex, 200 * sizeof(a3_UI_Quad), NULL);

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

void a3_UI_bufferDrawRects(a3_DemoState const* demoState, a3_UI_Quad* rects, a3ui32 count) {
	glBindBuffer(GL_ARRAY_BUFFER, demoState->vbo_ui_instances->handle->handle);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(a3_UI_Quad), rects, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void a3_UI_drawBufferedRects(a3_DemoState const* demoState, a3ui32 count, const a3_DemoStateShaderProgram* program, const a3_Texture* tex) {

	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

	a3_UI_Quad rects[200];

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
			x_pos + ((float)ui_char.left * font_scale),
			y_pos - ((float)ui_char.top * font_scale)
		};
		a3vec2 scale = (a3vec2){
			(float)ui_char.width,
			(float)ui_char.height
		};

		a3real2MulS(scale.v, font_scale);

		rects[rect_index] = (a3_UI_Quad){
			.coords = ui_char.coords,
			.pos = pos,
			.scale = scale,
		};
		rect_index++;

		x_pos += (a3real)ui_char.advance * font_scale;
	}


	a3_UI_bufferDrawRects(demoState, rects, rect_index);

	a3_UI_drawBufferedRects(demoState, rect_index, demoState->prog_drawText, demoState->tex_font);
}


/*
* LAYOUT STUFF
*
*/

void a3_UI_layoutAddButton(a3_UI_Layout* layout, a3_UI_Button** out, a3_UI_Button_Description descr) {
	a3_UI_Quad quad = (a3_UI_Quad){
		.pos = (a3vec2){ (a3real)descr.x, (a3real)descr.y },
		.scale = (a3vec2){ (a3real)descr.width, (a3real)descr.height},
		.coords = descr.coords
	};

	a3_UI_Button* btn = layout->buttons + layout->button_count;
	layout->button_count++;

	btn->x = descr.x;
	btn->y = descr.y;
	btn->width = descr.width;
	btn->height = descr.height;
	btn->color_hover = descr.color_hover;
	btn->color_click = descr.color_click;
	btn->color = descr.color;

	a3_UI_layoutAddQuad(layout, &btn->quad, quad);

	*out = btn;
}

void a3_UI_layoutAddStaticText(a3_UI_Layout* layout, a3_UI_Static_Text_Description descr) {
	a3ui32 text_len = min(((a3ui32)strlen(descr.text)), 200);
	a3real space_width = 40 * descr.size;

	a3real x_pos = (a3real)descr.x;
	a3real y_pos = (a3real)descr.y;


	for (a3ui32 i = 0; i < text_len; i++) {

		a3ui8 c = descr.text[i];

		if (c == 32) { // space 
			x_pos += space_width;
			continue;
		}
		// make any characters we cant handle a ?
		if (c < A3_UI_CHAR_START && c > A3_UI_CHAR_END) {
			c = '?';
		}
		// TODO: might want to handle newlines and stuff

		a3ui8 index = c - A3_UI_CHAR_START;
		if (index >= A3_UI_CHAR_COUNT) continue;
		a3_UI_Char ui_char = descr.font_chars[index];


		a3vec2 pos = (a3vec2){
			x_pos + ((float)ui_char.left * descr.size),
			y_pos - ((float)ui_char.top * descr.size)
		};
		a3vec2 scale = (a3vec2){
			(float)ui_char.width,
			(float)ui_char.height
		};

		a3real2MulS(scale.v, descr.size);

		a3_UI_Quad quad = (a3_UI_Quad){
			.coords = ui_char.coords,
			.pos = pos,
			.scale = scale,
		};

		a3_UI_layoutAddCharQuad(layout, quad);

		x_pos += (a3real)ui_char.advance * descr.size;
	}
}

void a3_UI_layoutAddCharQuad(a3_UI_Layout* layout, a3_UI_Quad quad) {
	if (layout->text_quad_count >= A3_UI_LAYOUT_MAX_QUADS) return;
	layout->text_quads[layout->text_quad_count] = quad;
	layout->text_quad_count++;
}

void a3_UI_layoutAddQuad(a3_UI_Layout* layout, a3_UI_Quad** out, a3_UI_Quad quad) {

	a3_UI_Quad* q = layout->quads + layout->quad_count;
	layout->quad_count++;
	*q = quad;

	if (out != 0)
		*out = q;
}

void a3_UI_layoutAddCheckbox(a3_UI_Layout* layout, a3_UI_Checkbox** out, a3_UI_Checkbox_Descriptor descr) {
	a3_UI_Button_Description btn_dscr = (a3_UI_Button_Description){
			.x = descr.x,
			.y = descr.y,
			.width = descr.size,
			.height = descr.size,
			.coords = descr.box_coords,
			.color_hover = (a3vec3){0.0f, 0.7f, 0.8f},
			.color_click = (a3vec3){0.4f, 0.4f, 0.5f},
			.color = (a3vec3){0.8f, 0.8f, 0.9f}
	};

	a3_UI_Checkbox* box = layout->checkboxes + layout->checkbox_count;
	layout->checkbox_count++;

	a3_UI_layoutAddButton(layout, &box->button, btn_dscr);


	a3_UI_Quad quad = *box->button->quad;
	quad.color = (a3vec3){ 0, 0, 0 };
	a3_UI_layoutAddQuad(layout, &box->checkmark, quad);

	*out = box;
}

void a3_UI_drawLayout(a3_DemoState* demoState, a3_UI_Layout* layout) {
	{
		glBindBuffer(GL_ARRAY_BUFFER, demoState->vbo_ui_instances->handle->handle);

		a3ui32 size = layout->quad_count * sizeof(a3_UI_Quad);
		glBufferData(GL_ARRAY_BUFFER, size, layout->quads, GL_DYNAMIC_DRAW);

		a3_UI_drawBufferedRects(demoState, layout->quad_count, demoState->prog_drawRect, demoState->tex_ui_sprites);
	}

	{
		glBindBuffer(GL_ARRAY_BUFFER, demoState->vbo_ui_instances->handle->handle);

		a3ui32 size = layout->text_quad_count * sizeof(a3_UI_Quad);
		glBufferData(GL_ARRAY_BUFFER, size, layout->text_quads, GL_DYNAMIC_DRAW);

		a3_UI_drawBufferedRects(demoState, layout->text_quad_count, demoState->prog_drawText, demoState->tex_font);
	}

}


void a3_UI_updateLayout(a3_DemoState* demoState, a3_UI_Layout* layout) {

	a3ret x = a3mouseGetX(demoState->mouse);
	a3ret y = a3mouseGetY(demoState->mouse);

	a3ret pressed = a3mouseIsPressed(demoState->mouse, a3mouse_left);
	a3ret held = a3mouseIsHeld(demoState->mouse, a3mouse_left);

	for (a3ui32 i = 0; i < layout->button_count; i++) {
		a3_UI_Button* btn = layout->buttons + i;
		a3ui32 cmp_x = x - btn->x;
		a3ui32 cmp_y = y - btn->y;

		if (cmp_x < btn->width && cmp_y < btn->height) {

			if (pressed || held)
				btn->quad->color = btn->color_click;
			else
				btn->quad->color = btn->color_hover;

			btn->pressed = pressed;
			btn->held = held;
		}
		else {
			btn->quad->color = btn->color;
			btn->pressed = false;
			btn->held = false;
		}


	}

	for (a3ui32 i = 0; i < layout->checkbox_count; i++) {
		a3_UI_Checkbox* box = layout->checkboxes + i;

		if (box->button->pressed) box->checked = !box->checked;

		if (box->checked) box->checkmark->coords = demoState->ui_atlas_check;
		else box->checkmark->coords = demoState->ui_atlas_coords[10];
	}
}