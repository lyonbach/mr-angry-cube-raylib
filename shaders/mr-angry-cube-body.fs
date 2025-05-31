#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
varying float moveBehaviourIndex;

void main()
{
    // Texel color fetching from texture sampler
    vec4 texelColor = texture2D(texture0, fragTexCoord);
    float offset = 1.0 / 100.0; // Adjust for texture size
    vec4 result = vec4(0.0);
    if (moveBehaviourIndex > 3.0)
    {
        result += texture2D(texture0, fragTexCoord + vec2(-offset, -offset)) * 0.0625;
        result += texture2D(texture0, fragTexCoord + vec2( 0.0,    -offset)) * 0.125;
        result += texture2D(texture0, fragTexCoord + vec2( offset, -offset)) * 0.0625;
        result += texture2D(texture0, fragTexCoord + vec2(-offset,     0.0)) * 0.125;
        result += texture2D(texture0, fragTexCoord + vec2( 0.0,        0.0)) * 0.25;
        result += texture2D(texture0, fragTexCoord + vec2( offset,     0.0)) * 0.125;
        result += texture2D(texture0, fragTexCoord + vec2(-offset,  offset)) * 0.0625;
        result += texture2D(texture0, fragTexCoord + vec2( 0.0,     offset)) * 0.125;
        result += texture2D(texture0, fragTexCoord + vec2( offset,  offset)) * 0.0625;
    }
    gl_FragColor = result * colDiffuse + texelColor;
}