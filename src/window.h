#ifndef SRC_WINDOW_H
#define SRC_WINDOW_H


#include "wx/wx.h"

#include "config.h"


class RaytracerApp: public wxApp {
  virtual bool OnInit();
};


class RaytracerFrame: public wxFrame {
  private:
    wxImage * image;

  public:
    RaytracerFrame(const wxString &title, const wxPoint &position,
                   const wxSize &size);

    void OnOpen(wxCommandEvent &event);
    void OnPaint(wxPaintEvent &event);
    void OnQuit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);

    DECLARE_EVENT_TABLE()
};


enum {
  ID_ABOUT,
  ID_OPEN,
  ID_QUIT,
};


BEGIN_EVENT_TABLE(RaytracerFrame, wxFrame)
  EVT_MENU(ID_ABOUT, RaytracerFrame::OnAbout)
  EVT_MENU(ID_OPEN, RaytracerFrame::OnOpen)
  EVT_MENU(ID_QUIT, RaytracerFrame::OnQuit)
END_EVENT_TABLE()


#endif
