#include "stdafx.h"
#include "Core/Window.h"
#include <Windows.h>

#define IDM_EXIT				105
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_COMMAND:
  {
    int wmId = LOWORD(wParam);
    switch (wmId)
    {
    case IDM_EXIT:
      DestroyWindow(hWnd);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
  }
  break;
  case WM_PAINT:
  {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
  }
  break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

Window::Window() :
  window_handle_(nullptr),
  is_open_(false)
{
}

bool Window::Create()
{
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = 0;
  wcex.hIcon = 0;
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = 0;
  wcex.lpszClassName = (LPCWSTR)"test";
  wcex.hIconSm = NULL;

  RegisterClassExW(&wcex);

  window_handle_ = CreateWindowW((LPCWSTR)"test", (LPCWSTR)"Clouded", WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, 0, 1280, 720, nullptr, nullptr, nullptr, nullptr);

  if (!window_handle_)
  {
    return false;
  }

  ShowWindow((HWND)window_handle_, SW_SHOW);
  UpdateWindow((HWND)window_handle_);
  is_open_ = true;

  return true;
}

void Window::ProcessMessages()
{
  MSG msg{};
  // Handle the windows messages.
  if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  if (msg.message == WM_QUIT)
  {
    is_open_ = false;
  }
}

void * Window::window_handle()
{
  return window_handle_;
}

void Window::Close()
{
  DestroyWindow((HWND)window_handle_);
  is_open_ = false;
}

bool Window::is_open()
{
  return is_open_;
}
