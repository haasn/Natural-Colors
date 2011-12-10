//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Common shader declarations and techniques
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/* Usage instructions:
 * 
 * - Use #define FXAA for FXAA Injector compatibility, and
 *       #define SES for Skyrim Enhanced Shaders compatibility, and
 *       #define ENB for ENBSeries Skyrim (v0099) compatibility
 * 
 */

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Textures and samplers
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifdef SES
texture2D texColor;
sampler2D SamplerColor = sampler_state
{
	Texture   = <texColor>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;//NONE;
	AddressU  = Clamp;
	AddressV  = Clamp;
	SRGBTexture=FALSE;
	MaxMipLevel=0;
	MipMapLodBias=0;
};
#endif

#ifdef FXAA
uniform extern texture gScreenTexture;
sampler SamplerColor = sampler_state
{
	Texture = <gScreenTexture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = BORDER;
	AddressV = BORDER;
	SRGBTexture = FALSE;
};
#endif

#ifdef ENB
texture2D texs0; // Color
sampler2D SamplerColor = sampler_state
{
	Texture   = <texs0>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = NONE;
	AddressU  = Clamp;
	AddressV  = Clamp;
	SRGBTexture=FALSE;
	MaxMipLevel=0;
	MipMapLodBias=0;
};

texture2D texs1; // Bloom skyrim
sampler2D SamplerBloom = sampler_state
{
	Texture   = <texs1>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;
	AddressU  = Clamp;
	AddressV  = Clamp;
	SRGBTexture=FALSE;
	MaxMipLevel=0;
	MipMapLodBias=0;
};

texture2D texs2; // Adaptation skyrim
sampler2D SamplerAdaptation = sampler_state
{
	Texture   = <texs2>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = NONE;
	AddressU  = Clamp;
	AddressV  = Clamp;
	SRGBTexture=FALSE;
	MaxMipLevel=0;
	MipMapLodBias=0;
};

float4	_c1 : register(c1);
float4	_c2 : register(c2);
float4	_c3 : register(c3);
float4	_c4 : register(c4);
float4	_c5 : register(c5);
#endif

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Structures
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifdef SES
struct VS_OUTPUT_POST {
	float4 vpos  : POSITION;
	float2 txcoord : TEXCOORD0;
};

struct VS_INPUT_POST {
	float3 pos  : POSITION;
	float2 txcoord : TEXCOORD0;
};
#endif

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Pass-through vertex shader
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifdef SES
VS_OUTPUT_POST VS_PostProcess_Def(VS_INPUT_POST IN)
{
	VS_OUTPUT_POST OUT;
	OUT.vpos = float4(IN.pos, 1);
	OUT.txcoord = IN.txcoord;
	return OUT;
}
#endif

// Misc defines
#ifdef FXAA
#define COLOR COLOR0
#endif

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Default pixel shader
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#if defined(SES) || defined(FXAA)
float4 PS_PostProcess_Def(float2 coord : TEXCOORD0) : COLOR
{
	return ColorPipeline(tex2D(SamplerColor, coord));
}
#endif

#ifdef ENB
float4 PS_PostProcess_Def(float2 coord : TEXCOORD0) : COLOR
{
	float4 _c7=float4(0.212500006, 0.715399981, 0.0720999986, 1.0);

	float4 r0;
	float4 r1;
	float4 r2;
	
	// Vanilla post processing
	r0.x=1.0/_c2.y;
    r1=tex2D(SamplerAdaptation, coord);
    r0.yz=r1.xy * _c1.y;
    r0.w=1.0/r0.y;
    r0.z=r0.w * r0.z;
	
    r1=tex2D(SamplerColor, coord);
    r1.xyz=r1 * _c1.y;
    r0.w=dot(_c7.xyz, r1.xyz);
    r1.w=r0.w * r0.z;
    r0.z=r0.z * r0.w + _c7.w;
    r0.z=1.0/r0.z;
    r0.x=r1.w * r0.x + _c7.w;
    r0.x=r0.x * r1.w;
    r0.x=r0.z * r0.x;
	
	if (r0.w<0) r0.x=0;
    
	r0.z=1.0/r0.w;
    r0.z=r0.z * r0.x;
    r0.x=saturate(-r0.x + _c2.x);
	
    r2=tex2D(SamplerBloom, coord);
    r2.xyz=r2 * _c1.y;
    r2.xyz=r0.x * r2;
    r1.xyz=r1 * r0.z + r2;
    r0.x=dot(r1.xyz, _c7.xyz);
    r1.w=_c7.w;
	r2=lerp(r0.x, r1, _c3.x);
    r1=r0.x * _c4 - r2;
    r1=_c4.w * r1 + r2;
    r1=_c3.w * r1 - r0.y;
    r0=_c3.z * r1 + r0.y;
	r1=-r0 + _c5;
	
	return ColorPipeline(_c5.w * r1 + r0);
}
#endif