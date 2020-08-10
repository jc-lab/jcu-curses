/**
 * @file	box_window.cc
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2020/08/10
 * @copyright Copyright (C) 2020 jc-lab.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#include "jcu-curses/box_window.h"

namespace jcu {
namespace curses {

CSBoxWindow::CSBoxWindow(CSWindow &parent, int nlines, int ncols, int begin_y, int begin_x)
    : CSWindow(parent, nlines, ncols, begin_y, begin_x),
      subwin_(*this, CSWindowNoInitialize {}) {
  ls_ = ACS_VLINE;
  rs_ = ACS_VLINE;
  ts_ = ACS_HLINE;
  bs_ = ACS_HLINE;
  tl_ = ACS_ULCORNER;
  tr_ = ACS_URCORNER;
  bl_ = ACS_LLCORNER;
  br_ = ACS_LRCORNER;
}

CSBoxWindow::CSBoxWindow(CSWindow &parent, const CSWindowNoInitialize &no_initialize)
    : CSWindow(parent, no_initialize),
      subwin_(*this,  CSWindowNoInitialize {}) {
  ls_ = ACS_VLINE;
  rs_ = ACS_VLINE;
  ts_ = ACS_HLINE;
  bs_ = ACS_HLINE;
  tl_ = ACS_ULCORNER;
  tr_ = ACS_URCORNER;
  bl_ = ACS_LLCORNER;
  br_ = ACS_LRCORNER;
}

void CSBoxWindow::initBox(
    chtype verch,
    chtype horch
) {
  ls_ = verch;
  rs_ = verch;
  ts_ = horch;
  bs_ = horch;
  tl_ = ACS_ULCORNER;
  tr_ = ACS_URCORNER;
  bl_ = ACS_LLCORNER;
  br_ = ACS_LRCORNER;
}
void CSBoxWindow::initBorder(
    chtype ls,
    chtype rs,
    chtype ts,
    chtype bs,
    chtype tl,
    chtype tr,
    chtype bl,
    chtype br) {
  ls_ = ls;
  rs_ = rs;
  ts_ = ts;
  bs_ = bs;
  tl_ = tl;
  tr_ = tr;
  bl_ = bl;
  br_ = br;
}

int CSBoxWindow::resize(int nlines, int ncols, int begin_y, int begin_x) {
  int rc;
  rc = CSWindow::resize(nlines, ncols, begin_y, begin_x);
  subwin_.resize(nlines - 2, ncols - 4,  begin_y + 1, begin_x + 2);
  return rc;
}

WINDOW *CSBoxWindow::get() {
  return subwin_.get();
}

void CSBoxWindow::destroy() {
  subwin_.destroy();
  CSWindow::destroy();
}

int CSBoxWindow::clear() {
  subwin_.clear();
  return CSWindow::clear();
}

int CSBoxWindow::refresh() {
  ::wborder(getSelfWindow(), ls_, rs_, ts_, bs_, tl_, tr_, bl_, br_);
  subwin_.refresh();
  return CSWindow::refresh();
}

} // namespace curses
} // namespace jcu

