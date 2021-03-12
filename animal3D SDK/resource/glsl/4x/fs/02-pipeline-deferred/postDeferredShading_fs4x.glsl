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
	
	postDeferredShading_fs4x.glsl
	Calculate full-screen deferred Phong shading.
*/

#version 450

#define MAX_LIGHTS 1024

// ****DONE:
//	-> this one is pretty similar to the forward shading algorithm (Phong NM) 
//		except it happens on a plane, given images of the scene's geometric 
//		data (the "g-buffers"); all of the information about the scene comes 
//		from screen-sized textures, so use the texcoord varying as the UV
//	-> declare point light data structure and uniform block
//	-> declare pertinent samplers with geometry data ("g-buffers")
//	-> use screen-space coord (the inbound UV) to sample g-buffers
//	-> calculate view-space fragment position using depth sample
//		(hint: modify screen-space coord, use appropriate matrix to get it 
//		back to view-space, perspective divide)
//	-> calculate and accumulate final diffuse and specular shading

#define iVal		0
#define iValSq 		1
#define iValInv 	2
#define iValInvSq	3


struct sPointLight {
	vec4 pos, worldPos, color, radiusInfo;
};

uniform ubLight {
	sPointLight uPointLight[MAX_LIGHTS];
};

layout (location = 0) out vec4 rtFragColor;

out vec4 vPosition;
out vec4 vNormal;
out vec4 vTexcoord;

out vec4 vPosition_screen;

in vec4 vTexcoord_atlas;

uniform int uCount;
uniform sampler2D uImage00; //Diffuse
uniform sampler2D uImage01; //Specular

uniform sampler2D uImage04; //Texcoord
uniform sampler2D uImage05; //Normal
uniform sampler2D uImage06; //Position
uniform sampler2D uImage07; //depth

uniform mat4 uPB_inv;

void calcPhongPoint(out vec4 diffuseColor, out vec4 specularColor, in vec4 eyeVec,
	in vec4 fragPos, in vec4 fragNrm, in vec4 fragColor,
	in vec4 lightPos, in vec4 lightRadiusInfo, in vec4 lightColor);

void main()
{
	vec4 screenTexcoord = texture(uImage04, vTexcoord_atlas.xy);
	vec4 diffuseSample = texture(uImage00, screenTexcoord.xy);
	vec4 specularSample = texture(uImage01, screenTexcoord.xy);

	vec4 normal = texture(uImage05, vTexcoord_atlas.xy);
	normal = (normal - 0.5) * 2;

	vec4 position_screen = vTexcoord_atlas;
	position_screen.z = texture(uImage07, position_screen.xy).r;



	vec4 position_view = uPB_inv * position_screen;
	position_view = position_view / position_view.w;

	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);

	for (int i = 0; i < uCount; i++) {
		vec4 diffuseColor, specularColor;
		vec4 eyeVec = normalize(vec4(0, 0, 0, 1) - position_view);

		calcPhongPoint(diffuseColor, specularColor, eyeVec,
			position_view, normal, vec4(1.0),
			uPointLight[i].pos, uPointLight[i].radiusInfo, uPointLight[i].color);

		diffuse += diffuseColor;
		specular += specularColor;
	}


	rtFragColor = diffuseSample * diffuse + specularSample * specular;
	rtFragColor.a = diffuseSample.a;
}
