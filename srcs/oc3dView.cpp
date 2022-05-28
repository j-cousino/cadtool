#include "oc3dView.h"

#if !defined(_WIN32) && (!defined(__APPLE__) || defined(MACOSX_USE_GLX)) && QT_VERSION < 0x050000
  #include <QX11Info>
#endif
#include <Standard_WarningsRestore.hxx>


#include <Graphic3d_GraphicDriver.hxx>
#include <Graphic3d_TextureEnv.hxx>

#include <AIS_InteractiveContext.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <WNT_Window.hxx>

#include "ocWindow.h"

BEGIN_EVENT_TABLE(oc3dView, wxPanel)
EVT_SIZE(oc3dView::OnSize)
EVT_IDLE(oc3dView::OnIdle)
END_EVENT_TABLE()

oc3dView::oc3dView( Handle(AIS_InteractiveContext) theContext, wxWindow* parent )
: wxPanel( parent )
{
    
#if !defined(_WIN32) && (!defined(__APPLE__) || defined(MACOSX_USE_GLX)) && QT_VERSION < 0x050000
    XSynchronize(x11Info().display(),true);
#endif
    myContext = theContext;
    init();

    myView->ZBufferTriedronSetup(Quantity_NOC_RED,Quantity_NOC_GREEN,Quantity_NOC_BLUE1,0.8,0.05,12);
    myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER,Quantity_NOC_WHITE,0.1,V3d_ZBUFFER);

}

oc3dView::~oc3dView(){}

void oc3dView::init()
{
    if ( myView.IsNull() )
        myView = myContext->CurrentViewer()->CreateView();

    Handle(WNT_Window) hWnd = new WNT_Window(this->GetHandle());
    myView->SetWindow (hWnd);
    
    if ( !hWnd->IsMapped() ){
        hWnd->Map();
    }
  
    myView->SetBackgroundColor (Quantity_NOC_BLACK);
    myView->MustBeResized();

}

void oc3dView::OnDraw(wxDC& dc)
{
    // myView->Redraw();
    myView->InvalidateImmediate();
}

void oc3dView::OnIdle(wxIdleEvent& event)
{
    myView->MustBeResized();
    myView->Redraw();
}

void oc3dView::OnSize(wxSizeEvent& event)
{
    if(!myView.IsNull())
        myView->MustBeResized(); // if the user changes the window size
    // this will re-center the Cascade View
    myView->Redraw();
    event.Skip();
}


