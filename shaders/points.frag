#version 430
in vec4 vColor;

out vec4 fColor;

uniform mat4 worldToCamera;
uniform mat4 lighting;
uniform float ambient;

void main()
{
    vec4 temp = worldToCamera * lighting * vec4(0,0,1,0);
    vec3 lightDir = normalize(temp.xyz);

    vec3 Normal;
    Normal.xy = gl_PointCoord * 2.0 - vec2(1.0);
    float mag = dot( Normal.xy, Normal.xy );
    if (mag > 1.0)
    {
        discard;
    }

    Normal.z = sqrt(1.0-mag);

    float diffuse = max(0.0, dot(lightDir, Normal));

    vec4 ambientVec = vec4(ambient);

    fColor = vColor*((vec4(1,1,1,1) * diffuse) + ambient);
}
