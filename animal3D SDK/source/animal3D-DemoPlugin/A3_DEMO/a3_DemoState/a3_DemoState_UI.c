#include "../a3_DemoState_UI.h"
#include "../a3_DemoState.h"
#include "animal3D-A3DG/a3graphics/a3_VertexBuffer.h"
#include "animal3D-A3DG/a3graphics/a3_VertexDescriptors.h"
#include "animal3D-A3DG/animal3D-A3DG.h"

// OpenGL
#ifdef _WIN32
#include <gl/glew.h>
#include <Windows.h>
#include <GL/GL.h>
#else	// !_WIN32
#include <OpenGL/gl3.h>
#endif	// _WIN32

a3_Screen_Rect a3demo_createScreenRect(a3ui32 width, a3ui32 height, a3ui32 x, a3ui32 y) {
	a3_Framebuffer fbo;
	a3framebufferCreate(&fbo, "fbo:c16x4:d24s8", 4, a3fbo_colorRGBA16, a3fbo_depthDisable, width, height);

	return (a3_Screen_Rect) { .width = width, .height = height, .x = x, .y = y, .buffer = fbo };
}



void a3demo_loadUIVertexArray(a3_DemoState* demoState) {
	a3vec2 vertices[4] = {
		{  0.0f,   0.0f },
		{  1.0f,   0.0f },
		{  0.0f,   1.0f },
		{  1.0f,   1.0f }
	};
	a3bufferCreate(demoState->vbo_ui_quad, "vbo:ui_quad", a3buffer_vertex, sizeof(vertices), vertices);
	a3bufferCreate(demoState->vbo_ui_instances, "vbo:ui_instances", a3buffer_vertex, 200 * sizeof(a3_UI_Rect), NULL);

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
	glVertexAttribDivisor(4, 1);

	glBindVertexArray(0);

}



void a3demo_render_UI_rects(const a3_DemoState* demoState, const a3_UI_Rect* rects, const a3ui32 count) {
	glDisable(GL_CULL_FACE);


	a3mat4 proj, invProj;
	/*
	a3framebufferDeactivateSetViewport(a3fbo_depth24_stencil8,
		-demoState->frameBorder, -demoState->frameBorder, demoState->frameWidth, demoState->frameHeight);
		*/
	a3real right = (a3real)demoState->frameWidth;
	a3real bottom = (a3real)demoState->frameHeight;
	a3real border = (a3real)demoState->frameBorder;
	a3real4x4MakeOrthographicProjectionPlanes(&proj.mm, &invProj.mm, right, -border, -border, bottom, -1, 10);
	const a3_DemoStateShaderProgram* currentProgram = demoState->prog_drawText;

	a3shaderProgramActivate(currentProgram->program);
	const a3f64 aspect = demoState->windowAspect;
	a3vec2 size = (a3vec2){ .x = (a3real)demoState->windowWidth, .y = (a3real)demoState->windowHeight };
	a3shaderUniformSendFloat(a3unif_vec2, currentProgram->uSize, 1, size.v);
	a3shaderUniformSendFloatMat(a3unif_mat4, false, currentProgram->uP, 1, proj.mm);
	a3textureActivate(demoState->tex_font, a3tex_unit00);



	//a3bufferRefill(demoState->vbo_ui_instances, 0, size, rects);


	glBindBuffer(GL_ARRAY_BUFFER, demoState->vbo_ui_instances->handle->handle);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(a3_UI_Rect), rects, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(demoState->vao_ui_quads->handle->handle);

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);
	glBindVertexArray(0);

}

void a3demo_drawText(const a3_DemoState* demoState, a3real base_x, a3real base_y, const char* text) {

	a3ui32 text_len = min(((a3ui32)strlen(text)), 200);

	a3_UI_Rect rects[200];

	a3ui32 space_width = 40;

	a3ui32 x_pos = (a3ui32)base_x;
	a3ui32 y_pos = (a3ui32)base_y + 50;

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
			(float)x_pos + ui_char.left,
			(float)y_pos - ui_char.top
		};
		a3vec2 scale = (a3vec2){
			(float)ui_char.width,
			(float)ui_char.height
		};

		rects[rect_index] = (a3_UI_Rect){
			.tex_coords = ui_char.tex_coords,
			.tex_scale = ui_char.tex_scale,
			.pos = pos,
			.scale = scale,
		};
		rect_index++;

		x_pos += ui_char.advance;
	}

	a3demo_render_UI_rects(demoState, (a3_UI_Rect*)rects, rect_index);
}