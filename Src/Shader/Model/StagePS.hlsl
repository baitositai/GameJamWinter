// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"
 
// IN
#define PS_INPUT VertexToPixelLitFog
 
// PS
// スロット8～10
// shadowMap0Sampler、shadowMap0Texture
#define SHADOWMAP 1
#include "../Common/Pixel/PixelShader3DHeader.hlsli"
 
// 定数バッファ：スロット4番目
cbuffer cbParam : register(b4)
{
    float4 g_diff_color;
    float3 g_light_dir;
    float g_sakura_boost_amount; // 桜の強調量 (例: 1.0f ～ 5.0f)
    float g_sakura_target_hue; // 桜の目標色相 (0.0f～1.0f)
}

// RGB (0.0～1.0) を HSV (0.0～1.0) に変換する関数
float3 RgbToHsv(float3 c)
{
    float4 K = float4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    
    // RGBの最小値と最大値を求める
    float M = max(c.r, max(c.g, c.b));
    float m = min(c.r, min(c.g, c.b));
    float d = M - m;
    
    float3 s_v;
    s_v.yz = M;
    s_v.x = (M > 0.0) ? (d / M) : 0.0;
    
    float3 P = (c.g < c.b) ? float3(K.z, K.w, K.y) : float3(K.y, K.w, K.z);
    
    float3 h_temp = c - M;
    h_temp.y += M;
    h_temp.z += M;
    
    float3 h_calc = h_temp * P.x + P.y;
    h_calc = (h_calc * d) / (d + 1e-6);
    
    float h = step(M, c.r) * h_calc.x +
              step(M, c.g) * h_calc.y +
              step(M, c.b) * h_calc.z;
    
    h = (h < 0.0) ? (h + 1.0) : h;
    h = (d == 0.0) ? 0.0 : h;
    
    return float3(h, s_v.x, s_v.y);
}
 
float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float4 color;
	// テクスチャーの色を取得
    color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);
    if (color.a < 0.01f)
    {
        discard;
    }
    float3 material = color.rgb;
    
    float3 hsv = RgbToHsv(material); // 仮の変換関数
    float currentHue = hsv.r; // Rチャンネルに色相(Hue)が格納されているとする
    
    // 現在の色相が目標の色相に近いか調べる
    float hueDiff = abs(currentHue - g_sakura_target_hue);
    hueDiff = min(hueDiff, 1.0f - hueDiff);
    
    // 検出した色相から強調マスクの生成
    float tolerance = 0.05f; // 許容する色相差の範囲
    float detectionMask = saturate(1.0f - (hueDiff / tolerance));
    
    // 暗い色などを除外
    float satValueMask = saturate(hsv.g * 2.0f) * saturate(hsv.b * 1.5f);
    
    // 最終的な強調マスク
    float sakuraMask = detectionMask * satValueMask;
    
    // 強調色
    float3 targetEmissiveColor = float3(1.0f, 0.7f, 0.8f); // 強調したい明るいピンク
    
    float3 sakuraBoost = targetEmissiveColor * sakuraMask * g_sakura_boost_amount;
    
    // 色の加算
    float3 boostColor = material + sakuraBoost;
    
    	// 【影】深度テクスチャの座標を算出
	//---------------------------------------------------------------------------------------------
	// PSInput.LPPosition.xy は -1.0f ～ 1.0f の値なので、これを 0.0f ～ 1.0f の値にする
    float2 depthUV;
    depthUV.x = (PSInput.lightAtPos.x + 1.0f) / 2.0f;
 
	// yは更に上下反転
    depthUV.y = 1.0f - (PSInput.lightAtPos.y + 1.0f) / 2.0f;
 
	// 深度バッファテクスチャから深度を取得
    float depth = shadowMap0Texture.Sample(shadowMap0Sampler, depthUV).r;
 
	// テクスチャに記録されている深度( +補正値 )よりＺ値が大きかったら奥にあるということで輝度を半分にする
    if (PSInput.lightAtPos.z > depth + 0.001f)
    {
        boostColor.rgb *= 0.5f;
    }
	//---------------------------------------------------------------------------------------------
 
    return float4(boostColor.rgb, color.a);
    
	// フォグ改良
 //   //color.rgb *= (PSInput.fogFactor + PSInput.lightPower * 0.8f);
	//// ポイントライト
 //   float3 plColor = float3(0.8f, 0.8f, 0.6f);
 //   color.rgb += (plColor * PSInput.lightPower);
	//// ④ワールド空間同士でライトの反対方向と法線の内積を取る
	//// 　同じ方向なほど 0.0 → 1.0 に近づくので、明るくなる
 //   float3 lightDot = dot(PSInput.normal, -g_light_dir);
 //   float3 rgb = color.rgb * lightDot * g_diff_color.rgb;

 
	//// ⑤色の乗算のため、暗くなりがち。微調整で明るくしてみる。
	//// 　超簡単なライト処理の出来上がり
 //   rgb *= 1.3f;
 
 
	//// 【影】深度テクスチャの座標を算出
	////---------------------------------------------------------------------------------------------
	//// PSInput.LPPosition.xy は -1.0f ～ 1.0f の値なので、これを 0.0f ～ 1.0f の値にする
 //   float2 depthUV;
 //   depthUV.x = (PSInput.lightAtPos.x + 1.0f) / 2.0f;
 
	//// yは更に上下反転
 //   depthUV.y = 1.0f - (PSInput.lightAtPos.y + 1.0f) / 2.0f;
 
	//// 深度バッファテクスチャから深度を取得
 //   float depth = shadowMap0Texture.Sample(shadowMap0Sampler, depthUV).r;
 
	//// テクスチャに記録されている深度( +補正値 )よりＺ値が大きかったら奥にあるということで輝度を半分にする
 //   if (PSInput.lightAtPos.z > depth + 0.001f)
 //   {
 //       rgb *= 0.5f;
 //   }
	////---------------------------------------------------------------------------------------------
 
 //   return float4(rgb, color.a);
 
}