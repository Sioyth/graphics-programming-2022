#version 330 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textCoord; // here for completness, but we are not using it just yet

uniform mat4 model; // represents model coordinates in the world coord space
uniform mat4 viewProjection;  // represents the view and projection matrices combined
uniform vec3 camPosition; // so we can compute the view vector

// send shaded color to the fragment shader
out vec4 shadedColor;

// TODO exercise 5 setup the uniform variables needed for lighting
// light uniform variables
uniform vec3 light1Position;
uniform vec3 light1Color;
uniform float diffuseReflectance;

uniform vec3 light2Position;
uniform vec3 light2Color;

// material properties
uniform vec3 ambientLightColor;
uniform float ambientReflectance; 
uniform vec3 reflectionColor;

uniform float specularReflectance;
uniform float specularExponent;

void main() {
   // vertex in world space (for light computation)
   vec4 P = model * vec4(vertex, 1.0);
   // normal in world space (for light computation)
   vec3 N = normalize(model * vec4(normal, 0.0)).xyz;

   // final vertex transform (for opengl rendering, not for lighting)
   gl_Position = viewProjection * P;

   // TODO exercises 5.1, 5.2 and 5.3 - Gouraud shading (i.e. Phong reflection model computed in the vertex shader)

   // TODO 5.1 ambient
   vec3 Ra = ambientLightColor * ambientReflectance * reflectionColor;

   // TODO 5.2 diffuse
   vec3 L = normalize(light1Position - vec3(P.xyz));
   float angle = dot(N, L);
   
   vec3 Rd = light1Color * diffuseReflectance * angle;
   // TODO 5.3 specular
   //vec3 r = 2* N * (angle) - L;
   vec3 H = normalize(L + camPosition);

   vec3 Rs = light1Color * specularReflectance * pow(dot(N, H), specularExponent);


   // TODO exercise 5.5 - attenuation - light 1

   vec3 R = Ra + Rd + Rs;
   // TODO set the output color to the shaded color that you have computed
   shadedColor = vec4(R, 1.0);

}