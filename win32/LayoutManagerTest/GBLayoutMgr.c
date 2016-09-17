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

}

BOOL GBLayoutAddCell(GBLayout* layout, const GBLayoutCell* cell)
{

}

BOOL GBLayoutGetCell(const GBLayout* layout, GBLayoutCell* cell, int x, int y)
{

}