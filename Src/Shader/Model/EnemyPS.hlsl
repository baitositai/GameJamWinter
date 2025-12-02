//VS・PS共通
#include "../Common/VertexToPixelHeader.hlsli"
#include "../Common/Pixel/PixelShaderCommonFunction.hlsli"

//IN
#define PS_INPUT VertexToPixelLit

// 法線マップ有効フラグ
#define BUMPMAP 1
#define USE_SPETEX 2

//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float3 g_light_dir;     // ライト方向
    float g_shininess;      // スペキュラの輝き度
    
    float3 g_ambient_color; // 環境光
    float dummy2;
    
    float3 g_spot_light_pos; // スポットライト位置
    float g_is_light; // ライトの電源(0オフ、1オン)
    
    float3 g_spot_light_dir; // スポットライト方向
    float dummy4;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{

    //// スペキュラーマップの計算
    //// 視線ベクトル
    //float viewVec = normalize(g_camera_pos - PSInput.vwPos); 
    
    //// ハーフベクトル
    //float3 vecHalf = normalize(lightDir + viewVec);
    
    //// スペキュラマップから強度を取得 (1.で計算)
    //float specularIntensity = specularMapTexture.Sample(specularMapSampler, uv).r;

    //// スペキュラ項の計算
    //float specPower = pow(max(0, dot(normal, vecHalf)), g_shininess);

    //// 最終カラー 
    //float3 litColor = saturate(ambientAttenuated + diffuse + specular);
 

    
    float2 uv = PSInput.uv;
 
    // ベースカラー
    float4 texColor = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    float3 material = texColor.rgb;
    
    // 法線計算
    float3 tanNormal = normalize(normalMapTexture.Sample(normalMapSampler, uv).xyz * 2 - 1);
    float3 normal = CalculateNormal(PSInput.tan, PSInput.bin, PSInput.normal, tanNormal);
    
    // 光の方向
    float3 lightDir = normalize(g_light_dir.xyz);
    float NdotL = max(0.0f, dot(normal, -lightDir));
    
    // 環境光（暗いマテリアルを持ち上げる）
    float3 ambientBase = material * g_ambient_color;
    
    // ディフューズ
    float3 diffuse = material * NdotL;
    
    // 最終的なカラー
    float3 finalColor = saturate(diffuse + ambientBase);
    
    //return float4(diffuse, texColor.a);
    
    // フォグ適用
    float3 foggedColor = ApplyFog(finalColor, PSInput.fogFactor);
    
    //// ポイントライト
    //foggedColor += (POINT_LIGHT_COLOR * PSInput.lightPower);
        
    // スポットライトの色計算
    float3 spotLight = CalculateSpotLite(PSInput.world, g_spot_light_pos, g_spot_light_dir, normal);

    // 色の加算
    foggedColor += spotLight * g_is_light;
    
    // 色の出力
    return float4(foggedColor, texColor.a);
}