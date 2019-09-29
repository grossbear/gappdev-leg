[[ Vertex Shader ]]
attribute vec4 vVertex;

uniform mat4 TransformMtx;

void main(void)
{
	vec4 pos = vVertex;
    gl_Position = TransformMtx*pos;
}

[[ Pixel Shader ]]

void main(void)
{
    vec4 Color = vec4(0.8,1.0,0.7,1.0);
    gl_FragColor = Color;
}