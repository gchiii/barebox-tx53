#ifndef READKEY_H
#define READKEY_H

#define CTL_CH(c)		((c) - 'a' + 1)

// Misc. non-Ascii keys
#define KEY_UP			CTL_CH('p')	// cursor key Up
#define KEY_DOWN		CTL_CH('n')	// cursor key Down
#define KEY_RIGHT		CTL_CH('f')	// Cursor Key Right
#define KEY_LEFT		CTL_CH('b')	// cursor key Left
#define KEY_HOME		CTL_CH('a')	// Cursor Key Home
#define KEY_ERASE_TO_EOL	CTL_CH('k')
#define KEY_REFRESH_TO_EOL	CTL_CH('e')
#define KEY_ERASE_LINE		CTL_CH('x')
#define KEY_INSERT		CTL_CH('o')
#define KEY_DEL7		(char)127
#define KEY_END			(char)133	// Cursor Key End
#define KEY_PAGEUP		(char)135	// Cursor Key Page Up
#define KEY_PAGEDOWN		(char)136	// Cursor Key Page Down
#define KEY_DEL			(char)137	// Cursor Key Del

char read_key(void);

#endif /* READKEY_H */
