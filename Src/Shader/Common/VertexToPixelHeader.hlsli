
//struct VertexToPixel
//{
//    float4 svPos		: SV_POSITION;
//    float4 diffuse		: COLOR0;
//    float4 specular		: COLOR1;
//    float2 uv			: TEXCOORD;
//};

//struct VertexToPixelLit
//{
//	float4 svPos		: SV_POSITION;	// 座標( プロジェクション空間 )
//	float2 uv			: TEXCOORD0;	// テクスチャ座標
//	float3 vwPos		: TEXCOORD1;	// 座標( ビュー座標 )
//    float3 world        : WORLD_POSITION;
//	float3 normal       : TEXCOORD2;	// 法線( ビュー座標 )
//	float4 diffuse      : COLOR0;		// ディフューズカラー
//    //float3 specular     : COLOR1;       // スペキュラカラー
//    float3 tan          : TANGENT;
//    float3 bin          : BINORMAL;
//    float fogFactor     : FOG;
//    float lightPower    : LIGHT_POW;
//};

//struct VertexToPixelSpecular
//{
//    float4 svPos : SV_POSITION; // 座標( プロジェクション空間 )
//    float2 uv : TEXCOORD0; // テクスチャ座標
//    float3 vwPos : TEXCOORD1; // 座標( ビュー座標 )
//    float3 normal : TEXCOORD2; // 法線( ビュー座標 )
//    float4 diffuse : COLOR0; // ディフューズカラー
//    float3 specular     : COLOR1;       // スペキュラカラー
//    float3 tan : TANGENT;
//    float3 bin : BINORMAL;
//    float fogFactor : FOG;
//    float lightPower : LIGHT_POW;
//};

struct VertexToPixelShadow
{
    // SV_POSITIONは、頂点シェーダーの出力時にのみ使用するべき
    float4 svPos : SV_POSITION; // 座標( プロジェクション空間 )
    float4 svPosRead	: TEXCOORD3;  // svPosReadが何の座標かによりますが、TEXCOORDで渡します
    
    float2 uv : TEXCOORD0; // テクスチャ座標
    float4 vwPos : TEXCOORD1; // 座標( ビュー座標 )
    float3 normal : TEXCOORD2; // 法線
    float4 diffuse : COLOR0; // ディフューズカラー
    float4 lpPos : TEXCOORD4; // LP_POSITION だったものを TEXCOORD3 に変更 (番号は空いているものを使う)
};


struct VertexToPixelLitFog
{
    float4 svPos : SV_POSITION; // 座標( プロジェクション空間 )
    float2 uv : TEXCOORD0; // テクスチャ座標
    float3 vwPos : TEXCOORD1; // 座標( ビュー座標 )
    float3 normal : TEXCOORD2; // 法線( ビュー座標 )
    float4 diffuse : COLOR0; // ディフューズカラー
    float3 lightAtPos : TEXCOORD3; // L_POSITION だったものを TEXCOORD3 に変更
    float fogFactor : TEXCOORD4; // FOGセマンティクスはDXライブラリ特有の可能性あり、もしエラーが出たら TEXCOORD4 に変更
    float lightPower : TEXCOORD5; // LIGHT_POWも同様、エラーが出たら TEXCOORD5 に変更
};