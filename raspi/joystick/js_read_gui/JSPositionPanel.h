#include "precompiled_headers.h"
#include "common.h"

#ifndef JS_POSITIONPANEL_H
#define JS_POSITIONPANEL_H

class JSPositionPanel : public wxPanel
{
public:
	JSPositionPanel(wxWindow* parent);
	void OnPaint(wxPaintEvent& event);
};

#endif //JS_POSITIONPANEL_H
