#include "Src\pch.h"
#include "IImGuiWindow.h"

#include "Src\Events\Events.h"
#include "Src\Events\EventDispatcher.h"
#include "Src\Events\EventObserver.h"

namespace Eos
{
    class MousePickingWindow
    {
    public:
        MousePickingWindow();
        void Update();
        EntityID m_SelectedID = NULLENTITY;
        bool SetMousePickingOn{ false };
    };
}