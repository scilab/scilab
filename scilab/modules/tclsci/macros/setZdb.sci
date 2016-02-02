// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function setZdb(zmin, zmax)
    global ged_handle; h=ged_handle
    select h.view
    case "2d"
        drawlater();
        h.view="3d";
        tmp=h.data_bounds;
        tmp(1,3)=zmin;
        tmp(2,3)=zmax;
        h.data_bounds=tmp;
        h.view="2d";
        drawnow();
        //      tst=execstr('h.data_bounds=tmp','errcatch','n');
        //      h.view='2d';
        //      if tst<>0 then
        //       disp 'Warning: Z data_bounds must contain double'
        //      end
    case "3d"
        tmp=h.data_bounds;
        tmp(1,3)=zmin;
        tmp(2,3)=zmax;
        h.data_bounds=tmp;
        //      tst=execstr('h.data_bounds=tmp','errcatch','n');
        //      if tst<>0 then
        //        disp 'Warning: Z data_bounds must contain double'
        //      end
    end
endfunction
