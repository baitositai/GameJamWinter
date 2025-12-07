//VSÅEPSã§í 
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"


float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float4 color = PSInput.vwPos.z;
    color.a = 1.0f;
    return color;
}