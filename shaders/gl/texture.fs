#version 460 core

in vec4  v_Color;
in vec2  v_TexCoord;
flat in int v_TexIndex;
flat in float v_ArrayLayer;
flat in float v_EffectMode;

layout(binding = 0)  uniform sampler2D      u_Textures[16];
layout(binding = 16) uniform sampler2DArray u_AtlasArray;

/// All the Layouts are 1::1, so always in the fs && vs needs to be the same.
layout(std140, binding = 0) uniform FrameData
{
    mat4  u_ViewProjection;
    float u_Time;
    float u_ScreenWidth;
    float u_ScreenHeight;
};

out vec4 FragColor;

void main()
{
    vec4 texColor;

    if (v_ArrayLayer >= 0.0) {
        texColor = texture(u_AtlasArray, vec3(v_TexCoord, v_ArrayLayer));
    } else {
        switch(v_TexIndex) {
            case 0:  texColor = texture(u_Textures[0],  v_TexCoord); break;
            case 1:  texColor = texture(u_Textures[1],  v_TexCoord); break;
            case 2:  texColor = texture(u_Textures[2],  v_TexCoord); break;
            case 3:  texColor = texture(u_Textures[3],  v_TexCoord); break;
            case 4:  texColor = texture(u_Textures[4],  v_TexCoord); break;
            case 5:  texColor = texture(u_Textures[5],  v_TexCoord); break;
            case 6:  texColor = texture(u_Textures[6],  v_TexCoord); break;
            case 7:  texColor = texture(u_Textures[7],  v_TexCoord); break;
            case 8:  texColor = texture(u_Textures[8],  v_TexCoord); break;
            case 9:  texColor = texture(u_Textures[9],  v_TexCoord); break;
            case 10: texColor = texture(u_Textures[10], v_TexCoord); break;
            case 11: texColor = texture(u_Textures[11], v_TexCoord); break;
            case 12: texColor = texture(u_Textures[12], v_TexCoord); break;
            case 13: texColor = texture(u_Textures[13], v_TexCoord); break;
            case 14: texColor = texture(u_Textures[14], v_TexCoord); break;
            case 15: texColor = texture(u_Textures[15], v_TexCoord); break;
            default: texColor = vec4(1.0); break;
        }
    }

    FragColor = texColor * v_Color;
    
    /// Effects with de DrawCall, its simple.
    switch(int(v_EffectMode))
    {
        case 1: // Mono 
        {
            float gray = dot(texColor.rgb, vec3(0.299, 0.587, 0.114));
            texColor.rgb = vec3(gray);
            break;
        }
        case 2: // Flash
        {
            texColor.rgb = vec3(1.0);
            break;
        }
        case 3: // Poison (Needs u_Time) 
        {
            float pulse = 0.5 + 0.5 * sin(u_Time * 6.0);
            vec3  poisonTint = vec3(0.3, 1.0, 0.3);
            texColor.rgb = mix(texColor.rgb, poisonTint, pulse * 0.5);
            break;
        }
        default: // Not Effect == 0.0f
            break;
    }
}
