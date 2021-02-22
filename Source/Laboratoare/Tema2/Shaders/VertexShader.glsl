#version 330

// TODO: get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_color;
layout(location = 2) in vec2 v_coord;
layout(location = 3) in vec3 v_normal;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 colorSet;

// TODO: output values to fragment shader
out vec3 frag_position;
out vec3 frag_color;
out vec2 frag_coord;
out vec3 frag_normal;

void main()
{
	// TODO: send output to fragment shader
	frag_position = v_position;
	frag_color = colorSet;
	frag_coord = v_coord;
	frag_normal = v_normal;

	// TODO: compute gl_Position
	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
