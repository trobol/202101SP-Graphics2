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
	
	drawLambert_fs4x.glsl
	Output Lambertian shading.
*/

#version 450

// ****TO-DO: 
//	-> declare varyings to receive lighting and shading variables
//	-> declare lighting uniforms
//		(hint: in the render routine, consolidate lighting data 
//		into arrays; read them here as arrays)
//	-> calculate Lambertian coefficient
//	-> implement Lambertian shading model and assign to output
//		(hint: coefficient * attenuation * light color * surface color)
//	-> implement for multiple lights
//		(hint: there is another uniform for light count)

layout (location = 0) out vec4 rtFragColor;

in vec4 vNormal;
in vec2 vTexcoord;
in vec4 vPosition;

uniform vec4 uLights_pos[4];
uniform float uLights_radius[4];
uniform sampler2D uImage00;
uniform vec4 uColor;

void main()
{
	float brightness = 0;
	for(int i = 0; i < 4; i++) {
		
		vec4 n_norm = normalize(vNormal);
		vec4 l = uLights_pos[i] - vPosition;
		float dist = length(l);
		vec4 l_norm = l / dist;
		brightness += max(dot(n_norm, l_norm), 0) * 0.2;
	}

	
	rtFragColor = uColor * texture2D(uImage00, vTexcoord) * brightness;
}
