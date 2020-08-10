/**
 * @file	window.h
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2020/08/10
 * @copyright Copyright (C) 2020 jc-lab.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef JCU_CURSES_WINDOW_H_
#define JCU_CURSES_WINDOW_H_

#include "types.h"

#include <mutex>

#include <curses.h>

namespace jcu {
namespace curses {

class CSWindow {
 protected:
  std::recursive_mutex winmtx_;
  CSWindow *parent_;
  WINDOW *win_;
  int begin_x_;
  int begin_y_;
  int width_;
  int height_;

 public:
  CSWindow();
  CSWindow(WINDOW *win);

  CSWindow(CSWindow& parent, int nlines, int ncols, int begin_y, int begin_x);

  CSWindow(CSWindow& parent, const CSWindowNoInitialize& no_initialize);

  virtual int resize(int nlines, int ncols, int begin_y, int begin_x);

  virtual ~CSWindow();

  virtual void destroy();

  virtual WINDOW* getSelfWindow();
  virtual WINDOW* get() {
    return getSelfWindow();
  }
  std::recursive_mutex& winmutex();

  int getWidth() const;
  int getHeight() const;
  int getBeginX() const;
  int getBeginY() const;
  int getEndX() const;
  int getEndY() const;

  virtual int clear();
  virtual int clearAll();
  virtual int refresh();
};

} // namespace curses
} // namespace jcu

#endif //JCU_CURSES_WINDOW_H_
