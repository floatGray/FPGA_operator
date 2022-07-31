#include "include.h"

const unsigned char colorpalette[PALETTE_LENGTH] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
		0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00,
		0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0xC0, 0xC0, 0xC0,
		0x00, 0xC0, 0xDC, 0xC0, 0x00, 0xF0, 0xCA, 0xA6, 0x00, 0x00, 0x20, 0x40,
		0x00, 0x00, 0x20, 0x60, 0x00, 0x00, 0x20, 0x80, 0x00, 0x00, 0x20, 0xA0,
		0x00, 0x00, 0x20, 0xC0, 0x00, 0x00, 0x20, 0xE0, 0x00, 0x00, 0x40, 0x00,
		0x00, 0x00, 0x40, 0x20, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x40, 0x60,
		0x00, 0x00, 0x40, 0x80, 0x00, 0x00, 0x40, 0xA0, 0x00, 0x00, 0x40, 0xC0,
		0x00, 0x00, 0x40, 0xE0, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x60, 0x20,
		0x00, 0x00, 0x60, 0x40, 0x00, 0x00, 0x60, 0x60, 0x00, 0x00, 0x60, 0x80,
		0x00, 0x00, 0x60, 0xA0, 0x00, 0x00, 0x60, 0xC0, 0x00, 0x00, 0x60, 0xE0,
		0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x20, 0x00, 0x00, 0x80, 0x40,
		0x00, 0x00, 0x80, 0x60, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0xA0,
		0x00, 0x00, 0x80, 0xC0, 0x00, 0x00, 0x80, 0xE0, 0x00, 0x00, 0xA0, 0x00,
		0x00, 0x00, 0xA0, 0x20, 0x00, 0x00, 0xA0, 0x40, 0x00, 0x00, 0xA0, 0x60,
		0x00, 0x00, 0xA0, 0x80, 0x00, 0x00, 0xA0, 0xA0, 0x00, 0x00, 0xA0, 0xC0,
		0x00, 0x00, 0xA0, 0xE0, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0xC0, 0x20,
		0x00, 0x00, 0xC0, 0x40, 0x00, 0x00, 0xC0, 0x60, 0x00, 0x00, 0xC0, 0x80,
		0x00, 0x00, 0xC0, 0xA0, 0x00, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0xC0, 0xE0,
		0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0xE0, 0x20, 0x00, 0x00, 0xE0, 0x40,
		0x00, 0x00, 0xE0, 0x60, 0x00, 0x00, 0xE0, 0x80, 0x00, 0x00, 0xE0, 0xA0,
		0x00, 0x00, 0xE0, 0xC0, 0x00, 0x00, 0xE0, 0xE0, 0x00, 0x40, 0x00, 0x00,
		0x00, 0x40, 0x00, 0x20, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x60,
		0x00, 0x40, 0x00, 0x80, 0x00, 0x40, 0x00, 0xA0, 0x00, 0x40, 0x00, 0xC0,
		0x00, 0x40, 0x00, 0xE0, 0x00, 0x40, 0x20, 0x00, 0x00, 0x40, 0x20, 0x20,
		0x00, 0x40, 0x20, 0x40, 0x00, 0x40, 0x20, 0x60, 0x00, 0x40, 0x20, 0x80,
		0x00, 0x40, 0x20, 0xA0, 0x00, 0x40, 0x20, 0xC0, 0x00, 0x40, 0x20, 0xE0,
		0x00, 0x40, 0x40, 0x00, 0x00, 0x40, 0x40, 0x20, 0x00, 0x40, 0x40, 0x40,
		0x00, 0x40, 0x40, 0x60, 0x00, 0x40, 0x40, 0x80, 0x00, 0x40, 0x40, 0xA0,
		0x00, 0x40, 0x40, 0xC0, 0x00, 0x40, 0x40, 0xE0, 0x00, 0x40, 0x60, 0x00,
		0x00, 0x40, 0x60, 0x20, 0x00, 0x40, 0x60, 0x40, 0x00, 0x40, 0x60, 0x60,
		0x00, 0x40, 0x60, 0x80, 0x00, 0x40, 0x60, 0xA0, 0x00, 0x40, 0x60, 0xC0,
		0x00, 0x40, 0x60, 0xE0, 0x00, 0x40, 0x80, 0x00, 0x00, 0x40, 0x80, 0x20,
		0x00, 0x40, 0x80, 0x40, 0x00, 0x40, 0x80, 0x60, 0x00, 0x40, 0x80, 0x80,
		0x00, 0x40, 0x80, 0xA0, 0x00, 0x40, 0x80, 0xC0, 0x00, 0x40, 0x80, 0xE0,
		0x00, 0x40, 0xA0, 0x00, 0x00, 0x40, 0xA0, 0x20, 0x00, 0x40, 0xA0, 0x40,
		0x00, 0x40, 0xA0, 0x60, 0x00, 0x40, 0xA0, 0x80, 0x00, 0x40, 0xA0, 0xA0,
		0x00, 0x40, 0xA0, 0xC0, 0x00, 0x40, 0xA0, 0xE0, 0x00, 0x40, 0xC0, 0x00,
		0x00, 0x40, 0xC0, 0x20, 0x00, 0x40, 0xC0, 0x40, 0x00, 0x40, 0xC0, 0x60,
		0x00, 0x40, 0xC0, 0x80, 0x00, 0x40, 0xC0, 0xA0, 0x00, 0x40, 0xC0, 0xC0,
		0x00, 0x40, 0xC0, 0xE0, 0x00, 0x40, 0xE0, 0x00, 0x00, 0x40, 0xE0, 0x20,
		0x00, 0x40, 0xE0, 0x40, 0x00, 0x40, 0xE0, 0x60, 0x00, 0x40, 0xE0, 0x80,
		0x00, 0x40, 0xE0, 0xA0, 0x00, 0x40, 0xE0, 0xC0, 0x00, 0x40, 0xE0, 0xE0,
		0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x20, 0x00, 0x80, 0x00, 0x40,
		0x00, 0x80, 0x00, 0x60, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0xA0,
		0x00, 0x80, 0x00, 0xC0, 0x00, 0x80, 0x00, 0xE0, 0x00, 0x80, 0x20, 0x00,
		0x00, 0x80, 0x20, 0x20, 0x00, 0x80, 0x20, 0x40, 0x00, 0x80, 0x20, 0x60,
		0x00, 0x80, 0x20, 0x80, 0x00, 0x80, 0x20, 0xA0, 0x00, 0x80, 0x20, 0xC0,
		0x00, 0x80, 0x20, 0xE0, 0x00, 0x80, 0x40, 0x00, 0x00, 0x80, 0x40, 0x20,
		0x00, 0x80, 0x40, 0x40, 0x00, 0x80, 0x40, 0x60, 0x00, 0x80, 0x40, 0x80,
		0x00, 0x80, 0x40, 0xA0, 0x00, 0x80, 0x40, 0xC0, 0x00, 0x80, 0x40, 0xE0,
		0x00, 0x80, 0x60, 0x00, 0x00, 0x80, 0x60, 0x20, 0x00, 0x80, 0x60, 0x40,
		0x00, 0x80, 0x60, 0x60, 0x00, 0x80, 0x60, 0x80, 0x00, 0x80, 0x60, 0xA0,
		0x00, 0x80, 0x60, 0xC0, 0x00, 0x80, 0x60, 0xE0, 0x00, 0x80, 0x80, 0x00,
		0x00, 0x80, 0x80, 0x20, 0x00, 0x80, 0x80, 0x40, 0x00, 0x80, 0x80, 0x60,
		0x00, 0x80, 0x80, 0x80, 0x00, 0x80, 0x80, 0xA0, 0x00, 0x80, 0x80, 0xC0,
		0x00, 0x80, 0x80, 0xE0, 0x00, 0x80, 0xA0, 0x00, 0x00, 0x80, 0xA0, 0x20,
		0x00, 0x80, 0xA0, 0x40, 0x00, 0x80, 0xA0, 0x60, 0x00, 0x80, 0xA0, 0x80,
		0x00, 0x80, 0xA0, 0xA0, 0x00, 0x80, 0xA0, 0xC0, 0x00, 0x80, 0xA0, 0xE0,
		0x00, 0x80, 0xC0, 0x00, 0x00, 0x80, 0xC0, 0x20, 0x00, 0x80, 0xC0, 0x40,
		0x00, 0x80, 0xC0, 0x60, 0x00, 0x80, 0xC0, 0x80, 0x00, 0x80, 0xC0, 0xA0,
		0x00, 0x80, 0xC0, 0xC0, 0x00, 0x80, 0xC0, 0xE0, 0x00, 0x80, 0xE0, 0x00,
		0x00, 0x80, 0xE0, 0x20, 0x00, 0x80, 0xE0, 0x40, 0x00, 0x80, 0xE0, 0x60,
		0x00, 0x80, 0xE0, 0x80, 0x00, 0x80, 0xE0, 0xA0, 0x00, 0x80, 0xE0, 0xC0,
		0x00, 0x80, 0xE0, 0xE0, 0x00, 0xC0, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x20,
		0x00, 0xC0, 0x00, 0x40, 0x00, 0xC0, 0x00, 0x60, 0x00, 0xC0, 0x00, 0x80,
		0x00, 0xC0, 0x00, 0xA0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xE0,
		0x00, 0xC0, 0x20, 0x00, 0x00, 0xC0, 0x20, 0x20, 0x00, 0xC0, 0x20, 0x40,
		0x00, 0xC0, 0x20, 0x60, 0x00, 0xC0, 0x20, 0x80, 0x00, 0xC0, 0x20, 0xA0,
		0x00, 0xC0, 0x20, 0xC0, 0x00, 0xC0, 0x20, 0xE0, 0x00, 0xC0, 0x40, 0x00,
		0x00, 0xC0, 0x40, 0x20, 0x00, 0xC0, 0x40, 0x40, 0x00, 0xC0, 0x40, 0x60,
		0x00, 0xC0, 0x40, 0x80, 0x00, 0xC0, 0x40, 0xA0, 0x00, 0xC0, 0x40, 0xC0,
		0x00, 0xC0, 0x40, 0xE0, 0x00, 0xC0, 0x60, 0x00, 0x00, 0xC0, 0x60, 0x20,
		0x00, 0xC0, 0x60, 0x40, 0x00, 0xC0, 0x60, 0x60, 0x00, 0xC0, 0x60, 0x80,
		0x00, 0xC0, 0x60, 0xA0, 0x00, 0xC0, 0x60, 0xC0, 0x00, 0xC0, 0x60, 0xE0,
		0x00, 0xC0, 0x80, 0x00, 0x00, 0xC0, 0x80, 0x20, 0x00, 0xC0, 0x80, 0x40,
		0x00, 0xC0, 0x80, 0x60, 0x00, 0xC0, 0x80, 0x80, 0x00, 0xC0, 0x80, 0xA0,
		0x00, 0xC0, 0x80, 0xC0, 0x00, 0xC0, 0x80, 0xE0, 0x00, 0xC0, 0xA0, 0x00,
		0x00, 0xC0, 0xA0, 0x20, 0x00, 0xC0, 0xA0, 0x40, 0x00, 0xC0, 0xA0, 0x60,
		0x00, 0xC0, 0xA0, 0x80, 0x00, 0xC0, 0xA0, 0xA0, 0x00, 0xC0, 0xA0, 0xC0,
		0x00, 0xC0, 0xA0, 0xE0, 0x00, 0xC0, 0xC0, 0x00, 0x00, 0xC0, 0xC0, 0x20,
		0x00, 0xC0, 0xC0, 0x40, 0x00, 0xC0, 0xC0, 0x60, 0x00, 0xC0, 0xC0, 0x80,
		0x00, 0xC0, 0xC0, 0xA0, 0x00, 0xF0, 0xFB, 0xFF, 0x00, 0xA4, 0xA0, 0xA0,
		0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0x00,
		0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF,
		0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00 };
