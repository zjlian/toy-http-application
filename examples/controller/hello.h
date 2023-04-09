#pragma once

#include "model/test.h"
#include "service/memcache.h"

#include <functional>
#include <memory>

#include "http-app/framework/controller_base.h"

namespace toy
{

    /// 控制器编写实例，当网页请求 /index 时，会显示 hello world !!!
    class HelloController : public ControllerBase
    {
        /// 注册 url 对应的处理函数
        ROUTER
        {
            // 注册路径 http://localhost:port/index 的处理函数为 Hello
            ADD_ROUTER(GET, "/index", Hello);
        }

        /// 自动注入组件
        INJECTION
        {
            LoadModule(cache_, "memcache");
            LoadModule(test_model_, "test");
        }

    public:
        int Hello(const HttpContextPtr &ctx)
        {
            // 给请求回复一个 hello world !!!
            // 从缓存组件 cache_ 中读取 "hello world !!!" 文本
            return ctx->sendString(cache_->Get("content").UnwrapOr("内容不存在"));
        }

    private:
        std::shared_ptr<MemcacheService> cache_;
        std::shared_ptr<TestModel> test_model_;
    };

} // namespace toy
