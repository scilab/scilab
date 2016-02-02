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

function txt=infer2txt(infer)
    txt=[]
    dims=[]
    if typeof(infer)=="infer" then
        for l=1:size(infer.dims)
            dims=[dims,string(infer.dims(l))]
        end
    else
        error(gettext("Not yet implemented."))
    end
    dims=strcat(dims," ")

    tp=infer.type.vtype
    if tp==1 then
        tp="Double"
    elseif tp==10 then
        tp="String"
    elseif or(tp==[4,6]) then
        tp="Boolean"
    elseif tp==16 then
        tp="Struct"
        for k=1:lstsize(infer.contents.index)
            if typeof(infer.contents.index(k))<>"list" then
                txt=[txt;expression2code(list(infer.contents.index(k)))+infer2txt(infer.contents.data(k))]
            else
                txt=[txt;expression2code(infer.contents.index(k))+infer2txt(infer.contents.data(k))]
            end
        end
    elseif tp==17 then
        tp="Cell"
        for k=1:lstsize(infer.contents.index)
            if typeof(infer.contents.index(k))<>"list" then
                txt=[txt;expression2code(list(infer.contents.index(k)))+infer2txt(infer.contents.data(k))]
            else
                txt=[txt;expression2code(infer.contents.index(k))+infer2txt(infer.contents.data(k))]
            end
        end
    elseif tp==9 then
        tp="Handle"
    else
        tp="Unknown"
    end
    if infer.type.property==Real then
        prop="Real"
    elseif infer.type.property==Complex then
        prop="Complex"
    else
        prop="Unknown"
    end
    txt=["|"+dims+"|"+tp+"|"+prop;txt]
endfunction
