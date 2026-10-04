#version 100

// Input vertex attributes
attribute vec3 vertexPosition;
attribute vec2 vertexTexCoord;
attribute vec3 vertexNormal;
attribute vec4 vertexColor;

// Input uniform values
uniform mat4 mvp;
uniform float uMoveBehaviourIndex;

// Output vertex attributes (to fragment shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;
varying float moveBehaviourIndex;

void main()
{
    // Send vertex attributes to fragment shader
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;
    moveBehaviourIndex = uMoveBehaviourIndex;

    // Calculate final vertex position
    gl_Position = mvp*vec4(vertexPosition, 1.0);
}