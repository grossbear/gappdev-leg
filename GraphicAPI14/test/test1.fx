// Direct3D Test High Level Shader

// Global Variables
float4x4 WorldViewProjMatrix;
float4x4 TextureMatrix = {	1, 0, 0, 0,
				0, -1,0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1	};
float4 darken;
//texture BoatTex;
//texture stripes;

struct VSOut
{
	float4 position: POSITION;
	float4 diffuse: COLOR0;
	float2 texcoord0 : TEXCOORD0;
	float  texcoord1 : TEXCOORD1;
};

struct PSIn
{
	float4 diffuse: COLOR0;
	float2 texcoord0 : TEXCOORD0;
	float2  texcoord1 : TEXCOORD1;
};

struct PSOut
{
	float4 color : COLOR;//Final Pixel Color
};

sampler2D BoatTex : register (s0);
sampler1D stripes : register (s1);
/*
sampler2D BoatTexSampler = sampler_state
{
	Texture = <BoatTex>;	
};

sampler StripesTexSampler = sampler_state
{
	Texture = <stripes>;
};*/

//Vertex Shader Main Function
VSOut VertexShaderProg(float3 pos : POSITION, float3 normal : NORMAL, float4 diffuse : COLOR, float2 texcoord0 : TEXCOORD0, float texcoord1 : TEXCOORD1)
{
	VSOut Out = (VSOut)0;
	
	Out.position = mul(float4(pos,1), WorldViewProjMatrix);
	
	Out.diffuse = diffuse;
	Out.texcoord0 = mul(float4(texcoord0,0,1), TextureMatrix).xy;
	Out.texcoord1 = texcoord1;
	
	return Out;
}


//Pixel Shader Main Function
PSOut PixelShaderProg(PSIn In)
{
	PSOut Out = (PSOut)0;
	float4 tex1 = tex2D(BoatTex, In.texcoord0)*darken*In.diffuse;
	float4 tex2 = tex1D(stripes, In.texcoord1);
	Out.color = lerp(tex1,tex2,0.4f);
	
	return Out;
}

//Shader Entry Point
technique main
{
	pass First
	{
		//Texture[0] = <BoatTex>;
		//Texture[1] = <stripes>;
		//Sampler[0] = (BoatTexSampler);
		//Sampler[1] = (StripesTexSampler);
		VertexShader = compile vs_1_1 VertexShaderProg();
		PixelShader = compile ps_1_1 PixelShaderProg();
	}
}
