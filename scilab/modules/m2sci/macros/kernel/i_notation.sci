// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function txt=i_notation(txt)
    // This function changes 1i ,... by 1*i,...

    // M2SCI kernel functions called :
    //  - isinstring


    // To succeed in this work, we successively suppress occurences which can be proved not to be complex notation
    // Until we are 'sure' to have a complex notation

    n=size(txt,"r")

    I="i";J="j"
    matches=[string(0:9)+I(ones(1,10)),".i",string(0:9)+J(ones(1,10)),".j"]
    symbs=["+","-","*","/","\","(","["," ","^"," ",",",";","=","{"]
    s1=["+","-","*","/","\",",",";"," ","^",".","&","|","''","]",")","}"]
    s2=[string(0:9),"d","e","D","E","."]

    for k=1:n
        st=strindex(txt(k),[";//","//"])
        if st<> [] then
            for stk=1:size(st,"*")
                if ~isinstring(txt(k),stk) then
                    break
                end
            end
            continue
        end
        tk=txt(k)+" "

        // Find possible occurence of complex notation
        kc=strindex(tk,matches)

        // Kill indexes which point to non complex values (e.g. : a1item...)
        for kk=size(kc,"*"):-1:1
            km=kc(kk)+2
            if find(part(tk,km)==s1)==[] then kc(kk)=[],end
        end

        kc=[0 kc]

        for kk=size(kc,"*"):-1:2
            km=kc(kk)
            num=%T
            // Reads numeric value leading complex variable
            while or(part(tk,km)==s2)
                km=km-1
                if km<=kc(kk-1)+1 then
                    km=kc(kk-1);
                    num=%F;
                    break
                end
            end

            tokill=%F
            num=part(tk,km+1:kc(kk)-1)
            ke=strindex(convstr(num),["e","d"])
            kd=strindex(convstr(num),".")

            // Searching for invalid numeric values (more than one dot...)
            if size(ke,2)>1|size(kd,2)>1 then
                tokill=%T
            elseif size(ke,2)==1&size(kd,2)==1 then
                if ke<kd then tokill=%T,end
            end


            if ~tokill then
                // If char which follows supposed complex notation is not an operation symbol
                if km<>kc(kk-1) then
                    if and(part(tk,km)<>symbs) then tokill=%T,end
                end
            end

            if ~tokill then
                km=kc(kk)
                // If supposed complex notation is not in a string
                if ~isinstring(tk,km) then
                    tk=part(tk,1:km)+"*%"+part(tk,km+1:length(tk))
                end
            end
        end
        txt(k)=tk
    end
endfunction
