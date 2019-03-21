#include "./src/config/traits.hpp"

#include <iostream>

namespace util
{
    class Vector
    {
    public:
        Vector() = default;
        
        template <typename ... Value>
        Vector(Value && ... values) :
            _vector{static_cast<double>(values)...}
        {

        }
        
        ~Vector() = default;

    // private:
        double _vector[Traits<Vector>::dimension]{0};
    };
} // util

using namespace util;

int main()
{
    Vector v1, v2(1), v3(2, 3), v4(4, 5, 6);

    std::cout << "V1 = [" << v1._vector[0] << ", " << v1._vector[1] << ", " << v1._vector[2] << "]\n";
    std::cout << "V2 = [" << v2._vector[0] << ", " << v2._vector[1] << ", " << v2._vector[2] << "]\n";
    std::cout << "V3 = [" << v3._vector[0] << ", " << v3._vector[1] << ", " << v3._vector[2] << "]\n";
    std::cout << "V4 = [" << v4._vector[0] << ", " << v4._vector[1] << ", " << v4._vector[2] << "]\n";
}
