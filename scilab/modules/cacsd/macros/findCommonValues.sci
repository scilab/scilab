// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function [c,v]=findCommonValues(v)
    if typeof(v)<>"ce" then
        error(msprintf(_("%s: Wrong type for input argument #%d: Cell expected.\n"),...
        "findCommonValues",1))
    end

    nv=size(v,"*")
    v1=v{1}
    for i=2:size(v,"*")
        if typeof(v{i})<>typeof(v1) then
            error(msprintf(_("%s: Wrong type for input argument #%d: cell entries must have the same type\n"),...
            "findCommonValues",1))
        end
    end
    r=v
    c=[]
    d1=[]
    for k=size(v1,"*"):-1:1
        L=zeros(1,nv);L(1)=k;
        for i=2:size(v,"*")
            l=find(v1(k)==v{i},1)
            if l==[] then break,end
            L(i)=l
        end
        if L(nv)<>0 then
            //v1(k) is a common value
            c=[c;v1(k)]
            for i=1:size(v,"*")
                v{i}(L(i))=[]
            end
        end
    end
endfunction
