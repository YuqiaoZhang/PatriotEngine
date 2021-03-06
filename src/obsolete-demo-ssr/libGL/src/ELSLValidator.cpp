#include <sdkddkver.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "../include/GLES3/gl31.h"
#include "../include/GLES3/gl2ext.h"
#include "../include/wglext.h"

int wmain(int argc,wchar_t *argv[])
{
	wchar_t *FileName_Vertex = NULL;
	wchar_t *FileName_TessellationControl = NULL;
	wchar_t *FileName_TessellationEvaluation = NULL;
	wchar_t *FileName_Geometry = NULL;
	wchar_t *FileName_Fragment = NULL;
	wchar_t *FileName_Compute = NULL;

	for (int i = 1; i < argc; ++i)
	{
		wchar_t *pSuffix = ::wcsrchr(argv[i], L'.') + 1;
		if (!::_wcsicmp(pSuffix, L"vert"))
		{
			assert(FileName_Vertex == NULL);
			FileName_Vertex = argv[i];
		}
		else if (!::_wcsicmp(pSuffix, L"tesc"))
		{
			assert(FileName_TessellationControl == NULL);
			FileName_TessellationControl = argv[i];
		}
		else if (!::_wcsicmp(pSuffix, L"tese"))
		{
			assert(FileName_TessellationEvaluation == NULL);
			FileName_TessellationEvaluation = argv[i];
		}
		else if (!::_wcsicmp(pSuffix, L"geom"))
		{
			assert(FileName_Geometry == NULL);
			FileName_Geometry = argv[i];
		}
		else if (!::_wcsicmp(pSuffix, L"frag"))
		{
			assert(FileName_Fragment == NULL);
			FileName_Fragment = argv[i];
		}
		else if (!::_wcsicmp(pSuffix, L"comp"))
		{
			assert(FileName_Compute == NULL);
			FileName_Compute = argv[i];
		}
	}

	HWND hWndParent = ::GetDesktopWindow();
	assert(hWndParent != NULL);

	HMONITOR hMonitor = ::MonitorFromWindow(hWndParent, MONITOR_DEFAULTTONEAREST);
	assert(hMonitor != NULL);

	MONITORINFOEXW MonitorInfo;
	MonitorInfo.cbSize = sizeof(MONITORINFOEXW);
	BOOL wbResult = ::GetMonitorInfoW(hMonitor, &MonitorInfo);
	assert(wbResult != FALSE);

	HMODULE hInstance = ::GetModuleHandleW(NULL);

	WNDCLASSEXW Desc = {
		sizeof(WNDCLASSEX),
		CS_OWNDC,
		[](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)->LRESULT
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	},
		0,
		0,
		hInstance,
		LoadIconW(NULL, IDI_APPLICATION),
		LoadCursorW(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		L"eglInitialize:0XFFFFFFFF",
		LoadIconW(NULL, IDI_APPLICATION),
	};
	ATOM hWndTempClass = ::RegisterClassExW(&Desc);
	assert(hWndTempClass != 0U);

	HWND hWndTemp = ::CreateWindowExW(WS_EX_APPWINDOW,
		MAKEINTATOM(hWndTempClass),
		L"eglInitialize:0XFFFFFFFF",
		WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		MonitorInfo.rcWork.left,
		MonitorInfo.rcWork.top,
		MonitorInfo.rcWork.right - MonitorInfo.rcWork.left,
		MonitorInfo.rcWork.bottom - MonitorInfo.rcWork.top,
		hWndParent,
		NULL,
		hInstance,
		NULL);
	assert(hWndTemp != NULL);

	HDC hDCTemp = ::GetDC(hWndTemp);
	assert(hDCTemp != NULL);

	PIXELFORMATDESCRIPTOR FormatDescriptor;
	int PixelFormatCount = ::DescribePixelFormat(hDCTemp, 1, sizeof(FormatDescriptor), &FormatDescriptor);//Load OpenGL ICD

	int PixelFormatIndex = 1;
	while (PixelFormatIndex <= PixelFormatCount)//从1开始
	{
		int iResult = ::DescribePixelFormat(hDCTemp, PixelFormatIndex, sizeof(FormatDescriptor), &FormatDescriptor);//Load OpenGL ICD
		assert(iResult != 0);

		if (((FormatDescriptor.dwFlags&(PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL)) == (PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL))
			&& (FormatDescriptor.iPixelType == PFD_TYPE_RGBA))
		{
			break;
		}

		++PixelFormatIndex;
	}

	wbResult = ::SetPixelFormat(hDCTemp, PixelFormatIndex, &FormatDescriptor);
	assert(wbResult != FALSE);

	HGLRC hWGLContext = ::wglCreateContext(hDCTemp);
	assert(hWGLContext != NULL);

	wbResult = ::wglMakeCurrent(hDCTemp, hWGLContext);
	assert(wbResult != FALSE);

	PFNWGLCREATECONTEXTATTRIBSARBPROC pFn_wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(::wglGetProcAddress("wglCreateContextAttribsARB"));
	assert(pFn_wglCreateContextAttribsARB != NULL);

	wbResult = ::wglMakeCurrent(NULL, NULL);
	assert(wbResult != FALSE);

	wbResult = ::wglDeleteContext(hWGLContext);
	assert(wbResult != FALSE);

	HGLRC hWGLContextARB;
	{
		int attribList[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB,3,
			WGL_CONTEXT_MINOR_VERSION_ARB,1,
			WGL_CONTEXT_PROFILE_MASK_ARB,WGL_CONTEXT_ES_PROFILE_BIT_EXT,
			WGL_CONTEXT_FLAGS_ARB ,WGL_CONTEXT_DEBUG_BIT_ARB,
			0 };
		hWGLContextARB = pFn_wglCreateContextAttribsARB(hDCTemp, NULL, attribList);
	}

	wbResult = ::wglMakeCurrent(hDCTemp, hWGLContextARB);
	assert(wbResult != FALSE);

	PFNGLCREATESHADERPROC pFn_glCreateShader = reinterpret_cast<PFNGLCREATESHADERPROC>(::wglGetProcAddress("glCreateShader"));
	assert(pFn_glCreateShader != NULL);
	PFNGLSHADERSOURCEPROC pFn_glShaderSource = reinterpret_cast<PFNGLSHADERSOURCEPROC>(::wglGetProcAddress("glShaderSource"));
	assert(pFn_glShaderSource != NULL);
	PFNGLCOMPILESHADERPROC pFn_glCompileShader = reinterpret_cast<PFNGLCOMPILESHADERPROC>(::wglGetProcAddress("glCompileShader"));
	assert(pFn_glCompileShader != NULL);
	PFNGLGETSHADERINFOLOGPROC pFn_glGetShaderInfoLog = reinterpret_cast<PFNGLGETSHADERINFOLOGPROC>(::wglGetProcAddress("glGetShaderInfoLog"));
	assert(pFn_glGetShaderInfoLog != NULL);

	PFNGLCREATEPROGRAMPROC pFn_glCreateProgram = reinterpret_cast<PFNGLCREATEPROGRAMPROC>(::wglGetProcAddress("glCreateProgram"));
	assert(pFn_glCreateProgram != NULL);
	PFNGLATTACHSHADERPROC pFn_glAttachShader = reinterpret_cast<PFNGLATTACHSHADERPROC>(::wglGetProcAddress("glAttachShader"));
	assert(pFn_glAttachShader != NULL);
	PFNGLLINKPROGRAMPROC pFn_glLinkProgram = reinterpret_cast<PFNGLLINKPROGRAMPROC>(::wglGetProcAddress("glLinkProgram"));
	assert(pFn_glLinkProgram != NULL);
	PFNGLGETPROGRAMINFOLOGPROC pFn_glGetProgramInfoLog = reinterpret_cast<PFNGLGETPROGRAMINFOLOGPROC>(::wglGetProcAddress("glGetProgramInfoLog"));
	assert(pFn_glGetProgramInfoLog != NULL);

	GLuint hShader_Vertex = 0;
	if (FileName_Vertex)
	{

		HANDLE hFile = ::CreateFileW(FileName_Vertex, FILE_READ_DATA, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		assert(hFile != INVALID_HANDLE_VALUE);
		LARGE_INTEGER FileSize;
		wbResult = ::GetFileSizeEx(hFile, &FileSize);
		assert(wbResult != FALSE);

		assert(FileSize.HighPart == 0U);
		if (FileSize.LowPart != 0U)
		{
			HANDLE hSection = ::CreateFileMappingW(hFile, NULL, PAGE_READONLY, FileSize.HighPart, FileSize.LowPart, NULL);
			assert(hSection != NULL);
			void *pFileData = ::MapViewOfFileEx(hSection, FILE_MAP_READ, 0U, 0U, FileSize.LowPart, NULL);

			hShader_Vertex = pFn_glCreateShader(GL_VERTEX_SHADER);

			char const *String[1] = { static_cast<char *>(pFileData) };
			pFn_glShaderSource(hShader_Vertex, 1U, String, NULL);

			pFn_glCompileShader(hShader_Vertex);

			char ShadeInfoLog[4096];
			pFn_glGetShaderInfoLog(hShader_Vertex, 4096, NULL, ShadeInfoLog);
			::printf(ShadeInfoLog);

			wbResult = ::UnmapViewOfFile(pFileData);
			assert(wbResult != FALSE);

			wbResult = ::CloseHandle(hSection);
			assert(wbResult != FALSE);
		}

		wbResult = ::CloseHandle(hFile);
		assert(wbResult != FALSE);
	}

	GLuint hShader_TessellationControl = 0;
	if (FileName_TessellationControl)
	{
		HANDLE hFile = ::CreateFileW(FileName_TessellationControl, FILE_READ_DATA, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		assert(hFile != INVALID_HANDLE_VALUE);
		LARGE_INTEGER FileSize;
		wbResult = ::GetFileSizeEx(hFile, &FileSize);
		assert(wbResult != FALSE);

		assert(FileSize.HighPart == 0U);
		if (FileSize.LowPart != 0U)
		{
			HANDLE hSection = ::CreateFileMappingW(hFile, NULL, PAGE_READONLY, FileSize.HighPart, FileSize.LowPart, NULL);
			assert(hSection != NULL);
			void *pFileData = ::MapViewOfFileEx(hSection, FILE_MAP_READ, 0U, 0U, FileSize.LowPart, NULL);

			hShader_TessellationControl = pFn_glCreateShader(GL_TESS_CONTROL_SHADER_OES);

			char const *String[1] = { static_cast<char *>(pFileData) };
			pFn_glShaderSource(hShader_TessellationControl, 1U, String, NULL);

			pFn_glCompileShader(hShader_TessellationControl);

			char ShadeInfoLog[4096];
			pFn_glGetShaderInfoLog(hShader_TessellationControl, 4096, NULL, ShadeInfoLog);
			::printf(ShadeInfoLog);

			wbResult = ::UnmapViewOfFile(pFileData);
			assert(wbResult != FALSE);

			wbResult = ::CloseHandle(hSection);
			assert(wbResult != FALSE);
		}

		wbResult = ::CloseHandle(hFile);
		assert(wbResult != FALSE);
	}

	GLuint hShader_TessellationEvaluation = 0;
	if (FileName_TessellationEvaluation)
	{
		HANDLE hFile = ::CreateFileW(FileName_TessellationEvaluation, FILE_READ_DATA, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		assert(hFile != INVALID_HANDLE_VALUE);
		LARGE_INTEGER FileSize;
		wbResult = ::GetFileSizeEx(hFile, &FileSize);
		assert(wbResult != FALSE);

		assert(FileSize.HighPart == 0U);
		if (FileSize.LowPart != 0U)
		{
			HANDLE hSection = ::CreateFileMappingW(hFile, NULL, PAGE_READONLY, FileSize.HighPart, FileSize.LowPart, NULL);
			assert(hSection != NULL);
			void *pFileData = ::MapViewOfFileEx(hSection, FILE_MAP_READ, 0U, 0U, FileSize.LowPart, NULL);

			hShader_TessellationEvaluation = pFn_glCreateShader(GL_TESS_EVALUATION_SHADER_OES);

			char const *String[1] = { static_cast<char *>(pFileData) };
			pFn_glShaderSource(hShader_TessellationEvaluation, 1U, String, NULL);

			pFn_glCompileShader(hShader_TessellationEvaluation);

			char ShadeInfoLog[4096];
			pFn_glGetShaderInfoLog(hShader_TessellationEvaluation, 4096, NULL, ShadeInfoLog);
			::printf(ShadeInfoLog);

			wbResult = ::UnmapViewOfFile(pFileData);
			assert(wbResult != FALSE);

			wbResult = ::CloseHandle(hSection);
			assert(wbResult != FALSE);
		}

		wbResult = ::CloseHandle(hFile);
		assert(wbResult != FALSE);
	}

	GLuint hShader_Geometry = 0;
	if(FileName_Geometry)
	{
		HANDLE hFile = ::CreateFileW(FileName_Geometry, FILE_READ_DATA, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		assert(hFile != INVALID_HANDLE_VALUE);
		LARGE_INTEGER FileSize;
		wbResult = ::GetFileSizeEx(hFile, &FileSize);
		assert(wbResult != FALSE);

		assert(FileSize.HighPart == 0U);
		if (FileSize.LowPart != 0U)
		{
			HANDLE hSection = ::CreateFileMappingW(hFile, NULL, PAGE_READONLY, FileSize.HighPart, FileSize.LowPart, NULL);
			assert(hSection != NULL);
			void *pFileData = ::MapViewOfFileEx(hSection, FILE_MAP_READ, 0U, 0U, FileSize.LowPart, NULL);

			hShader_Geometry = pFn_glCreateShader(GL_GEOMETRY_SHADER_OES);

			char const *String[1] = { static_cast<char *>(pFileData) };
			pFn_glShaderSource(hShader_Geometry, 1U, String, NULL);

			pFn_glCompileShader(hShader_Geometry);

			char ShadeInfoLog[4096];
			pFn_glGetShaderInfoLog(hShader_Geometry, 4096, NULL, ShadeInfoLog);
			::printf(ShadeInfoLog);

			wbResult = ::UnmapViewOfFile(pFileData);
			assert(wbResult != FALSE);

			wbResult = ::CloseHandle(hSection);
			assert(wbResult != FALSE);
		}

		wbResult = ::CloseHandle(hFile);
		assert(wbResult != FALSE);
	}

	GLuint hShader_Fragment = 0;
	if (FileName_Fragment)
	{

		HANDLE hFile = ::CreateFileW(FileName_Fragment, FILE_READ_DATA, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		assert(hFile != INVALID_HANDLE_VALUE);
		LARGE_INTEGER FileSize;
		wbResult = ::GetFileSizeEx(hFile, &FileSize);
		assert(wbResult != FALSE);

		assert(FileSize.HighPart == 0U);
		if (FileSize.LowPart != 0U)
		{
			HANDLE hSection = ::CreateFileMappingW(hFile, NULL, PAGE_READONLY, FileSize.HighPart, FileSize.LowPart, NULL);
			assert(hSection != NULL);
			void *pFileData = ::MapViewOfFileEx(hSection, FILE_MAP_READ, 0U, 0U, FileSize.LowPart, NULL);

			hShader_Fragment = pFn_glCreateShader(GL_FRAGMENT_SHADER);

			char const *String[1] = { static_cast<char *>(pFileData) };
			pFn_glShaderSource(hShader_Fragment, 1U, String, NULL);

			pFn_glCompileShader(hShader_Fragment);

			char ShadeInfoLog[4096];
			pFn_glGetShaderInfoLog(hShader_Fragment, 4096, NULL, ShadeInfoLog);
			::printf(ShadeInfoLog);


			wbResult = ::UnmapViewOfFile(pFileData);
			assert(wbResult != FALSE);

			wbResult = ::CloseHandle(hSection);
			assert(wbResult != FALSE);
		}

		wbResult = ::CloseHandle(hFile);
		assert(wbResult != FALSE);
	}

	GLuint hProgramGraphics = 0;
	{
		hProgramGraphics = pFn_glCreateProgram();

		pFn_glAttachShader(hProgramGraphics, hShader_Vertex);
		pFn_glAttachShader(hProgramGraphics, hShader_TessellationControl);
		pFn_glAttachShader(hProgramGraphics, hShader_TessellationEvaluation);
		pFn_glAttachShader(hProgramGraphics, hShader_Geometry);
		pFn_glAttachShader(hProgramGraphics, hShader_Fragment);

		pFn_glLinkProgram(hProgramGraphics);

		char ProgramInfoLog[4096];
		pFn_glGetProgramInfoLog(hProgramGraphics, 4096, NULL, ProgramInfoLog);
		::printf(ProgramInfoLog);
	}

	GLuint hShader_Compute = 0;
	if (FileName_Compute)
	{

		HANDLE hFile = ::CreateFileW(FileName_Compute, FILE_READ_DATA, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		assert(hFile != INVALID_HANDLE_VALUE);
		LARGE_INTEGER FileSize;
		wbResult = ::GetFileSizeEx(hFile, &FileSize);
		assert(wbResult != FALSE);

		assert(FileSize.HighPart == 0U);
		if (FileSize.LowPart != 0U)
		{
			HANDLE hSection = ::CreateFileMappingW(hFile, NULL, PAGE_READONLY, FileSize.HighPart, FileSize.LowPart, NULL);
			assert(hSection != NULL);
			void *pFileData = ::MapViewOfFileEx(hSection, FILE_MAP_READ, 0U, 0U, FileSize.LowPart, NULL);

			hShader_Compute = pFn_glCreateShader(GL_COMPUTE_SHADER);

			char const *String[1] = { static_cast<char *>(pFileData) };
			pFn_glShaderSource(hShader_Compute, 1U, String, NULL);

			pFn_glCompileShader(hShader_Compute);

			char ShadeInfoLog[4096];
			pFn_glGetShaderInfoLog(hShader_Compute, 4096, NULL, ShadeInfoLog);
			::printf(ShadeInfoLog);

			wbResult = ::UnmapViewOfFile(pFileData);
			assert(wbResult != FALSE);

			wbResult = ::CloseHandle(hSection);
			assert(wbResult != FALSE);
		}

		wbResult = ::CloseHandle(hFile);
		assert(wbResult != FALSE);
	}

	GLuint hProgramCompute = 0;
	{
		hProgramCompute = pFn_glCreateProgram();

		pFn_glAttachShader(hProgramCompute, hShader_Compute);

		pFn_glLinkProgram(hProgramCompute);

		char ProgramInfoLog[4096];
		pFn_glGetProgramInfoLog(hProgramCompute, 4096, NULL, ProgramInfoLog);
		::printf(ProgramInfoLog);
	}

	wbResult = ::wglMakeCurrent(NULL, NULL);
	assert(wbResult != FALSE);

	wbResult = ::wglDeleteContext(hWGLContextARB);
	assert(wbResult != FALSE);

	wbResult = ::DestroyWindow(hWndTemp);
	assert(wbResult != FALSE);

    return 0;
}

