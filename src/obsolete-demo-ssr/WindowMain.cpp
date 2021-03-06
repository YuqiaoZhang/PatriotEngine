/*
 * Copyright (C) YuqiaoZhang(HanetakaYuminaga)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <sdkddkver.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <process.h>

#define PTWIN32
#define PTX86
#include "D:\OSChina\PatriotEngine\Runtime\Public\Math\PTMath.h"

#include <DirectXMath.h>

extern PTVector3F volatile g_EyePosition;
extern PTVector3F volatile g_EyeDirection;
extern PTVector3F volatile g_UpDirection;

int APIENTRY wWinMain(HINSTANCE hInstance,HINSTANCE,LPWSTR lpCmdLine,int nCmdShow)
{
	ATOM hWndCls;
	{
		WNDCLASSEXW Desc = {
			sizeof(WNDCLASSEX),
			CS_OWNDC,
			[](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)->LRESULT
		{
			static float g_SpeedTranslate = 0.5f;

			switch (message)
			{
			case WM_DESTROY:
				::PostQuitMessage(0);
				return 0;
			case WM_KEYDOWN:
			{

				switch (wParam)
				{
				case 'W':
				{
					g_EyePosition.x += g_EyeDirection.x*g_SpeedTranslate;
					g_EyePosition.y += g_EyeDirection.y*g_SpeedTranslate;
					g_EyePosition.z += g_EyeDirection.z*g_SpeedTranslate;
				}
				break;
				case 'S':
				{
					g_EyePosition.x -= g_EyeDirection.x*g_SpeedTranslate;
					g_EyePosition.y -= g_EyeDirection.y*g_SpeedTranslate;
					g_EyePosition.z -= g_EyeDirection.z*g_SpeedTranslate;
				}
				break;
				case 'A':
				{
					PTVector3F EyeDirection = { g_EyeDirection.x,g_EyeDirection.y,g_EyeDirection.z };
					PTVector3F UpDirection = { g_UpDirection.x,g_UpDirection.y,g_UpDirection.z };

					PTVectorSIMD NegEyeDirection = ::PTVectorSIMDNegate(PTVector3FLoad(&EyeDirection));
					PTVectorSIMD AxisForwardDirection = ::PTVector3SIMDNormalize(NegEyeDirection);
					PTVectorSIMD AxisRightDirection = ::PTVector3SIMDNormalize(::PTVector3SIMDCross(PTVector3FLoad(&UpDirection), AxisForwardDirection));
					//PTVectorSIMD AxisViewUpDirection = ::PTVector3SIMDCross(AxisForwardDirection, AxisRightDirection);

					PTVector3F AxisRightDirectionF;
					PTVector3FStore(&AxisRightDirectionF, AxisRightDirection);


					g_EyePosition.x -= AxisRightDirectionF.x*g_SpeedTranslate;
					g_EyePosition.y -= AxisRightDirectionF.y*g_SpeedTranslate;
					g_EyePosition.z -= AxisRightDirectionF.z*g_SpeedTranslate;
				}
				break;
				case 'D':
				{
					PTVector3F EyeDirection = { g_EyeDirection.x,g_EyeDirection.y,g_EyeDirection.z };
					PTVector3F UpDirection = { g_UpDirection.x,g_UpDirection.y,g_UpDirection.z };

					PTVectorSIMD NegEyeDirection = ::PTVectorSIMDNegate(PTVector3FLoad(&EyeDirection));
					PTVectorSIMD AxisForwardDirection = ::PTVector3SIMDNormalize(NegEyeDirection);
					PTVectorSIMD AxisRightDirection = ::PTVector3SIMDNormalize(::PTVector3SIMDCross(PTVector3FLoad(&UpDirection), AxisForwardDirection));
					//PTVectorSIMD AxisViewUpDirection = ::PTVector3SIMDCross(AxisForwardDirection, AxisRightDirection);

					PTVector3F AxisRightDirectionF;
					PTVector3FStore(&AxisRightDirectionF, AxisRightDirection);


					g_EyePosition.x += AxisRightDirectionF.x*g_SpeedTranslate;
					g_EyePosition.y += AxisRightDirectionF.y*g_SpeedTranslate;
					g_EyePosition.z += AxisRightDirectionF.z*g_SpeedTranslate;
				}
				break;
				case 'Q':
				{
					PTVector3F EyeDirection = { g_EyeDirection.x,g_EyeDirection.y,g_EyeDirection.z };
					PTVector3F UpDirection = { g_UpDirection.x,g_UpDirection.y,g_UpDirection.z };

					PTVectorSIMD NegEyeDirection = ::PTVectorSIMDNegate(PTVector3FLoad(&EyeDirection));
					PTVectorSIMD AxisForwardDirection = ::PTVector3SIMDNormalize(NegEyeDirection);
					PTVectorSIMD AxisRightDirection = ::PTVector3SIMDNormalize(::PTVector3SIMDCross(PTVector3FLoad(&UpDirection), AxisForwardDirection));
					PTVectorSIMD AxisViewUpDirection = ::PTVector3SIMDCross(AxisForwardDirection, AxisRightDirection);

					PTVector3F AxisViewUpDirectionF;
					PTVector3FStore(&AxisViewUpDirectionF, AxisViewUpDirection);

					g_EyePosition.x -= AxisViewUpDirectionF.x*g_SpeedTranslate;
					g_EyePosition.y -= AxisViewUpDirectionF.y*g_SpeedTranslate;
					g_EyePosition.z -= AxisViewUpDirectionF.z*g_SpeedTranslate;
				}
				break;
				case 'E':
				{	
					PTVector3F EyeDirection = { g_EyeDirection.x,g_EyeDirection.y,g_EyeDirection.z };
					PTVector3F UpDirection = { g_UpDirection.x,g_UpDirection.y,g_UpDirection.z };

					PTVectorSIMD NegEyeDirection = ::PTVectorSIMDNegate(PTVector3FLoad(&EyeDirection));
					PTVectorSIMD AxisForwardDirection = ::PTVector3SIMDNormalize(NegEyeDirection);
					PTVectorSIMD AxisRightDirection = ::PTVector3SIMDNormalize(::PTVector3SIMDCross(PTVector3FLoad(&UpDirection), AxisForwardDirection));
					PTVectorSIMD AxisViewUpDirection = ::PTVector3SIMDCross(AxisForwardDirection, AxisRightDirection);

					PTVector3F AxisViewUpDirectionF;
					PTVector3FStore(&AxisViewUpDirectionF, AxisViewUpDirection);


					g_EyePosition.x += AxisViewUpDirectionF.x*g_SpeedTranslate;
					g_EyePosition.y += AxisViewUpDirectionF.y*g_SpeedTranslate;
					g_EyePosition.z += AxisViewUpDirectionF.z*g_SpeedTranslate;
				}
				break;
				}
			}
			return 0;
			case WM_MOUSEMOVE:
			{
				static int Previous_X;
				static int Previous_Y;

				int Current_X = GET_X_LPARAM(lParam);
				int Current_Y = GET_Y_LPARAM(lParam);

				if (wParam&MK_RBUTTON)
				{
					int Offset_X = Current_X - Previous_X;
					int Offset_Y = Current_Y - Previous_Y;

					float OffsetNormalized_X = static_cast<float>(Offset_X) / 800.0f;
					float OffsetNormalized_Y = static_cast<float>(Offset_Y) / 600.0f;

					float LengthNormalized = ::sqrtf(OffsetNormalized_X*OffsetNormalized_X + OffsetNormalized_Y*OffsetNormalized_Y);

					if ((Offset_Y < Offset_X) && (Offset_Y > -Offset_X))
					{
						//右
						PTVector3F EyeDirection = { g_EyeDirection.x,g_EyeDirection.y,g_EyeDirection.z };
						PTVector3F UpDirection = { g_UpDirection.x,g_UpDirection.y,g_UpDirection.z };

						PTVectorSIMD NegEyeDirection = ::PTVectorSIMDNegate(PTVector3FLoad(&EyeDirection));
						PTVectorSIMD AxisForwardDirection = ::PTVector3SIMDNormalize(NegEyeDirection);
						PTVectorSIMD AxisRightDirection = ::PTVector3SIMDNormalize(::PTVector3SIMDCross(PTVector3FLoad(&UpDirection), AxisForwardDirection));
						PTVectorSIMD AxisViewUpDirection = ::PTVector3SIMDCross(AxisForwardDirection, AxisRightDirection);

						PTVector3F EyeDirectionNew;
						::PTVector3FStore(&EyeDirectionNew,
							::PTVectorSIMDNegate(
								::PTVector3SIMDTransform(
									::PTMatrix4x4SIMDRotationQuaternion(::PTQuaternionSIMDRotationNormalAxis(AxisViewUpDirection, -2.0f*LengthNormalized)),
									AxisForwardDirection
								)
							)
						);
						g_EyeDirection.x = EyeDirectionNew.x;
						g_EyeDirection.y = EyeDirectionNew.y;
						g_EyeDirection.z = EyeDirectionNew.z;
					}
					else if ((Offset_Y < -Offset_X) && (Offset_Y > Offset_X))
					{
						//左
						PTVector3F EyeDirection = { g_EyeDirection.x,g_EyeDirection.y,g_EyeDirection.z };
						PTVector3F UpDirection = { g_UpDirection.x,g_UpDirection.y,g_UpDirection.z };

						PTVectorSIMD NegEyeDirection = ::PTVectorSIMDNegate(PTVector3FLoad(&EyeDirection));
						PTVectorSIMD AxisForwardDirection = ::PTVector3SIMDNormalize(NegEyeDirection);
						PTVectorSIMD AxisRightDirection = ::PTVector3SIMDNormalize(::PTVector3SIMDCross(PTVector3FLoad(&UpDirection), AxisForwardDirection));
						PTVectorSIMD AxisViewUpDirection = ::PTVector3SIMDCross(AxisForwardDirection, AxisRightDirection);

						PTVector3F EyeDirectionNew;
						::PTVector3FStore(&EyeDirectionNew,
							::PTVectorSIMDNegate(
								::PTVector3SIMDTransform(
									::PTMatrix4x4SIMDRotationQuaternion(::PTQuaternionSIMDRotationNormalAxis(AxisViewUpDirection, 2.0f*LengthNormalized)),
									AxisForwardDirection
								)
							)
						);
						g_EyeDirection.x = EyeDirectionNew.x;
						g_EyeDirection.y = EyeDirectionNew.y;
						g_EyeDirection.z = EyeDirectionNew.z;
					}
					else if ((Offset_Y < Offset_X) && (Offset_Y < -Offset_X))
					{
						//上
						PTVector3F EyeDirection = { g_EyeDirection.x,g_EyeDirection.y,g_EyeDirection.z };
						PTVector3F UpDirection = { g_UpDirection.x,g_UpDirection.y,g_UpDirection.z };

						PTVectorSIMD NegEyeDirection = ::PTVectorSIMDNegate(PTVector3FLoad(&EyeDirection));
						PTVectorSIMD AxisForwardDirection = ::PTVector3SIMDNormalize(NegEyeDirection);
						PTVectorSIMD AxisRightDirection = ::PTVector3SIMDNormalize(::PTVector3SIMDCross(PTVector3FLoad(&UpDirection), AxisForwardDirection));
						PTVectorSIMD AxisViewUpDirection = ::PTVector3SIMDCross(AxisForwardDirection, AxisRightDirection);

						PTVector3F EyeDirectionNew;
						::PTVector3FStore(&EyeDirectionNew,
							::PTVectorSIMDNegate(
								::PTVector3SIMDTransform(
									::PTMatrix4x4SIMDRotationQuaternion(::PTQuaternionSIMDRotationNormalAxis(AxisRightDirection, 2.0f*LengthNormalized)),
									AxisForwardDirection
								)
							)
						);
						g_EyeDirection.x = EyeDirectionNew.x;
						g_EyeDirection.y = EyeDirectionNew.y;
						g_EyeDirection.z = EyeDirectionNew.z;
					}
					else if ((Offset_Y > Offset_X) && (Offset_Y > -Offset_X))
					{
						//下
						PTVector3F EyeDirection = { g_EyeDirection.x,g_EyeDirection.y,g_EyeDirection.z };
						PTVector3F UpDirection = { g_UpDirection.x,g_UpDirection.y,g_UpDirection.z };

						PTVectorSIMD NegEyeDirection = ::PTVectorSIMDNegate(PTVector3FLoad(&EyeDirection));
						PTVectorSIMD AxisForwardDirection = ::PTVector3SIMDNormalize(NegEyeDirection);
						PTVectorSIMD AxisRightDirection = ::PTVector3SIMDNormalize(::PTVector3SIMDCross(PTVector3FLoad(&UpDirection), AxisForwardDirection));
						PTVectorSIMD AxisViewUpDirection = ::PTVector3SIMDCross(AxisForwardDirection, AxisRightDirection);

						PTVector3F EyeDirectionNew;
						::PTVector3FStore(&EyeDirectionNew,
							::PTVectorSIMDNegate(
								::PTVector3SIMDTransform(
									::PTMatrix4x4SIMDRotationQuaternion(::PTQuaternionSIMDRotationNormalAxis(AxisRightDirection, -2.0f*LengthNormalized)),
									AxisForwardDirection
								)
							)
						);
						g_EyeDirection.x = EyeDirectionNew.x;
						g_EyeDirection.y = EyeDirectionNew.y;
						g_EyeDirection.z = EyeDirectionNew.z;
					}

				}

				Previous_X = Current_X;
				Previous_Y = Current_Y;
			}
			return 0;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		},
			0,
			0,
			hInstance,
			LoadIconW(NULL, IDI_APPLICATION),
			LoadCursorW(NULL, IDC_ARROW),
			(HBRUSH)(COLOR_WINDOW + 1),
			NULL,
			L"LeranOpenGL:0XFFFFFFFF",
			LoadIconW(NULL, IDI_APPLICATION),
		};
		hWndCls = ::RegisterClassExW(&Desc);
	}
	HWND hWnd;
	{
		HWND hDesktop = ::GetDesktopWindow();
		HMONITOR hMonitor = ::MonitorFromWindow(hDesktop, MONITOR_DEFAULTTONEAREST);
		MONITORINFOEXW MonitorInfo;
		MonitorInfo.cbSize = sizeof(MONITORINFOEXW);
		::GetMonitorInfoW(hMonitor, &MonitorInfo);

		hWnd = ::CreateWindowExW(WS_EX_APPWINDOW,
			MAKEINTATOM(hWndCls),
			PROJECT_NAME,
			WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
			(MonitorInfo.rcWork.left + MonitorInfo.rcWork.right) / 2 - 400,
			(MonitorInfo.rcWork.bottom + MonitorInfo.rcWork.top) / 2 - 300,
			800,
			600,
			hDesktop,
			NULL,
			hInstance,
			NULL);
	}
	
	unsigned __stdcall RenderMain(void *);
	::_beginthreadex(NULL, 0U, RenderMain, hWnd, 0U, NULL);
	
	// 主消息循环: 
	MSG msg;
	while (::GetMessageW(&msg, NULL, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessageW(&msg);
	}

	return (int)msg.wParam;
}