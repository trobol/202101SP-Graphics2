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
	
	drawPhongPointLight_fs4x.glsl
	Output Phong shading components while drawing point light volume.
*/

#version 450

#define MAX_LIGHTS 1024

// ****DONE:
//	-> declare biased clip coordinate varying from vertex shader
//	-> declare point light data structure and uniform block
//	-> declare pertinent samplers with geometry data ("g-buffers")
//	-> calculate screen-space coordinate from biased clip coord
//		(hint: perspective divide)
//	-> use screen-space coord to sample g-buffers
//	-> calculate view-space fragment position using depth sample
//		(hint: same as deferred shading)
//	-> calculate final diffuse and specular shading for current light only

flat in int vInstanceID;

struct sPointLight {
	vec4 pos, worldPos, color, radiusInfo;
};

uniform ubLight {
	sPointLight uPointLight[MAX_LIGHTS];
};

layout (location = 0) out vec4 rtFragDiffuse;
layout (location = 1) out vec4 rtFragSpecular;

uniform sampler2D uImage04;	//normal
uniform sampler2D uImage05;	//depth
uniform mat4 uPB_inv;
uniform vec4 uColor;

in vec4 vPosition_screen;
void calcPhongPoint(out vec4 diffuseColor, out vec4 specularColor, in vec4 eyeVec,
	in vec4 fragPos, in vec4 fragNrm, in vec4 fragColor,
	in vec4 lightPos, in vec4 lightRadiusInfo, in vec4 lightColor);

void main()
{
	vec4 position_screen = vPosition_screen / vPosition_screen.w;
	position_screen.z = texture(uImage05, position_screen.xy).r;

	vec4 normal = texture(uImage04, position_screen.xy);
	normal = (normal - 0.5) * 2;


	vec4 position_view = uPB_inv * position_screen;
	position_view = position_view / position_view.w;

	vec4 eyeVec = normalize(vec4(0, 0, 0, 1) - position_view);
	
	vec4 diffuseColor, specularColor;
	int i = vInstanceID;

	calcPhongPoint(diffuseColor, specularColor, eyeVec,
		position_view, normal, vec4(1),
		uPointLight[i].pos, uPointLight[i].radiusInfo, uPointLight[i].color);


	rtFragDiffuse = diffuseColor;
	rtFragSpecular = specularColor;
}
