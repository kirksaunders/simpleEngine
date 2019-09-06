#ifndef ALWAYSFALSE_HPP
#define ALWAYSFALSE_HPP

namespace Render3D {
    template <class T>
    struct always_false { 
        static const bool value = false;
    };
};

#endif