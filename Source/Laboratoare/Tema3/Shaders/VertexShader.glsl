#version 330
// TODO: get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 colorSet;

// TODO: output values to fragment shader
//iluminare
out vec3 world_normal;
out vec3 world_position;
out vec3 frag_position;
out vec3 frag_color;
out vec2 frag_coord;
out vec3 frag_normal;
out vec2 texcoord;

void main()
{
	//iluminare
	world_position = (Model * vec4(v_position, 1)).xyz;
	//world_normal = normalize(vec3(Model * vec4(v_normal,0)));
	world_normal = normalize( mat3(Model) * normalize(v_normal) );

	// TODO: send output to fragment shader
	texcoord = v_texture_coord;
	frag_color = colorSet;

	// TODO: compute gl_Position
	gl_Position = Projection * View * Model * vec4(v_position, 1.0);

}
