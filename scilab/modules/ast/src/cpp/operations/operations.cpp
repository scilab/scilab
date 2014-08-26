// operations.cpp : Defines the exported functions for the DLL application.
//

#include "operations.hxx"
#include "types_addition.hxx"
#include "types_subtraction.hxx"
#include "types_opposite.hxx"

void initOperationArray()
{
    fillAddFunction();
    fillOppositeFunction();
    fillSubtractFunction();
}
