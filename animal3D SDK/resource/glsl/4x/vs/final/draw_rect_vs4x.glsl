

#version 450

#define MAX_RECTS 256

layout (location = 0) in vec4 aPosition;

struct sRect {
	vec2 pos, scale;
	vec2 tex_coord, tex_scale;
};

uniform sRect[MAX_RECTS] uRects 

flat out int vVertexID;
flat out int vInstanceID;

void main()
{
	// DUMMY OUTPUT: directly assign input position to output position
	gl_Position = aPosition;

	vVertexID = gl_VertexID;
	vInstanceID = gl_InstanceID;
}
