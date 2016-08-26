#include "precompiled_headers.h"
#include "common.h"

#ifndef DRAWINGPANEL_H
#define DRAWINGPANEL_H

class DrawingPanel : public wxPanel
{
public:
	DrawingPanel(wxWindow* parent);
	void OnPaint(wxPaintEvent& event);
};

#endif //DRAWINGPANEL_H
