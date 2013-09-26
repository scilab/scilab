#include "double.hxx"
#include "types_addition.hxx"

int main()
{
    types::Double* pdbl1 = types::Double::Empty();
    int res = AddDoubleToDouble(pdbl1, pdbl1, &pdbl1);

    return res;
}


