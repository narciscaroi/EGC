#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;

void main()
{
	// TODO: compute world space vectors
	vec3 worldPos = (Model * vec4(v_position, 1)).xyz;
	vec3 worldNormal = normalize(mat3(Model) * normalize(v_normal));

	vec3 L = normalize(light_position - worldPos);
	vec3 V = normalize(eye_position - worldPos);
	vec3 H = normalize(L+V);
	vec3 R = normalize(reflect(-L, worldNormal));

	// TODO: define ambient light component
	float ambient_light = material_kd * 0.25;

	// TODO: compute diffuse light component
	//float diffuse_light = 0;
	float diffuse_light = material_kd * max(dot(worldNormal,L),0);

	// TODO: compute specular light component
	float specular_light = 0;

	if (diffuse_light > 0)
	{
		specular_light = material_ks * pow(max(dot(normalize(V), R), 0), material_shininess);
	}

	// TODO: compute light
	float distance = distance(light_position, v_position);
	float attenuation_factor = 1 / (distance * distance);
	float light = ambient_light + attenuation_factor *  (specular_light + diffuse_light);

	// TODO: send color light output to fragment shader
	color = object_color * light;

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
