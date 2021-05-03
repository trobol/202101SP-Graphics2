
#version 450

in vec4 vTexcoord_atlas;

uniform sampler2D uTex_dm;
uniform vec4 uColor;

#define SOFT_EDGE_MIN 0.495
#define SOFT_EDGE_MAX 0.50

layout (location = 0) out vec4 rtFragColor;



void main()
{

	float edgeDistance = 0.5;
	float dist = texture(uTex_dm,  vTexcoord_atlas.xy/2).r;
	float edgeWidth = 0.7 * length(vec2(dFdx(dist), dFdy(dist)));
	float opacity = smoothstep(edgeDistance - edgeWidth, edgeDistance + edgeWidth, dist);

	rtFragColor = uColor;
	rtFragColor.a = opacity;
}