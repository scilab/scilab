//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012-2012 - Scilab Enterprises - Bruno JOFRET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//
function demo_bubbleSort()
    function res=bubbleSort(in, polyline)
        for (i = 1:size(in, '*'))
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
    delete(gcf())
endfunction
