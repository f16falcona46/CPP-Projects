#include "precompiled_headers.h"
#include "common.h"
#include "DrawingPanel.h"


//// can also add wxFULL_REPAINT_ON_RESIZE to styles instead of this->Refresh()
DrawingPanel::DrawingPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(100, 100), wxTAB_TRAVERSAL)
{
	this->SetMinSize(this->GetSize());
	this->Bind(wxEVT_PAINT, &DrawingPanel::OnPaint, this);
}

void DrawingPanel::OnPaint(wxPaintEvent& event) {
	this->Refresh();
	wxPaintDC dc(this);
	wxSize size = this->GetSize();
	dc.Clear();
	dc.DrawLine(0, 0, size.GetWidth(), size.GetHeight());
}
