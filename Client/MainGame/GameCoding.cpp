#include "pch.h"
#include "framework.h"
#include "GameCoding.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;  
HWND g_hWnd;  

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{

    // 1. 윈도우 창 등록
    ::MyRegisterClass(hInstance);

    // 2. 윈도우 생성
    if (!InitInstance (hInstance, nCmdShow))
        return FALSE;
    
    Game game;
    game.Init(g_hWnd);

    // WndProc에 game을 넘겨줘야 하는데 전역으로 만들긴 싫으니까
    SetWindowLongPtr(g_hWnd, GWLP_USERDATA, (LONG_PTR)&game);

    MSG msg = {};
    uint64 prevTick = 0;

    const double TARGET_FPS = 100.0;
    const double FRAME_TIME = 1.0 / TARGET_FPS;

    LARGE_INTEGER frequency;
    LARGE_INTEGER prevTime, currentTime;

    ::QueryPerformanceFrequency(&frequency);
    ::QueryPerformanceCounter(&prevTime);

    // 3. Main loop
    while (msg.message != WM_QUIT)
    {
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        else
        {
            ::QueryPerformanceCounter(&currentTime);
            double elapsedTime = static_cast<double>(currentTime.QuadPart - prevTime.QuadPart) / frequency.QuadPart;

            if (elapsedTime >= FRAME_TIME)
            {
                game.Update();
                game.Render();

                prevTime = currentTime;
            }
        }
    }

    return (int) msg.wParam;
}


//  함수: MyRegisterClass()

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMECODING));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName = L"GameCoding";
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   RECT windowRect = { 0, 0, GWinSizeX, GWinSizeY };
   ::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

   HWND hWnd = CreateWindowW(L"GameCoding", L"Client", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, 
       nullptr, nullptr, hInstance, nullptr);

   g_hWnd = hWnd;

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CHAR:
        // Intentional fall through
    case WM_IME_COMPOSITION: 
    {
        Game* gamePtr = reinterpret_cast<Game*>(GetWindowLongPtr(g_hWnd, GWLP_USERDATA));
        if (gamePtr)
            gamePtr->HandleChatInput(message, wParam, lParam);
        
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_EXIT:
            ::DestroyWindow(hWnd);
            break;
        default:
            return ::DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;

        HDC hdc = ::BeginPaint(hWnd, &ps);

        ::EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        break;
    default:
        return ::DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

