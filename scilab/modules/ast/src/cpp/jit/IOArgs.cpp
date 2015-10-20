/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

extern "C"
{
    void * get_in_list()
    {
	return new types::typed_list();
    }

    void * get_out_list()
    {
	return new types::typed_list();
    }

    void delete_list(void * list)
    {
	delete reinterpret_cast<types::typed_list *>(list);
    }

    void push_Sd(void * list, const double x)
    {
	jit::push<double, types::Double>(list, x);
    }

    void push_Md(void * list, const double * x, const int64_t x_r, const int64_t x_c, )
    {
	jit::push<double, types::Double>(list, x);
    }
}
