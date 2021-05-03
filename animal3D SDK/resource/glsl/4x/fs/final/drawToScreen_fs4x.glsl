
#version 450

in vec4 vTexcoord_atlas;

uniform sampler2D uTex_dm;
uniform vec4 uColor;

#define SOFT_EDGE_MIN 0.48
#define SOFT_EDGE_MAX 0.5

layout (location = 0) out vec4 rtFragColor;

void main()
{
	// DUMMY OUTPUT: all fragments are OPAQUE YELLOW
	//rtFragColor = vec4(1.0, 1.0, 0.0, 1.0);
	vec4 sample_dm = texture(uTex_dm, vTexcoord_atlas.xy);

	rtFragColor = uColor;
	//rtFragColor.a = 1;
	rtFragColor.a = smoothstep(SOFT_EDGE_MIN, SOFT_EDGE_MAX, sample_dm.r);
}