//VS・PS共通
#include "../Common/VertexToPixelHeader.hlsli"

//IN
#define PS_INPUT VertexToPixelShadow

// 法線マップ有効フラグ
#define BUMPMAP 1

// サブテクスチャの有効フラグ
#define SHADOWMAP 8

//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 g_color; // 色
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float2 uv = PSInput.uv;
 
    // ベースカラー
    float4 texColor = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    float3 material = texColor.rgb;
    
    float TextureDepth;
    float2 DepthTexCoord;

	// 出力カラー = ディフューズカラー
    material = PSInput.diffuse * texColor;

	// 出力アルファ = ディフューズアルファ * テクスチャアルファ
    float alpha = PSInput.diffuse.a * texColor.a;

	// 深度テクスチャの座標を算出
	// PSInput.LPPosition.xy は -1.0f ～ 1.0f の値なので、これを 0.0f ～ 1.0f の値にする
    DepthTexCoord.x = (PSInput.lpPos.x + 1.0f) / 2.0f;

	// yは更に上下反転
    DepthTexCoord.y = 1.0f - (PSInput.lpPos.y + 1.0f) / 2.0f;

	// 深度バッファテクスチャから深度を取得
    TextureDepth = shadowMap0Texture.Sample(shadowMap0Sampler, DepthTexCoord);

	// テクスチャに記録されている深度( +補正値 )よりＺ値が大きかったら奥にあるということで輝度を半分にする
    if (PSInput.lpPos.z > TextureDepth + 25.0f)
    {
        material.rgb *= 0.5f;
    }

	// 出力カラーをセット
    return float4(material, alpha);
}