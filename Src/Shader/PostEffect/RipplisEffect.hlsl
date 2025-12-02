#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 波の速度
static const float TIME_FACTOR = 5.0f;      

// 波の密度の細かさ
static const float DISTANCE_FACTOR = 60.0f; 

// 歪みの全体の強さ
static const float TOTAL_FACTOR = 1.0f;

// 波紋の幅
static const float WAVE_WIDTH = 0.1f; 

// 開始位置
static const float2 START_UV = { 0.5f, 0.5f };

// ラインの色
static const float3 LINE_COLOR = { 0.0f, 0.0f, 0.0f };

// ラインの太さ
static const float LINE_WIDTH = 0.00005f;

// ブロックの分割数 (32x32ブロック)
static const float BLOCK_SIZE = 32.0f;

// ブロックの最大ずれ幅 (UV空間)
static const float MAX_BLOCK_OFFSET = 0.05f; 

// スロット4番目(b4)に登録
cbuffer cbParam : register(b4)
{
    float g_ripplis_step;           // リップル更新用
    float g_wave_distance;  // 波紋が発生している中心からの距離
    float g_aspect_ratio;   // 画面アスペクト比
    float g_grain_power;    // フィルムグレインの影響力
    
    float g_grain_step;     // フィルムグレインの更新
    float2 g_screen_size;   // 画面サイズ
    float g_is_line_active; // ラインの描画判定
    
    float g_glitch_strength; // グリッチの強さ (0.0～1.0)
    float g_glitch_frequency; // グリッチの発生頻度
};

float Random(in float2 uv, in float time)
{
    // 時間 (Time) をランダム性に加えることで、ノイズを毎フレーム動かす
    float t = time * 0.05f; // ノイズの変化速度を調整
    
    // UV座標と時間を使って、特定の大きな値とドット積を計算し、サインで周期的な値を得る
    float value = dot(uv + t, float2(12.9898, 78.233));
    
    // frac()で小数部を取り出し、さらに大きな値で乗算してから小数部を取ることで、ランダム性を高める
    return frac(sin(value) * 43758.5453123);
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // UV値取得
    float2 uv = PSInput.uv;
 
    // UVから中心点へのベクトルを計算
    float2 uvToVec = START_UV.xy - uv;
    
    // アスペクト比による補正
    uvToVec.x = uvToVec.x * g_aspect_ratio;

    // 距離の計算
    float dis = length(uvToVec);
    
    // 波形のオフセットの計算
    float sinFactor = sin(dis * DISTANCE_FACTOR + g_ripplis_step * TIME_FACTOR) * TOTAL_FACTOR * 0.01f;
    
    // 波紋の範囲制限
    float discardFactor = clamp(WAVE_WIDTH - abs(g_wave_distance - dis), 0.0f, 1.0f) / WAVE_WIDTH;

    // ベクトルを正規化
    float2 vec = normalize(uvToVec);
    
    // 距離が非常に小さい場合
    if (dis < 0.0001f)
    {
        vec = float2(0.0f, 0.0f);
    }
    
    // 最終的なオフセットを計算
    float2 offset = vec * sinFactor * discardFactor;
    
    // UVを調整
    float2 warpedUv = offset + uv;
    
    // ブロック座標 (例: 32x32)
    float2 blockCoord = floor(uv * BLOCK_SIZE);

    // ランダムな値を取得 (グリッチの発生確率に使う)
    float blockRandom = Random(blockCoord, g_grain_step * g_glitch_frequency);
    
    // グリッチ強度とランダム値に基づき、グリッチを発生させるか判定
    if (blockRandom < g_glitch_strength)
    {
        // ブロックごとのランダムなずれ幅を生成
        float2 randomOffset = Random(blockCoord + 0.1f, g_grain_step * g_glitch_frequency * 1.5f) * 2.0f - 1.0f;
        
        // warpedUv (波紋で歪んだUV) をさらにグリッチで歪ませる
        warpedUv.x += randomOffset.x * MAX_BLOCK_OFFSET * g_glitch_strength;
    }

    // 調整した座標でテクスチャを参照
    float4 color = tex.Sample(texSampler, warpedUv);
    
    // ノイズの生成
    float noise = Random(uv, g_grain_step);
    
    // ノイズ値を範囲内に調整
    float grain = (noise * 2.0f - 1.0f) * g_grain_power;

    // ノイズをもとの色に加算
    color.rgb += grain * g_is_line_active;
    
    // スクロール位置の計算
    float scrollPos = fmod(g_grain_step * 0.1f, 1.0f); 

    // 線の中心位置
    float lineCenter = scrollPos;
    
    // 線の描画判定
    float dist = abs(PSInput.uv.x - lineCenter);
    
    // 線の濃さ
    float alpha = 1.0f - smoothstep(0.0f, LINE_WIDTH, dist);
    
    // 点滅 
    alpha *= g_is_line_active;
    
    // 最終的な色
    float3 finalColor = color.rgb * (1.0 - alpha) + LINE_COLOR * alpha;
    
    // 出力
    return float4(finalColor, 1.0f);
}