#version 330 core
layout (location = 0) in vec2 pos;   // the position variable has attribute position 0

// TODO 4.2 add velocity and timeOfBirth as vertex attributes
layout (location = 1) in vec2 velocity;
layout (location = 2) in float timeOfBirth;

uniform float currentTime;
const float maxAge = 10.0;

out float elapsedTime;
// TODO 4.3 create and use a float uniform for currentTime

// TODO 4.6 create out variable to send the age of the particle to the fragment shader

void main()
{
    //float elapsedTime;
    vec2 _pos;
    _pos = pos;
    elapsedTime = currentTime - timeOfBirth;
    // TODO 4.3 use the currentTime to control the particle in different stages of its lifetime
    if(timeOfBirth == 0 || elapsedTime > maxAge)
        _pos = vec2(-2.0, 2);
    else if(timeOfBirth > 0)
        _pos += velocity * elapsedTime;
    
    gl_Position = vec4(_pos, 0.0, 1.0);
    // TODO 4.6 send the age of the particle to the fragment shader using the out variable you have created

    // this is the output position and and point size (this time we are rendering points, instad of triangles!)
    // (birthtime = 0) 0.1 - 20 (maxAge);
    //float growth = elapsedTime;

    
    gl_PointSize = mix(0.1, 20.0, (elapsedTime / maxAge));
}