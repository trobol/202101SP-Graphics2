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
	
	postDeferredLightingComposite_fs4x.glsl
	Composite results of light pre-pass in deferred pipeline.
*/

#version 450

// ****DONE:
//	-> declare samplers containing results of light pre-pass
//	-> declare samplers for texcoords, diffuse and specular maps
//	-> implement Phong sum with samples from the above
//		(hint: this entire shader is about sampling textures)

in vec4 vTexcoord_atlas;

uniform sampler2D uImage00;
uniform sampler2D uImage01;

uniform sampler2D uImage04; // texcoords
uniform sampler2D uImage05; // diffuse
uniform sampler2D uImage06; // specular

layout (location = 0) out vec4 rtFragColor;

void main()
{
	vec4 screen_texcoord = texture(uImage04, vTexcoord_atlas.xy);
	vec4 diffuseSample = texture(uImage00, screen_texcoord.xy);
	vec4 specularSample = texture(uImage01, screen_texcoord.xy);

	vec4 diffuseLight = texture(uImage05, vTexcoord_atlas.xy);
	vec4 specularLight = texture(uImage06, vTexcoord_atlas.xy);

	rtFragColor = diffuseLight * diffuseSample + specularLight * specularSample;
	rtFragColor.a = diffuseSample.a;
}
