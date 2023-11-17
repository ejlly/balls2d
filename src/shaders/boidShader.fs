#version 430 core

in vec3 myColor;

out vec4 color;


/*
uniform vec3 lightPos; 
uniform vec3 lightColor;
*/

void main(){

	vec3 lightColor = vec3(1.0f);

	//ambient
	float ambientStrength = .7f;
	vec3 ambient = ambientStrength * lightColor;

    vec3 diffuse = vec3(.0f, .0f, 0.f);
            
	vec3 result = (ambient + diffuse) * myColor;
		
	color = vec4(myColor, 1.f);
}
