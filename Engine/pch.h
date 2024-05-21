// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


// DirectX 런타임 헤더 파일입니다.
#include <wrl.h>
#include <d3d12.h>
#include "External/Include/d3dx12.h"
#include "External/Include/DirectXTex.h"
#include "External/Include/DirectXTex.inl"
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "comdef.h"
#include "dxgidebug.h"

using Microsoft::WRL::ComPtr;

// DirectX 라이브러리 링크 
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


#if defined(DEBUG) | defined(_DEBUG)
#pragma comment(lib,"External/Lib/DirectXTex_debug.lib")
#else
#pragma comment(lib,"External/Lib/DirectXTex.lib")
#endif

#ifdef max 
#undef max 
#endif

#ifdef min 
#undef min 
#endif


#include "framework.h"
#include "../Engine/System/Exeption.h"

enum class ShaderType {
	VertexShader,
	GeometryShader,
	HullShader,
	DomainShader,
	PixelShader,
	ShaderTypeCount
};

enum class KEY_STATE {
	/// <summary>
	///  nothing state 
	/// </summary>
	NONE,

	/// <summary>
	///  pressed on time 
	/// </summary>
	DOWN,

	/// <summary>
	///  pressing during time 
	/// </summary>
	PRESS,

	/// <summary>
	///  released on time  
	/// </summary>
	RELEASE
};
#endif //PCH_H
