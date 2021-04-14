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
	
	passTangentBasis_morph_transform_vs4x.glsl
	Calculate and pass morphed tangent basis.
*/

#version 450

#define MAX_OBJECTS 128

// ****TO-DO: 
//	-> declare morph target attributes
//	-> declare and implement morph target interpolation algorithm
//	-> declare interpolation time/param/keyframe uniform
//	-> perform morph target interpolation using correct attributes
//		(hint: results can be stored in local variables named after the 
//		complete tangent basis attributes provided before any changes)
struct sMorphTarget {
	vec4 position;
	vec4 normal;
	vec4 tangent;
};

layout (location = 0) in sMorphTarget aMorphTargets[5];
layout (location = 15) in vec4 aTexcoord;


struct sModelMatrixStack
{
	mat4 modelMat;						// model matrix (object -> world)
	mat4 modelMatInverse;				// model inverse matrix (world -> object)
	mat4 modelMatInverseTranspose;		// model inverse-transpose matrix (object -> world skewed)
	mat4 modelViewMat;					// model-view matrix (object -> viewer)
	mat4 modelViewMatInverse;			// model-view inverse matrix (viewer -> object)
	mat4 modelViewMatInverseTranspose;	// model-view inverse transpose matrix (object -> viewer skewed)
	mat4 modelViewProjectionMat;		// model-view-projection matrix (object -> clip)
	mat4 atlasMat;						// atlas matrix (texture -> cell)
};

uniform ubTransformStack
{
	sModelMatrixStack uModelMatrixStack[MAX_OBJECTS];
};
uniform int uIndex;
uniform float uTime;
uniform float uSize;
uniform	vec4 uColor;
uniform vec4 uColor0;
uniform mat4 uP;

out vbVertexData {
	mat4 vTangentBasis_view;
	vec4 vTexcoord_atlas;
};

flat out int vVertexID;
flat out int vInstanceID;

void main()
{
	// DUMMY OUTPUT: directly assign input position to output position
	//gl_Position = aPosition;
	uint i0 = uint(uTime) % 5;
	uint i1 = uint(i0 + 1) % 5;
	float param = uTime - floor(uTime);
	sMorphTarget t0 = aMorphTargets[i0];
	sMorphTarget t1 = aMorphTargets[i1];
	vec4 position = mix(t0.position, t1.position, param);
	vec3 normal = mix(t0.normal.xyz, t1.normal.xyz, param);
	vec3 tangent = mix(t0.tangent.xyz, t1.tangent.xyz, param);
	vec3 biTangent= cross(normal, tangent);
	

	
	sModelMatrixStack t = uModelMatrixStack[uIndex];
	
	vTangentBasis_view = t.modelViewMatInverseTranspose * mat4(tangent, 0.0, biTangent, 0.0, normal, 0.0, vec4(0.0));
	vTangentBasis_view[3] = t.modelViewMat * position;
	gl_Position = t.modelViewProjectionMat * position;
	
	vTexcoord_atlas = t.atlasMat * aTexcoord;
	

	vVertexID = gl_VertexID;
	vInstanceID = gl_InstanceID;
}
