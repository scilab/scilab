//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012-2012 - Scilab Enterprises - Bruno JOFRET
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
function demo_bubbleSort()
    function res=bubbleSort(in, polyline)
        for (i = 1:size(in, "*"))
            for (j = 1:i)
                if (in(i) < in(j))
                    tmp = in(j);
                    in(j) = in(i);
                    in(i) = tmp;
                    polyline.data(:,2) = in';
                end
            end
        end
        res = in;
    endfunction

    nbValues = 200;
    x = floor(rand(1, nbValues) * nbValues);
    scf()
    demo_viewCode(SCI+"/modules/graphics/demos/anim/bubbleSort/bubbleSort.sci");
    plot(x, "diamond-");
    a = gca();
    e = a.children(1).children(1);
    x = bubbleSort(x, e);
endfunction
