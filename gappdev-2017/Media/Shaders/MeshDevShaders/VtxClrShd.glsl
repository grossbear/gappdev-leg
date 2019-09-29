[[ Vertex Shader ]]
attribute vec4 vVertex;
attribute vec4 vColor;

varying vec4 varColor;

void main(void)
{
	vec4 pos = vVertex;
	varColor = vColor;
    gl_Position = pos;
}

[[ Pixel Shader ]]
varying vec4 varColor;
void main(void)
{
    vec4 color = varColor;
    gl_FragColor = color;
}