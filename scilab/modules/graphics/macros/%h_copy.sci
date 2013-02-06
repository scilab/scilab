//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2013-2013 - Scilab Enterprises - Bruno JOFRET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

function h=%h_copy(h,ax)
    filename = tempname();
    save(filename,'h')
    if argn(2)<2 then
        load(filename)
    else
        if ax.type=="Axes"  then
            a=get('current_axes')
            set('current_axes', ax)
            load(filename)
            set('current_axes', a)
        else
            a = gcf()
            scf(ax)
            load(filename)
            scf(a)
        end
    end
    mdelete(filename);
endfunction
