#pragma once
class Input;
class Window
{
public:
  Window();

  bool Create();
  void ProcessMessages(Input& input);
  void* window_handle();
  void Close();
  bool is_open();
private:
  void* window_handle_;
  bool is_open_;
};
