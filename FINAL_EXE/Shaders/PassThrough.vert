#version 420

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform uint uAni = 0;
uniform float uTime;

layout(location = 0) in vec3 in_vert1;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal1;

layout(location = 3) in vec3 in_vert2;
layout(location = 4) in vec3 in_normal2;


out vec2 texcoord;
out vec3 norm;
out vec3 pos;

void main()
{
     
    texcoord = in_uv;
    texcoord.y = 1 - texcoord.y;

   vec3 normal = mix(in_normal1, in_normal2, uTime);
   vec3 vert = mix(in_vert1, in_vert2, uTime);


    norm =  mat3(uModel) * mat3(uView) * normal;
    
    vec4 viewSpace =  uView * uModel * vec4(vert, 1.0f); 
    
    gl_Position =  uProj * viewSpace;
    
    pos = gl_Position.xyz;
}