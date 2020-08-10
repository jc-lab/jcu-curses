#include <stdio.h>
#include <jcu-curses/screen.h>
#include <jcu-curses/window.h>
#include <jcu-curses/box_window.h>
#include <jcu-curses/progress.h>

using namespace jcu::curses;

int main(void) {
  setbuf(stdout, 0);
  setbuf(stderr, 0);

  printf("TP-1 %s // %d\n", getenv("NCURSES_TRACE"), TRACE_MAXIMUM);

  CSScreen main_screen("/dev/tty1");

  main_screen.setDummyKeyinput(true);

  CSWindow win_title(main_screen, CSWindowNoInitialize{});
  CSBoxWindow win_head(main_screen, CSWindowNoInitialize{});
  CSWindow win_body(main_screen, CSWindowNoInitialize{});
  CSProgress win_progress(main_screen, CSWindowNoInitialize{});

  main_screen.setOnResized([&](const NativeWindowSize& ws) -> void {
    printf("setOnResized => %d x %d\n", ws.ws_row, ws.ws_col);
    win_title.resize(1, main_screen.getWidth(), 0, 0);
    win_head.resize(10, main_screen.getWidth() - 1, win_title.getEndY() + 1, 0);
    win_body.resize(main_screen.getHeight() - win_head.getEndY() - 2, main_screen.getWidth(), win_head.getEndY() + 1, 0);
    win_progress.resize(1, main_screen.getWidth(), win_body.getBeginY() + win_body.getHeight(), 0);

    printf("win_title = {%d, %d}\n", win_title.getBeginY(), win_title.getEndY());
    printf("win_head = {%d, %d}\n", win_head.getBeginY(), win_head.getEndY());
    printf("win_body = {%d, %d}\n", win_body.getBeginY(), win_body.getEndY());
    printf("win_progress = {%d, %d}\n", win_progress.getBeginY(), win_progress.getEndY());

    win_title.clearAll();
    win_head.clearAll();
    win_body.clearAll();
    main_screen.clearAll();
    scrollok(win_body.get(), true);

    wprintw(win_head.get(), "abcd");

    wprintw(win_title.get(), "TEST APP TITLE");
    win_title.refresh();
    win_head.refresh();
    win_body.refresh();
    main_screen.refresh();
  });
  main_screen.onResized(main_screen.getNativeWindowSize());

  printf("TP-2 %d, %d\n",
         getmaxx(main_screen.get()),
         getmaxy(main_screen.get())
  );

  start_color();

  main_screen.refresh();

  float progress = 0;
  while(1) {
    do {
      std::unique_lock<std::recursive_mutex> lock(main_screen.winmutex());

      wprintw(win_body.get(), "abcd : %d\n", time(nullptr));
      win_body.refresh();

      if (progress > 1.0f) {
        progress = 0;
      }

      win_progress.printProgress(progress);
    } while (0);

    progress += 0.00125f;

    {
      struct timespec timeout = {.tv_sec = 0, .tv_nsec = 1000000 * 10};
      clock_nanosleep(CLOCK_MONOTONIC, 0 , &timeout, nullptr);
    }
  }

  printf("TP-end: %d\n", time(nullptr));

  return EXIT_SUCCESS;
}
