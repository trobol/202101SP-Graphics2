

#version 450

layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aPos;
layout (location = 2) in vec2 aSize;
layout (location = 3) in vec2 aTexcoord_start;
layout (location = 4) in vec2 aTexcoord_size;
layout (location = 5) in vec3 aColor;


uniform mat4 uP;
uniform vec2 uSize;

out vec4 vTexcoord_atlas;
out vec3 vColor;

flat out int vVertexID;
flat out int vInstanceID;
	



void main()
{
	
	// because the quad origin is in the upper left, 
	// we have to flip the y axis for texture space
	vec2 texSpacePos = vec2(aPosition.x, 1-aPosition.y);

	vTexcoord_atlas = vec4(aTexcoord_start + (texSpacePos * aTexcoord_size), 0, 0);
	vColor = aColor;
	//vTexcoord_atlas = vec4(aPosition, 0, 0);
	vec2 pos = aPos + 200 + (aPosition * aSize);

	gl_Position = uP * vec4(pos, 0, 1);


	vVertexID = gl_VertexID;
	vInstanceID = gl_InstanceID;
}
