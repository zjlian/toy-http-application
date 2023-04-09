#include "http-app/framework/http_application.h"
#include "http-app/framework/controller_base.h"
#include "http-app/framework/model_base.h"
#include "http-app/framework/service_base.h"

#include <cassert>
#include <mutex>
#include <shared_mutex>
#include <utility>

namespace toy
{

    void HttpApplication::Run(int port)
    {
        http_server_.setPort(port);
        http_server_.registerHttpService(&http_router_);
        std::cout << "running ..." << std::endl;
        http_server_.run();
    }

    void HttpApplication::RegisterController(const std::string &name, std::shared_ptr<ControllerBase> controller)
    {
        assert(!name.empty());
        assert(controller);

        controller->RegisterRouter(http_router_);
        controller->SetApplictaion(weak_from_this());
        controller->Injection();
        std::unique_lock<std::shared_mutex> write_lock{mutex};
        controllers_.emplace(name, std::move(controller));
    };

    void HttpApplication::RegisterService(const std::string &name, std::shared_ptr<ServiceBase> service)
    {
        assert(!name.empty());
        assert(service);

        service->SetApplictaion(weak_from_this());
        service->Injection();
        std::unique_lock<std::shared_mutex> write_lock{mutex};
        services_.emplace(name, std::move(service));
    }

    void HttpApplication::RegisterModel(const std::string &name, std::shared_ptr<ModelBase> model)
    {
        assert(!name.empty());
        assert(model);

        model->SetApplictaion(weak_from_this());
        model->Injection();
        std::unique_lock<std::shared_mutex> write_lock{mutex};
        models_.emplace(name, std::move(model));
    }

    void ModuleBase::SetApplictaion(std::weak_ptr<HttpApplication> app)
    {
        assert(!app.expired());
        application_ = std::move(app);
    }

    void ModuleBase::Injection()
    {
    }

} // namespace toy