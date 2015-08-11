// operations.cpp : Defines the exported functions for the DLL application.
//

#include "operations.hxx"
#include "types_addition.hxx"
#include "types_subtraction.hxx"
#include "types_opposite.hxx"
#include "types_dotmultiplication.hxx"
#include "types_dotdivide.hxx"
#include "types_and.hxx"
#include "types_or.hxx"
#include "types_comparison_eq.hxx"
#include "types_comparison_ne.hxx"

void initOperationArray()
{
    fillAddFunction();
    fillOppositeFunction();
    fillSubtractFunction();
    fillDotMulFunction();
    fillDotDivFunction();
    fillAndFunction();
    fillOrFunction();
    fillComparisonEqualFunction();
    fillComparisonNoEqualFunction();
}
