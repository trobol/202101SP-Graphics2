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

// ****DONE:
// 1) Phong shading
//	-> identical to outcome of last project
// 2) shadow mapping
//	-> declare shadow map texture
//	-> declare shadow coordinate varying
//	-> perform manual "perspective divide" on shadow coordinate
//	-> perform "shadow test" (explained in class)

layout (location = 0) out vec4 rtFragColor;

uniform sampler2D uTex_dm;

uniform vec4 uColor;
uniform int uCount;
uniform sampler2D uTex_shadow;

struct sPointLight {
	vec4 viewPos, worldPos, color;
	float radius, radiusSq, radiusInv, radiusInvSq;
};

uniform ubLight {
	sPointLight uLights[8];
};


in vec4 vNormal;
in vec4 vPosition;

in vec2 vTexcoord;
in vec4 vShadowCoord;


void main()
{
	vec3 tex_color = texture(uTex_dm, vTexcoord).rgb;
	vec3 light;
	for(int i = 0; i < uCount; i++) {
		
		// from opengl blue book
		vec3 N = vNormal.xyz;
		vec3 L = uLights[i].viewPos.xyz - vPosition.xyz;
		vec3 V = - vPosition.xyz;

		float dist = length(L);

		// Normalize all three vectors
		N = normalize(N);
		L = normalize(L);
		V = normalize(V);

		vec3 R = reflect(-L, N);

		// source: https://geom.io/bakery/wiki/index.php?title=Point_Light_Attenuation
		// this is allegedly the formula unity uses
		float a = dist/uLights[i].radius * 3;
		float attenuation = 1.0/ ((a*a) + 1);
		float diffuse = max(dot(N, L), 0)  * attenuation;
		float specular = pow(max(dot(R, V), 0.0), 128) * attenuation;
		float ambient = 0;
		light += (diffuse * uLights[i].color.rgb * tex_color )+ (specular * uLights[i].color.rgb * tex_color);
	}


	vec4 shadowCoord = vShadowCoord / vShadowCoord.w;
	vec4 shadowSample = texture(uTex_shadow, shadowCoord.xy);
	float shadow = float(shadowSample.r > shadowCoord.z - 0.0001);

	light.xyz *= shadow;
	rtFragColor = vec4( light, 1);

}
