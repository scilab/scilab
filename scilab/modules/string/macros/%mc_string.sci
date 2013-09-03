// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011 - Allan CORNET
// Copyright (C) INRIA - 2011 - Serge STEER
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [vars_out, vars_in, fun_text] = %mc_string(pfunc)

    vars_out = [];
    vars_in = [];
    fun_text = [];

    if type(pfunc) <> 13 then
        error(999, msprintf(gettext("%s: Wrong type for input argument #%d: a function expected.\n"), "string", 1));
    end

    //fun2string referenced to make its subfunctions known there
    prot = funcprot();
    funcprot(0);
    fun2string = fun2string;
    funcprot(prot);
    //get the function pseudo code
    lst = mmodlst(macr2lst(pfunc));

    //get the input and output variable names
    vars_in = lst(3);
    vars_out = lst(2);

    //re-generate the source code from pseudo-code (see fun2string)
    lcount = 1;
    level = [0, 0];
    quote = "''";
    dquote = """";
    CR = "\" + ascii(10);
    opstable();
    sciexp = 0;
    crp = ins2sci(lst, 4);
    fun_text = crp(1:$-2);
    if fun_text(1) == "" then fun_text(1) = " ", end
    if fun_text($) == "" then fun_text($) = " ", end

endfunction
