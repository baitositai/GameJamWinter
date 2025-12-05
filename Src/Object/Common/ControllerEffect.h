#pragma once
#include <DxLib.h>
#include "../../Common/Quaternion.h"

class ControllerEffect
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">エフェクトハンドル</param>
	ControllerEffect(const int handle);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ControllerEffect();

	/// <summary>
	/// 再生
	/// </summary>
	void Play(const VECTOR& pos, const Quaternion& quaRot, const VECTOR scl, const float speed, const bool isLoop = true);
	
	/// <summary>
	/// 停止
	/// </summary>
	void Stop();

	/// <summary>
	/// 終了判定
	/// </summary>
	/// <returns>trueの場合終了済み, falseの場合実行中</returns>
	bool IsEnd() const;	
	
	/// <summary>
	/// 更新(毎フレーム動く場合に用いる)
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="quaRot">回転情報</param>
	/// <param name="scl">スケール</param>
	/// <param name="speed">速度</param>
	void Update(const VECTOR& pos, const Quaternion& quaRot, const VECTOR scl, const float speed);

	/// <summary>
	/// 位置の設定
	/// </summary>
	/// <param name="pos">位置</param>
	void SetPos(const VECTOR& pos);

	/// <summary>
	/// 回転情報の設定
	/// </summary>
	/// <param name="quaRot">回転情報</param>
	void SetQuaternion(const Quaternion& quaRot);

	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="scl">スケール</param>
	void SetScale(const VECTOR& scl);

	/// <summary>
	/// 速度設定
	/// </summary>
	/// <param name="speed"></param>
	void SetSpeed(const float speed);

private:

	// 再生ID
	int playId_;	
	
	// 速度
	float speed_;

	// 座標
	VECTOR pos_;

	// 回転情報
	Quaternion quaRot_;

	// スケール
	VECTOR scl_;
};