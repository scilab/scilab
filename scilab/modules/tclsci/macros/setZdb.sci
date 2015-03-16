// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


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
