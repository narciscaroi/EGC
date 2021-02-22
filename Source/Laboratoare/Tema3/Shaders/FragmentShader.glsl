#version 330
// TODO: get values from fragment shader
uniform sampler2D texture;
uniform int has_texture;
uniform int isSphere;

//iluminare
uniform vec3 punctiform_light_position;
uniform vec3 spot_light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ka;
uniform float material_ks;
uniform int material_shininess;

in vec3 world_position;
in vec3 world_normal;
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
		vec3 tex_color = vec3(texture2D(texture, texcoord));
		//iluminare
		vec3 L = normalize(spot_light_position - world_position);
		vec3 L2 = normalize(punctiform_light_position - world_position);
		vec3 V = normalize(eye_position - world_position);
		vec3 R = reflect(L, world_normal);
		vec3 R2 = reflect(L2, world_normal);

		// ambient light component
		float ambient_light =  material_ka * 0.5f;

		// compute diffuse light component
		float diffuse_light_spot = material_kd * max(dot(L, world_normal), 0);

		// compute specular light component
		float specular_light_spot = 0;
		
		if(diffuse_light_spot > 0) {
			specular_light_spot = material_ks * pow(max(dot(R,V),0), material_shininess);
		}

		// spotlight
		vec3 spot_light = vec3(0,0,0);
		float cut_off = radians(45);
		float sLight = dot(-L, vec3(0,0,-1));
		if(sLight > cos(cut_off)) {
			float dist = distance(spot_light_position, world_position);
			float linearAtt = (sLight - cos(cut_off)) / (1 - cos(cut_off));
			float lightAttFactor = linearAtt * linearAtt;
			spot_light = vec3(0.882f, 0.886f, 1) * lightAttFactor * diffuse_light_spot + specular_light_spot * (1/dist);
		}

		//punctiform
		float diffuse_light_punctiform = material_kd * max(dot(L2, world_normal), 0);
		float specular_light_punctiform = 0;
		
		if(diffuse_light_punctiform > 0) {
			specular_light_punctiform = material_ks * pow(max(dot(R2,V),0), material_shininess);
		}

		float distancee = distance(punctiform_light_position, world_position);
		float attenuation_factor = (-0.001f * distancee + 1);
		vec3 punctiform_light = attenuation_factor * vec3(0.96f, 0.85f, 0.474f) * (diffuse_light_punctiform + specular_light_punctiform);

		if(isSphere == 0) {
			out_color = vec4(tex_color * ambient_light + spot_light + punctiform_light, 1);
		}
		else {
			float emisiva = 0.1f;
			out_color = vec4(tex_color * ambient_light + spot_light + punctiform_light + emisiva, 1);
		}
	}
}