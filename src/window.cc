/**
 * @file	window.cc
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2020/08/10
 * @copyright Copyright (C) 2020 jc-lab.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#include "jcu-curses/window.h"

namespace jcu {
namespace curses {

CSWindow::CSWindow()
    : winmtx_(), parent_(nullptr), win_(nullptr), width_(0), height_(0), begin_x_(0), begin_y_(0) {}

CSWindow::CSWindow(WINDOW *win)
    : winmtx_(), parent_(nullptr), win_(win), width_(0), height_(0), begin_x_(0), begin_y_(0) {}

CSWindow::CSWindow(CSWindow &parent, int nlines, int ncols, int begin_y, int begin_x)
    : winmtx_(), parent_(&parent) {
  resize(nlines, ncols, begin_y, begin_x);
}

CSWindow::CSWindow(CSWindow &parent, const CSWindowNoInitialize &no_initialize)
    : winmtx_(), parent_(&parent), win_(nullptr), width_(0), height_(0), begin_x_(0), begin_y_(0) {}

int CSWindow::resize(int nlines, int ncols, int begin_y, int begin_x) {
  std::unique_lock<std::recursive_mutex> lock(winmtx_);
  if (win_) {
    delwin(win_);
  }
  win_ = subwin(parent_->getSelfWindow(), nlines, ncols, begin_y, begin_x); // nlines, ncols, begin_y, begin_x
//  printf("win_ => %p ,, %p :: %d, %d, %d, %d\n", parent_->getSelfWindow(), win_, nlines, ncols, begin_y, begin_x);
  height_ = nlines;
  width_ = ncols;
  begin_x_ = begin_x;
  begin_y_ = begin_y;
  return 0;
}

CSWindow::~CSWindow() {
  destroy();
}

void CSWindow::destroy() {
  if (win_) {
    delwin(win_);
    win_ = nullptr;
  }
}

WINDOW *CSWindow::getSelfWindow() {
  std::unique_lock<std::recursive_mutex> lock(winmtx_);
  return win_;
}

std::recursive_mutex &CSWindow::winmutex() {
  return winmtx_;
}

int CSWindow::getWidth() const {
  return width_;
}

int CSWindow::getHeight() const {
  return height_;
}

int CSWindow::getBeginX() const {
  return begin_x_;
}

int CSWindow::getBeginY() const {
  return begin_y_;
}

int CSWindow::getEndX() const {
  return begin_x_ + width_;
}

int CSWindow::getEndY() const {
  return begin_y_ + height_;
}

int CSWindow::clear() {
  return ::wclear(get());
}

int CSWindow::clearAll() {
  std::unique_lock<std::recursive_mutex> lock(winmtx_);
  auto w = get();
  ::werase(w);
  ::wclear(w);
  ::wclrtoeol(w);
  ::redrawwin(w);
  return 0;
}

int CSWindow::refresh() {
  return ::wrefresh(get());
}

} // namespace curses
} // namespace jcu
