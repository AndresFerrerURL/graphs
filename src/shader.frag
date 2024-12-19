#version 330

out vec4 fragColor;

in vec3 v_color;

void main(void)
{
	// We're just going to paint the interpolated colour from the vertex shader
	fragColor =  vec4(v_color,1.0);
}
