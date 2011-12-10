//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Post-processing color functions
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

float4 ColorPipeline(float4 inp)
{
	#ifdef Pass1
	inp = Pass1(inp);
	#endif
	
	#ifdef Pass2
	inp = Pass2(inp);
	#endif
	
	#ifdef Pass3
	inp = Pass3(inp);
	#endif
	
	#ifdef Pass4
	inp = Pass4(inp);
	#endif
	
	#ifdef Pass5
	inp = Pass5(inp);
	#endif
	
	#ifdef Pass6
	inp = Pass6(inp);
	#endif
	
	#ifdef Pass7
	inp = Pass7(inp);
	#endif
	
	#ifdef Pass8
	inp = Pass8(inp);
	#endif
	
	#ifdef Pass9
	inp = Pass9(inp);
	#endif
	
	#ifdef Pass10
	inp = Pass10(inp);
	#endif
	
	return inp;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Techniques for ENB and FXAA compatibility
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "include/common.h"

#ifdef SES
technique PostProcess
{
	pass P0
	{
		VertexShader = compile vs_3_0 VS_PostProcess_Def();
		PixelShader = compile ps_3_0 PS_PostProcess_Def();
		
		DitherEnable=FALSE;
		ZEnable=FALSE;
		CullMode=NONE;
		ALPHATESTENABLE=FALSE;
		SEPARATEALPHABLENDENABLE=FALSE;
		AlphaBlendEnable=FALSE;
		StencilEnable=FALSE;
		FogEnable=FALSE;
		SRGBWRITEENABLE=FALSE;
	}
}
#endif

#if defined(FXAA) || defined(ENB)
technique PostProcess1
{
	pass p1
	{
		PixelShader = compile ps_3_0 PS_PostProcess_Def();
	}
}
#endif