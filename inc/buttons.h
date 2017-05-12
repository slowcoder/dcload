#pragma once

typedef enum {
    eButton_Invalid = 0,
    eButton_BTN0,
    eButton_BTN1,
    eButton_BTN2,
    eButton_Left,
    eButton_Right,
    eButton_MAX
} eButton;

int     buttons_init(void);
eButton buttons_poll(void);
