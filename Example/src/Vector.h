#pragma once
#include <numbers>
#include <cmath>

int screenWidth = 1920;
int screenHeight = 1080;
//improvized Matrix4x4 (Chat_GPT)
struct Matrix4x4 {
	float m[4][4];
};

typedef struct D3DXMATRIX {
	float m[4][4];
} D3DXMATRIX, * LPD3DXMATRIX;

struct View_matrix_t
{
	float* operator[ ](int index)
	{
		return matrix[index];
	}


	float matrix[4][4];
};

struct Vector3
{
	constexpr Vector3(
		const float x = 0.f,
		const float y = 0.f,
		const float z = 0.f) noexcept :
		x(x), y(y), z(z) { }

	constexpr const Vector3& operator-(const Vector3& other) const noexcept
	{
		return Vector3{ x - other.x, y - other.y, z - other.z };
	}
	constexpr const Vector3& operator+(const Vector3& other) const noexcept
	{
		return Vector3{ x + other.x, y + other.y, z + other.z };
	}
	constexpr const Vector3& operator/(const float factor) const noexcept
	{
		return Vector3{ x / factor, y / factor, z / factor };
	}
	constexpr const Vector3& operator*(const float factor) const noexcept
	{
		return Vector3{ x * factor, y * factor, z * factor };
	}
	//improvized world to screen (Chat_GPT)
	/*
	bool hunt::w2s(Coords game_pos, Coords& screen_pos) {
		Coords4 transformed{}, projected{};
		int viewport[4] = { 0, 0, 1920, 1080 };

		transformed.x = (float)(game_pos.x * m_renderViewMatrix.m[0][0]) + (float)(game_pos.y * m_renderViewMatrix.m[1][0]) + (float)(game_pos.z * m_renderViewMatrix.m[2][0]) + m_renderViewMatrix.m[3][0];
		transformed.y = (float)(game_pos.x * m_renderViewMatrix.m[0][1]) + (float)(game_pos.y * m_renderViewMatrix.m[1][1]) + (float)(game_pos.z * m_renderViewMatrix.m[2][1]) + m_renderViewMatrix.m[3][1];
		transformed.z = (float)(game_pos.x * m_renderViewMatrix.m[0][2]) + (float)(game_pos.y * m_renderViewMatrix.m[1][2]) + (float)(game_pos.z * m_renderViewMatrix.m[2][2]) + m_renderViewMatrix.m[3][2];
		transformed.w = (float)(game_pos.x * m_renderViewMatrix.m[0][3]) + (float)(game_pos.y * m_renderViewMatrix.m[1][3]) + (float)(game_pos.z * m_renderViewMatrix.m[2][3]) + m_renderViewMatrix.m[3][3];

		bool visible = transformed.z < 0.0f;

		if (!visible) {
			return false;
		}

		projected.x = (float)(transformed.x * m_renderProjectionMatrix.m[0][0]) + (float)(transformed.y * m_renderProjectionMatrix.m[1][0]) + (float)(transformed.z * m_renderProjectionMatrix.m[2][0]) + (float)(transformed.w * m_renderProjectionMatrix.m[3][0]);
		projected.y = (float)(transformed.x * m_renderProjectionMatrix.m[0][1]) + (float)(transformed.y * m_renderProjectionMatrix.m[1][1]) + (float)(transformed.z * m_renderProjectionMatrix.m[2][1]) + (float)(transformed.w * m_renderProjectionMatrix.m[3][1]);
		projected.z = (float)(transformed.x * m_renderProjectionMatrix.m[0][2]) + (float)(transformed.y * m_renderProjectionMatrix.m[1][2]) + (float)(transformed.z * m_renderProjectionMatrix.m[2][2]) + (float)(transformed.w * m_renderProjectionMatrix.m[3][2]);
		projected.w = (float)(transformed.x * m_renderProjectionMatrix.m[0][3]) + (float)(transformed.y * m_renderProjectionMatrix.m[1][3]) + (float)(transformed.z * m_renderProjectionMatrix.m[2][3]) + (float)(transformed.w * m_renderProjectionMatrix.m[3][3]);

		if (projected.z == 0.0f) {
			return false;
		}

		projected.x /= projected.w;
		projected.y /= projected.w;
		projected.z /= projected.w;

		/*visible = visible && (abs(projected.x) <= 1.0f) && (abs(projected.y) <= 1.0f);

		if (!visible) {
			return {};
		}//place a * / right here after uncommenting

		screen_pos.x = viewport[0] + (1.0f + projected.x) * viewport[2] * 0.5f;
		screen_pos.y = viewport[1] + (1.0f - projected.y) * viewport[3] * 0.5f;
		screen_pos.z = projected.z;

		return true; 
	}*/





	float x, y, z;
};