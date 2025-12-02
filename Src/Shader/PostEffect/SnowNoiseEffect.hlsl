#include "../Common/Pixel/PixelShader2DHeader.hlsli"
#include "../Common/Pixel/PixelShaderCommonFunction.hlsli"

// 画面中心
static float2 CENTER = { 0.5f, 0.5f };

// 方向数
static float DIRECTIONS = 16.0f;

// 各方向でのサンプル数
static float QUALITY = 3.0f;

// 2π
static float TWO_PI = 6.28318530718f;

// 定数バッファ：スロット4番目
cbuffer cbParam : register(b4)
{
    float g_time;
    float2 g_screen_size;
    float g_distance;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // UV値取得
    float2 uv = PSInput.uv;
    
    // UV座標とテクスチャを参照して、最適な色を取得する
    float4 color = tex.Sample(texSampler, uv);
    
    // ノイズUVの計算
    float2 lineNoiseUv = uv * float2(50.0, 200.0);
    
    // 横方向へ動かす
    lineNoiseUv.x += g_time * 2000.0f;
    
    // 各ノイズブロックの境界を床関数で取得
    float2 noiseBlock = floor(lineNoiseUv);
    
    // 乱数生成
    float noise = Rand(noiseBlock); // 横縞のブロックごとに同じ乱数を生成
    noise = saturate(noise - 0.3f); // 少し暗めにするために調整
     
    float4 noiseColor = float4(noise, noise, noise, 1.0f);
    
    float area = sin(uv.y * 2.0f - g_time * 0.5f);
   
    float isArea = step(0.996f, area * area);

    noiseColor.rgb -= abs(sin(uv.y * 60.0f + g_time * 1.0f)) * 0.05f;
    noiseColor.rgb -= abs(sin(uv.y * 100.0f - g_time * 2.0f)) * 0.15f;
    
    // 一定エリア処理
	// ------------------------------------------------------------------------------
	// 一定エリア以外、間隔を空けて、縞々模様(下地)を作る(色の減算で色を暗くする)
    noiseColor.rgb -= (1.0f - isArea) * abs(sin(uv.y * 60.0f + g_time * 1.0f)) * 0.05f;
    noiseColor.rgb -= (1.0f - isArea) * abs(sin(uv.y * 100.0f - g_time * 2.0f)) * 0.15f;

	// 特定範囲だけ明るくする(色の加算で明るくする)
    noiseColor.rgb += isArea * 0.5f;
	// ------------------------------------------------------------------------------
    
    
    return noiseColor;
}