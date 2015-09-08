#ifndef EVENT_CONTEXT_HPP
#define EVENT_CONTEXT_HPP

#include <functional>
#include <map>
#include <experimental/any>
#include <experimental/tuple>
#include <tuple>
#include <vector>

namespace event {

class context {
private:
    std::multimap<std::string, std::experimental::any> m_listeners;
    std::vector<std::pair<std::string, std::function<void ()>>> m_events;
public:
    context();
    ~context();

    template<typename ...T>
    void add_listener(const char *event_name,
        std::function<void (T...)> function) {

        m_listeners.insert(std::make_pair(std::string(event_name), function));
    }

    template<typename ...T>
    void invoke_all(std::string name, std::tuple<T...> params) {
        auto it = m_listeners.find(name);
        while(it != m_listeners.end() && it->first == name) {
            auto f = std::experimental::any_cast<std::function<void (T...)>>(
                it->second);
            std::experimental::apply(f, params);
            it ++;
        }
    }

    template<typename ...T>
    void queue(const char *event_name, std::tuple<T...> params) {
        m_events.push_back(std::make_pair(std::string(event_name),
            std::bind(&context::invoke_all(event_name, params))));
    }

    void fire_queued();
};

} // namespace event

#endif
