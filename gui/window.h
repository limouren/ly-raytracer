#ifndef SRC_WINDOW_H
#define SRC_WINDOW_H


#include "config.h"

#include "wx/wx.h"

#include "raytracer.h"


// TODO(kent): Avoid evil global variables
// All attempts to make these class variables of the application have caused
// memory corruption issues, so this is a temporary necessary evil
RAYTRACER_NAMESPACE::RayTracer * rayTracer;
wxImage * image;
wxPanel * panel;


class RaytracerApp: public wxApp {
  virtual bool OnInit();
};


class RaytracerFrame: public wxFrame {
  public:
    RaytracerFrame(const wxString &title, const wxPoint &position,
                   const wxSize &size);

    void OnChar(wxKeyEvent &event);
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
