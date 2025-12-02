//VS・PS共通
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

// 法線マップ有効フラグ
#define BUMPMAP 1

// サブテクスチャの有効フラグ
//#define SUBTEXTUREMODE 7

//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

SamplerState subSampler : register(s3); // サブテクスチャ
Texture2D subTexture : register(t3); // サブテクスチャ

static float3 POINT_LIGHT_COLOR = { 0.4f, 0.4f, 0.3f };
static float3 FOG_COLOR = { 0.0f, 0.0f, 0.0f };

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 g_color; // 色
    float3 g_light_dir; // ライト方向
    float dummy;
    float3 g_ambient_color; // 環境光
    float dummy2;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float2 uv = PSInput.uv;
 
    // ベースカラー
    float4 texColor = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    float3 material = texColor.rgb;
 
    // 法線計算
    const float3 tanNormal = normalize(normalMapTexture.Sample(normalMapSampler, uv).xyz * 2 - 1);
    const float3x3 tangentViewMat = transpose(float3x3(normalize(PSInput.tan), normalize(PSInput.bin), normalize(PSInput.normal)));
    const float3 normal = normalize(mul(tangentViewMat, tanNormal));
 
    // 光の方向
    float3 lightDir = normalize(g_light_dir.xyz);
    float NdotL = max(0.0f, dot(normal, -lightDir));
 
    // 環境光（暗いマテリアルを持ち上げる）
    float3 ambientBase = material * g_ambient_color.rgb;
    
    ////// 暗部補正（暗い色ほど環境光を強める）
    //float brightness = dot(material, float3(0.299, 0.587, 0.114)); // 輝度
    //float darkBoost = saturate(1.0 - brightness); // 暗いほど1.0に近い
    //float3 ambientBoost = ambientBase * (1.0 + darkBoost * 0.6); // 最大+60%強化
    
    // 明部減衰（明るい色ほど環境光を弱める）
    float brightness = dot(material, float3(0.299, 0.587, 0.114)); // 輝度 (0.0 ～ 1.0)
    float lightAttenuation = saturate(brightness); // 明るいほど1.0に近い
    float attenuationFactor = 1.0 - lightAttenuation * 1.0f; // 最大40%減衰
    float3 ambientAttenuated = ambientBase * attenuationFactor;
    
    // ディフューズ
    float3 diffuse = material * NdotL;
    //float3 diffuse = material;
 
    // 最終カラー 
    float3 litColor = saturate(ambientAttenuated + diffuse) * 0.4f;    
    
    // 手形テクスチャ色の取得
    float4 handTexColor = subTexture.Sample(subSampler, uv);
    litColor += handTexColor.rgb;
 
    // フォグ適用
    float fogFactor = saturate(1.0f - PSInput.fogFactor); // 0=カメラ近, 1=遠
    float3 foggedColor = lerp(litColor, FOG_COLOR, fogFactor);
    
    // ポイントライト
    foggedColor += (POINT_LIGHT_COLOR * PSInput.lightPower);

    return float4(foggedColor, texColor.a);
}