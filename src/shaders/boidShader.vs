#version 430 core

layout(location = 0) in vec3 position;
//layout(location = 1) in mat4 instanceModel;
//layout(location = 2) in vec3 instanceColor;

out vec3 myColor;

uniform mat4 instanceModel;
uniform vec3 instanceColor;

void main(){
	vec4 actualPos = instanceModel * vec4(position, 1.0f);
	gl_Position = actualPos;
	myColor = instanceColor;
}
