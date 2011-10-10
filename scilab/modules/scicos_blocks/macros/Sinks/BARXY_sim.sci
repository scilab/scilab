//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

function block=BARXY_sim(block,flag)
//disp("Calling BARXY_sim with flag = "+string(flag))
    if flag == 4 | flag == 6
// Initialisation || Re-Init
// if already exists (stopped) then reuse
        f = findobj("Tag", block.label);
        if f == [] then
            f = figure("Tag", block.label, "Figure_name", "BARXY");
        else
            scf(f);
        end
// Create an empty figure without Menu's nor toolbar
        clf();
        f.background = -2;
        a=gca();
        drawlater();
        a.data_bounds = block.rpar';
        a.isoview = "on";
        u1=block.inptr(1);
        u2=block.inptr(2);
        j = 0;
        xsegs(u1, u2, 1:size(u1,"*")/2)
        e = gce();
        e.thickness = block.ipar;
    end

    if flag ==  1
// Output update
        u1=block.inptr(1);
        u2=block.inptr(2);

        f = findobj("Tag", block.label);
        a = f.children;

        drawlater()
        a.children(1).data = [u1, u2]
        drawnow();
    end

endfunction