#version 330 core
// FRAGMENT SHADER

// TODO exercise 2.4
out vec4 fragColor;
// Create an 'in float' variable to receive the depth value from the vertex shader,
// the variable must have the same name as the 'out variable' in the vertex shader.
in float zCoord;
float coord;

uniform vec3 color;

void main()
{
    // Use the interpolated z-coordinate to draw the distance of this fragment
    // relative to the center of the cone.
    // Make sure that the z-coordinate is in the [0, 1] range (if it is not, place it in that range).
    // You can use non-linear transformations of the z-coordinate, such as the 'pow' or 'sqrt' functions,
    // to make the change in grey tone more evident.

    coord = zCoord;    

    if(coord > 1)
        coord = 1;
    else if(coord < 0)
        coord = 0;

    float l = -1 + ( -1 - 1) * zCoord;
    fragColor = vec4(l, l, l, 1.0);
}