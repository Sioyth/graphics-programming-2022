#version 330 core

uniform vec3 camPosition; // so we can compute the view vector
out vec4 FragColor; // the output color of this fragment

// TODO exercise 5.4 setup the 'uniform' variables needed for lighting
// light uniforms
uniform vec3 light1Position;
uniform vec3 light1Color;
uniform vec3 light2Position;
uniform vec3 light2Color;
uniform float diffuseReflectance;

// material properties
uniform vec3 ambientLightColor;
uniform float ambientReflectance; 
uniform vec3 reflectionColor;

uniform float specularReflectance;
uniform float specularExponent;
// TODO exercise 5.4 add the 'in' variables to receive the interpolated Position and Normal from the vertex shader
in vec4 Pos;
in vec3 Normal;

void main()
{

   // TODO exercise 5.4 - phong shading (i.e. Phong reflection model computed in the fragment shader)
   // ambient component

   // TODO 5.1 ambient
   vec3 Ra = ambientLightColor * ambientReflectance * reflectionColor;

   // TODO 5.2 diffuse
   vec3 L = normalize(light1Position - vec3(Pos.xyz));
   float angle = dot(Normal, L);
   
   vec3 Rd = light1Color * diffuseReflectance * angle;
   // TODO 5.3 specular
   //vec3 r = 2* N * (angle) - L;
   vec3 H = normalize(L + camPosition);

   vec3 Rs = light1Color * specularReflectance * pow(dot(Normal, H), specularExponent);
   
   // Attenuation
   vec3 p = vec3(Pos.xyz);
   
   float d = length(light1Position - p); 
   float attenuation = 1 / pow(d, 2);


   // TODO exercise 5.6 - multiple lights, compute diffuse, specular and attenuation of light 2

	// TODO 5.1 ambient
   vec3 Ra2 = ambientLightColor * ambientReflectance * reflectionColor;

   // TODO 5.2 diffuse
   vec3 L2 = normalize(light1Position - vec3(Pos.xyz));
   float angle2 = dot(Normal, L2);
   
   vec3 Rd2 = light2Color * diffuseReflectance * angle2;
   // TODO 5.3 specular
   //vec3 r = 2* N * (angle) - L;
   vec3 H2 = normalize(L2 + camPosition);

   vec3 Rs2 = light2Color * specularReflectance * pow(dot(Normal, H2), specularExponent);
   
   // Attenuation
   vec3 p2 = vec3(Pos.xyz);
   
   float d2 = length(light2Position - p2); 
   float attenuation2 = 1 / pow(d2, 2);

   // TODO compute the final shaded color (e.g. add contribution of the attenuated lights 1 and 2)

   //vec3 R2 = Ra2 + attenuation2 * (Rd2 + Rs2);

   vec3 R = Ra + attenuation * (Rd + Rs) + attenuation2 * (Rd2 + Rs2);
   // TODO set the output color to the shaded color that you have computed
   FragColor = vec4(R, 1.0);
}
