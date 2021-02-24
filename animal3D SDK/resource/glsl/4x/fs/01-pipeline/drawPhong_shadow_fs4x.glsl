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
	
	drawPhong_shadow_fs4x.glsl
	Output Phong shading with shadow mapping.
*/

#version 450

// ****TO-DO:
// 1) Phong shading
//	-> identical to outcome of last project
// 2) shadow mapping
//	-> declare shadow map texture
//	-> declare shadow coordinate varying
//	-> perform manual "perspective divide" on shadow coordinate
//	-> perform "shadow test" (explained in class)

layout (location = 0) out vec4 rtFragColor;

uniform vec4 uLights_pos[4];
uniform float uLights_radius[4];
uniform vec4 uLights_color[4];
uniform sampler2D uImage00;
uniform vec4 uColor;
uniform int uCount;
uniform sampler2D uShadowMap;

in vec4 vNormal;
in vec2 vTexcoord;
in vec4 vPosition;
in vec4 vShadowcoord;

void main()
{
	vec3 light;
	for(int i = 0; i < 4; i++) {
		
		// from opengl blue book
		vec3 N = vNormal.xyz;
		vec3 L = uLights_pos[i].xyz - vPosition.xyz;
		vec3 V = - vPosition.xyz;

		float dist = length(L);

		// Normalize all three vectors
		N = normalize(N);
		L = normalize(L);
		V = normalize(V);

		vec3 R = reflect(-L, N);

		// source: https://geom.io/bakery/wiki/index.php?title=Point_Light_Attenuation
		// this is allegedly the formula unity uses
		float a = dist/uLights_radius[i] * 5;
		float attenuation = 1.0/ ((a*a) + 1);
		float diffuse = max(dot(N, L), 0) * attenuation;
		float specular = pow(max(dot(R, V), 0.0), 128) * attenuation;
		float ambient = 0;
		light += diffuse + specular * uLights_color[i].rgb ;
	}


	vec4 color = texture2D(uImage00, vTexcoord);
	color *= uColor;
	color.xyz *= light;
	rtFragColor = color;
}
