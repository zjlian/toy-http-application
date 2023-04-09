#include "controller/hello.h"
#include "http-app/framework/http_application.h"
#include "model/test.h"
#include "service/memcache.h"

#include <iostream>
#include <memory>

/// 注册服务组件
void Register(toy::HttpApplication &app)
{
    using namespace toy;
    // 需要注意组件的依赖顺序，从依赖最底层的组件开始注册
    // 组件之间禁止出现循环依赖
    app.RegisterModule<MemcacheService>("memcache");
    app.RegisterModule<TestModel>("test");
    app.RegisterModule<HelloController>("hello");
}

int main(int, char **)
{
    std::cout << "Hello, world!\n";

    // 创建 http 服务程序
    auto app = std::make_shared<toy::HttpApplication>();
    Register(*app);
    app->Run(8080);
}
