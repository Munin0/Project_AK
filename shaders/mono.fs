#version 330 core

in vec4  v_Color;
in vec2  v_TexCoord;
flat in int v_TexIndex;
flat in float v_ArrayLayer;

uniform sampler2D u_Textures[16];
uniform sampler2DArray u_AtlasArray;

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

    // Luminancia perceptual (pesos estándar Rec. 709)
    float gray = dot(texColor.rgb, vec3(0.2126, 0.7152, 0.0722));

    FragColor = vec4(vec3(gray), texColor.a) * v_Color;
}
