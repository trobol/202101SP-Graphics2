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
	
	drawPhongNM_fs4x.glsl
	Output Phong shading with normal mapping.
*/

#version 450

#define MAX_LIGHTS 1024

// ****DONE:
//	-> declare view-space varyings from vertex shader
//	-> declare point light data structure and uniform block
//	-> declare uniform samplers (diffuse, specular & normal maps)
//	-> calculate final normal by transforming normal map sample
//	-> calculate common view vector
//	-> declare lighting sums (diffuse, specular), initialized to zero
//	-> implement loop in main to calculate and accumulate light
//	-> calculate and output final Phong sum

struct sPointLight {
	vec4 pos, worldPos, color, radiusInfo;
};

uniform ubLight {
	sPointLight uPointLight[MAX_LIGHTS];
};

uniform sampler2D uImage00; //Diffuse
uniform sampler2D uImage01; //Specular
uniform sampler2D uImage02; //normal
uniform sampler2D uImage03; //height

uniform int uCount;
uniform mat4 uPB_inv;

layout (location = 0) out vec4 rtFragColor;

// location of viewer in its own space is the origin
const vec4 kEyePos_view = vec4(0.0, 0.0, 0.0, 1.0);

// declaration of Phong shading model
//	(implementation in "utilCommon_fs4x.glsl")
//		param diffuseColor: resulting diffuse color (function writes value)
//		param specularColor: resulting specular color (function writes value)
//		param eyeVec: unit direction from surface to eye
//		param fragPos: location of fragment in target space
//		param fragNrm: unit normal vector at fragment in target space
//		param fragColor: solid surface color at fragment or of object
//		param lightPos: location of light in target space
//		param lightRadiusInfo: description of light size from struct
//		param lightColor: solid light color
void calcPhongPoint(
	out vec4 diffuseColor, out vec4 specularColor,
	in vec4 eyeVec, in vec4 fragPos, in vec4 fragNrm, in vec4 fragColor,
	in vec4 lightPos, in vec4 lightRadiusInfo, in vec4 lightColor
);

in vec4 vPosition;
in vec4 vNormal;
in vec4 vTexcoord;
in vec4 vTangent;
in vec4 vBiTangent;



void main()
{
	vec4 diffuseSample = texture(uImage00, vTexcoord.xy);
	vec4 specularSample = texture(uImage01, vTexcoord.xy);

	// from bluebook
	vec4 normal = normalize(vNormal);
	vec4 tangent = normalize(vTangent);
	vec4 bitangent = normalize(vBiTangent);


	mat4 tangentBasis = mat4(tangent, bitangent, normal, vec4(0, 0, 0, 1));

	
	vec4 normal_sample = texture(uImage02, vTexcoord.xy);
	normal_sample = (normal_sample - 0.5) * 2;
	vec4 normal_view = tangentBasis * normal_sample;

	vec4 position_screen = vPosition;
	position_screen.z = texture(uImage03, position_screen.xy).r;

	
	vec4 position_view = uPB_inv * vPosition;
	position_view = position_view / position_view.w;

	vec4 diffuse = vec4(0.0);
	vec4 specular = vec4(0.0);

	for (int i = 0; i < uCount; i++) {
		vec4 diffuseColor, specularColor;
		vec4 eyeVec = normalize(vec4(0, 0, 0, 1) - position_view);

		calcPhongPoint(diffuseColor, specularColor, eyeVec,
			position_view, normal_view, vec4(1.0),
			uPointLight[i].pos, uPointLight[i].radiusInfo, uPointLight[i].color);

		diffuse += diffuseColor;
		specular += specularColor;
	}


	rtFragColor = diffuseSample * diffuse + specularSample * specular;
	rtFragColor.a = diffuseSample.a;
}
