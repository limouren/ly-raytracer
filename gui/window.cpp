#include "config.h"

#include "window.h"

#include "raytracer.h"


bool RaytracerApp::OnInit() {
  RaytracerFrame * frame = new RaytracerFrame(_("Interactive Raytracer"),
                                              wxPoint(50, 50),
                                              wxSize(450, 450));
  frame->Show(true);
  SetTopWindow(frame);
  return true;
}


RaytracerFrame::RaytracerFrame(const wxString &title, const wxPoint &pos,
                               const wxSize &size):
    wxFrame(NULL, -1, title, pos, size) {
  wxMenu * menuFile = new wxMenu;
  menuFile->Append(ID_OPEN, _("&Open"));
  menuFile->AppendSeparator();
  menuFile->Append(ID_QUIT, _("E&xit"));

  wxMenu * menuHelp = new wxMenu;
  menuHelp->Append(ID_ABOUT, _("&About"));

  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(menuFile, _("&File"));
  menuBar->Append(menuHelp, _("&Help"));

  SetMenuBar(menuBar);

  image = new wxImage();

  CreateStatusBar();
  SetStatusText(_("Interactive Raytracer Ready"));

  Connect(wxEVT_PAINT, wxPaintEventHandler(RaytracerFrame::OnPaint));
  Center();
}


void RaytracerFrame::OnAbout(wxCommandEvent &WXUNUSED(event)) {
  wxMessageBox(_("Interactive raytracer program written by Kent Lui."),
               _("About Interactive Raytracer"),
               wxOK | wxICON_INFORMATION, this);
}


void RaytracerFrame::OnOpen(wxCommandEvent &WXUNUSED(event)) {
  wxFileDialog * openFileDialog = new wxFileDialog(this);

  if (openFileDialog->ShowModal() == wxID_OK) {
    wxString openFilePath = openFileDialog->GetPath();
    RAYTRACER_NAMESPACE::RayTracer raytracer(openFilePath.char_str());

    delete image;
    image = new wxImage(raytracer.screen->width, raytracer.screen->height,
                        raytracer.rayTrace(), false);

    this->SetClientSize(image->GetWidth(), image->GetHeight());

    wxClientDC imagePainter(this);
    imagePainter.DrawBitmap(wxBitmap(*image), 0, 0, false);
  }
}


void RaytracerFrame::OnPaint(wxPaintEvent &WXUNUSED(event)) {
  if (image->IsOk()) {
    wxPaintDC imagePainter(this);
    imagePainter.DrawBitmap(wxBitmap(*image), 0, 0, false);
  }
}


void RaytracerFrame::OnQuit(wxCommandEvent &WXUNUSED(event)) {
  Close(true);
}
