/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - INRIA - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <iostream>

#include "alltypes.hxx"
#include "cosinus.hxx"

ScilabFunction::ReturnValue Cosinus::call(types::Int *in)
{
	std::cout << "cos( in int : ";
	std::cout << in->real_get() ;
	std::cout << " )" << std::endl;
	return ScilabFunction::AllGood;
}

ScilabFunction::ReturnValue Cosinus::call(types::String *in)
{
	std::cout << "cos( in string : ";
	std::cout << in->string_get(0,0);
	std::cout << " )" << std::endl;
	return ScilabFunction::AllGood;
}

ScilabFunction::ReturnValue Cosinus::callout(types::String *out)
{
	std::cout << "cos( out string : ";
	std::cout << out->string_get(0,0);
	std::cout << " )" << std::endl;
	return ScilabFunction::AllGood;
}

ScilabFunction::ReturnValue Cosinus::callUntyped(types::GenericType *in, types::typed_list &out)
{
	int *piVal = NULL;
	types::Int *myInt;
	myInt = new types::Int(1,1, &piVal);
	piVal[0]	= 42;
	std::cout << "Cosinus::callUntyped" << std::endl;
	piVal[0] = 42;
	out.push_back(myInt);
	out.push_back(new types::Double(42));
	out.push_back(new types::String("42"));
	return ScilabFunction::AllGood;
}



ScilabFunction::ReturnValue Cosinus::call(types::typed_list& in, types::typed_list& out)
{
	if (out.size() > 1)
	{
		std::cerr << "[ERROR] Cos must have one or no return value." << std::endl;
		return ScilabFunction::WrongParamNumber;
	}

	if (in.size() == 0 && out.size() == 1
		&& out[0]->isString())
	{
		return callout(out[0]->getAsString());
	}

	if (in.size() == 1 && out.size() == 0
		&& in[0]->isString())
	{
		return call(in[0]->getAsString());
	}
	/*
	if (in.size() == 1 && out.size() == 0
	&& in[0]->isInt())
	{
	return call(in[0]->getAsInt());
	}
	*/
	if (in.size() == 1)
	{
		return callUntyped(in[0], out);
	}


	std::cerr << "[ERROR] Generic Error." << std::endl;
	return ScilabFunction::WrongParamNumber;
}

