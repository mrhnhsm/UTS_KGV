#version 330

uniform float vSize;

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;

out vec3 color;

void main()
{
	gl_Position = vec4(vPos.x + vPos.x * vSize, vPos.y + vPos.y * vSize, vPos.z + vPos.z * vSize, 1.0);
	color = vColor;
}
