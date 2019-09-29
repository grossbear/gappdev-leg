[[ Vertex Shader ]]
attribute vec4 vVertex;
attribute vec4 vColor;

uniform mat4 TransformMtx;

varying vec4 varColor;

void main(void)
{
	vec4 pos = vVertex;
	varColor = vColor;
    //gl_Position = gl_ModelViewProjectionMatrix*pos;
    gl_Position = TransformMtx*pos;
}

[[ Pixel Shader ]]
varying vec4 varColor;
void main(void)
{
    vec4 color = varColor;
    gl_FragColor = color;
}