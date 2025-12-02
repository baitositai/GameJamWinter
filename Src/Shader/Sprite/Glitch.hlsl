#include "../common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ（DXライブラリで設定可能）
cbuffer ConstantBuffer : register(b4)
{
    float4 g_color;         // 通常は未使用（DXライブラリの仕様）
    float g_time;           // 経過時間（DXライブラリでSetShaderConstantFで渡す）
    float g_glitch_strength;// グリッチの強度（0.0～1.0くらい）
    float g_glitch_speed;   // グリッチの変化速度
    float g_divX;
};

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // UV値取得
    float2 uv = PSInput.uv;
    
    // グリッチの効果を与えるUV幅を計算
    float widthUv = 1.0f / g_divX;
    
    // 現在の番号を取得
    int index = floor(uv.x / widthUv);
    
    // ノイズのランダムシートを生成
    float noiseSeed = floor(uv.y * 10.0f + g_time * g_glitch_speed) * 10.0f + (float) index * 123.45f;
    
    // 時間ごとに変化するノイズを生成
    float randomVal = frac(sin(noiseSeed * 12.9898f) * 43758.5453f);
    
    // 範囲変換(-1.0fから1.0f)
    float randShift = (randomVal * 2.0f - 1.0f);
    
    // ズレの量を計算、強度(GlitchStrength)とランダム値を乗算
    float xShift = randShift * g_glitch_strength * widthUv * 0.5f;
    
    // グリッチ効果を発生させたい頻度を調整
    xShift *= pow(abs(randShift), 4.0f) * 4.0f;
    
    // x座標をずらす
    float2 shiftedUV = uv + float2(xShift, 0.0f);
    
    // ずらしたUV座標からテクスチャの色を取得
    float4 color = tex.Sample(texSampler, shiftedUV);
    
    // R成分を少しずらして読み出す
    float2 red = shiftedUV + g_glitch_strength * 0.001f;
    // B成分を反対方向にずらして読み出す
    float2 blue = shiftedUV - g_glitch_strength * 0.001f;
    
    color.r = tex.Sample(texSampler, red).r;
    color.b = tex.Sample(texSampler, blue).b;
   
    return color;
    
    // テクスチャの色を返す
    return tex.Sample(texSampler, PSInput.uv);
}