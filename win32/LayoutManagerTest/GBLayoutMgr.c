#include "GBLayoutMgr.h"

#define DEFAULT_CELL_CAPACITY 10

struct GBLayout_ {
	GBLayoutCell* cells;
	INT numCells;
	INT capacity;
};

void GBLayoutInitCell(GBLayoutCell* cell)
{
	cell->hwnd = NULL;
	cell->x = 0;
	cell->y = 0;
	cell->xSpan = 1;
	cell->ySpan = 1;
	cell->weightx = 0;
	cell->weighty = 0;
	cell->preferredx = -1;
	cell->preferredy = -1;
}

GBLayout* GBLayoutCreate()
{
	GBLayout* result = (GBLayout*)HeapAlloc(GetProcessHeap(), 0, sizeof(GBLayout));
	result->cells = (GBLayoutCell*)HeapAlloc(GetProcessHeap(), 0, sizeof(GBLayoutCell) * DEFAULT_CELL_CAPACITY);
	result->numCells = 0;
	result->capacity = DEFAULT_CELL_CAPACITY;
	return result;
}

void GBLayoutDestroy(GBLayout* layout)
{
	HeapFree(GetProcessHeap(), 0, layout->cells);
	HeapFree(GetProcessHeap(), 0, layout);
}

BOOL GBLayoutPerform(HWND parent, const GBLayout* layout)
{
	//incomplete
	RECT size;
	GetClientRect(parent, &size);
	int totalRows = 0;
	int totalCols = 0;
	for (int i = 0; i < layout->numCells; ++i) {
		
	}
	float* totalsWeightsX = HeapAlloc(GetProcessHeap(), 0, sizeof(float) * totalCols);
	float* totalsWeightY = HeapAlloc(GetProcessHeap(), 0, sizeof(float) * totalCols);
}

BOOL GBLayoutAddCell(GBLayout* layout, const GBLayoutCell* cell)
{
	if (layout->numCells >= layout->capacity) {
		GBLayoutCell* ptr = HeapReAlloc(GetProcessHeap(), 0, layout->cells, sizeof(GBLayoutCell) * layout->capacity * 2);
		if (ptr == NULL) return FALSE;
		else {
			layout->capacity = layout->capacity * 2;
			layout->cells = ptr;
		}
	}
	layout->cells[layout->numCells] = *cell;
	++layout->numCells;
	return TRUE;
}

BOOL GBLayoutRemoveCell(GBLayout* layout, int x, int y)
{
	for (int i = 0; i < layout->numCells; ++i) {
		if (layout->cells[i].x == x && layout->cells[i].y == y) {
			for (int j = i + 1; j < layout->numCells; ++j) {
				layout->cells[j - 1] = layout->cells[j];
			}
			--layout->numCells;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL GBLayoutGetCell(const GBLayout* layout, GBLayoutCell* cell, int x, int y)
{
	for (int i = 0; i < layout->numCells; ++i) {
		if (layout->cells[i].x == x && layout->cells[i].y == y) {
			*cell = layout->cells[i];
			return TRUE;
		}
	}
	return FALSE;
}