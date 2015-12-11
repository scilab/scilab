// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function count=get_profile(lst,ilst)
    count=[];
    ops=0;
    nlst=size(lst);
    while ilst<=nlst then
        if type(lst(ilst))==15 then
            typ=lst(ilst)(1);
            c1=[];
            for ic=2:size(lst(ilst))
                c1=[c1;get_profile(lst(ilst)(ic),1)];
            end
            count=[count;c1]
            ilst=ilst+1;
        else
            ops=ops+1;
            //real timing line found
            if lst(ilst)(1)=="25" then
                count=[count;[evstr(lst(ilst)(2:3)),max(0,ops-2)]];
                ops=0;
            elseif lst(ilst)(1)=="3" then
                //possibly, non interpreted code line (e.g. subfunction definition header)
                ncode=1;
            elseif lst(ilst)(1)=="26" then
                //non interpreted code lines (e.g. subfunction definition body)
                // macr2lst keeps whitespace (even too much... leading whitespace
                //   makes a new code line), fun2string no. See bug 2413
                codelines=lst(ilst); codelines=codelines(4:$);
                ncode=length(stripblanks(codelines)<>"");
            elseif lst(ilst)(1)=="20" &  lst(ilst)(2)=="deff" then
                //subfunction definition trailer
                count=[count;zeros(ncode,3)];
            end
        end
        ilst=ilst+1;
    end
endfunction
