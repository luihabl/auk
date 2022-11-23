#include <auk.h>
#include <unordered_map>

namespace auk
{
    
    template <typename T>
    class TypeTable
    {
    public:
        TypeTable() = default;
        ~TypeTable()
        {
            clear();
        }

        void add(T* c)
        {
            table[c->type].push_back(c);
            n++;
        }

        void release(T* c)
        {
            auto& vec = table[c->type]; 

            for (size_t i = 0; i < vec.size(); i++)
            {
                if (vec[i] == (T*)c)
                {
                    vec[i] = vec.back();
                    vec.pop_back();
                    n--;

                    return;
                }
            }
        }

        template <typename G>
        G* pop()
        {
            auto& vec = table[auk::Type::of<G>()];
            if (vec.size() > 0)
            {
                G* c = (G*) vec.back();
                vec.pop_back();
                n--;

                return c;
            }

            return nullptr;
        }
        
        void erase(T* c)
        {
            auto& vec = table[c->type]; // <---- maybe change to auk::Type::of<G>()?

            for (size_t i = 0; i < vec.size(); i++)
            {
                if (vec[i] == (T*) c)
                {
                    vec[i] = vec.back();
                    vec.pop_back();
                    n--;

                    delete c;
                    return;
                }
            }
        }

        void clear()
        {
            for (auto [id, items] : table)
                for (auto item : items)
                    delete item;

            table.clear();
        }

        void release_all()
        {
            table.clear();
        }

        template <typename G>
        const std::vector<T*>& get_group()
        {
            return table[auk::Type::of<G>()];
        }

        template <typename G>
        size_t count()
        {
            return table[auk::Type::of<G>()].size();
        }

        size_t count(T* example)
        {
            return table[example->type].size();
        }

        auto begin()
        {
            return table.begin();
        }

        auto begin() const
        {
            return table.begin();
        }

        auto end()
        {
            return table.end();
        }

        auto end() const
        {
            return table.end();
        }

        size_t size()
        {
            return n;
        }

    private:
        std::unordered_map<size_t, std::vector<T*>> table;
        size_t n = 0;

    };
}