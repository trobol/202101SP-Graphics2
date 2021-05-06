

#version 450

uniform sampler2D uTex_dm;

layout (location = 0) out vec4 rtFragColor;

in vec3 vColor;
in vec4 vTexcoord_atlas;

void main()
{

	float mono = texture(uTex_dm,  vTexcoord_atlas.xy).r;
	rtFragColor = vec4(vColor * mono, mono);
	
}