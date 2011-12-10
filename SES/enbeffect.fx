//===============================
// Natural Colors v0.8, SES edit
//===============================


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Configuration
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Chromatic adaptation: (makes all tones more neutral)
#define EADAPTATION

// The target white point (0-10, higher = more blue)
// If default is too blue, try 4, if it's too red, try 6
#define WHITEPOINT 5

// Saturation controls: (Uncomment the #define to enable them)
// 0 is grayscale, 1 is normal, higher values = more saturated
//#define SATURATION 1.0

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// End of configuration
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



// ./include/naturalcolors.h

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Matrices and constants
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// sRGB to XYZ
float3x3 M2XYZ = float3x3(0.4124564, 0.3575761, 0.1804375, 0.2126729, 0.7151522, 0.0721750, 0.0193339, 0.1191920, 0.9503041);

// Scaling matrix
float3x3 Bradford = float3x3(0.8951, 0.2664, -0.1614, -0.7502, 1.7135, 0.0367, 0.0389, -0.0685, 1.0296);
float3x3 Backford = float3x3(0.986992905466712, -0.14705425642099, 0.159962651663731, 0.432305269723395, 0.518360271536777, 0.0492912282128556, -0.00852866457517733, 0.0400428216540849, 0.96848669578755);

// XYZ to sRGB
float3x3 M2RGB = float3x3(3.2404542, -1.5371385, -0.4985314, -0.9692660, 1.8760108, 0.0415560, 0.0556434, -0.2040259, 1.0572252);

// The Skyrim constant (measured white point ingame as XYZ)
float3 skyWP = float3(157, 172.6, 181.2);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Color temperature conversion function
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

float3 FromTemperature(float T)
{
	float x;
	
    if (4000 <= T && T <= 7000) {
        x = -4.6070E9 / (T*T*T) + 2.9678E6 / (T*T) + 9.911E1 / T + 0.244063;
    } else if (7000 < T && T <= 25000) {
        x = -2.0064E9 / (T*T*T) + 1.9018E6 / (T*T) + 2.4748E2 / T + 0.237040;
    } else {
        x = 0;
    }
	
	float y = -3 * x * x + 2.87 * x - 0.275;
	
	// Convert to XYZ
	return float3(x/y, 1, (1-x-y)/y);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Post-processing pixel shader
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define f(t) (t>pow(6/29.,3)?pow(t,1/3.):pow(29/6.,2)/3*t+4/29.)
#define f_(t) (t>6/29.?pow(t,3):pow(6/29.,2)*3*(t-4/29.))

float4 NaturalColors(float4 inp)
{
	// Preshader optimization
	#ifdef EADAPTATION
	float3 dstWP = FromTemperature(WHITEPOINT * 600 + 4000);
	float3 srcWP = mul(M2XYZ, pow(skyWP / 255, 2.2)); // Our whitepoint is sRGB so decode it first
	srcWP /= srcWP.y; // Normalize it to Y=1
	float3 CS = mul(Bradford, srcWP);
	float3 CD = mul(Bradford, dstWP);
	float3x3 CA = mul(mul(Backford, float3x3(CD.x / CS.x, 0, 0, 0, CD.y / CS.y, 0, 0, 0, CD.z / CS.z)), Bradford);
	#endif
	
	// SMAA already lifts the gamma but FXAA and ENB do not
	inp = pow(inp, 2.2);
	
	// Convert from sRGB primaries to XYZ
	float3 col = mul(M2XYZ, inp.xyz);

	// Chromatic adaptation matrix - simply multiply
	#ifdef EADAPTATION	
	col = mul(CA, col);
	#endif

	// Saturation controls in LC*h light linear space
	#ifdef SATURATION
	col.xyz = float3(116*f(col.y)-16,500*(f(col.x/0.95047)-f(col.y)),200*(f(col.y)-f(col.z/1.08883)));
	col.yz = float2(sqrt(col.y*col.y+col.z*col.z)*SATURATION,atan2(col.z,col.y));
	col.xyz = float3((col.x+16)/116,cos(col.z)*col.y,sin(col.z)*col.y);
	col.yzx = float3(f_(col.x),1.08883*f_(col.x-col.z/200),0.95047*f_(col.x+col.y/500));
	#endif

	// Convert from XYZ back to sRGB primaries and clip levels
	col = saturate(mul(M2RGB, col));

	// Encode into gamma space again
	col = pow(col, 1/2.2);
	
	return float4(col, 1);
}

// ./include/common.h

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Textures and samplers
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
	MinFilter = LINEAR;//
	MagFilter = LINEAR;//
	MipFilter = NONE;//LINEAR;
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
	MinFilter = LINEAR;//
	MagFilter = LINEAR;//
	MipFilter = NONE;//LINEAR;
	AddressU  = Clamp;
	AddressV  = Clamp;
	SRGBTexture=FALSE;
	MaxMipLevel=0;
	MipMapLodBias=0;
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Default pixel shader
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

float4	_c1 : register(c1);
float4	_c2 : register(c2);
float4	_c3 : register(c3);
float4	_c4 : register(c4);
float4	_c5 : register(c5);

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
	
	return NaturalColors(_c5.w * r1 + r0);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Technique for SES compatibility
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
technique PostProcess
{
	pass P0
	{
		PixelShader = compile ps_3_0 PS_PostProcess_Def();
	}
}