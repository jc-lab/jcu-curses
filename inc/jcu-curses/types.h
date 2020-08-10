/**
 * @file	types.h
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2020/08/10
 * @copyright Copyright (C) 2020 jc-lab.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef JCU_CURSES_TYPES_H_
#define JCU_CURSES_TYPES_H_

#include <functional>

namespace jcu {
namespace curses {

struct NativeWindowSize {
  unsigned short int ws_row;
  unsigned short int ws_col;
  unsigned short int ws_xpixel;
  unsigned short int ws_ypixel;
};

typedef std::function<void(const NativeWindowSize& new_window_size)> OnResizedHandler;

struct CSWindowNoInitialize {
  int x;
  CSWindowNoInitialize() : x(0) {}
};

} // namespace curses
} // namespace jcu

#endif //JCU_CURSES_TYPES_H_
