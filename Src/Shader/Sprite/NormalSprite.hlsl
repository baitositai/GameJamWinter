#include "../common/Pixel/PixelShader2DHeader.hlsli"

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // テクスチャの色を返す
    return tex.Sample(texSampler, PSInput.uv);
}