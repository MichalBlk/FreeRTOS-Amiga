#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <cdefs.h>

typedef enum {
  KEY_BACKQUOTE = 0x00,
  KEY_1 = 0x01,
  KEY_2 = 0x02,
  KEY_3 = 0x03,
  KEY_4 = 0x04,
  KEY_5 = 0x05,
  KEY_6 = 0x06,
  KEY_7 = 0x07,
  KEY_8 = 0x08,
  KEY_9 = 0x09,
  KEY_0 = 0x0a,
  KEY_MINUS = 0x0b,
  KEY_EQUAL = 0x0c,
  KEY_BACKSLASH = 0x0d,
  KEY_KP_0 = 0x0f,
  KEY_Q = 0x10,
  KEY_W = 0x11,
  KEY_E = 0x12,
  KEY_R = 0x13,
  KEY_T = 0x14,
  KEY_Y = 0x15,
  KEY_U = 0x16,
  KEY_I = 0x17,
  KEY_O = 0x18,
  KEY_P = 0x19,
  KEY_LBRACKET = 0x1a,
  KEY_RBRACKET = 0x1b,
  KEY_KP_1 = 0x1d,
  KEY_KP_2 = 0x1e,
  KEY_KP_3 = 0x1f,
  KEY_A = 0x20,
  KEY_S = 0x21,
  KEY_D = 0x22,
  KEY_F = 0x23,
  KEY_G = 0x24,
  KEY_H = 0x25,
  KEY_J = 0x26,
  KEY_K = 0x27,
  KEY_L = 0x28,
  KEY_SEMICOLON = 0x29,
  KEY_QUOTE = 0x2a,
  KEY_KP_4 = 0x2d,
  KEY_KP_5 = 0x2e,
  KEY_KP_6 = 0x2f,
  KEY_Z = 0x31,
  KEY_X = 0x32,
  KEY_C = 0x33,
  KEY_V = 0x34,
  KEY_B = 0x35,
  KEY_N = 0x36,
  KEY_M = 0x37,
  KEY_COMMA = 0x38,
  KEY_PERIOD = 0x39,
  KEY_SLASH = 0x3a,
  KEY_KP_PERIOD = 0x3c,
  KEY_KP_7 = 0x3d,
  KEY_KP_8 = 0x3e,
  KEY_KP_9 = 0x3f,
  KEY_SPACE = 0x40,
  KEY_BACKSPACE = 0x41,
  KEY_TAB = 0x42,
  KEY_KP_ENTER = 0x43,
  KEY_RETURN = 0x44,
  KEY_ESCAPE = 0x45,
  KEY_DELETE = 0x46,
  KEY_KP_MINUS = 0x4a,
  KEY_UP = 0x4c,
  KEY_DOWN = 0x4d,
  KEY_RIGHT = 0x4e,
  KEY_LEFT = 0x4f,
  KEY_F1 = 0x50,
  KEY_F2 = 0x51,
  KEY_F3 = 0x52,
  KEY_F4 = 0x53,
  KEY_F5 = 0x54,
  KEY_F6 = 0x55,
  KEY_F7 = 0x56,
  KEY_F8 = 0x57,
  KEY_F9 = 0x58,
  KEY_F10 = 0x59,
  KEY_KP_LPAREN = 0x5a,
  KEY_KP_RPAREN = 0x5b,
  KEY_KP_DIVIDE = 0x5c,
  KEY_KP_MULTIPLY = 0x5d,
  KEY_KP_PLUS = 0x5e,
  KEY_HELP = 0x5f,
  KEY_LSHIFT = 0x60,
  KEY_RSHIFT = 0x61,
  KEY_CAPSLOCK = 0x62,
  KEY_CONTROL = 0x63,
  KEY_LALT = 0x64,
  KEY_RALT = 0x65,
  KEY_LAMIGA = 0x66,
  KEY_RAMIGA = 0x67
} KeyCode_t;

typedef enum {
  MOD_PRESSED = BIT(0),
  MOD_CONTROL = BIT(1),
  MOD_LSHIFT = BIT(2),
  MOD_RSHIFT = BIT(3),
  MOD_LALT = BIT(4),
  MOD_RALT = BIT(5),
  MOD_LAMIGA = BIT(6),
  MOD_RAMIGA = BIT(7),

  MOD_SHIFT = MOD_LSHIFT | MOD_RSHIFT,
  MOD_ALT = MOD_LALT | MOD_RALT,
  MOD_AMIGA = MOD_LAMIGA | MOD_RAMIGA
} KeyMod_t;

#define EV_KEY 2

typedef struct KeyEvent {
  uint8_t type;     /* always set to EV_KEY */
  uint8_t modifier;
  uint8_t code;
  char ascii;
} KeyEvent_t;

typedef void (*KeyEventNotify_t)(const KeyEvent_t *);

/* Notify procedure will always be called from ISR! */
void KeyboardInit(KeyEventNotify_t notify);
void KeyboardKill(void);

#endif /* !_KEYBOARD_H_ */
