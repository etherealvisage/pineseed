#ifndef EVENT_CONTEXT_HPP
#define EVENT_CONTEXT_HPP

#include <iostream> // debugging

#include <functional>
#include <map>
#include <experimental/any>
#include <experimental/tuple>
#include <tuple>
#include <vector>

namespace event {


class base_context {
public:
    virtual ~base_context() {}

    virtual void fire_queued() = 0;
};

class default_proxy {
public:
    template<typename ...T>
    void proxy(std::string name, const std::tuple<T...> &params) {}
};

template<typename proxy_t = default_proxy>
class proxy_context : public base_context {
private:
    std::multimap<std::string, std::experimental::any> m_listeners;
    std::vector<std::function<void ()>> m_events;
    proxy_t m_proxy;
public:
    proxy_context() {}
    proxy_context(proxy_t &&proxy) : m_proxy(proxy) {}

    template<typename ...T>
    void add_listener(const char *event_name,
        std::function<void (T...)> function) {

        m_listeners.insert(std::make_pair(std::string(event_name), function));
    }

    template<typename ...T>
    void invoke_all(std::string name, std::experimental::any params) {
        std::tuple<T...> tparams
            = std::experimental::any_cast<decltype(tparams)>(params);

        auto it = m_listeners.find(name);
        while(it != m_listeners.end() && it->first == name) {
            auto f = std::experimental::any_cast<std::function<void (T...)>>(
                it->second);
            std::experimental::apply(f, tparams);
            it ++;
        }

        m_proxy.proxy(name, tparams);
    }

    template<typename ...T>
    void queue(const char *event_name, std::tuple<T...> params) {
        m_events.push_back(std::bind(&proxy_context<proxy_t>::invoke_all<T...>,
            this, std::string(event_name), params));
    }

    virtual void fire_queued() {
        decltype(m_events) events_copy;
        std::swap(events_copy, m_events);

        for(auto &event : events_copy) {
            event();
        }
    }
};

using context = proxy_context<>;

} // namespace event

#endif
