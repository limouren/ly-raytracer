#include "config.h"

#include "window.h"


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
  wxMenu *menuFile = new wxMenu;

  menuFile->Append(ID_ABOUT, _("&About..."));
  menuFile->AppendSeparator();
  menuFile->Append(ID_QUIT, _("E&xit"));

  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(menuFile, _("&File"));

  SetMenuBar(menuBar);

  CreateStatusBar();
  SetStatusText(_("Welcome to wxWidgets!"));
}


void RaytracerFrame::OnQuit(wxCommandEvent &WXUNUSED(event)) {
  Close(true);
}


void RaytracerFrame::OnAbout(wxCommandEvent &WXUNUSED(event)) {
  wxMessageBox(_("Interactive raytracer program written by Kent Lui."),
               _("About Interactive Raytracer"),
               wxOK | wxICON_INFORMATION, this);
}
