#ifndef SCOPEGUARD_HPP
#define SCOPEGUARD_HPP

#include <utility>

namespace Render3D {
    template <class Callable>
    class ScopeGuard {
     public:
        ScopeGuard() = delete;
        ScopeGuard(Callable&& c) : rollback(std::forward<Callable>(c)), committed(false) {}
        ScopeGuard& operator=(const ScopeGuard&) = delete;

        ~ScopeGuard() {
            if (!committed) {
                rollback();
            }
        }

        void commit() { committed = true; }
     private:
        Callable rollback;
        bool committed;
    };

    template <class Callable>
    ScopeGuard<Callable> makeScopeGuard(Callable&& c) {
        return ScopeGuard<Callable>(std::forward<Callable>(c));
    }
}

#endif