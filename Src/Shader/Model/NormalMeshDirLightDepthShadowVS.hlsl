// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_1FRAME

// OUT
#define VS_OUTPUT VertexToPixelShadow
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

matrix cfLightViewMatrix : register(c43);       // ライトのワールド　→　ビュー行列
matrix cfLightProjectionMatrix : register(c47); // ライトのビュー　→　射影行列

cbuffer cbParam : register(b7)
{
    float3 g_camera_pos;
    float g_fog_start;
    
    float3 g_light_pos;
    float g_fog_end;
}

VS_OUTPUT main(VS_INPUT VSInput)
{
    VS_OUTPUT ret;
    float4 lLocalPosition;
    float4 lWorldPosition;
    float4 lViewPosition;
    float4 lLViewPosition;
    float3 lWorldNrm;
    float3 lViewNrm;


	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

    // float3 → float4
    lLocalPosition.xyz = VSInput.pos;
    lLocalPosition.w = 1.0f;
    
    // ローカル座標をワールド座標に変換(剛体)
    lWorldPosition.w = 1.0f;
    lWorldPosition.xyz = mul(lLocalPosition, g_base.localWorldMatrix);
    //ret.w = lWorldPosition.xyz;
    
    // ワールド座標をビュー座標に変換
    lViewPosition.w = 1.0f;
    lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);
    ret.vwPos.xyz = lViewPosition.xyz;
    
    // ビュー座標を射影座標に変換
    ret.svPos = mul(lViewPosition, g_base.projectionMatrix);

	// 頂点座標変換 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )



	// ライトの処理 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// 法線をビュー空間の角度に変換 =========================================( 開始 )

	// ローカルベクトルをワールドベクトルに変換
    lWorldNrm.x = dot(VSInput.norm, g_base.localWorldMatrix[0].xyz);
    lWorldNrm.y = dot(VSInput.norm, g_base.localWorldMatrix[1].xyz);
    lWorldNrm.z = dot(VSInput.norm, g_base.localWorldMatrix[2].xyz);

	// ワールドベクトルをビューベクトルに変換
    lViewNrm.x = dot(lWorldNrm, g_base.viewMatrix[0].xyz);
    lViewNrm.y = dot(lWorldNrm, g_base.viewMatrix[1].xyz);
    lViewNrm.z = dot(lWorldNrm, g_base.viewMatrix[2].xyz);

	// 法線を正規化
    lViewNrm = normalize(lViewNrm);

	// 法線をビュー空間の角度に変換 =========================================( 終了 )


	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// ディフューズカラー
    ret.diffuse = VSInput.diffuse;

	// テクスチャ座標のセット
    ret.uv.x = VSInput.uv0.x;
    ret.uv.y = VSInput.uv0.y;

	// 出力パラメータセット ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )

	// 深度影用のライトから見た射影座標を算出 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	// ワールド座標をライトのビュー座標に変換
    lLViewPosition = mul(cfLightViewMatrix, lWorldPosition);

	// ライトのビュー座標をライトの射影座標に変換
    ret.lpPos = mul(cfLightProjectionMatrix, lLViewPosition);

	// Ｚ値だけはライトのビュー座標にする
    ret.lpPos.z = lLViewPosition.z;

	// 深度影用のライトから見た射影座標を算出 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 終了 )


	// 出力パラメータを返す
    return ret;
}