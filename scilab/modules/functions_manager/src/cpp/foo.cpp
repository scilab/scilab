/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
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
#include "foo.hxx"

ScilabFunction::ReturnValue Foo::call(types::Int *in)
{
	std::cout << "foo( in int : ";
	std::cout << in->real_get() ;
	std::cout << " )" << std::endl;
	return ScilabFunction::AllGood;
}

ScilabFunction::ReturnValue Foo::call(types::String *in)
{
	std::cout << "foo( in string : ";
	std::cout << in->string_get(0,0);
	std::cout << " )" << std::endl;
	return ScilabFunction::AllGood;
}

ScilabFunction::ReturnValue Foo::callout(int _iRetCount, types::String *out)
{
	std::cout << "foo( out string : ";
	std::cout << out->string_get(0,0);
	std::cout << " )" << std::endl;
	return ScilabFunction::AllGood;
}

ScilabFunction::ReturnValue Foo::callUntyped(types::GenericType *in, int _iRetCount, types::typed_list &out)
{
	if(in[0].isDouble())
		out.push_back(in[0].getAsDouble());
	if(in[0].isString())
		out.push_back(in[0].getAsString());
	if(in[0].isInt())
		out.push_back(in[0].getAsInt());

	return ScilabFunction::AllGood;
}

/*
ReturnValue Foo::foo(types::Type *in1, types::Type *in2)
{
std::cout << "[ERROR] Generic two values call." <<std::endl;
return 1;
}
*/


ScilabFunction::ReturnValue Foo::call(types::typed_list& in, int _iRetCount, types::typed_list& out)
{
	if (out.size() > 1)
	{
		std::cerr << "[ERROR] Foo must have one or no return value." << std::endl;
		return ScilabFunction::WrongParamNumber;
	}

	if (in.size() == 0 && out.size() == 1
		&& out[0]->isString())
	{
		return callout(_iRetCount, out[0]->getAsString());
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
		return callUntyped(in[0], _iRetCount, out);
	}

	/*
	switch (in.size()) {
	case 0 :
	if (out.size() == 1)
	{
	if (out[0]->isString())
	{
	return callout(out[0]->getAsString());
	}
	}
	else
	{
	std::cerr << "[ERROR] Foo must have one or no return value." << std::endl;
	return -1;
	}
	break;
	case 1 :
	if (in[0]->isString())
	{
	return call(in[0]->getAsString());
	}
	if (in[0]->isInt())
	{
	return call(in[0]->getAsInt());
	}
	std::cerr << "[ERROR] Argument must be a String or an Integer." << std::endl;
	return -1;
	break;
	default :
	std::cerr << "[ERROR] Foo takes only one parameter." << std::endl;
	return 1;
	}
	*/
	std::cerr << "[ERROR] Generic Error." << std::endl;
	return ScilabFunction::WrongParamNumber;
}

