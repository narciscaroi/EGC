#version 330
//iluminare
in vec3 world_position;
in vec3 world_normal;

// TODO: get values from fragment shader
uniform sampler2D texture;
uniform int has_texture;

//iluminare
uniform vec3 punctiform_light_position;
uniform vec3 spot_light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ka;
uniform float material_ks;
uniform int material_shininess;

in vec3 frag_normal;
in vec2 frag_coord;
in vec3 frag_color;
in vec3 frag_position;
in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO: write pixel out color
	if(has_texture == 0) {
		out_color = vec4(frag_color, 1);
	}
	else if(has_texture == 1) {
		out_color = texture2D(texture, texcoord);
	}
}