// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// INTERNAL Default function formating datatips for curves in polar frames
function Label = polarplot_datatip_display(dtip)
    xy = dtip.data(1:2)
    r = sqrt(xy(1)^2+xy(2)^2);
    a = atan(xy(2), xy(1)) / %pi * 180;
    if dtip.parent.display_function_data>=0 & a<0 then
        a = a+360;
    end

    if r<1000 & r>=0.1 then // 123.4  12.34  1.234  0.1234
       nm = 1 + int(log10(r));
       nd = 1 + (3-nm);
       f = msprintf("%%%d.%df",nm,nd);
       f = strsubst("R = %s\n$\\large\\theta = %6.1f°$\n", "%s", f);
       Label = msprintf(f, r, a);
    elseif r>=1000
        nm = int(log10(r));
        m = r/10^nm;
        Label = msprintf("$\\large R = %5.2f\\ 10^{%d}$\n"+..
                         "$\\large \\theta = %6.1f\\ °$\n", m, nm, a);
    else  // r < 0.1
        nm = int(log10(r));
        m = r/10^(nm-1);
        Label = msprintf("$\\large R = %5.2f\\ 10^{%d}$\n"+..
                         "$\\large \\theta = %6.1f\\ °$\n", m, nm-1, a);
    end
endfunction
