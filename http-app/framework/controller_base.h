#pragma once

#include "http-app/framework/http_application.h"

#include "hv/HttpService.h"

namespace toy
{

    class ControllerBase : public ModuleBase
    {
    public:
        friend class HttpApplication;

    protected:
        /// 注册路由信息
        virtual void RegisterRouter(hv::HttpService &router) = 0;
    };

} // namespace toy

#define ADD_ROUTER(METHOD, NAME, MEMBER_HANDER)             \
    router.METHOD(NAME, [this](const HttpContextPtr &ctx) { \
        return MEMBER_HANDER(ctx);                          \
    })
