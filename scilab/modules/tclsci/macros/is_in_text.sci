// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function r=is_in_text(h,xy)
    if h.Type == "Text" & h.text_box_mode=="filled" then
        r=(xy(1)>h.data(1)&xy(1)<h.data(1)+h.text_box(1))&(xy(2)>h.data(2)&xy(2)<h.data(2)+h.text_box(2))
    else
        r = stringbox(h);
        r=[r r(:,1)];
        r=and([xy(2) -xy(1)]*diff(r,1,2)+(r(1,1:$-1).*r(2,2:$)-r(1,2:$).*r(2,1:$-1))<0)
    end
endfunction
