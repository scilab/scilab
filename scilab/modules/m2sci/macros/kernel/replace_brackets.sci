// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function txt=replace_brackets(txt)

    symbs=[",",";","=",")","]","("]
    // This file will be use to deal with cells...
    for k=1:size(txt,"r")

        // select-case
        if strindex(txt(k),"case")<>[] then
            txt(k)=strsubst(strsubst(txt(k),"{","makecell("),"}",")")
        else
            tk=strsubst(txt(k)," ","")

            ko=strindex(tk,"{")
            if ko<>[] then
                teq=strindex(tk,"=")

                ///
                if ko(1)==1 then
                    txt(k)=strsubst(txt(k),"{}","makecell()")
                    txt(k)=strsubst(strsubst(txt(k),"{","(makecell([cell(),"),"}","]))")
                elseif or(part(tk,ko(1)-1)==symbs) then
                    txt(k)=strsubst(txt(k),"{}","makecell()")
                    txt(k)=strsubst(strsubst(txt(k),"{","(makecell([cell(),"),"}","]))")
                else // Cell index
                    txt(k)=strsubst(strsubst(txt(k),"{","("),"}",").entries")
                end

                ////

                for kk=2:size(ko,"*")
                    if or(part(tk,ko(kk)-1)==symbs) then // Cell creation
                        txt(k)=strsubst(txt(k),"{}","makecell()")
                        txt(k)=strsubst(strsubst(txt(k),"{","(makecell([cell(),"),"}","]))")
                    else // Cell index
                        txt(k)=strsubst(strsubst(txt(k),"{","("),"}",").entries")
                    end
                end
            elseif ~isempty(strindex(txt(k),"}")) then
                txt(k)=strsubst(txt(k),"}","]))")
            end
        end
    end
endfunction
