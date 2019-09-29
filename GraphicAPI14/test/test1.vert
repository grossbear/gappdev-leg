// GLSL Vertex Program

varying vec2 TexCoord0;
varying float TexCoord1;

varying vec4 Diffuse;

//uniform vec4 darken;

void main(void)
{
	TexCoord0 = gl_MultiTexCoord0.xy;
	TexCoord1 = gl_MultiTexCoord1.x;
	Diffuse = gl_Color;

  	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
