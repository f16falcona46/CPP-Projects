#pragma once
#include <windows.h>

typedef struct GBLayoutCell_ {
	HWND hwnd;
	INT x;
	INT y;
	INT xSpan;
	INT ySpan;
	FLOAT weightx;
	FLOAT weighty;
	INT preferredx;
	INT preferredy;
	UINT align;
	UINT fill;
} GBLayoutCell;

struct GBLayout_;

enum GBAlign {
	GBAlign_CENTER = 0x00,
	GBAlign_LEFT = 0x01,
	GBAlign_RIGHT = 0x02,
	GBAlign_TOP = 0x04,
	GBAlign_BOTTOM = 0x08
};

enum GBFill {
	GBFill_NONE = 0x00,
	GBFill_VERT = 0x01,
	GBFill_HORIZ = 0x02
};

typedef struct GBLayout_ GBLayout;

void GBLayoutInitCell(GBLayoutCell* cell);
GBLayout* GBLayoutCreate();
void GBLayoutDestroy(GBLayout* layout);
BOOL GBLayoutPerform(HWND parent, const GBLayout* layout);
BOOL GBLayoutAddCell(GBLayout* layout, const GBLayoutCell* cell);
BOOL GBLayoutRemoveCell(GBLayout* layout, int x, int y);
BOOL GBLayoutGetCell(const GBLayout* layout, GBLayoutCell* cell, int x, int y);