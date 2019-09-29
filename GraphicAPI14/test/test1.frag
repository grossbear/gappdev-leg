// GLSL Fragment Program


uniform sampler2D boatTex;
uniform sampler1D stripes;

varying vec4 Diffuse;

uniform vec4 darken;

varying vec2 TexCoord0;
varying float TexCoord1;

void main(void)
{
	vec4 tex1 = texture2D(boatTex,TexCoord0)*darken*Diffuse;
	vec4 tex2 = texture1D(stripes,TexCoord1);
	
	gl_FragColor = mix(tex1,tex2,0.4);
}
