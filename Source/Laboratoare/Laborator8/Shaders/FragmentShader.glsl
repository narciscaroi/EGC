#version 330

// TODO: get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;
uniform int modeSpotLightt;
uniform float cutOffAnglee;

layout(location = 0) out vec4 out_color;

void main()
{
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L+V);
	vec3 R = reflect(L, world_normal);

	// TODO: define ambient light component
	//float ambient_light = 0.25;
	float ambient_light = material_kd * 0.25;

	// TODO: compute diffuse light component
	//float diffuse_light = 0;
	float diffuse_light = material_kd * max(dot(L, world_normal), 0);

	// TODO: compute specular light component
	float specular_light = 0;

	if (diffuse_light > 0)
	{
		float specular_light = material_ks * 
						pow(max(dot(R,V),0), material_shininess);
	}

	// TODO: compute light
	float light = 0;

	if(modeSpotLightt == 1) {
		float cutOff = radians(cutOffAnglee);
		float spotLight = dot(-L, light_direction);
		float spotLightLimit = cos(cutOff);
		if(spotLight > spotLightLimit) {
			float linearAtt = (spotLight - spotLightLimit) / (1 - spotLightLimit);
			float lightAttFactor = linearAtt * linearAtt;
			light = ambient_light + lightAttFactor * (diffuse_light + specular_light);
		}
		else {
			light = ambient_light;
		}
	}
	else {
		float distance = distance(light_position, world_position);
		float attenuation_factor = 1 / (distance * distance);
		light = ambient_light + attenuation_factor * (diffuse_light + specular_light);
	}


	// TODO: write pixel out color
	vec3 clr = object_color * light;
	out_color = vec4(clr, 1);
}