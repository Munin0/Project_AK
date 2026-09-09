#version 460 core

layout(location = 0) in vec3  a_Position;
layout(location = 1) in vec4  a_Color;
layout(location = 2) in vec2  a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_ArrayLayer;

uniform mat4 u_ViewProjection;

out vec4  v_Color;
out vec2  v_TexCoord;
flat out int v_TexIndex;
flat out float v_ArrayLayer;

void main()
{
    v_Color      = a_Color;
    v_TexCoord   = a_TexCoord;
    v_TexIndex   = int(a_TexIndex);
    v_ArrayLayer = a_ArrayLayer;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}
