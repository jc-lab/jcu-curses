/**
 * @file	box_window.h
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2020/08/10
 * @copyright Copyright (C) 2020 jc-lab.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef JCU_CURSES_BOX_WINDOW_H_
#define JCU_CURSES_BOX_WINDOW_H_

#include "window.h"

namespace jcu {
namespace curses {

class CSBoxWindow : public CSWindow {
 protected:
  chtype ls_;
  chtype rs_;
  chtype ts_;
  chtype bs_;
  chtype tl_;
  chtype tr_;
  chtype bl_;
  chtype br_;

  CSWindow subwin_;

 public:
  CSBoxWindow(CSWindow &parent, int nlines, int ncols, int begin_y, int begin_x);
  CSBoxWindow(CSWindow &parent, const CSWindowNoInitialize& no_initialize);

  void initBox(chtype verch, chtype horch);
  void initBorder(chtype ls, chtype rs, chtype ts, chtype bs, chtype tl, chtype tr, chtype bl, chtype br);

  int resize(int nlines, int ncols, int begin_y, int begin_x) override;

  WINDOW *get() override;

  void destroy() override;
  int clear() override;
  int refresh() override;
};

} // namespace curses
} // namespace jcu

#endif //JCU_CURSES_BOX_WINDOW_H_
