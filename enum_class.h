#include "enum.h"

class EnumClass: public Enum<EnumClass>
{
    protected:
        explicit EnumClass(int value):
            Enum<EnumClass>(value) { }
    public:
        static const EnumClass value1;
        static const EnumClass value2;
};

template<typename T>
typename Enum<T>::instances Enum<T>::s_instances;
