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
	
	drawGBuffers_fs4x.glsl
	Output g-buffers for use in future passes.
*/

#version 450

// ****DONE:
//	-> declare view-space varyings from vertex shader
//	-> declare MRT for pertinent surface data (incoming attribute info)
//		(hint: at least normal and texcoord are needed)
//	-> declare uniform samplers (at least normal map)
//	-> calculate final normal
//	-> output pertinent surface data

layout (location = 0) out vec4 rtTexcoord;
layout (location = 1) out vec4 rtNormal;
layout (location = 3) out vec4 rtPosition;

in vec4 vPosition;
in vec4 vNormal;
in vec4 vTexcoord;
in vec4 vTangent;
in vec4 vBiTangent;

in vec4 vPosition_screen;

uniform sampler2D uImage02;
uniform sampler2D uImage03; // height map

float height_scale = 0.0005;
void main()
{


	// from bluebook
	vec4 normal = normalize(vNormal);
	vec4 tangent = normalize(vTangent);
	vec4 bitangent = normalize(vBiTangent);

	mat4 tangentBasis = mat4(tangent, bitangent, normal, vec4(0, 0, 0, 1));

	vec4 normal_sample = texture(uImage02, vTexcoord.xy);
	normal_sample = (normal_sample - 0.5) * 2;
	vec4 normal_view = tangentBasis * normal_sample;


	
	// from https://learnopengl.com/Advanced-Lighting/Parallax-Mapping
    float height = texture(uImage03, vTexcoord.xy).r;    
    vec2 tex_offset = vPosition.xy * (height * height_scale);
	
	
	rtNormal = vec4((normal_view.xyz * 0.5) + 0.5, 1.0);
	rtTexcoord = vec4(vTexcoord.xy - tex_offset, vTexcoord.zw);
	rtPosition = vPosition / vPosition.w;
}
