#include <stack-c.h>
#include <sciprint.h>

int sci_print_list(char * fname)
{
    int m_list_in, n_list_in, l_list_in;
    int m_type,    n_type;
    int m_var1,    n_var1,    l_var1;
    int m_var2,    n_var2,    l_var2;
    char ** LabelList = NULL;

    CheckRhs(1, 1); // We accept only 1 parameter

    GetRhsVar(1, "m", &m_list_in, &n_list_in, &l_list_in); // Get a mlist

    // Get the type and the name of the variables (the first element of the mlist)
    GetListRhsVar(1, 1, "S", &m_type, &n_type, &LabelList);

    if (strcmp(LabelList[0], "mytype") != 0)
    {
        sciprint("error, you must ship a mlist or type mytype\n");
        return 0;
    }

    // Get the first variable (a string)
    GetListRhsVar(1, 2, "c", &m_var1, &n_var1, &l_var1);
    sciprint("var1 = %s\n", cstk(l_var1));

    // Get the second variable (a double matrix)
    GetListRhsVar(1, 3, "d", &m_var2, &n_var2, &l_var2);
    sciprint("var2 = [%f %f %f %f]\n", *stk(l_var2 + 0),
             *stk(l_var2 + 1),
             *stk(l_var2 + 2),
             *stk(l_var2 + 3));

    return 0;
}
