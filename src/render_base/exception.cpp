#include "render_base/exception.hpp"

using namespace Render3D;

Exception::Exception(const std::string& msg) {
    message = msg;
}

std::string Exception::what() const {
    return message;
}

std::ostream& Render3D::operator<<(std::ostream& outs, const Exception& e) {
    outs << e.what();
    
    return outs;
}
