/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/

#include <iostream>
#include <vector>


#include "alltypes.hxx"
#include "foo.hxx"
#include "scilabFunction.hxx"
#include "funcmanager.hxx"
#include <sstream>
#include <assert.h>

int main(void)
{
    //FuncManager FM;


    cout << "Start" << endl;

    /*
    	for(int i = 0 ; i < __SIZE_MAP__ ; i++)
    	{

    		types::typed_list in;
    		types::typed_list out;

    		int *piVal = NULL;
    		types::Int *myInt = new types::Int(1,1, &piVal);
    		ostringstream os;

    		in.erase(in.begin(), in.end());
    		in.push_back(myInt);
    		os << "foo" << i+1;
    		string sz(os.str());
    		ScilabFunction *f = FM.m_mapFuncList.find(sz)->second;
    		if(f == NULL)
    			continue;
    		f->call(in, 1, out);

    		for(unsigned int j  = 0 ; j < out.size() ; j++)
    		{
    			if(out[j]->isDouble())
    				assert(in[0]->getAsDouble()->real_get(0,0) == out[j]->getAsDouble()->real_get(0,0));
    			if(out[j]->isInt())
    				assert(in[0]->getAsInt()->real_get(0,0) == out[j]->getAsInt()->real_get(0,0));
    			if(out[j]->isString())
    				assert(in[0]->getAsString()->string_get(0,0) == out[j]->getAsString()->string_get(0,0));
    		}


    		//
    		// [] = foo()
    		//
    		std::cout << "--> foo(empty, empty)" << std::endl;

    		ostringstream os;
    		os << "foo" << i+1;
    		string sz(os.str());
    		ScilabFunction *f = FM.m_mapFuncList.find(sz)->second;
    		f->call(in, out);
    		std::cout << std::endl << std::endl;


    		//
    		// [plop] = foo()
    		//
    		std::cout << "--> foo(empty, [\"plop\"])" << std::endl;
    		types::String *str = new types::String("plop");
    		in.push_back(str);
    		f->call(out, in);

    		std::cout << std::endl << std::endl;

    		//
    		// [] = foo("plop")
    		//
    		std::cout << "--> foo([\"plop\"], empty)" << std::endl;
    		f->call(in, out);

    		std::cout << std::endl << std::endl;

    		//
    		// [] =foo(int32(51))
    		//
    		std::cout << "--> foo([51], empty)" << std::endl;
    		types::Int *value = new types::Int(51);
    		in.erase(in.begin(), in.end());
    		in.push_back(value);
    		f->call(in, out);
    		//out is altered -> add 3 items in the list

    		for(unsigned int i  = 0 ; i < out.size() ; i++)
    		{
    			if(out[i]->isDouble())
    				std::cout << "Double : " << out[i]->getAsDouble()->value_get() << std::endl;
    			if(out[i]->isInt())
    				std::cout << "Int : " << out[i]->getAsInt()->value_get() << std::endl;
    			if(out[i]->isString())
    				std::cout << "String : " << out[i]->getAsString()->value_get() << std::endl;
    		}

    		std::cout << std::endl << std::endl;

    		//
    		// [] =foo(51)
    		//
    		std::cout << "--> foo([51], empty)" << std::endl;
    		types::Double *dblValue = new types::Double(51);
    		in.erase(in.begin(), in.end());
    		in.push_back(dblValue);
    		f->call(in, out);

    		std::cout << std::endl << std::endl;

    		//
    		// [a] =foo(51)
    		//
    		std::cout << "--> foo([51], [a])" << std::endl;
    		in.erase(in.begin(), in.end());
    		out.erase(out.begin(), out.end());
    		in.push_back(value);
    		f->call(in, out);

    	}
    */
    cout << "End" << endl;
    return 0;
}
