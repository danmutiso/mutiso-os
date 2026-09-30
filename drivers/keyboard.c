#include "keyboard.h"
#include "port.h"
#include "vga.h"
#include "../interrupts/pic.h"

static int shift_pressed = 0;
static int caps_pressed = 0;
static const char keyboard_map[] = {
    0,       // 0x00
    0,       // 0x01 Escape
    '1',     // 0x02
    '2',     // 0x03
    '3',     // 0x04
    '4',     // 0x05
    '5',     // 0x06
    '6',     // 0x07
    '7',     // 0x08
    '8',     // 0x09
    '9',     // 0x0A
    '0',     // 0x0B
    '-',     // 0x0C
    '=',     // 0x0D
    0,       // 0x0E Backspace
    0,       // 0x0F Tab

    'q',     // 0x10
    'w',     // 0x11
    'e',     // 0x12
    'r',     // 0x13
    't',     // 0x14
    'y',     // 0x15
    'u',     // 0x16
    'i',     // 0x17
    'o',     // 0x18
    'p',     // 0x19
    '[',     // 0x1A
    ']',     // 0x1B
    0,       // 0x1C Enter
    0,       // 0x1D Ctrl

    'a',     // 0x1E
    's',     // 0x1F
    'd',     // 0x20
    'f',     // 0x21
    'g',     // 0x22
    'h',     // 0x23
    'j',     // 0x24
    'k',     // 0x25
    'l',     // 0x26
    ';',     // 0x27
    '\'',    // 0x28
    '`',     // 0x29
    0,       // 0x2A Left Shift
    '\\',    // 0x2B

    'z',     // 0x2C
    'x',     // 0x2D
    'c',     // 0x2E
    'v',     // 0x2F
    'b',     // 0x30
    'n',     // 0x31
    'm',     // 0x32
    ',',     // 0x33
    '.',     // 0x34
    '/',     // 0x35
    0,       // 0x36 Right Shift
    0,       // 0x37 Keypad *
    0,       // 0x38 Alt
    ' ',     // 0x39 Space
    0,       // 0x3A Caps Lock
    0,       // 0x3B F1
    0,       // 0x3C F2
    0,       // 0x3D F3
    0,       // 0x3E F4
    0,       // 0x3F F5
    0,       // 0x40 F6
    0,       // 0x41 F7
    0,       // 0x42 F8
    0,       // 0x43 F9
    0,       // 0x44 F10
    0,       // 0x45 Num Lock
    0,       // 0x46 Scroll Lock
    0,       // 0x47 Keypad 7
    0,       // 0x48 Keypad 8
    0,       // 0x49 Keypad 9
    0,       // 0x4A Keypad -
    0,       // 0x4B Keypad 4
    0,       // 0x4C Keypad 5
    0,       // 0x4D Keypad 6
    0,       // 0x4E Keypad +
    0,       // 0x4F Keypad 1
    0,       // 0x50 Keypad 2
    0,       // 0x51 Keypad 3
    0,       // 0x52 Keypad 0
    0,       // 0x53 Keypad .
    0,       // 0x54
    0,       // 0x55
    0,       // 0x56
    0,       // 0x57 F11
    0        // 0x58 F12
};

static const char shift_keyboard_map[] = {
    0,       // 0x00
    0,       // 0x01 Escape
    '!',     // 0x02
    '@',     // 0x03
    '#',     // 0x04
    '$',     // 0x05
    '%',     // 0x06
    '^',     // 0x07
    '&',     // 0x08
    '*',     // 0x09
    '(',     // 0x0A
    ')',     // 0x0B
    '_',     // 0x0C
    '+',     // 0x0D
    0,       // 0x0E Backspace
    0,       // 0x0F Tab

    'Q',     // 0x10
    'W',     // 0x11
    'E',     // 0x12
    'R',     // 0x13
    'T',     // 0x14
    'Y',     // 0x15
    'U',     // 0x16
    'I',     // 0x17
    'O',     // 0x18
    'P',     // 0x19
    '{',     // 0x1A
    '}',     // 0x1B
    0,       // 0x1C Enter
    0,       // 0x1D Ctrl

    'A',     // 0x1E
    'S',     // 0x1F
    'D',     // 0x20
    'F',     // 0x21
    'G',     // 0x22
    'H',     // 0x23
    'J',     // 0x24
    'K',     // 0x25
    'L',     // 0x26
    ':',     // 0x27
    '"',     // 0x28
    '~',     // 0x29
    0,       // 0x2A Left Shift
    '|',     // 0x2B

    'Z',     // 0x2C
    'X',     // 0x2D
    'C',     // 0x2E
    'V',     // 0x2F
    'B',     // 0x30
    'N',     // 0x31
    'M',     // 0x32
    '<',     // 0x33
    '>',     // 0x34
    '?',     // 0x35
    0,       // 0x36 Right Shift
    0,       // 0x37 Keypad *
    0,       // 0x38 Alt
    ' ',     // 0x39 Space
    0,       // 0x3A Caps Lock
    0,       // 0x3B F1
    0,       // 0x3C F2
    0,       // 0x3D F3
    0,       // 0x3E F4
    0,       // 0x3F F5
    0,       // 0x40 F6
    0,       // 0x41 F7
    0,       // 0x42 F8
    0,       // 0x43 F9
    0,       // 0x44 F10
    0,       // 0x45 Num Lock
    0,       // 0x46 Scroll Lock
    0,       // 0x47 Keypad 7
    0,       // 0x48 Keypad 8
    0,       // 0x49 Keypad 9
    0,       // 0x4A Keypad -
    0,       // 0x4B Keypad 4
    0,       // 0x4C Keypad 5
    0,       // 0x4D Keypad 6
    0,       // 0x4E Keypad +
    0,       // 0x4F Keypad 1
    0,       // 0x50 Keypad 2
    0,       // 0x51 Keypad 3
    0,       // 0x52 Keypad 0
    0,       // 0x53 Keypad .
    0,       // 0x54
    0,       // 0x55
    0,       // 0x56
    0,       // 0x57 F11
    0        // 0x58 F12
};

static const char caps_keyboard_map[] = {
    0,       // 0x00
    0,       // 0x01 Escape
    '1',     // 0x02
    '2',     // 0x03
    '3',     // 0x04
    '4',     // 0x05
    '5',     // 0x06
    '6',     // 0x07
    '7',     // 0x08
    '8',     // 0x09
    '9',     // 0x0A
    '0',     // 0x0B
    '-',     // 0x0C
    '=',     // 0x0D
    0,       // 0x0E Backspace
    0,       // 0x0F Tab

    'Q',     // 0x10
    'W',     // 0x11
    'E',     // 0x12
    'R',     // 0x13
    'T',     // 0x14
    'Y',     // 0x15
    'U',     // 0x16
    'I',     // 0x17
    'O',     // 0x18
    'P',     // 0x19
    '[',     // 0x1A
    ']',     // 0x1B
    0,       // 0x1C Enter
    0,       // 0x1D Ctrl

    'A',     // 0x1E
    'S',     // 0x1F
    'D',     // 0x20
    'F',     // 0x21
    'G',     // 0x22
    'H',     // 0x23
    'J',     // 0x24
    'K',     // 0x25
    'L',     // 0x26
    ';',     // 0x27
    '\'',    // 0x28
    '`',     // 0x29
    0,       // 0x2A Left Shift
    '\\',    // 0x2B

    'Z',     // 0x2C
    'X',     // 0x2D
    'C',     // 0x2E
    'V',     // 0x2F
    'B',     // 0x30
    'N',     // 0x31
    'M',     // 0x32
    ',',     // 0x33
    '.',     // 0x34
    '/',     // 0x35
    0,       // 0x36 Right Shift
    0,       // 0x37 Keypad *
    0,       // 0x38 Alt
    ' ',     // 0x39 Space
    0,       // 0x3A Caps Lock
    0,       // 0x3B F1
    0,       // 0x3C F2
    0,       // 0x3D F3
    0,       // 0x3E F4
    0,       // 0x3F F5
    0,       // 0x40 F6
    0,       // 0x41 F7
    0,       // 0x42 F8
    0,       // 0x43 F9
    0,       // 0x44 F10
    0,       // 0x45 Num Lock
    0,       // 0x46 Scroll Lock
    0,       // 0x47 Keypad 7
    0,       // 0x48 Keypad 8
    0,       // 0x49 Keypad 9
    0,       // 0x4A Keypad -
    0,       // 0x4B Keypad 4
    0,       // 0x4C Keypad 5
    0,       // 0x4D Keypad 6
    0,       // 0x4E Keypad +
    0,       // 0x4F Keypad 1
    0,       // 0x50 Keypad 2
    0,       // 0x51 Keypad 3
    0,       // 0x52 Keypad 0
    0,       // 0x53 Keypad .
    0,       // 0x54
    0,       // 0x55
    0,       // 0x56
    0,       // 0x57 F11
    0        // 0x58 F12
};

void keyboard_handler(void)
{
    unsigned char scan_code;
    unsigned char character;

    scan_code = inb(KEYBOARD_DATA_PORT);

    if (scan_code == KEY_LEFT_SHIFT_P || scan_code == KEY_RIGHT_SHIFT_P) {
        shift_pressed = 1;
        pic_send_eoi(1);
        return;
    }

    if (scan_code == KEY_LEFT_SHIFT_R || scan_code == KEY_RIGHT_SHIFT_R) {
        shift_pressed = 0;
        pic_send_eoi(1);
        return;
    }

    if (scan_code == KEY_ENTER){
        newline();
        pic_send_eoi(1);
        return;
    }

    if (scan_code == KEY_BACKSPACE){
        backspace();
        pic_send_eoi(1);
        return;
    }

    if (scan_code == KEY_CAPS_LOCK){
        caps_pressed = !caps_pressed;
        pic_send_eoi(1);
        return;
    }

    if (scan_code & KEY_RELEASE) {
        pic_send_eoi(1);
        return;
    }

    /* Prevent array access outside our maps */
    if (scan_code >= sizeof(keyboard_map)) {
        pic_send_eoi(1);
        return;
    }

    if (shift_pressed) {
        character = shift_keyboard_map[scan_code];
    }else if(caps_pressed){
        character = caps_keyboard_map[scan_code];
    }else{
        character = keyboard_map[scan_code];
    }

    if(character){
        vga_putc(character, LIGHT_GREEN);
    }

    pic_send_eoi(1);
}
