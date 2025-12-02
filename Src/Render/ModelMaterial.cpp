#include "../Application.h"
#include "ModelMaterial.h"

ModelMaterial::ModelMaterial(int vertexShader, int constBufFloat4SizeVS, int pixelShader, int constBufFloat4SizePS)
{
	// 頂点シェーダの格納
	shaderVS_ = vertexShader;

	// 頂点定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	constBufFloat4SizeVS_ = constBufFloat4SizeVS;

	// 頂点シェーダー用の定数バッファを作成
	constBufVS_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizeVS);

	// ピクセルシェーダの格納
	shaderPS_ = pixelShader;

	// ピクセル定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	constBufFloat4SizePS_ = constBufFloat4SizePS;

	// ピクセルシェーダー用の定数バッファを作成
	constBufPS_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizePS);

	// テクスチャアドレス
	texAddress_ = TEXADDRESS::CLAMP;
}

void ModelMaterial::AddConstBufVS(const FLOAT4& contBuf)
{

	if (constBufFloat4SizeVS_ > constBufsVS_.size())
	{
		constBufsVS_.emplace_back(contBuf);
	}

}

void ModelMaterial::AddConstBufPS(const FLOAT4& contBuf)
{

	if (constBufFloat4SizePS_ > constBufsPS_.size())
	{
		constBufsPS_.emplace_back(contBuf);
	}

}

void ModelMaterial::SetConstBufVS(int idx, const FLOAT4& contBuf)
{

	if (idx >= constBufsVS_.size())
	{
		return;
	}

	constBufsVS_[idx] = contBuf;

}

void ModelMaterial::SetConstBufPS(int idx, const FLOAT4& contBuf)
{

	if (idx >= constBufsPS_.size())
	{
		return;
	}

	constBufsPS_[idx] = contBuf;

}

void ModelMaterial::SetTextureBuf(int slot, int texDiffuse)
{

	if (textures_.count(slot) == 0)
	{
		textures_.emplace(slot, texDiffuse);
	}
	else
	{
		textures_[slot] = texDiffuse;
	}

}

const std::vector<FLOAT4>& ModelMaterial::GetConstBufsVS(void) const
{
	return constBufsVS_;
}

const std::vector<FLOAT4>& ModelMaterial::GetConstBufsPS(void) const
{
	return constBufsPS_;
}

const std::map<int, int>& ModelMaterial::GetTextures(void) const
{
	return textures_;
}

ModelMaterial::TEXADDRESS ModelMaterial::GetTextureAddress(void) const
{
	return texAddress_;
}

void ModelMaterial::SetTextureAddress(TEXADDRESS texA)
{
	texAddress_ = texA;
}

int ModelMaterial::GetShaderVS(void) const
{
	return shaderVS_;
}

int ModelMaterial::GetShaderPS(void) const
{
	return shaderPS_;
}

int ModelMaterial::GetConstBufVS(void) const
{
	return constBufVS_;
}

int ModelMaterial::GetConstBufPS(void) const
{
	return constBufPS_;
}

ModelMaterial::~ModelMaterial(void)
{
	DeleteShaderConstantBuffer(constBufVS_);
	DeleteShaderConstantBuffer(constBufPS_);


	//DeleteShader(shaderVS_);
	//DeleteShader(shaderPS_);
}
