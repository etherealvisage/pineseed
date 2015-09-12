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

    virtual void queue(const char *event_name, std::experimental::any params) = 0;
    virtual void fire_queued() = 0;
};

class default_proxy {
public:
    template<typename ...T>
    void proxy(std::string name, const std::experimental::any &params) {}
};

template<typename proxy_t = default_proxy>
class proxy_context : public base_context {
private:
    std::multimap<std::string,
        std::function<void (const std::experimental::any &)>> m_listeners;
    std::vector<std::pair<std::string, std::experimental::any>> m_events;
    proxy_t m_proxy;
private:
    template<typename ...T>
    static void invoker(const std::function<void (T...)> &function,
        const std::experimental::any &params) {

        std::tuple<T...> tparams =
            std::experimental::any_cast<decltype(tparams)>(params);

        std::experimental::apply(function, tparams);
    }
public:
    proxy_context() {}
    proxy_context(proxy_t &&proxy) : m_proxy(proxy) {}

    template<typename ...T>
    void add_listener(const char *event_name,
        std::function<void (T...)> function) {

        m_listeners.insert(std::make_pair(std::string(event_name),
            std::bind(invoker<T...>, function, std::placeholders::_1)));
    }

    virtual void queue(const char *event_name, std::experimental::any params) {
        m_events.push_back(std::make_pair(std::string(event_name), params));
    }

    virtual void fire_queued() {
        decltype(m_events) events_copy;
        std::swap(events_copy, m_events);

        for(auto &event : events_copy) {
            auto it = m_listeners.find(event.first);
            while(it != m_listeners.end() && it->first == event.first) {
                it->second(event.second);
                it ++;
            }
        }
    }
};

} // namespace event

#endif
