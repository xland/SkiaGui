#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
#include <string>
#include <any>
#include <atomic>
#include <algorithm>

class Eventer {
public:
    using HandlerId = size_t;

    template <typename Func>
    HandlerId on(const std::string& name, Func&& handler) {
        auto id = nextId++;
        using DecayedFunc = std::decay_t<Func>;
        handlers[name].emplace_back(id, std::function(std::forward<Func>(handler)));
        return id;
    }

    template <typename... Args>
    void emit(const std::string& name, Args&&... args) {
        if (handlers.contains(name)) {
            for (auto& [id, handlerAny] : handlers[name]) {
                if (auto* handler = std::any_cast<std::function<void(Args...)>>(&handlerAny)) {
                    (*handler)(std::forward<Args>(args)...);
                }
            }
        }
    }

    void off(const std::string& name, const HandlerId& handle) {
        if (handlers.contains(name)) {
            auto& vec = handlers[name];
            vec.erase(std::remove_if(vec.begin(), vec.end(),
                [&](const auto& pair) { return pair.first == handle; }),
                vec.end());
            if (vec.empty()) {
                handlers.erase(name);
            }
        }
    }

private:
    std::unordered_map<std::string, std::vector<std::pair<HandlerId, std::any>>> handlers;
    std::atomic<HandlerId> nextId{ 0 };
};