//
// Created by Tau on 2019/4/30.
//

#ifndef HG_EDITOR_H
#define HG_EDITOR_H

#include <termios.h>

#define CTRL_KEY(k) ((k) & 0x1f)

typedef struct StatementBufferTag Buffer;

struct StatementBufferTag {
    char *buf;
    Buffer *next;
    int len;
    int size;
};

typedef struct EditorConfigTag {
    struct termios originalTermios;
    int screenRows, screenCols;
    int cx, cy; // position of cursor
    Buffer *buffer;
} EditorConfig;

Buffer *initBuffer(Buffer *next);

typedef enum {
    ARROW_LEFT_KEY = 1000,
    ARROW_RIGHT_KEY,
    ARROW_UP_KEY,
    ARROW_DOWN_KEY,
    PAGE_UP_KEY,
    PAGE_DOWN_KEY,
    HOME_KEY,
    END_KEY,
    DEL_KEY,
    ENTER_KEY,
    TAB_KEY,
} EditorKey;

// recover to the original mode when `exit`
void enableRawMode();

EditorKey readKey();

void refreshScreen();

int getWindowSize(int *rows, int *cols);

void processKeypress();

void initEditor();

void statementEnd(void(*call)(char *));

#endif //HG_EDITOR_H
