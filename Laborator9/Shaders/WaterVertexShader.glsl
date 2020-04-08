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
out vec3 worldPos;

void main()
{

	texcoord = v_texture_coord;
	float phase =  v_position.x + v_position.z+ v_position.y;
	phase = phase * 12;
	vec3 pos = v_position + 0.05 * v_normal * sin( 2 * 3.14 *time + phase);
	worldPos = (Model * vec4(pos, 1.0)).xyz;
	gl_Position = Projection * View * vec4(worldPos,1);
}
 