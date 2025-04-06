#include "MousePicking.h"

Eos::MousePickingWindow::MousePickingWindow() :
    SetMousePickingOn(false)
{
    

    //
    //  Get the flag from imguiManager
    //
    Eos::PE_SUBSCRIBE_EVENT
    (
        Eos::SetPickingEvent,
        [this](Eos::SetPickingEvent& _evn)
        {
            SetMousePickingOn = _evn.togglePicking;
        }
    );
}

void Eos::MousePickingWindow::Update()
{

}
