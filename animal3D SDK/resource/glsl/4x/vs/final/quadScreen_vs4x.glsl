#version 450

layout (location = 0) in vec4 aPosition;

flat out int vVertexID;
flat out int vInstanceID;

void main()
{
	gl_Position = aPosition;

	vVertexID = gl_VertexID;
	vInstanceID = gl_InstanceID;
}
