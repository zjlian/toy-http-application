#pragma once

#include "http-app/common/macro_utility.h"

#include <cstdlib>
#include <iostream>
#include <system_error>
#include <utility>

#include <boost/optional/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/outcome/result.hpp>

template <typename T>
class MUST_USE Option : public boost::optional<T>
{
public:
    using boost::optional<T>::optional;

    bool IsOk()
    {
        return this->is_initialized();
    }

    /// 获取 Option 存储的值，如果不存在就结束程序
    T &Unwrap() &noexcept
    {
        return this->get();
    }
    const T &Unwrap() const &noexcept
    {
        return this->get();
    }
    T Unwrap() &&noexcept
    {
        return std::move(this->get());
    }

    /// 获取 Option 存储的值，如果不存在就替换传入的默认值
    template <class U>
    T UnwrapOr(U &&v) &noexcept
    {
        if (this->is_initialized())
        {
            return this->get();
        }
        return std::forward<U>(v);
    }
    template <class U>
    T UnwrapOr(U &&v) const &noexcept
    {
        if (this->is_initialized())
        {
            return this->get();
        }
        return std::forward<U>(v);
    }
    template <class U>
    T UnwrapOr(U &&v) &&noexcept
    {
        if (this->is_initialized())
        {
            return std::move(this->get());
        }
        return std::forward<U>(v);
    }
};

constexpr static auto None = boost::none;

template <class R, class S = std::errc,
          class NoValuePolicy =
              boost::outcome_v2::policy::default_policy<R, S, void>>
class MUST_USE Result
    : protected boost::outcome_v2::result<R, S, NoValuePolicy>
{
public:
    using boost::outcome_v2::result<R, S, NoValuePolicy>::result;

    ~Result()
    {
        if (UNLIKELY(!checked_))
        {
            std::cerr << "存在未进行错误处理的 Result 实例" << std::endl;
            abort();
        }
    }

    bool IsOk() const noexcept
    {
        return this->has_value();
    }
    bool IsError() const noexcept
    {
        return this->has_failure();
    }

    /// 直接获取结果，如果结果是错误时，程序结束
    R &Unwrap() &noexcept
    {
        checked_ = true;
        return this->assume_value();
    }
    const R &Unwrap() const &noexcept
    {
        checked_ = true;
        return this->assume_value();
    }
    R &&Unwrap() &&noexcept
    {
        checked_ = true;
        return this->assume_value();
    }
    const R &&Unwrap() const &&noexcept
    {
        checked_ = true;
        return this->assume_value();
    }

    /// 直接获取结果，如果结果是错误时，输出提示并程序结束
    R &Expect(const std::string &error_message) &noexcept
    {
        checked_ = true;
        if (UNLIKELY(this->has_failure()))
        {
            std::cerr << error_message << " : " << this->assume_error() << std::endl;
            abort();
        }
        return this->assume_value();
    }
    const R &Expect(const std::string &error_message) const &noexcept
    {
        checked_ = true;
        if (UNLIKELY(this->has_failure()))
        {
            std::cerr << error_message << " : " << this->assume_error() << std::endl;
            abort();
        }
        return this->assume_value();
    }
    R &&Expect(const std::string &error_message) &&noexcept
    {
        checked_ = true;
        if (UNLIKELY(this->has_failure()))
        {
            std::cerr << error_message << " : " << this->assume_error() << std::endl;
            abort();
        }
        return this->assume_value();
    }
    const R &&Expect(const std::string &error_message) const &&noexcept
    {
        checked_ = true;
        if (UNLIKELY(this->has_failure()))
        {
            std::cerr << error_message << " : " << this->assume_error() << std::endl;
            abort();
        }
        return this->assume_value();
    }

    /// 直接获取错误，如果没有错误时，程序结果
    auto &UnwrapError() &noexcept
    {
        checked_ = true;
        return this->assume_error();
    }
    const auto &UnwrapError() const &noexcept
    {
        checked_ = true;
        return this->assume_error();
    }
    auto &&UnwrapError() &&noexcept
    {
        checked_ = true;
        return this->assume_error();
    }
    const auto &&UnwrapError() const &&noexcept
    {
        checked_ = true;
        return this->assume_error();
    }

private:
    mutable bool checked_{false};
};
