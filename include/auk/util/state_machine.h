#pragma once 

#include <functional>
#include <unordered_map>
#include <iostream>

namespace auk
{
    template<typename T>
    class StateMachine 
    {
        size_t last = 0;
        size_t state = 0;

        using FType = std::function<void(T*, StateMachine*)>;

        struct State
        {
            FType begin = nullptr;
            FType update = nullptr;
            FType end = nullptr;
        };

        std::unordered_map<size_t, State> states;
        
    public:

        StateMachine() = default;

        void add(size_t id, FType begin = nullptr, FType update = nullptr, FType end = nullptr)
        {
            states[id] = { begin, update, end };
        }

        void set(size_t id, T* e)
        {
            if (id != state)
            {
                last = state;
                state = id;

                if (states[last].end)
                    states[last].end(e, this);

                if (states[state].begin)
                    states[state].begin(e, this);
            }
        }

        void update(T* e)
        {
            if (states[state].update)
                states[state].update(e, this);
        }

        size_t get_state() { return state; }
        size_t get_last() { return last; }
        
    };
}
