/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <iostream>
#include <assert.h>
#include "symbol.hxx"
#include "stack.hxx"
#include "types.hxx"
#include "int.hxx"

using symbol::Symbol;
using symbol::Stack;
using types::Int;


void stack_dump(Stack *stack)
{
    std::cout << "----------------" << std::endl;
    std::cout << "  Stack Dump :" << std::endl;
    std::cout << "----------------" << std::endl;
    std::cout << *stack;
    std::cout << "----------------" << std::endl;
    std::cout << std::endl;
}

static int stack_test(void)
{
    // Add "toto"
    Symbol toto ("toto");
    // Add "pouet"
    Symbol pouet ("pouet");

    Stack stack;

    int *piVal1		= NULL;
    int *piVal2		= NULL;
    int *piVal11	= NULL;
    int *piVal22	= NULL;
    int *piVal111	= NULL;

    Int i_1(1, 1, &piVal1);
    piVal1[0] = 1;
    Int i_2(1, 1, &piVal2);
    piVal2[0] = 2;
    Int i_11(1, 1, &piVal11);
    piVal11[0] = 11;
    Int i_22(1, 1, &piVal22);
    piVal22[0] = 22;
    Int i_111(1, 1, &piVal111);
    piVal111[0] = 111;


    /** /{ SCOPE env */
    {
        stack.scope_begin();
        // toto = 1
        stack.put(toto, i_1);
        // pouet = 2
        stack.put(pouet, i_2);

        stack_dump(&stack);
        assert(stack.get(toto) == &i_1);
        assert(stack.get(pouet) == &i_2);

        /** /{ SCOPE env.1 */
        {
            stack.scope_begin();
            // toto = 11
            stack.put(toto, i_11);
            // pouet = 22
            stack.put(pouet, i_22);

            stack_dump(&stack);
            assert(stack.get(toto) == &i_11);
            assert(stack.get(pouet) == &i_22);

            /** /{ SCOPE env.1.1 */
            {
                stack.scope_begin();
                // pouet = 111
                stack.put(pouet, i_111);

                stack_dump(&stack);
                assert(stack.get(pouet) == &i_111);
                assert(stack.get(toto) == &i_11);
                stack.scope_end();
            }
            /** /} ! env.1.1 */
            assert(stack.get(toto) == &i_11);
            assert(stack.get(pouet) == &i_22);
            stack.scope_end ();
        }
        /** /} ! env.1 */
        assert(stack.get(toto) == &i_1);
        assert(stack.get(pouet) == &i_2);
        stack.scope_end ();
    }
    /** /} ! env */
    return 0;
}

int main(void)
{
    stack_test();
    return 0;
}
