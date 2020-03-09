//
// Created by Tau on 2019/4/30.
//
// FIXME: END OF LINE
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>
#include "editor.h"

#define errorHandler(msg) _errorHandler(msg)

void _errorHandler(char *msg) {
    fprintf(stderr, "Error: \r\n%s\r\n", msg);
}


static EditorConfig E;

int getEndOfLine() {
    return E.buffer->len;
}

void editorMoveCursor(EditorKey key) {
    switch (key) {
        case ARROW_LEFT_KEY:
            if (E.cx > 0)
                E.cx--;
            break;
        case ARROW_RIGHT_KEY:
            if (E.cx < getEndOfLine())
                E.cx++;
            break;
        case ARROW_UP_KEY:
            if (E.cy > 0)
                E.cy--;
            break;
        case ARROW_DOWN_KEY:
            if ((E.cy < E.screenRows))
                E.cy++;
            break;
        case HOME_KEY:
            E.cx = 0;
            break;
        case END_KEY:
            E.cx = getEndOfLine();
            break;
        case ENTER_KEY:
            // todo: scroll
            if ((E.cy < E.screenRows))
                E.cy++;
            E.cx = 0;
            break;
    }

}

void addToBuffer(char c, Buffer *buffer, int pos) {
    if (pos == getEndOfLine())
        buffer->buf[buffer->len++] = c;
    else {
        memmove(buffer->buf + pos + 1, buffer->buf + pos, buffer->len - pos + 1);
        buffer->buf[pos] = c;
        buffer->len++;
    }
    if (buffer->len == buffer->size) {
        buffer->size <<= 1;
        if (buffer->size <= 0) {
            errorHandler("buffer alloc");
        }
        buffer->buf = realloc(buffer->buf, buffer->size);

    }
    // TODO: refresh

    buffer->buf[buffer->len] = '\0';
}


void refresh() {
    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, 0);
    write(STDOUT_FILENO, &buf, strlen(buf));
    write(STDIN_FILENO, E.buffer->buf, E.buffer->len);
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1);
    write(STDOUT_FILENO, &buf, strlen(buf));
}

static void (*callAtEnd)(char *);

void nextLine() {
    callAtEnd(E.buffer->buf);
    E.buffer = initBuffer(E.buffer);
}


void statementEnd(void(*call)(char *)) {
    callAtEnd = call;
}

void showAllText() {
    Buffer *p = E.buffer;
    while (p != NULL) {
        Buffer *q = p->next;
        printf("\r\nlen: %d\r\ncontent:\r\n%s\r\n", p->len, p->buf);
        p = q;
    }
}


void processKeypress() {
    EditorKey c = readKey();
    switch (c) {
        case CTRL_KEY('c'):
            exit(0);
        case CTRL_KEY('t'):
            showAllText();
//            printf("\r\nlen: %d\r\ncontent:\r\n%s\r\n", E.buffer->len, E.buffer->buf);
            break;
//        case ARROW_UP_KEY:
//        case ARROW_DOWN_KEY:
//        TODO: up and down
        case HOME_KEY:
        case END_KEY:
        case ARROW_LEFT_KEY:
        case ARROW_RIGHT_KEY:
            editorMoveCursor(c);
            break;
        case ENTER_KEY:
            editorMoveCursor(c);
            // next line
            nextLine();
            break;
        default:
            if (!iscntrl(c)) {
                addToBuffer(c, E.buffer, E.cx);
                E.cx++;
            }
//            else {
//                printf("%d\r\n", c);
//            }
            break;
    }
    refresh();
}

static void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.originalTermios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &E.originalTermios); // get original termios
    atexit(disableRawMode); //call when exit

    struct termios raw = E.originalTermios;
//    raw.c_iflag &= ~(
//            IXON | // Ctrl-Q and Ctrl-S
//            ICRNL  // Ctrl-M
//    );
//    raw.c_oflag &= ~(OPOST); // turn off all output processing ("\n" -> "\r\n")
    raw.c_lflag &= ~(
            ECHO      // echo
            | ICANON   // canonical mode(read by line)
            | ISIG      // Ctrl-C and Ctrl-D
//              | IEXTEN     // Ctrl-V
    );
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw); // set termios
}

EditorKey readKey() {
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1) {
            errorHandler("read");
        }
    }
    switch (c) {
        case 10:
            return ENTER_KEY;
        default:
            break;
    }
    if (c == '\x1b') {
        char seq[3];
        if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';
        if (seq[0] == '[') {
            if (seq[1] >= '0' && seq[1] <= '9') {
                if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
                if (seq[2] == '~') {
                    switch (seq[1]) {
                        case '1':
                            return HOME_KEY;
                        case '3':
                            return DEL_KEY;
                        case '4':
                            return END_KEY;
                        case '5':
                            return PAGE_UP_KEY;
                        case '6':
                            return PAGE_DOWN_KEY;
                        case '7':
                            return HOME_KEY;
                        case '8':
                            return END_KEY;
                    }
                }
            } else {
                switch (seq[1]) {
                    case 'A':
                        return ARROW_UP_KEY;
                    case 'B':
                        return ARROW_DOWN_KEY;
                    case 'C':
                        return ARROW_RIGHT_KEY;
                    case 'D':
                        return ARROW_LEFT_KEY;
                    case 'H':
                        return HOME_KEY;
                    case 'F':
                        return END_KEY;
                }
            }
        } else if (seq[0] == 'O') {
            switch (seq[1]) {
                case 'H':
                    return HOME_KEY;
                case 'F':
                    return END_KEY;
            }
        }
        return '\x1b';
    } else {
        return c;
    }
}


void refreshScreen() {
    write(STDOUT_FILENO, "\x1b[2J\x1b[H", 7);
}

int getWindowSize(int *rows, int *cols) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        return -1;
    } else {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}


static void release() {
    //
    Buffer *p = E.buffer;
    while (p != NULL) {
        Buffer *q = p->next;
        free(p->buf);
        free(p);
        p = q;
    }
}

Buffer *initBuffer(Buffer *next) {
    Buffer *buffer = malloc(sizeof(Buffer));
    buffer->len = 0;
    buffer->size = 1024;
    buffer->buf = malloc(buffer->size);
    buffer->buf[0] = '\0';
    buffer->next = next;
    return buffer;
}

void initEditor() {
    // initial position of cursor
    E.cx = 0, E.cy = 0;

    // init buffer
    E.buffer = initBuffer(NULL);

    if (getWindowSize(&E.screenRows, &E.screenCols) == -1)
        errorHandler("getWindowSize");
    refreshScreen();
    enableRawMode();
    atexit(refreshScreen);
    atexit(release);
}

