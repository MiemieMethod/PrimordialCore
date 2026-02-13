#include <algorithm>
#include <any>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

namespace primordial_core {

template <typename T>
struct function_traits;

template <typename R, typename... Args>
struct function_traits<R (*)(Args...)> {
    using args_type = std::tuple<Args...>;
};

template <typename R, typename... Args>
struct function_traits<std::function<R(Args...)>> {
    using args_type = std::tuple<Args...>;
};

template <typename C, typename R, typename... Args>
struct function_traits<R (C::*)(Args...) const> {
    using args_type = std::tuple<Args...>;
};

template <typename T>
struct function_traits : public function_traits<decltype(&T::operator())> {};

class CallbackManager {
    template <typename... Args>
    struct CallbackList {
        struct Entry {
            int                          priority;
            std::function<void(Args...)> func;
            uint64_t                     id;

            static bool compare(const Entry& a, const Entry& b) {
                return a.priority > b.priority;
            }
        };
        std::vector<Entry> list;

        void add(int p, std::function<void(Args...)> f, uint64_t id) {
            list.push_back({p, std::move(f), id});
            std::stable_sort(list.begin(), list.end(), Entry::compare);
        }

        void invoke(Args... args) const {
            for (const auto& e : list) {
                e.func(args...);
            }
        }
    };

    std::map<std::string, std::any> events;
    mutable std::shared_mutex       mutex;
    uint64_t                        next_id = 0;

    template <typename... Args>
    void register_impl(const std::string& name, std::function<void(Args...)> f, int p) {
        std::unique_lock lock(mutex);
        using ListT = CallbackList<Args...>;

        if (events.find(name) == events.end()) {
            events[name] = ListT{};
        }

        try {
            auto& container = std::any_cast<ListT&>(events[name]);
            container.add(p, std::move(f), ++next_id);
        } catch (const std::bad_any_cast&) {
            throw std::runtime_error("Signature mismatch for event: " + name);
        }
    }

    template <typename Callable, typename... Args>
    void deduce_and_register(const std::string& name, Callable&& c, int p, std::tuple<Args...>) {
        std::function<void(Args...)> f = std::forward<Callable>(c);
        register_impl<Args...>(name, std::move(f), p);
    }

public:
    template <typename Callable>
    void addCallback(const std::string& name, Callable&& cb, int priority = 0) {
        using traits = function_traits<std::decay_t<Callable>>;
        using ArgsT  = typename traits::args_type;
        deduce_and_register(name, std::forward<Callable>(cb), priority, ArgsT{});
    }

    template <typename... Args>
    void invokeCallback(const std::string& name, Args&&... args) {
        std::shared_lock lock(mutex);
        auto             it = events.find(name);
        if (it == events.end()) {
            std::cerr << "Warning: Event '" << name << "' not found\n";
            return;
        }

        using ListT = CallbackList<std::decay_t<Args>...>;

        try {
            const auto& container = std::any_cast<const ListT&>(it->second);
            container.invoke(std::forward<Args>(args)...);
        } catch (const std::bad_any_cast&) {
            std::cerr << "Error: Invoke arguments do not match stored signature for '" << name << "'\n";
        }
    }
};

} // namespace primordial_core
