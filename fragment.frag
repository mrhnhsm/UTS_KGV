#version 330

out vec4 col;
in vec3 color;

void main()
{
	col = vec4(color, 1.0f);
}
