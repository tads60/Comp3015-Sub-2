#version 460

#define PI 3.14159265

in vec4 Position;
in vec3 Normal;
in vec3 Vec;
layout (binding = 0) uniform samplerCube SkyBoxTex;
layout (location = 0) out vec4 FragColor;
layout (binding = 1) uniform sampler2D NoiseTex;

//
//uniform vec3 Colour;
//uniform bool isSkyBox;
uniform float noiseScale = 1.0;
uniform float noiseStrength = 0.1;

uniform vec4 SkyColour = vec4(0.3, 0.3, 0.9, 1.0);
uniform vec4 CloudColour = vec4(1.0, 1.0, 1.0, 1.0);

in vec2 TexCoord;




uniform struct LightInfo{
	vec4 Position;
	vec3 La; //ambient
	vec3 Ld; //diffuse
	vec3 Ls; //specular
} lights[3];

uniform struct MaterialInfo{
	vec3 Ka; //Ambient
	vec3 Kd; //diffuse
	vec3 Ks; //specular
	float Shininess; //shininess factor
} Material;

vec3 phong(int light, vec3 n,vec4 pos){

	vec3 ambient = lights[light].La*Material.Ka;

	vec3 s = normalize(vec3(lights[light].Position-pos));
	float sDotN = max(dot(s,n),0.0);
	vec3 diffuse = lights[light].Ld*Material.Kd*sDotN;
	vec3 spec = vec3(0.0);
	if(sDotN>0.0)
	{
		vec3 v = normalize(-pos.xyz);
		vec3 r = reflect(-s,n);
		spec = lights[light].Ls*Material.Ks*pow(max(dot(r,v),0.0),Material.Shininess);
	}
	return ambient + diffuse + spec;

}

vec3 blinnphong(int light, vec3 n, vec4 pos)
{	
	vec3 ambient = lights[light].La*Material.Ka;

	vec3 s = normalize(vec3(lights[light].Position-(pos*lights[light].Position.w)));

	float sDotN = max(dot(s,n),0.0);
	vec3 diffuse = lights[light].Ld*Material.Kd*sDotN;
	vec3 spec = vec3 (0.0);
	if(sDotN > 0.0)
	{
		vec3 v = normalize(-pos.xyz);
		vec3 h = normalize(v+s);
		spec = lights[light].Ls*Material.Ks*pow(max(dot(h,n), 0.0), Material.Shininess);
	}
	return ambient + diffuse + spec;


}



void main() {

	vec3 Colour = vec3(0.0);
	for (int i = 0; i < 3; i++)
		Colour+=blinnphong(i, Normal, Position);

	//
	vec3 noiseVec = normalize(Vec) * noiseScale;
	float noiseVal = texture(NoiseTex, noiseVec.xy).r;
	noiseVal = (noiseVal - 0.5) * 2.0;
	
	if (noiseStrength > 0.0) {
		
		vec3 noiseColor = vec3(0.5, 0.5, 0.5) + noiseVal * noiseStrength;
		vec3 finalColor = mix(SkyColour.rgb, CloudColour.rgb, noiseColor);
		FragColor = vec4(Colour + finalColor, 1.0);
	} else {
	
		//vec3 texColour = texture(SkyBoxTex, normalize(Vec)).rgb;
		//texColour += noiseVal * noiseStrength;
		FragColor = vec4(Colour, 1.0);
	}
}
