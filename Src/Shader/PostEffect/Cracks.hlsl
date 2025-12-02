#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
// ノーマルテクスチャ用
Texture2D norm : register(t1);

cbuffer cbColor : register(b4)
{
    float g_time;
    float3 dummy;
}

float Rand2dTo1d(float2 value, float2 dotDir = float2(12.9898, 78.233))
{
    float2 smallValue = sin(value);
    float random = dot(smallValue, dotDir);
    random = frac(sin(random) * 143758.5453);
    return random;
}

float Rand1dTo1d(float value, float mutator = 0.546)
{
    float random = frac(sin(value + mutator) * 143758.5453);
    return random;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{    
    // ノーマルマップ画像の位置調整
    float2 offset = norm.Sample(texSampler, PSInput.uv).rg;
    offset = (offset * 2) - 1;
    
    // uv値を調整
    float2 uv = PSInput.uv + offset * 0.15f;
    
    // 時間からランダム値の取得
    float t = Rand1dTo1d(g_time);
    float t2 = Rand1dTo1d(g_time + 0.001);
    
    // UV値からランダム値を取得
    float rand = Rand2dTo1d(uv + t);
    float rnd = Rand2dTo1d(uv - fmod(uv, 0.02) + float2(t, g_time)) * 0.55;
    
    // 最終的な位置からテクスチャの色を取得
    float4 texcol = tex.Sample(texSampler, uv);
    
    // 色を返す
    return texcol;

}
