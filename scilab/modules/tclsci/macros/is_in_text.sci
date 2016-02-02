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

function r=is_in_text(h,xy)
    if h.Type == "Text" & h.text_box_mode=="filled" then
        r=(xy(1)>h.data(1)&xy(1)<h.data(1)+h.text_box(1))&(xy(2)>h.data(2)&xy(2)<h.data(2)+h.text_box(2))
    else
        r = stringbox(h);
        r=[r r(:,1)];
        r=and([xy(2) -xy(1)]*diff(r,1,2)+(r(1,1:$-1).*r(2,2:$)-r(1,2:$).*r(2,1:$-1))<0)
    end
endfunction
