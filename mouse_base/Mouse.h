#ifndef Mouse_h
#define Mouse_h

class Mouse {
public:
  Mouse(void);
  void run_motors(int left, int right);
  void enable_ir(int direction);
  void disable_ir(int direction);
};
#endif
