/*
	Copyright 2011-2021 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein

	a3_DemoState_unloading.c/.cpp
	Demo state function implementations.

	****************************************************
	*** THIS IS ONE OF YOUR DEMO'S MAIN SOURCE FILES ***
	*** Implement your demo logic pertaining to      ***
	***     UNLOADING in this file.                  ***
	****************************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoState.h"

#include <stdio.h>


//-----------------------------------------------------------------------------
// UNLOADING
// this is where the union array style comes in handy; don't need a single 
//	release statement for each and every object... just iterate and release!

// utility to unload geometry
void a3demo_unloadGeometry(a3_DemoState* demoState)
{
	// ****TO-DO: 
	//	-> uncomment model unloading algorithm
	a3_BufferObject* currentBuff = demoState->drawDataBuffer,
		* const endBuff = currentBuff + demoStateMaxCount_drawDataBuffer;
	a3_VertexArrayDescriptor* currentVAO = demoState->vertexArray,
		* const endVAO = currentVAO + demoStateMaxCount_vertexArray;
	a3_VertexDrawable* currentDraw = demoState->drawable,
		* const endDraw = currentDraw + demoStateMaxCount_drawable;

	while (currentBuff < endBuff)
		a3bufferRelease(currentBuff++);
	while (currentVAO < endVAO)
		a3vertexArrayReleaseDescriptor(currentVAO++);
	while (currentDraw < endDraw)
		a3vertexDrawableRelease(currentDraw++);
}

// utility to unload shaders
void a3demo_unloadShaders(a3_DemoState* demoState)
{
	// ****TO-DO: 
	//	-> uncomment shader program unloading algorithm
	a3_DemoStateShaderProgram* currentProg = demoState->shaderProgram,
		* const endProg = currentProg + demoStateMaxCount_shaderProgram;

	while (currentProg < endProg)
		a3shaderProgramRelease((currentProg++)->program);
}


// utility to unload textures
void a3demo_unloadTextures(a3_DemoState* demoState)
{
	// ****TO-DO: 
	//	-> uncomment texture unloading algorithm
	a3_Texture* currentTex = demoState->texture,
		* const endTex = currentTex + demoStateMaxCount_texture;

	while (currentTex < endTex)
		a3textureRelease(currentTex++);
}


// utility to unload framebuffers
void a3demo_unloadFramebuffers(a3_DemoState* demoState)
{
	// ****LATER

}


//-----------------------------------------------------------------------------

// confirm that all graphics objects were unloaded
void a3demo_unloadValidate(const a3_DemoState* demoState)
{
	// ****TO-DO: 
	//	-> uncomment unload validation algorithm
	a3ui32 handle;
	const a3_BufferObject* currentBuff = demoState->drawDataBuffer,
		* const endBuff = currentBuff + demoStateMaxCount_drawDataBuffer;
	const a3_VertexArrayDescriptor* currentVAO = demoState->vertexArray,
		* const endVAO = currentVAO + demoStateMaxCount_vertexArray;
	const a3_DemoStateShaderProgram* currentProg = demoState->shaderProgram,
		* const endProg = currentProg + demoStateMaxCount_shaderProgram;
	const a3_Texture* currentTex = demoState->texture,
		* const endTex = currentTex + demoStateMaxCount_texture;

	handle = 0;
	while (currentBuff < endBuff)
		handle += (currentBuff++)->handle->handle;
	if (handle)
		printf("\n A3 Warning: One or more draw data buffers not released.");

	handle = 0;
	while (currentVAO < endVAO)
		handle += (currentVAO++)->handle->handle;
	if (handle)
		printf("\n A3 Warning: One or more vertex arrays not released.");

	handle = 0;
	while (currentProg < endProg)
		handle += (currentProg++)->program->handle->handle;
	if (handle)
		printf("\n A3 Warning: One or more shader programs not released.");

	handle = 0;
	while (currentTex < endTex)
		handle += (currentTex++)->handle->handle;
	if (handle)
		printf("\n A3 Warning: One or more textures not released.");
}


//-----------------------------------------------------------------------------
