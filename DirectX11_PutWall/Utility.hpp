#pragma once

#pragma region Float2

struct Float2 : public DirectX::XMFLOAT2
{
public:
				Float2() :DirectX::XMFLOAT2()
				{

				}
				Float2(float x, float y) :DirectX::XMFLOAT2(x, y)
				{

				}
				Float2(float value) :DirectX::XMFLOAT2(value, value)
				{

				}
				Float2(const DirectX::XMVECTOR& vector) :DirectX::XMFLOAT2()
				{
								DirectX::XMStoreFloat2(this, vector);
				}
				Float2& operator=(const DirectX::XMVECTOR& vector)
				{
								DirectX::XMStoreFloat2(this, vector);
								return *this;
				}
				operator DirectX::XMVECTOR() const noexcept
				{
								return DirectX::XMLoadFloat2(this);
				}
				Float2 operator+() const
				{
								return Float2(this->x, this->y);
				}
				Float2 operator-() const
				{
								return Float2(-this->x, -this->y);
				}
				Float2& operator=(const Float2& value)
				{
								x = value.x;
								y = value.y;
								return *this;
				}
				Float2& operator=(const float& value)
				{
								x = value;
								y = value;
								return *this;
				}
				Float2& operator+=(const Float2& value)
				{
								x += value.x;
								y += value.y;
								return *this;
				}
				Float2& operator+=(const float& value)
				{
								x += value;
								y += value;
								return *this;
				}
				Float2& operator-=(const Float2& value)
				{
								x -= value.x;
								y -= value.y;
								return *this;
				}
				Float2& operator-=(const float& value)
				{
								x -= value;
								y -= value;
								return *this;
				}
				Float2& operator*=(const Float2& value)
				{
								x *= value.x;
								y *= value.y;
								return *this;
				}
				Float2& operator*=(const float& value)
				{
								x *= value;
								y *= value;
								return *this;
				}
				Float2& operator/=(const Float2& value)
				{
								x /= value.x;
								y /= value.y;
								return *this;
				}
				Float2& operator/=(const float& value)
				{
								x /= value;
								y /= value;
								return *this;
				}
};

inline Float2 operator+(const Float2& t1, const Float2& t2)
{
				return Float2(t1) += t2;
}

inline Float2 operator+(const Float2& t1, const float& t2)
{
				return Float2(t1) += t2;
}

inline Float2 operator-(const Float2& t1, const Float2& t2)
{
				return Float2(t1) -= t2;
}

inline Float2 operator-(const Float2& t1, const float& t2)
{
				return Float2(t1) -= t2;
}

inline Float2 operator*(const Float2& t1, const Float2& t2)
{
				return Float2(t1) *= t2;
}

inline Float2 operator*(const Float2& t1, const float& t2)
{
				return Float2(t1) *= t2;
}

inline Float2 operator/(const Float2& t1, const Float2& t2)
{
				return Float2(t1) /= t2;
}

inline Float2 operator/(const Float2& t1, const float& t2)
{
				return Float2(t1) /= t2;
}
#pragma endregion

#pragma region Float3

struct Float3 : public DirectX::XMFLOAT3
{
public:
				Float3() :DirectX::XMFLOAT3()
				{

				}
				Float3(float x, float y, float z) :DirectX::XMFLOAT3(x, y, z)
				{

				}
				Float3(float value) :DirectX::XMFLOAT3(value, value, value)
				{

				}
				Float3(const DirectX::XMVECTOR& vector) :DirectX::XMFLOAT3()
				{
								DirectX::XMStoreFloat3(this, vector);
				}
				Float3& operator=(const DirectX::XMVECTOR& vector)
				{
								DirectX::XMStoreFloat3(this, vector);
								return *this;
				}
				operator DirectX::XMVECTOR() const noexcept
				{
								return DirectX::XMLoadFloat3(this);
				}
				Float3 operator+() const
				{
								return Float3(this->x, this->y, this->z);
				}
				Float3 operator-() const
				{
								return Float3(-this->x, -this->y, -this->z);
				}
				Float3& operator=(const Float3& value)
				{
								x = value.x;
								y = value.y;
								z = value.z;
								return *this;
				}
				Float3& operator=(const float& value)
				{
								x = value;
								y = value;
								z = value;
								return *this;
				}
				Float3& operator+=(const Float3& value)
				{
								x += value.x;
								y += value.y;
								z += value.z;
								return *this;
				}
				Float3& operator+=(const float& value)
				{
								x += value;
								y += value;
								z += value;
								return *this;
				}
				Float3& operator-=(const Float3& value)
				{
								x -= value.x;
								y -= value.y;
								z -= value.z;
								return *this;
				}
				Float3& operator-=(const float& value)
				{
								x -= value;
								y -= value;
								z -= value;
								return *this;
				}
				Float3& operator*=(const Float3& value)
				{
								x *= value.x;
								y *= value.y;
								z *= value.z;
								return *this;
				}
				Float3& operator*=(const float& value)
				{
								x *= value;
								y *= value;
								z *= value;
								return *this;
				}
				Float3& operator/=(const Float3& value)
				{
								x /= value.x;
								y /= value.y;
								z /= value.z;
								return *this;
				}
				Float3& operator/=(const float& value)
				{
								x /= value;
								y /= value;
								z /= value;
								return *this;
				}
};

inline Float3 operator+(const Float3& t1, const Float3& t2)
{
				return Float3(t1) += t2;
}

inline Float3 operator+(const Float3& t1, const float& t2)
{
				return Float3(t1) += t2;
}

inline Float3 operator-(const Float3& t1, const Float3& t2)
{
				return Float3(t1) -= t2;
}

inline Float3 operator-(const Float3& t1, const float& t2)
{
				return Float3(t1) -= t2;
}

inline Float3 operator*(const Float3& t1, const Float3& t2)
{
				return Float3(t1) *= t2;
}

inline Float3 operator*(const Float3& t1, const float& t2)
{
				return Float3(t1) *= t2;
}

inline Float3 operator/(const Float3& t1, const Float3& t2)
{
				return Float3(t1) /= t2;
}

inline Float3 operator/(const Float3& t1, const float& t2)
{
				return Float3(t1) /= t2;
}
#pragma endregion

#pragma region Float4

struct Float4 : public DirectX::XMFLOAT4
{
public:
				Float4() :DirectX::XMFLOAT4()
				{

				}
				Float4(float x, float y, float z, float w) :DirectX::XMFLOAT4(x, y, z, w)
				{

				}
				Float4(float value) :DirectX::XMFLOAT4(value, value, value, value)
				{

				}
				Float4(const DirectX::XMVECTOR& vector) :DirectX::XMFLOAT4()
				{
								DirectX::XMStoreFloat4(this, vector);
				}
				Float4& operator=(const DirectX::XMVECTOR& vector)
				{
								DirectX::XMStoreFloat4(this, vector);
								return *this;
				}
				operator DirectX::XMVECTOR() const noexcept
				{
								return DirectX::XMLoadFloat4(this);
				}
				Float4 operator+() const
				{
								return Float4(this->x, this->y, this->z, this->w);
				}
				Float4 operator-() const
				{
								return Float4(-this->x, -this->y, -this->z, -this->w);
				}
				Float4& operator=(const Float4& value)
				{
								x = value.x;
								y = value.y;
								z = value.z;
								w = value.w;
								return *this;
				}
				Float4& operator=(const float& value)
				{
								x = value;
								y = value;
								z = value;
								w = value;
								return *this;
				}
				Float4& operator+=(const Float4& value)
				{
								x += value.x;
								y += value.y;
								z += value.z;
								w += value.w;
								return *this;
				}
				Float4& operator+=(const float& value)
				{
								x += value;
								y += value;
								z += value;
								w += value;
								return *this;
				}
				Float4& operator-=(const Float4& value)
				{
								x -= value.x;
								y -= value.y;
								z -= value.z;
								w -= value.w;
								return *this;
				}
				Float4& operator-=(const float& value)
				{
								x -= value;
								y -= value;
								z -= value;
								w -= value;
								return *this;
				}
				Float4& operator*=(const Float4& value)
				{
								x *= value.x;
								y *= value.y;
								z *= value.z;
								w *= value.w;
								return *this;
				}
				Float4& operator*=(const float& value)
				{
								x *= value;
								y *= value;
								z *= value;
								w *= value;
								return *this;
				}
				Float4& operator/=(const Float4& value)
				{
								x /= value.x;
								y /= value.y;
								z /= value.z;
								w /= value.w;
								return *this;
				}
				Float4& operator/=(const float& value)
				{
								x /= value;
								y /= value;
								z /= value;
								w /= value;
								return *this;
				}
};

inline Float4 operator+(const Float4& t1, const Float4& t2)
{
				return Float4(t1) += t2;
}

inline Float4 operator+(const Float4& t1, const float& t2)
{
				return Float4(t1) += t2;
}

inline Float4 operator-(const Float4& t1, const Float4& t2)
{
				return Float4(t1) -= t2;
}

inline Float4 operator-(const Float4& t1, const float& t2)
{
				return Float4(t1) -= t2;
}

inline Float4 operator*(const Float4& t1, const Float4& t2)
{
				return Float4(t1) *= t2;
}

inline Float4 operator*(const Float4& t1, const float& t2)
{
				return Float4(t1) *= t2;
}

inline Float4 operator/(const Float4& t1, const Float4& t2)
{
				return Float4(t1) /= t2;
}

inline Float4 operator/(const Float4& t1, const float& t2)
{
				return Float4(t1) /= t2;
}
#pragma endregion

struct Vertex
{
				Float3 position;
				Float3 normal;
				Float2 uv;
				uint32_t blendIndices[8] = { 999,999, 999, 999, 999, 999, 999, 999 };
				float blendWeights[8] = { 0.0f,0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

				Vertex() {}

				Vertex(Float3 position, Float3 normal, Float2 uv)
				{
								this->position = position;
								this->normal = normal;
								this->uv = uv;
				}
};

//６面のボックスのデータを作成時に各面のテクスチャを変えたいときに使用する
#pragma region UvData
struct UvData
{
				Float2 uv;			//画像分割数 1から分割数-1
				Float2 numUv[6];	//分割した場合どこを描画するか(四角の場合に6面違うテクスチャを使う可能性があるため配列6)

				//初期化
				UvData()
				{
								uv = Float2(1.0f, 1.0f);
								for (int i = 0; i < 6; i++)
								{
												numUv[i] = Float2(0.0f, 0.0f);
								}
				}
				//引数分だけ分割して分割一つの大きさを入れる
				void SetDivide(Float2 uv)
				{
								if (uv.x < 0 && uv.y < 0)
								{
												//例外処理
								}
								uv = uv;
				}

				//6面全部のデータを変更する
				void SetNumUvAll(Float2 numUv)
				{
								if (numUv.x < 0.0f || numUv.x < uv.x ||
												numUv.y < 0.0f || numUv.y < uv.y)
								{
												//例外処理
								}
								for (int i = 0; i < 6; i++)
								{
												this->numUv[i] = numUv;
								}
				}

				//指定した面のuvを設定する
				void SetNumUvSelect(Float2 numUv,int num)
				{
								if (numUv.x < 0.0f || numUv.x < uv.x ||
												numUv.y < 0.0f || numUv.y < uv.y)
								{
												//例外処理
								}
								this->numUv[num] = numUv;
				}
};
#pragma endregion