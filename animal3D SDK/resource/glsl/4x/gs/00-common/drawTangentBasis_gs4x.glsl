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
	
	drawTangentBases_gs4x.glsl
	Draw tangent bases of vertices and/or faces, and/or wireframe shapes, 
		determined by flag passed to program.
*/

#version 450

// ****DONE: 
//	-> declare varying data to read from vertex shader
//		(hint: it's an array this time, one per vertex in primitive)
//	-> use vertex data to generate lines that highlight the input triangle
//		-> wireframe: one at each corner, then one more at the first corner to close the loop
//		-> vertex tangents: for each corner, new vertex at corner and another extending away 
//			from it in the direction of each basis (tangent, bitangent, normal)
//		-> face tangents: ditto but at the center of the face; need to calculate new bases
//	-> call "EmitVertex" whenever you're done with a vertex
//		(hint: every vertex needs gl_Position set)
//	-> call "EndPrimitive" to finish a new line and restart
//	-> experiment with different geometry effects

// (2 verts/axis * 3 axes/basis * (3 vertex bases + 1 face basis) + 4 to 8 wireframe verts = 28 to 32 verts)
#define MAX_VERTICES 32

layout (triangles) in;

layout (line_strip, max_vertices = MAX_VERTICES) out;

uniform mat4 uP;

in vbVertexData {
	mat4 vTangentBasis_view;
	vec4 vTexcoord_atlas;
} vVertexData[];

out vec4 vColor;


void emitLine(vec4 start, vec4 end) {
		gl_Position = start;
		EmitVertex();
		gl_Position = end;
		EmitVertex();
		EndPrimitive();
}

void emitRayProj(vec4 start, vec4 dir) {
	emitLine(uP * start, uP * (start + normalize(dir) * 0.1));
}


void main()
{
	// triangle
	for( int i = 0; i < 3; i++) {
		vColor = vec4(0, 0, 0, 1);
		vColor[i] = 1;
		emitLine(gl_in[i].gl_Position, 
		         gl_in[(i+1)%3].gl_Position);
	}

	
	vec4 p0 = vVertexData[0].vTangentBasis_view[3];
	vec4 p1 = vVertexData[1].vTangentBasis_view[3];
	vec4 p2 = vVertexData[2].vTangentBasis_view[3];

	vColor = vec4(1, 0.5, 1, 1);
	emitRayProj( p0, vVertexData[0].vTangentBasis_view[0]);

	vColor = vec4(1, 1, 0.5, 1);
	emitRayProj( p0, vVertexData[0].vTangentBasis_view[1]);

	vColor = vec4(0.5, 1, 1, 1);
	emitRayProj( p0, vVertexData[0].vTangentBasis_view[2]);
	
	
	// face tangent bitan normal pos
	vec2 tex0 = vVertexData[0].vTexcoord_atlas.xy;
	vec2 tex1 = vVertexData[1].vTexcoord_atlas.xy;
	vec2 tex2 = vVertexData[2].vTexcoord_atlas.xy;


	vec4 center = (p0 + p1 + p2) / 3;

	vec4 dp1 = p1 - p0;
	vec4 dp2 = p2 - p0;

	vec4 N = vec4(cross(dp1.xyz, dp2.xyz), 0);

	float du1 = tex1.x - tex0.x;
	float du2 = tex2.x - tex0.x;

	float dv1 = tex1.y - tex0.y;
	float dv2 = tex2.y - tex0.y;

	mat2x3 TB = mat2x3(dp1.xyz, dp2.xyz) * inverse(mat2(du1, dv1, du2, dv2));

	vec4 T = vec4(TB[0], 0);
	vec4 B = vec4(TB[1], 0);


	vColor = vec4(1, 0.5, 1, 1);
	emitRayProj( center, N);

	vColor = vec4(1, 1, 0.5, 1);
	emitRayProj( center, T);

	vColor = vec4(0.5, 1, 1, 1);
	emitRayProj( center, B);
}
