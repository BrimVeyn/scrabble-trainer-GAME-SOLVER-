#include "../../include/hashTableDefine.h"


TextVar getTextVar(char *text, int font_size) {
    return (TextVar) {
		.text_size = MeasureText(text, font_size),
		.font_size = font_size,
		.text = text,
	};
}

TextVar getText(int c, int font_size) {
	switch (c) {
		case TWORD:
			return getTextVar("MT", font_size);
		case DWORD:
			return getTextVar("MD", font_size);
		case DLETTER:
			return getTextVar("LD", font_size);
		case TLETTER:
			return getTextVar("LT", font_size);
		default:
			return (TextVar) {0};
	}
}

Color getColor(int c) {
	switch (c) {
		case TWORD:
			return RED;
		case DWORD:
			return BEIGE;
		case DLETTER:
			return SKYBLUE;
		case TLETTER:
			return DARKBLUE;
		default:
			return WHITE;
	}
}
