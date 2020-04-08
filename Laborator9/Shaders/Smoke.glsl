#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;


out vec2 texcoord;

void main()
{

	texcoord = v_texture_coord;
	if(time >=0 )
		texcoord = vec2(sin(texcoord.x * time * 20+ 18), texcoord.y);
	vec3 pos = vec3(sin(v_position.x * time + 10 ),v_position.y ,sin((v_position.x+v_position.y)*time+0.4)); 
	gl_Position = Projection * View * Model * vec4(pos, 1.0);
}
 