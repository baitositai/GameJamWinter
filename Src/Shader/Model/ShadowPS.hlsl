// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"
 
// IN
#define PS_INPUT VertexToPixelShadow
 
SamplerState g_SrcSampler : register(s0);
Texture2D g_SrcTexture : register(t0);
 
float4 main(PS_INPUT pIn) : SV_TARGET
{
 
	// Z値(深度、カメラから離れているほど1.0)
    float4 shadowDepth;
    shadowDepth.rgb = pIn.svPosRead.z;
    shadowDepth.a = 1.0f;
 
	// 完全透過は描画しない
    float4 color = g_SrcTexture.Sample(g_SrcSampler, pIn.uv);
    if (color.a < 0.0001f)
    {
		//shadowDepth.rgb = 1.0f;
        discard;
    }
 
    return shadowDepth;
 
}