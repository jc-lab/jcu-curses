/**
 * @file	progress.h
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2020/08/10
 * @copyright Copyright (C) 2020 jc-lab.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#ifndef JCU_CURSES_PROGRESS_H_
#define JCU_CURSES_PROGRESS_H_

#include "window.h"

namespace jcu {
namespace curses {

class CSProgress : public CSWindow {
 public:
  CSProgress(CSWindow &parent, int nlines, int ncols, int begin_y, int begin_x);

  CSProgress(CSWindow &parent, const CSWindowNoInitialize& no_initialize);

  void printProgress(float progress);
};

} // namespace curses
} // namespace jcu

#endif //JCU_CURSES_PROGRESS_H_
