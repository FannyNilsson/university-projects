#version 440
layout(location=0) in vec3 pos;
layout(location=1) in vec2 UV;
layout(location=2) in vec4 normal;
layout(location=3) in vec4 tangent;
layout(location=4) in vec4 biNormal;
layout(location=5) in vec4 skinWeight;
layout(location=6) in vec4 indice;

layout(location=0) out vec2 uv;
layout(location=1) out vec3 vertPos;
layout(location=2) out vec4 lightPosition;
uniform mat4 skeleton[21];
uniform mat4 rotation;

uniform mat4 transMatrix;
uniform mat4 VPmat;

uniform vec3 cameraPosition;

out vec3 color;
const int arr[21] = {19, 18, 17, 0, 20, 15, 13, 1, 12, 9, 2, 10, 3, 11, 14, 16, 4, 5, 8, 6, 7};
void main()
{
    uv.x = UV.x;
    uv.y = UV.y;
    mat4 joint = skeleton[arr[int(indice[0])]] * skinWeight[0] +
    skeleton[arr[int(indice[1])]] * skinWeight[1] +
    skeleton[arr[int(indice[2])]] * skinWeight[2] +
    skeleton[arr[int(indice[3])]] * skinWeight[3];
    vec4 newPosition = rotation * joint * vec4(pos, 1.0f);
    newPosition.x *=0.3;
    newPosition.y *=0.3;
    newPosition.z *=0.3;
    vec4 T = rotation * joint * normalize(tangent);
    vec4 B = rotation * joint * normalize(biNormal);
    vec4 N = rotation * joint * normalize(normal);
    gl_Position = VPmat*newPosition;
    mat3 TBN = transpose(mat3(vec3(T), vec3(B), vec3(N)));
    lightPosition = vec4(TBN*vec3(0,0,2),1.0);
    vertPos = TBN * (vec3(newPosition)/newPosition.w);
}
