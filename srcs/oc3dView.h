#ifndef __OC3DVIEW_H__
#define __OC3DVIEW_H__

#include <wx/wx.h>

#include <AIS_ViewController.hxx>
#include <V3d_View.hxx>

class oc3dView : public wxPanel
{
public:
    oc3dView(Handle(AIS_InteractiveContext) theContext, wxWindow *parent);
    ~oc3dView();

    virtual void init();

    virtual void OnDraw(wxDC &dc);
    void OnSize(wxSizeEvent& event);
    void OnPaint(wxPaintEvent& event);
    void OnIdle(wxIdleEvent& event);

protected:
    Handle(V3d_View)& getView();
    Handle(AIS_InteractiveContext)& getContext();


private:    
    Handle(V3d_View) myView;
    Handle(AIS_InteractiveContext) myContext;

    DECLARE_EVENT_TABLE();
};

#endif // __OC3DVIEW_H__

