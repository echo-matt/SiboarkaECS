#pragma once

#include <unordered_map>
#include <typeindex>
#include <any>
#include <vector>

class EventBus
{
public:
    template<typename T>
    void emit(T event)
    {
        auto key = std::type_index(typeid(T));
        if (m_events.find(key) == m_events.end())
        {
            m_events[key] = std::vector<T>{};
        }
        std::any_cast<std::vector<T>&>(m_events[key]).push_back(std::move(event));
    }
    
    template<typename T>
    const std::vector<T>& getEvents() const
    {
        //Get the key for the events vector to return, i.e. return a vector of events of type <PlayerDied>
        auto key = std::type_index(typeid(T));
        
        //Iterator
        auto it = m_events.find(key);
        
        //If said vector does not exists
        if (it == m_events.end())
        {
            //create and return an empty vector of type <PlayerDied>
            static const std::vector<T> empty{};
            return empty;
        }
        
        //If it exists, return a reference to it
        //I do not know the type of the vector so we use an any_cast of type std::vector<T>, which in this case is <PlayerDied>
        return std::any_cast<const std::vector<T>&>(m_events.at(key));
    }

    void clear()
    {
        m_events.clear();
    }

private:
    std::unordered_map<std::type_index, std::any> m_events;
};