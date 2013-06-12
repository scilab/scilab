#include <stack-c.h>
#include <string.h>

int sci_create_list(char * fname)
{
    int m_list_out, n_list_out;
    int m_var1,     n_var1,     l_var1,  l_list_var1;
    int m_var2,     n_var2,     l_var2,  l_list_var2;
    int m_mlist,    n_mlist,    l_mlist;

    // The labels of our mlist
    static const char * ListLabels [] = {"mylist", "var1", "var2"};

    // First, we create the variables using a classical way
    // The size of the Scilab variables
    m_var1  = 1;
    n_var1  = strlen("a string") + 1; // a null terminated string
    m_var2  = 2;
    n_var2  = 2; // A 2x2 double matrix
    m_mlist = 3;
    n_mlist = 1; // A mlist with 3 elements

    // Creation of the Scilab variables
    // A('var1')
    CreateVar(1, "c", &m_var1,  &n_var1,  &l_var1);
    // A('var2')
    CreateVar(2, "d", &m_var2,  &n_var2,  &l_var2);
    // A
    CreateVar(3, "m", &m_mlist, &n_mlist, &l_mlist);

    // We store values in the create variables
    // The matrix will be stored in A('var2')
    *stk(l_var2 + 0) = 1;
    *stk(l_var2 + 1) = 2;
    *stk(l_var2 + 2) = 3;
    *stk(l_var2 + 3) = 4;

    // The string will be stored in A('var1')
    strncpy(cstk(l_var1), "a string\0", n_var1);

    m_list_out = 3;
    n_list_out = 1;

    // now, affect the variable  to the mlist
    // The labels (it corresponds to A = mlist(['mylist','var1','var2'], ...
    CreateListVarFromPtr(3, 1, "S", &m_list_out, &n_list_out, ListLabels);
    // The value stored in A('var1') (it corresponds to A = ...,'a string', ...
    CreateListVarFrom(3, 2, "c", &m_var1, &n_var1, &l_list_var1, &l_var1);
    // The value stored in A('var2') (it corresponds to A = ...,[1 2,3 4]);
    CreateListVarFrom(3, 3, "d", &m_var2, &n_var2, &l_list_var2, &l_var2);

    // We return only the mlist which has been created at position 3
    LhsVar(1) = 3;

    return 0;
}
