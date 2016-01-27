// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===

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
