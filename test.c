#include <ncurses.h>

int main() {
    initscr();
    noecho();
    cbreak();

    int height, width;
    getmaxyx(stdscr, height, width);

    // スクロールエリアを 1行目〜最下行 に固定（0行目は除外）
    setscrreg(1, height - 1);
    scrollok(stdscr, TRUE);

    while (1) {
        // 1. 画面クリア（もし毎フレーム更新するなら）
        // clear(); // ※もし毎回消すならここで

        // 2. ヘッダーへの書き込み（絶対座標 0, 0）
        mvprintw(0, 0, "--- FIXED HEADER ---");

        // 3. スクロールエリアへの書き込み（絶対座標の最下行を指定）
        // ここに文字が入ると、1〜height-1 の間だけで自動スクロールが起きます
        mvprintw(height - 1, 0, "New log message...");

        // 4. 反映
        refresh();
        
        break; // ループのサンプルなので一旦抜けます
    }

    getch();
    endwin();
    return 0;
}
