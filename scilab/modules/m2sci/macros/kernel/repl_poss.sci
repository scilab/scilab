// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function []=repl_poss(expr,varargin)
    // Display information for replacement for a statement in converted file

    txt=[]
    nposs=size(varargin)

    if nposs==0 | fix(nposs/3)*3~=nposs then
        error(gettext("Wrong number of inputs."))
    end

    txt=[expression2code(expr)+gettext(" may be replaced by:")]
    for k=1:3:nposs
        vars=""
        if typeof(varargin(k+1))<>"list" then
            vars=expression2code(varargin(k+1))
        else
            for l=1:size(varargin(k+1))
                vars=vars+expression2code(varargin(k+1)(l))
                if l<size(varargin(k+1)) then
                    vars=vars+" and "
                end
            end
        end
        txt=[
        txt
        "   --> "+expression2code(varargin(k))+" "+gettext("if")+" "+vars+" "+varargin(k+2)]
    end

    set_infos(txt,1)

endfunction
