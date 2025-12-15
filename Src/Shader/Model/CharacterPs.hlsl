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
    float dummy;
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
        color.rgb *= 0.5f;
    }
	//---------------------------------------------------------------------------------------------
 
    return float4(color.rgb, color.a);
    
	// フォグ改良
    //color.rgb *= (PSInput.fogFactor + PSInput.lightPower * 0.8f);
	// ポイントライト
    float3 plColor = float3(0.8f, 0.8f, 0.6f);
    color.rgb += (plColor * PSInput.lightPower);
	// ④ワールド空間同士でライトの反対方向と法線の内積を取る
	// 　同じ方向なほど 0.0 → 1.0 に近づくので、明るくなる
    float3 lightDot = dot(PSInput.normal, -g_light_dir);
    float3 rgb = color.rgb * lightDot * g_diff_color.rgb;

 
	// ⑤色の乗算のため、暗くなりがち。微調整で明るくしてみる。
	// 　超簡単なライト処理の出来上がり
    rgb *= 1.3f;
 
}