#include "window.h"


bool RaytracerApp::OnInit() {
  RaytracerFrame * frame = new RaytracerFrame(_("Interactive Raytracer"),
                                              wxPoint(50, 50),
                                              wxSize(800, 600));
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

  wxMenuBar * menuBar = new wxMenuBar;
  menuBar->Append(menuFile, _("&File"));
  menuBar->Append(menuHelp, _("&Help"));

  SetMenuBar(menuBar);

  panel = new wxPanel(this);
  rayTracer = new RAYTRACER_NAMESPACE::RayTracer();

  CreateStatusBar();
  SetStatusText(_("Interactive Raytracer Ready"));

  Connect(wxEVT_PAINT, wxPaintEventHandler(RaytracerFrame::OnPaint));
  Center();
}


void RaytracerFrame::OnChar(wxKeyEvent &event) {
  switch (event.GetKeyCode()) {
    case 83:
    case 115:
      rayTracer->scene->camera->move('s', 0.2f);
      break;
    case 87:
    case 119:
      rayTracer->scene->camera->move('w', 0.2f);
      break;
    default:
      event.Skip();
      return;
  }

  delete image;
  image = new wxImage(rayTracer->screen->width, rayTracer->screen->height,
                      rayTracer->rayTrace());
  wxClientDC imagePainter(panel);
  imagePainter.DrawBitmap(wxBitmap(*image), 0, 0, false);
}


void RaytracerFrame::OnAbout(wxCommandEvent &WXUNUSED(event)) {
  wxMessageBox(_("Interactive raytracer written by Kent Lui."),
               _("About Interactive Raytracer"),
               wxOK | wxICON_INFORMATION, this);
}


void RaytracerFrame::OnOpen(wxCommandEvent &WXUNUSED(event)) {
  wxFileDialog * openFileDialog = new wxFileDialog(this);

  if (openFileDialog->ShowModal() == wxID_OK) {
    wxString openFilePath = openFileDialog->GetPath();

    rayTracer->init(openFilePath.char_str());
    panel->SetClientSize(rayTracer->screen->width, rayTracer->screen->height);
    this->Fit();

    image = new wxImage(rayTracer->screen->width, rayTracer->screen->height,
                        rayTracer->rayTrace());
    wxClientDC imagePainter(panel);
    imagePainter.DrawBitmap(wxBitmap(*image), 0, 0, false);
    panel->Connect(wxEVT_CHAR, wxKeyEventHandler(RaytracerFrame::OnChar));
  }
}


void RaytracerFrame::OnPaint(wxPaintEvent &WXUNUSED(event)) {
  if (image != NULL) {
    wxPaintDC imagePainter(panel);
    imagePainter.DrawBitmap(wxBitmap(*image), 0, 0, false);
  }
}


void RaytracerFrame::OnQuit(wxCommandEvent &WXUNUSED(event)) {
  Close(true);
}
