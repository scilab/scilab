// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
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

function [x, ind] = lex_sort(x, varargin)

    // lexicographic matrix rows  sorting

    rhs = argn(2);
    if rhs < 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "lex_sort", 1));
    end

    // default argument values
    uniqueflag = %f
    sel = 1:size(x,2)

    // get arguments

    if size(varargin) > 2 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At most %d expected.\n"),"lex_sort",3));
    end

    for k=1:size(varargin)
        if type(varargin(k))==1 then //sel given
            sel=varargin(k)
        elseif type(varargin(k))==10 then // string given is it 'unique'?
            if varargin(k)==part("unique",1:length(varargin(k))) then
                uniqueflag=%t
            else
                error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'' expected.\n"),"lex_sort",k+1,"unique"));
            end
        else
            error(msprintf(gettext("%s: Wrong type for input argument #%d.\n"),"lex_sort",k+1));
        end
    end

    // sort rows in lexicographic order
    ind = 1:size(x,1);
    for i=size(sel,"*"):-1:1,
        [s,k] = gsort(x(ind,sel(i)),"g","d");
        ind = ind(k);
    end
    ind=ind(:)
    ind=ind($:-1:1)
    x = x(ind,:);

    if uniqueflag then
        k=find(sum(abs(x(2:$,sel)-x(1:$-1,sel)),2)==0)
        x(k,:)=[]
        ind(k)=[]
    end
endfunction
