#version 130

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 color0;

uniform vec3 lightDirection;
uniform vec3 lightColor;

// Values that stay constant for the whole mesh.
uniform sampler2D texture;

void main()
{
    vec3 tmp = dot(-lightDirection, normal0) * color0 ;

	gl_FragColor = texture2D( texture, texCoord0 );
}
