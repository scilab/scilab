//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2013-2013 - Scilab Enterprises - Bruno JOFRET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
//

function h=%h_copy(h,ax)
    filename = tempname();
    save(filename,"h")
    if argn(2)<2 then
        load(filename)
    else
        if ax.type=="Axes"  then
            a=get("current_axes")
            set("current_axes", ax)
            load(filename)
            set("current_axes", a)
        else
            a = gcf()
            scf(ax)
            load(filename)
            scf(a)
        end
    end
    mdelete(filename);
endfunction
