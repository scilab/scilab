// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2021 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function r = %c_b_s(varargin)
    if size(varargin)==3 & type(varargin(3))==10 & ..
        length(ascii(varargin(1)))==1 & length(ascii(varargin(3)))==1
            r = ascii(ascii(varargin(1)):varargin(2):ascii(varargin(3)))
    else
        msg = _("%s: Non implemented feature.\n");
        error(msprintf(msg, "char:num"));
    end
endfunction
//assert_checkequal("a":2:"z", "acegikmoqsuwy");
//assert_checkequal("z":-1:"a", "zyxwvutsrqponmlkjihgfedcba");
//assert_checkequal("z":-2:"a", "zxvtrpnljhfdb");
//assert_checkequal("a":-1:"z", "");
//assert_checkequal("z":2:"a", "");
