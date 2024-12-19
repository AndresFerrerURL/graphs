#version 330
 
in vec3 a_vertex;
in vec3 a_color;

out vec3 v_color;

void main()
{
	// position of the vertex
	gl_Position = vec4( a_vertex , 1.0 );

	// pass the colour to the fragment shader
	v_color = a_color;
}

