//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Common shader declarations and techniques
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/* Usage instructions:
 * 
 * - Use #define FXAA for FXAA Injector compatibility, and
 *       #define ENB for Skyrim Enhanced Shaders / ENBSeries compatibility
 * 
 */

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Textures and samplers
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifdef ENB
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

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Structures
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifdef ENB
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

#ifdef ENB
VS_OUTPUT_POST VS_PostProcess_Def(VS_INPUT_POST IN)
{
	VS_OUTPUT_POST OUT;
	OUT.vpos = float4(IN.pos, 1);
	OUT.txcoord = IN.txcoord;
	return OUT;
}
#endif

// Misc defines
#define E_SHADER_3_0

#ifdef FXAA
#define COLOR COLOR0
#endif

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Default pixel shader
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#if defined(ENB) || defined(FXAA)
float4 PS_PostProcess_Def(float2 coord : TEXCOORD0) : COLOR
{
	return ColorPipeline(tex2D(SamplerColor, coord));
}
#endif