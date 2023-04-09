#pragma once

#include "http-app/common/result.h"

#include <map>
#include <string>
#include <utility>

#include "http-app/framework/service_base.h"

namespace toy
{

    class MemcacheService : public ServiceBase
    {
    public:
        MemcacheService()
        {
            data_["content"] = "hello world !!!";
        }

        void Set(const std::string &key, const std::string &value)
        {
            data_.insert(std::make_pair(key, value));
        }

        Option<std::string> Get(const std::string &key)
        {
            auto iterator = data_.find(key);
            if (iterator == data_.end())
            {
                return None;
            }
            return iterator->second;
        }

    private:
        std::map<std::string, std::string> data_;
    };

} // namespace toy