#include "modules\ClientDLL.hpp"
#include "modules\EngineDLL.hpp"
#include "modules\ServerDLL.hpp"
#include "modules\VstdlibDLL.hpp"
#include "modules\vguimatsurfaceDLL.hpp"

EngineDLL engineDLL;
ClientDLL clientDLL;
ServerDLL serverDLL;
VstdlibDLL vstdlibDLL;
#ifndef OE
VGui_MatSurfaceDLL vgui_matsurfaceDLL;
#endif