#version 430
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;
out vec4 vColor;

uniform mat4 modelToWorld;
uniform mat4 worldToView;

void main()
{
  gl_Position = worldToView * modelToWorld * vec4(position, 1.0);
  vColor = vec4(colour, 1.0);
}
