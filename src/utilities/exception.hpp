#ifndef RENDER3D_EXCEPTION_HPP
#define RENDER3D_EXCEPTION_HPP

#include <iostream>

namespace Render3D {
    class Exception {
     public:
        Exception(const std::string& msg = "");

        std::string what() const;

     private:
        std::string message;
    };

    std::ostream& operator<<(std::ostream& outs, const Exception& e);
};

#endif