#include "precompiled_headers.h"
#include "common.h"
#include "DrawingPanel.h"

DrawingPanel::DrawingPanel(wxWindow* parent) : wxPanel(parent)
{
	this->SetSize(100, 100);
	this->SetMinSize(this->GetSize());
	this->Bind(wxEVT_PAINT, &DrawingPanel::OnPaint, this);
}

void DrawingPanel::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);
	wxSize size = this->GetSize();
	this->Refresh();
	dc.DrawLine(0, 0, size.GetWidth(), size.GetHeight());
}
