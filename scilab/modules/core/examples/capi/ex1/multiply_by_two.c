#include <stack-c.h>

int sci_multiply_by_two(char * fname)
{
    int m_in_var, n_in_var, l_in_var;
    int m_out_var, n_out_var, l_out_var;
    int i_row, j_col;

    // First, access to the input variable (a matrix of doubles)
    GetRhsVar(1, "d", &m_in_var, &n_in_var, &l_in_var);

    // Create the returned variable (a matrix of doubles)
    m_out_var = m_in_var;
    n_out_var = n_in_var;
    CreateVar(2, "d", &m_out_var, &n_out_var, &l_out_var);

    // Perform some simple operations on the matrix
    for (i_row = 0; i_row < m_in_var; i_row++)
    {
        for (j_col = 0; j_col < n_in_var; j_col++)
        {
            *stk(l_out_var + i_row + j_col * m_out_var) = 2 * (*stk(l_in_var + i_row + j_col * m_in_var));
        }
    }

    // Return the output variable
    LhsVar(1) = 2;

    return 0;
}
