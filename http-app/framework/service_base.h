#pragma once

#include "http-app/framework/http_application.h"

namespace toy
{

    class ServiceBase : public ModuleBase
    {
    public:
        friend class HttpApplication;
    };

} // namespace toy