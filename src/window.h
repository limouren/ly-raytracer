#ifndef SRC_WINDOW_H
#define SRC_WINDOW_H


#include "config.h"

#include "wx/wx.h"


class RaytracerApp: public wxApp {
  virtual bool OnInit();
};


class RaytracerFrame: public wxFrame {
  public:
    RaytracerFrame(const wxString &title, const wxPoint &position,
                   const wxSize &size);

    void OnQuit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);

    DECLARE_EVENT_TABLE()
};


enum {
  ID_QUIT = 1,
  ID_ABOUT,
};


BEGIN_EVENT_TABLE(RaytracerFrame, wxFrame)
  EVT_MENU(ID_QUIT, RaytracerFrame::OnQuit)
  EVT_MENU(ID_ABOUT, RaytracerFrame::OnAbout)
END_EVENT_TABLE()


#endif
