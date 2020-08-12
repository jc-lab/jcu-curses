/**
 * @file	progress.cc
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2020/08/10
 * @copyright Copyright (C) 2020 jc-lab.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#include <vector>
#include <stdio.h>
#include <string.h>

#include "jcu-curses/progress.h"

namespace jcu {
namespace curses {

CSProgress::CSProgress(CSWindow &parent, int nlines, int ncols, int begin_y, int begin_x)
: CSWindow(parent, nlines, ncols, begin_y, begin_x)
{};

CSProgress::CSProgress(CSWindow &parent, const CSWindowNoInitialize& no_initialize)
: CSWindow(parent, no_initialize)
{};

void CSProgress::printProgress(float progress, const char* text_fmt) {
  std::vector<char> buffer(width_ + 1);
  int pos = 0;
  int i;

  int max_bar_size = width_ - 3;
  int filled_bar_length = progress * max_bar_size;

  char text_buffer[32];
  int text_length = snprintf(text_buffer, sizeof(text_buffer), text_fmt, progress * 100.0f);

  max_bar_size -= text_length;

  if (filled_bar_length > max_bar_size) {
    filled_bar_length = max_bar_size;
  }

  buffer[pos++] = '[';
  for (i = 0; i < filled_bar_length; i++) {
    buffer[pos++] = '#';
  }
  for (; i < max_bar_size; i++) {
    buffer[pos++] = ' ';
  }
  buffer[pos++] = ']';

  for (i = 0; i < text_length; i++) {
    buffer[pos++] = text_buffer[i];
  }

  for (; pos < width_; pos++) {
    buffer[pos] = ' ';
  }

  wmove(get(), 0, 0);
  waddnstr(get(), buffer.data(), pos);

  refresh();
}

} // namespace curses
} // namespace jcu
