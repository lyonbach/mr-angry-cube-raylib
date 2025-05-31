#version 100

// Input vertex attributes
attribute vec3 vertexPosition;
attribute vec2 vertexTexCoord;
attribute vec3 vertexNormal;
attribute vec4 vertexColor;

// Input uniform values
uniform mat4 mvp;

// Output vertex attributes (to fragment shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Custom for animation
uniform vec2 uvOffset;
uniform vec2 uvScale;

void main()
{
    // Send vertex attributes to fragment shader
    fragTexCoord = vec2(vertexTexCoord.x + uvOffset.x, vertexTexCoord.y + uvOffset.y) * uvScale;
    fragColor = vertexColor;

    // Calculate final vertex position
    gl_Position = mvp*vec4(vertexPosition, 1.0);
}
