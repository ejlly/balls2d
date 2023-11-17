#version 430 core

layout(location = 0) in vec3 position;

out vec3 myColor;

uniform vec3 color;
uniform mat4 model;

void main(){
	vec4 actualPos = model * vec4(position, 1.0f);
	gl_Position = actualPos;
	myColor = color;
}
