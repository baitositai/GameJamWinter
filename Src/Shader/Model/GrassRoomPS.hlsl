//VS・PS共通
#include "../Common/VertexToPixelHeader.hlsli"
#include "../Common/Pixel/PixelShaderCommonFunction.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

// 法線マップ有効フラグ
#define BUMPMAP 1

// サブテクスチャの有効フラグ
#define SUBTEXTUREMODE 7

//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// スポットライトの角度
static float SPOT_LIGHT_ANGLE_COS = cos(radians(30.0f));

// タイリング
static float TILING = 300.0f;

// 境界までの幅
static float EDGE_WIDTH = 5.0f;

// ハイライト色
static float3 HIGHLIGHT_COLOR = (1.0f, 1.0f, 1.0f);

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float3 g_light_dir;         // ライト方向
    float dummy;
    
    float3 g_ambient_color;     // 環境光
    float dummy2;
    
    float3 g_spot_light_pos;    // スポットライト位置
    float g_is_light;           // ライトの電源(0オフ、1オン)
    
    float3 g_spot_light_dir;    // スポットライト方向
    float dummy4;
    
    float3 g_start_pos;         // 開始位置
    float g_distance;           // 開始位置からの距離
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float2 uv = PSInput.uv;
 
    // ベースカラー
    float4 texColor = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    float3 material = texColor.rgb;
    
    // 法線計算
    float3 tanNormal = normalize(normalMapTexture.Sample(normalMapSampler, uv).xyz * 2 - 1);
    float3 normal = CalculateNormal(PSInput.tan, PSInput.bin, PSInput.normal, tanNormal);
    
    // 開始位置とワールド座標の差を計算
    float3 diff = g_start_pos - PSInput.world;
    
    // 求めた差から距離を計算
    float distance = length(diff);
    
    // ハイライトを計算する用
    float highlightIntensity = 0.0f;
    
    // 頂点位置までの距離が範囲内の場合
    if (g_distance > distance)
    {
        // 3方向のUVを生成（それぞれタイリングとfracを適用）
        float2 uv_xz = frac(PSInput.world.xz / TILING);
        float2 uv_yz = frac(PSInput.world.yz / TILING);
        float2 uv_xy = frac(PSInput.world.xy / TILING);

        // 3方向からサンプリング
        float4 color_xz = subTexture.Sample(subSampler, uv_xz);
        float4 color_yz = subTexture.Sample(subSampler, uv_yz);
        float4 color_xy = subTexture.Sample(subSampler, uv_xy);

        // 法線の絶対値をブレンドウェイトとして使用
        float3 blend_weights = abs(normal);

        // ウェイトを正規化（合計を1.0にする）
        // エッジでの切り替わりを滑らかにするため、ウェイトを累乗しても良い (例: pow(abs(normal), 2.0))
        float weight_sum = blend_weights.x + blend_weights.y + blend_weights.z;
        blend_weights /= weight_sum;

        // 最終的な色をブレンドして決定
        float4 subColor = color_xz * blend_weights.y + // Y軸支配(床/天井)
                  color_yz * blend_weights.x + // X軸支配(壁)
                  color_xy * blend_weights.z; // Z軸支配(壁)
        
        // 色を加算
        material = subColor.rgb;
        
        // 拡大距離に対する現在のdistanceの比率
        float ratio = distance / g_distance;
        
        // 境界付近でピークになるように計算(smoothstep(最小値, 最大値, 現在の値))
        highlightIntensity = 0.5f * (1.0f - smoothstep(0.8f, 1.0f, 1.0f - (1.0f - ratio) * 2.0f));
        
        // 境界からの距離
        float distEdge = abs(g_distance - distance);
        
        // 境界からの距離が0近いほど1に近くなる
        highlightIntensity = 1.0f - saturate(distEdge / EDGE_WIDTH);
        
        // 強度調整
        highlightIntensity *= 2.0f; 
    }
 
    // 光の方向
    float3 lightDir = normalize(g_light_dir.xyz);
    float NdotL = max(0.0f, dot(normal, -lightDir));
    
    // 明部減衰（明るい色ほど環境光を弱める）
    float3 ambientAttenuated = CalculateAmbientAttenuation(material, g_ambient_color);
    
    // ディフューズ
    float3 diffuse = material * NdotL;
 
    // 最終カラー 
    float3 litColor = saturate(ambientAttenuated + diffuse) * 0.8f;
    
    // サブテクスチャ色の取得
    float4 subTexColor = subTexture.Sample(subSampler, uv);
    litColor += subTexColor.rgb * 0.0f;
   
    // フォグ適用
    float3 foggedColor = ApplyFog(litColor, PSInput.fogFactor);
    
    //// ポイントライト
    //foggedColor += (POINT_LIGHT_COLOR * PSInput.lightPower);
    
    // 電源がオンの場合
    // スポットライトの色計算
    float3 spotLight = CalculateSpotLite(PSInput.world, g_spot_light_pos, g_spot_light_dir, normal);
    
    // ハイライトの加算
    foggedColor += HIGHLIGHT_COLOR * highlightIntensity;

    // 色の加算(電源がオフの場合0乗算で追加値なし)
    foggedColor += spotLight * g_is_light;
    
    // 色の出力
    return float4(foggedColor, texColor.a);
}