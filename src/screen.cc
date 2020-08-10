/**
 * @file	screen.cc
 * @author	Joseph Lee <development@jc-lab.net>
 * @date	2020/08/10
 * @copyright Copyright (C) 2020 jc-lab.\n
 *            This software may be modified and distributed under the terms
 *            of the Apache License 2.0.  See the LICENSE file for details.
 */

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <termios.h>

#include <signal.h>

#include "jcu-curses/screen.h"

#include "utils.h"

namespace jcu {
namespace curses {

class CSScreen::Intl {
 public:
  struct termios saved_term;
};

CSScreen::~CSScreen() {
  delete intl_;
  intl_ = nullptr;
}

CSScreen::CSScreen(const char *ncurses_device)
: dummy_keyinput_(false), intl_(new Intl())
{
  struct termios term_opts = { 0 };

  running_.store(true);

  term_fd_ = open(ncurses_device, O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (term_fd_ <= 0) {
    fprintf(stderr, "Error opening device 1: %s.\n", ncurses_device);
    return;
  }

  if (tcgetattr(term_fd_, &term_opts) == -1) {
    fprintf(stderr, "Error tcgetattr\n");
  }
  intl_->saved_term = term_opts;

  term_opts.c_lflag = ICANON;
  for (int i = 0; i < NCCS; i++) {
    term_opts.c_cc[i] = 0;
  }
  tcflush(term_fd_, TCIFLUSH);
  if (tcsetattr(term_fd_, TCSAFLUSH, &term_opts) == -1) {
    fprintf(stderr, "Error tcsetattr\n");
  }

  term_fp_ = fdopen(term_fd_, "r+");
  if (!term_fp_) {
    fprintf(stderr, "Error opening device 2: %s.\n", ncurses_device);
    return;
  }

  const char* term_type = getenv("TERM");
  if (!term_type) {
    term_type = "xterm-256color";
  }
  term_type_ = term_type;
  scr_ = newterm(term_type, term_fp_, term_fp_);
  if (!scr_) {
    fprintf(stderr, "Error newterm: %d\n", errno);
    return;
  }

  set_term(scr_);

  win_ = stdscr;
  width_ = 0;
  height_ = 0;
  recheckSize();

  read_thread_ = std::thread([this]() -> void {
    while (running_.load()) {
      int rc;

      recheckSize();

      if (dummy_keyinput_.load()) {
        struct pollfd term_poll = { 0 };
        term_poll.fd = term_fd_;
        term_poll.events = POLLIN;

        rc = poll(&term_poll, 1, 250);

        if (rc == 1) {
          unsigned char buf[16];
          int read_bytes = read(term_fd_, buf, sizeof(buf));
          if (read_bytes >= 0) {
//            printf("read_bytes = %d\n", read_bytes);
          }
        }
      } else {
        mySleepMs(250);
      }
    }
  });
}

std::string CSScreen::getTermType() const {
  return term_type_;
}

void CSScreen::recheckSize() {
  struct winsize w;
  int rc = ioctl(term_fd_, TIOCGWINSZ, &w);
  if (rc == 0) {
    if ((w.ws_col != width_) || (w.ws_row != height_)) {
      std::unique_lock<std::recursive_mutex> lock(winmtx_);
      NativeWindowSize nw{};
      nw.ws_col = w.ws_col;
      nw.ws_row = w.ws_row;
      nw.ws_xpixel = w.ws_xpixel;
      nw.ws_ypixel = w.ws_ypixel;

      onResized(nw);
    }
  }
}

void CSScreen::destroy() {
  running_.store(false);

  if (read_thread_.joinable()) {
    read_thread_.join();
  }

  CSWindow::destroy();
  if (scr_) {
    ::delscreen(scr_);
    scr_ = nullptr;
  }
  if (term_fp_) {
    fclose(term_fp_);
    term_fp_ = nullptr;
  }

  if (term_fd_ > 0) {
    if (tcsetattr(term_fd_, TCSANOW, &intl_->saved_term) == -1) {
      fprintf(stderr, "Error tcgetattr\n");
    }

    ::close(term_fd_);
    term_fd_ = 0;
  }
}

void CSScreen::setOnResized(const OnResizedHandler &handler) {
  resized_handler_ = handler;
}

void CSScreen::onResized(const NativeWindowSize &new_window_size) {
  std::unique_lock<std::recursive_mutex> lock(winmtx_);
  ::resizeterm(new_window_size.ws_row, new_window_size.ws_col);
  width_ = new_window_size.ws_col;
  height_ = new_window_size.ws_row;
  if (resized_handler_) {
    resized_handler_(new_window_size);
  }
}

NativeWindowSize CSScreen::getNativeWindowSize() const {
  struct winsize w;
  int ret = ioctl(term_fd_, TIOCGWINSZ, &w);

  NativeWindowSize nw{};
  nw.ws_col = w.ws_col;
  nw.ws_row = w.ws_row;
  nw.ws_xpixel = w.ws_xpixel;
  nw.ws_ypixel = w.ws_ypixel;

  return nw;
}

void CSScreen::setDummyKeyinput(bool enable) {
  dummy_keyinput_.store(enable);
}

} // namespace curses
} // namespace jcu
