#pragma once
#include <DxLib.h>
#include <functional>
#include <unordered_map>
#include "../../Common/Quaternion.h"

class Transform;
class InputManager;

class Camera
{

public:

	// カメラスピード(度)
	static constexpr float SPEED = 1.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 10.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_FAR = 30000.0f;

	// カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// 追従位置からカメラ位置までの相対座標
	static constexpr VECTOR LOCAL_F2C_POS_FOLLOW = { 0.0f, 50.0f, -400.0f };
	static constexpr VECTOR LOCAL_F2C_POS_FPS = { 0.0f, 130.0f, 20.0f };

	// 追従位置から注視点までの相対座標
	static constexpr VECTOR LOCAL_F2T_POS_FOLLOW = { 0.0f, 0.0f, 200.0f };
	static constexpr VECTOR LOCAL_F2T_POS_FPS = { 0.0f, 0.0f, 2000.0f };

	// カメラのX回転上限度角
	static constexpr float LIMIT_X_UP_RAD = 40.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD = 15.0f * (DX_PI_F / 180.0f);

	//FPS視点のカメラのX回転上限度角
	static constexpr float LIMIT_X_UP_RAD_FPS = -20.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD_FPS = 50.0f * (DX_PI_F / 180.0f);
	
	/// <summary>
	/// カメラのモード
	/// </summary>
	enum class MODE
	{
		NONE,			// 初期化前
		FIXED_POINT,	// 固定点
		FOLLOW,			// 追従
		FPS,			// FPS視点
		FREE,			// 自由
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name=""></param>
	Camera();

	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <param name=""></param>
	~Camera() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name=""></param>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name=""></param>
	void Update(void) {};

	/// <summary>
	/// カメラの設定描画
	/// </summary>
	void SetBeforeDraw();	

	/// <summary>
	/// カメラの設定
	/// </summary>
	void CameraSetting();
		
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name=""></param>
	void Draw() {};

	/// <summary>
	/// カメラモードの変更
	/// </summary>
	/// <param name="mode">カメラモード</param>
	void ChangeMode(MODE mode);

	/// <summary>
	/// カメラ位置の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>カメラ位置</returns>
	VECTOR GetPos() const;
	
	/// <summary>
	/// カメラアングルの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>カメラアングル</returns>
	VECTOR GetAngles() const;
	
	/// <summary>
	/// カメラの注視点の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>カメラ注視点</returns>
	VECTOR GetTargetPos() const;

	/// <summary>
	/// カメラ角度の取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>カメラ角度</returns>
	Quaternion GetQuaRot() const;
	
	/// <summary>
	/// X回転を抜いたカメラ角度
	/// </summary>
	/// <param name=""></param>
	/// <returns> X回転を抜いたカメラ角度</returns>
	Quaternion GetQuaRotOutX() const;
	
	/// <summary>
	/// カメラの前方方向ベクトルの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns>カメラの前方方向ベクトル</returns>
	VECTOR GetForward() const;

	/// <summary>
	/// モードの取得
	/// </summary>
	/// <returns>モード</returns>
	MODE GetMode() const;

	/// <summary>
	/// ライトカメラ座標を返す
	/// </summary>
	/// <returns></returns>
	const VECTOR& GetCameraLightPos() const;

	/// <summary>
	/// カメラ位置の設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPos(const VECTOR& pos);

	/// <summary>
	/// 追従対象の設定
	/// </summary>
	/// <param name="follow">追従対象</param>
	void SetFollow(const Transform* follow);

	/// <summary>
	/// 注視点の設定
	/// </summary>
	/// <param name="targetPos">注視点</param>
	void SetTargetPos(const VECTOR& targetPos);

	/// <summary>
	/// 角度の設定
	/// </summary>
	/// <param name="angles">角度</param>
	void SetAngles(const VECTOR& angles);

private:

	InputManager& input_;

	// カメラが追従対象とするTransform
	const Transform* followTransform_;

	// カメラモード
	MODE mode_;

	// カメラの位置
	VECTOR pos_;

	// カメラ角度(rad)
	VECTOR angles_;

	// X軸回転が無い角度
	Quaternion rotOutX_;

	// カメラ角度
	Quaternion rot_;

	// 注視点
	VECTOR targetPos_;

	// カメラの上方向
	VECTOR cameraUp_;

	// カメラライト座標
	VECTOR cameraLightPos_;

	// モード別更新ステップ関数管理
	std::function<void()> beforeDrawFunc_;

	// モード遷移処理
	std::unordered_map<MODE, std::function<void()>> changeModeMap_;

	// カメラを初期位置に戻す
	void SetDefault();

	// 追従対象との位置同期を取る
	void SyncFollow();
	void SyncFollowFps();

	// カメラ操作
	void ProcessRotFollow();	// 追従モードの回転操作
	void ProcessRotFps();		// FPSモードの回転操作
	void ProcessRotFree();		// 自由モードの回転操作

	//モード別状態遷移処理
	void ChangeModeNone();
	void ChangeModeFixedPoint();
	void ChangeModeFollow();
	void ChangeModeFps();
	void ChangeModeFree();

	// モード別更新ステップ
	void SetBeforeDrawNone() {};
	void SetBeforeDrawFixedPoint();
	void SetBeforeDrawFollow();
	void SetBeforeDrawFps();
	void SetBeforeDrawFree();
};

