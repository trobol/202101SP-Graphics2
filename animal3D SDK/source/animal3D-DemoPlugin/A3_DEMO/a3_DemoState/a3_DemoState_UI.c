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

void a3demo_loadUIBuffers(a3_DemoState* demoState) {
	a3_VertexAttributeDescriptor attribs[16] = { 0 };
	a3ui32 attribCount = 4;
	a3_VertexFormatDescriptor format[1] = { 0 };

	a3vec2 vertices[] = {
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },

		{ 0.0f, 0.0f },
		{ 0.0f, 1.0f },
		{ 1.0f, 1.0f }
	};
	a3ui32 quadVAO, quadVBO, instanceVBO;

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glGenBuffers(1, &instanceVBO);

	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVAO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);


	// instance data
	glBindBuffer(GL_ARRAY_BUFFER);
	glEnableVertexArrayAttrib(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
	glVertexAttribDivisor(1, 1);

	glEnableVertexArrayAttrib(2);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));
	glVertexAttribDivisor(2, 1);

	glEnableVertexArrayAttrib(3);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
	glVertexAttribDivisor(3, 1);

	glEnableVertexArrayAttrib(4);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glVertexAttribDivisor(4, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void a3_drawUI(a3_UI_Controller* controllers) {

}

void a3demo_drawScreenRects(a3_DemoState* demoState, a3_Screen_Rect rects[], a3ui32 count) {

}

