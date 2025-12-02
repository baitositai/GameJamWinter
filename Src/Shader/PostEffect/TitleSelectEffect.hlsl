#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// スロット4番目(b4)に登録
cbuffer cbParam : register(b4)
{
    float g_time;               // 更新用
    float g_glitch_strength;    // グリッチの強さ
    float g_screen_height;      // 画面高さ
    float dummy;
};

float Hash(float p)
{
    // Y座標と時間を混ぜてフラクショナルパート（小数部分）を取り出す
    return frac(sin(p * 12.9898 + g_time * 78.233) * 43758.5453);
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // UV値取得
    float2 uv = PSInput.uv;
    
    // ピクセル単位のY座標取得
    float pixelY = uv.y * g_screen_height;
    
    // ブロック状のオフセットを計算
    float blockY = floor(pixelY / 15.0f);   // 15ピクセルごとに同じ値を返すブロックを作成
    
    // ハッシュ関数でランダムなずれの大きさを計算
    float hash = Hash(blockY);
    
    // ハッシュ値が条件値未満の場合
    if (hash < 0.7)
    {
        // ズレを発生しない
        hash = 0.0f;
    }
    else
    {
        // ズレを計算
        hash = (hash - 0.7f) / 0.3f;
    }
    
    // オフセットを計算してズレ幅を決定
    float offset = (Hash(blockY + 10.0) - 0.5) * 2.0; // -1.0～1.0
    float u = offset * hash * g_glitch_strength;
    
    // 新しいUV座標
    float newUv = float2(uv.x + u, uv.y);
    
    // 調整した座標でテクスチャを参照
    float4 color = tex.Sample(texSampler, newUv);

    // 8. (オプション) わずかなデジタルノイズをブレンド
    // ピクセルY座標とTimeを使って、ランダムな輝度ノイズを生成
    float noise = Hash(pixelY * 50.0 + uv.x * 100.0) * 0.1; // 0.0～0.1
    
    // グリッチが発生していない場合
    if (hash == 0.0)
    {
        // ノイズを加える
        color.rgb += noise;
    }

    return color;
}   