#version 330 core

out vec4 fragColor;

const float midAge = 5.0;
const float maxAge = 10.0;
const vec3 startCol = vec3(1.0, 1.0, 0.05);
const vec3 midCol = vec3(1.0, 0.5, 0.01);
const vec3 endCol = vec3(0.0, 0.0, 0.0);

// TODO 4.6: should receive the age of the particle as an input variable
in float elapsedTime;
void main()
{
    // TODO 4.4 set the alpha value to 0.2 (alpha is the 4th value of the output color)
    float alpha = 0.2f;
    // TODO 4.5 and 4.6: improve the particles appearance
    vec2 p = gl_PointCoord;
    vec2 dvec= (gl_PointCoord - vec2(.5, .5)) * 2;
    //dvec = abs(dvec);
    //float d = sqrt(pow(dvec.x, 2) + pow(dvec.y, 2));
    
    //    if(d > 0.5)
    //        d = 0;
   
     float d = sqrt(dot(dvec, dvec));
    alpha = mix(1.0 - d, 0, elapsedTime / maxAge);

     vec3 color =  mix(startCol, midCol, elapsedTime / midAge);
     if(elapsedTime > midAge)
     color =  mix(midCol, endCol, elapsedTime / midAge);

    // remember to replace the default output (vec4(1.0,1.0,1.0,1.0)) with the color and alpha values that you have computed
    fragColor = vec4(color, alpha);

}