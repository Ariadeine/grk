#version 430 core

uniform vec3 objectColor;
uniform sampler2D uni; 
in vec3 interpNormal;
in vec2 Coor;

void main()
{
	vec4 textureColor = texture2D(uni, Coor); 
	gl_FragColor = vec4(textureColor.rgb, 1.0);
}
