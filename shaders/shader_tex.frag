#version 430 core

uniform vec3 objectColor;
uniform vec3 lightDir;
uniform vec3 skyColor;
uniform sampler2D uni; //+

in vec3 interpNormal;
in vec2 Coor;
in float visibility;

void main()
{

	vec2 modifiedTexCoor = vec2(Coor.x, 1.0 - Coor.y);
	vec4 rgba = texture2D(uni, modifiedTexCoor).rgba;
	if(rgba.a < 0.1) discard;

	vec3 normal = normalize(interpNormal);
	float diffuse = max(dot(normal, -lightDir), 0.0)*2;

	gl_FragColor = rgba * diffuse;
	gl_FragColor = mix(vec4(skyColor, 1.0), gl_FragColor, visibility);
	
	//vec4 textureColor = texture2D(uni, Coor); 
	//gl_FragColor = vec4(textureColor.rgb * diffuse, 1.0);
}
