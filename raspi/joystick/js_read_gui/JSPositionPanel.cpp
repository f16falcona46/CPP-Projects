#include "precompiled_headers.h"
#include "common.h"
#include "JSPositionPanel.h"

JSPositionPanel::JSPositionPanel(wxWindow* parent) : wxPanel(parent)
{
	this->SetSize(100, 100);
	this->SetMinSize(this->GetSize());
	this->Bind(wxEVT_PAINT, &JSPositionPanel::OnPaint, this);
}

void JSPositionPanel::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);
	wxSize size = this->GetSize();
	this->Refresh();
	dc.DrawLine(0, 0, size.GetWidth(), size.GetHeight());
}
