#pragma once

#include "http-app/framework/http_application.h"

namespace toy
{

    class ModelBase : public ModuleBase
    {
    public:
        friend class HttpApplication;
    };

} // namespace toy