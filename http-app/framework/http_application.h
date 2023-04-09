#pragma once

#include "http-app/common/macro_utility.h"
#include "http-app/common/result.h"

#include <cassert>
#include <cstdlib>
#include <memory>
#include <shared_mutex>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "hv/HttpServer.h"
#include "hv/HttpService.h"

namespace toy
{
    class ModuleBase;
    class ControllerBase;
    class ServiceBase;
    class ModelBase;

    class HttpApplication : public std::enable_shared_from_this<HttpApplication>
    {
    public:
        DISABLE_COPY_AND_MOVE(HttpApplication);

        HttpApplication() = default;

        /// 启动 HTTP 服务
        void Run(int port);

        /// 注册全局唯一的 controller service model 组件
        void RegisterController(const std::string &name,
                                std::shared_ptr<ControllerBase> controller);
        void RegisterService(const std::string &name,
                             std::shared_ptr<ServiceBase> service);
        void RegisterModel(const std::string &name, std::shared_ptr<ModelBase> model);

        /// 便捷组件注册
        template <typename ModuleType>
        void RegisterModule(const std::string &name)
        {
            static_assert(std::is_base_of_v<ModuleBase, ModuleType>);

            if constexpr (std::is_base_of_v<ControllerBase, ModuleType>)
            {
                std::cout << "注册 Controller 组件 " << name << std::endl;
                RegisterController(name, std::make_shared<ModuleType>());
            }
            else if constexpr (std::is_base_of_v<ServiceBase, ModuleType>)
            {
                std::cout << "注册 Service 组件 " << name << std::endl;
                RegisterService(name, std::make_shared<ModuleType>());
            }
            else if constexpr (std::is_base_of_v<ModelBase, ModuleType>)
            {
                std::cout << "注册 Model 组件 " << name << std::endl;
                RegisterModel(name, std::make_shared<ModuleType>());
            }
            else
            {
                abort();
            }
        }

        /// 获取全局唯一的 controller 实例
        template <typename ControllerType>
        Option<std::shared_ptr<ControllerType>>
        GetController(const std::string &name)
        {
            assert(!name.empty());

            std::shared_lock<std::shared_mutex> read_lock{mutex};
            auto iterator = controllers_.find(name);
            if (iterator == controllers_.end())
            {
                return None;
            }

            auto resutl = std::dynamic_pointer_cast<ControllerType>(iterator->second);
            if (!resutl)
            {
                return None;
            }

            return resutl;
        }

        /// 获取全局唯一的 service 实例
        template <typename ServiceType>
        Option<std::shared_ptr<ServiceType>> GetService(const std::string &name)
        {
            assert(!name.empty());

            std::shared_lock<std::shared_mutex> read_lock{mutex};
            auto iterator = services_.find(name);
            if (iterator == services_.end())
            {
                return None;
            }

            auto resutl = std::dynamic_pointer_cast<ServiceType>(iterator->second);
            if (!resutl)
            {
                return None;
            }

            return resutl;
        }

        /// 获取全局唯一的 model 实例
        template <typename ModelType>
        Option<std::shared_ptr<ModelType>> GetModel(const std::string &name)
        {
            assert(!name.empty());

            std::shared_lock<std::shared_mutex> read_lock{mutex};
            auto iterator = models_.find(name);
            if (iterator == models_.end())
            {
                return None;
            }

            auto resutl = std::dynamic_pointer_cast<ModelType>(iterator->second);
            if (!resutl)
            {
                return None;
            }

            return resutl;
        }

    private:
        std::shared_mutex mutex;
        std::unordered_map<std::string, std::shared_ptr<ControllerBase>> controllers_;
        std::unordered_map<std::string, std::shared_ptr<ServiceBase>> services_;
        std::unordered_map<std::string, std::shared_ptr<ModelBase>> models_;

        hv::HttpService http_router_;
        hv::HttpServer http_server_;
    };

    class ModuleBase
    {
    public:
        DISABLE_COPY(ModuleBase);
        ModuleBase() = default;
        friend class HttpApplication;

    protected:
        void SetApplictaion(std::weak_ptr<HttpApplication> app);

        /// 半自动注入依赖组件的虚函数接口
        virtual void Injection();

        template <typename ControllerType>
        Option<std::shared_ptr<ControllerType>>
        GetController(const std::string &name)
        {
            assert(!name.empty());

            auto app = application_.lock();
            if (app)
            {
                return app->GetController<ControllerType>(name);
            }
            return None;
        }

        template <typename ServiceType>
        Option<std::shared_ptr<ServiceType>> GetService(const std::string &name)
        {
            assert(!name.empty());

            auto app = application_.lock();
            if (app)
            {
                return app->GetService<ServiceType>(name);
            }
            return None;
        }

        template <typename ModelType>
        Option<std::shared_ptr<ModelType>> GetModel(const std::string &name)
        {
            assert(!name.empty());

            auto app = application_.lock();
            if (app)
            {
                return app->GetModel<ModelType>(name);
            }
            return None;
        }

        /// 获取任意类型的组件
        template <typename AnyModulePtr>
        void LoadModule(AnyModulePtr &ptr, const std::string &name)
        {
            using ModuleType = typename AnyModulePtr::element_type;
            static_assert(std::is_base_of_v<ModuleBase, ModuleType>);
            assert(ptr == nullptr);

            if constexpr (std::is_base_of_v<ControllerBase, ModuleType>)
            {
                ptr = GetController<ModuleType>(name).Unwrap();
            }
            else if constexpr (std::is_base_of_v<ServiceBase, ModuleType>)
            {
                ptr = GetService<ModuleType>(name).Unwrap();
            }
            else if constexpr (std::is_base_of_v<ModelBase, ModuleType>)
            {
                ptr = GetModel<ModuleType>(name).Unwrap();
            }
            else
            {
                /// TODO: 生成详细的错误信息或编译时报错
                abort();
            }
        }

    private:
        std::weak_ptr<HttpApplication> application_;
    };

} // namespace toy

#define ROUTER void RegisterRouter(hv::HttpService &router) override

#define INJECTION void Injection() override
