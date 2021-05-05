

#version 450

uniform vec4 uColor;

layout (location = 0) out vec4 rtFragColor;

in vec4 vColor;
in vec4 vTexcoord_atlas;

void main()
{
	rtFragColor = vColor;
	
	rtFragColor = vec4(vTexcoord_atlas.xy, 0, 1);
}