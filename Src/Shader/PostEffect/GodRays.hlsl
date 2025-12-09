#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
cbuffer cbColor : register(b4)
{
    float2 g_sun_screen_pos;
    float g_decay;
    float g_exposure;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{ 
    // サンプリング回数とステップサイズ
    const int NUM_SAMPLES = 60;
    const float WEIGHT = 1.0f / (float) NUM_SAMPLES;
    
    // uv値
    float2 uv = PSInput.uv;
    
    // テクスチャをサンプリング
    float4 color = tex.Sample(texSampler, uv);
    
    // 現在のピクセルから太陽の位置に向かうベクトル
    float2 deltaUV = g_sun_screen_pos - uv;
    
    // サンプリングステップのサイズ（ベクトルを NUM_SAMPLES で割る）
    float2 stepUV = deltaUV / (float) NUM_SAMPLES;
    
    // 蓄積する光の色
    float4 resultColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    // 光の減衰（累積）
    float currentDecay = 1.0f;
    
    // 放射状のサンプリングを実行
    for (int i = 0; i < NUM_SAMPLES; ++i)
    {
        // サンプリング位置を計算
        float2 sampleUV = uv + stepUV * (float) i;
        
        // 画面の端からはみ出さないようにクリップ（任意）
        // if (sampleUV.x < 0.0 || sampleUV.x > 1.0 || sampleUV.y < 0.0 || sampleUV.y > 1.0) continue;
        
        // テクスチャから色を取得
        float4 sampleColor = tex.Sample(texSampler, sampleUV);
        
        // 太陽光の筋（レイ）として扱うために、明るい部分だけを抽出する処理を入れるとより自然になる
        // ここでは簡単に、色の強度が高い部分を光として扱う
        float rayIntensity = (sampleColor.r + sampleColor.g + sampleColor.b) / 3.0; // 平均強度
        
        // 光の蓄積 (光の強度 * 現在の減衰率 * 重み)
        resultColor += sampleColor * currentDecay * WEIGHT;
        
        // 減衰率を更新
        currentDecay *= g_decay;
    }
    
    // 最終的な光の筋を元の色に合成
    // exposure で全体を調整
    float4 finalColor = tex.Sample(texSampler, uv);
    
    // 元の色にゴッドレイの効果を足し合わせる
    finalColor.rgb += resultColor.rgb * g_exposure;
    
    return finalColor;
}