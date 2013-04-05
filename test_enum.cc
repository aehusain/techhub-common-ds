#include "enum_class.h"

template class Enum<EnumClass>;

const EnumClass EnumClass::value1(1);
const EnumClass EnumClass::value2(4);

int
main(int argc, char *argv[])
{
    using std::cout;
    using std::endl;

    for (EnumClass::c_iterator itr = Enum<EnumClass>::begin();
         itr != Enum<EnumClass>::end();
         ++itr) {
        cout << (*itr)->get_value() << "\n";
    }

    cout << Enum<EnumClass>::is_valid(5) << endl;

    return 0;
}

