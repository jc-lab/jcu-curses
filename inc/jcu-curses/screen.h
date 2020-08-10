/**
 * @file	screen.h
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2020/08/10
 * @copyright Copyright (C) 2020 jc-lab.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef JCU_CURSES_SCREEN_H_
#define JCU_CURSES_SCREEN_H_

#include <memory>
#include <atomic>
#include <thread>

#include "window.h"

namespace jcu {
namespace curses {

class CSScreen : public CSWindow {
 private:
  class Intl;
  Intl* intl_;

  int term_fd_;
  FILE* term_fp_;

  SCREEN* scr_;
  std::string term_type_;

  std::atomic_bool running_;
  std::thread read_thread_;

  OnResizedHandler resized_handler_;

  std::atomic_bool dummy_keyinput_;

  void recheckSize();

 public:
  CSScreen(const char *ncurses_device);
  virtual ~CSScreen();

  void destroy() override;

  std::string getTermType() const;

  int resize(int nlines, int ncols, int begin_y, int begin_x) override {
    return 0;
  }

  void setOnResized(const OnResizedHandler& handler);

  virtual void onResized(const NativeWindowSize& new_window_size);

  NativeWindowSize getNativeWindowSize() const;

  void setDummyKeyinput(bool enable);
};

} // namespace curses
} // namespace jcu

#endif //JCU_CURSES_SCREEN_H_
