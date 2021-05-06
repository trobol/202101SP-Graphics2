
#version 450

in vec4 vTexcoord_atlas;
in vec3 vColor;


uniform sampler2D uTex_dm;

#define SOFT_EDGE_MIN 0.4
#define SOFT_EDGE_MAX 0.55



// SOURCES:
// Improved Alpha-Tested Magnification for Vector Textures and Special Effects
// Chris Green
// Valve
// https://steamcdn-a.akamaihd.net/apps/valve/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf
//
// 2D Shape Rendering by Distance Fields   
// Stefan Gustavson
// Linköping University Electronic Press
// https://www.diva-portal.org/smash/get/diva2:618269/FULLTEXT02.pdf


layout (location = 0) out vec4 rtFragColor;



// 'threshold ' is constant , 'distance ' is smoothly varying
float aastep ( float threshold , float distance ) {
	float afwidth = 0.7 * length ( vec2 ( dFdx ( distance ) , dFdy ( distance )) );
	return smoothstep ( threshold - afwidth , threshold + afwidth , distance );
}

void main()
{
	float edgeDistance = 0.5;
	float dist = texture(uTex_dm,  vTexcoord_atlas.xy).r;
	

	float opacity = aastep(0.5, dist);

	rtFragColor = vec4(vColor, opacity);

	//rtFragColor = vec4(vTexcoord_atlas.xy, 0, 1);
	//rtFragColor = texture(uTex_dm,  vTexcoord_atlas.xy);
}