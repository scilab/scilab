
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Ghislain HELIOT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function []=demo_function_choice()
    while %t then
        num=x_choose(demolist(:,1),"Choose a demo");
        if num==0 then
            xdel(0);
            lines(oldln(1));
            return;
        else
            xdel(0);
            clf();
            execstr(demolist(num,2));
        end,
    end
    mode(0);
endfunction
