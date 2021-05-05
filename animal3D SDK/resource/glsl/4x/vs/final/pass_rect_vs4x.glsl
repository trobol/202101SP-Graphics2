

#version 450

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aPos;
layout (location = 2) in vec2 aSize;
layout (location = 3) in vec2 aTexcoord_start;
layout (location = 4) in vec2 aTexcorrd_size;
layout (location = 5) in vec3 aColor;


uniform mat4 uP;
uniform vec2 uSize;

out vec4 vTexcoord_
;
out vec3 vColor;

flat out int vVertexID;
flat out int vInstanceID;
	


float l = 0; //left
float r = uSize.x; // right

float t = 0; // top
float b = uSize.y; //bottom
	
mat4 ortho = mat4( 2 / (r - l), 0, 0, -(r+l)/(r-l),
				   0, 2 / (t - b), 0, -(t+b)/(t-b),
				   0, 0, -2 / 1, -1,
				   0, 0, 0, 1);

void main()
{
	// DUMMY OUTPUT: directly assign input position to output position
	//gl_Position = vec4(aPos + (aPosition.xy * aScale), 1, 0);
	
	vTexcoord_atlas = vec4(aTexcoord_start + (aPosition * aTexcorrd_size), 0, 0);
	vColor = aColor;
	//vTexcoord_atlas = vec4(aPosition, 0, 0);
	vec2 pos = aPos  + (aPosition * aSize);

	gl_Position = uP * vec4(pos, 0, 1);


	vVertexID = gl_VertexID;
	vInstanceID = gl_InstanceID;
}
