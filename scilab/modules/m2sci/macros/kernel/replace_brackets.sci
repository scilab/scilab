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
