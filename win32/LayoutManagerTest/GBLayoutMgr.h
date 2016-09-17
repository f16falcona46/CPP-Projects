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
} GBLayoutCell;

struct GBLayout_;

typedef struct GBLayout_ GBLayout;

void GBLayoutInitCell(GBLayoutCell* cell);
GBLayout* GBLayoutCreate();
void GBLayoutDestroy(GBLayout* layout);
BOOL GBLayoutPerform(HWND parent, const GBLayout* layout);
BOOL GBLayoutAddCell(GBLayout* layout, const GBLayoutCell* cell);
BOOL GBLayoutGetCell(const GBLayout* layout, GBLayoutCell* cell, int x, int y);