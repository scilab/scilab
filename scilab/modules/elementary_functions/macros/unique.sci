// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2018 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [x,k]=unique(x,orient)
    // extract unique components of a vector

    if argn(2)<2 then orient = "*",end
    sz = size(x);
    if size(x, orient)==1 then
        k = 1
        return
    end

    // PROCESSING complex numbers
    if or(type(x)==[1 5]) then
        if ~isreal(x)
            if isreal(x,0)
                x = real(x);
            else
                if orient=="*"
                    x = [real(x(:)) imag(x(:))]
                    if argn(1)==1
                        x = unique(x,"r")
                    else
                        [x, k] = unique(x,"r")
                    end
                    x = complex(x(:,1),x(:,2));
                    if sz(1)==1 // => put results in row
                        x = x.'
                        if argn(1)>1
                            k = k'
                        end
                    end
                elseif orient=="r" | orient==1
                    x = [real(x) imag(x)]
                    if argn(1)==1
                        x = unique(x,"r")
                    else
                        [x, k] = unique(x,"r")
                    end
                    x = complex(x(:,1:sz(2)), x(:,sz(2)+1:$));
                elseif orient=="c" | orient==2
                    x = [real(x) ; imag(x)]
                    if argn(1)==1
                        x = unique(x,"c")
                    else
                        [x, k] = unique(x,"c")
                    end
                    x = complex(x(1:sz(1),:), x(sz(1)+1:$,:));
                else
                    msg = _("%s: Argument #%d: Must be in the set {%s}.\n")
                    error(msprintf(msg, "unique", 2, "1,""r"",2,""c"""));
                end
                return
            end
        end
    end

    // PROCESSING text and other numerical types
    if orient=="*" then
        if argn(1) == 2 then
            [x,k] = gsort(x,"g","d");
            keq = find( x(2:$) == x(1:$-1) );
            if keq<>[] then keq = keq+1;end
            x(keq) = [];
            k(keq) = [];
            k = k($:-1:1);
            x = x($:-1:1);
        else
            x = gsort(x,"g","d");
            x = x($:-1:1);
            x( find(x(2:$) == x(1:$-1)) ) = [];
        end
    elseif  orient==1|orient=="r" then
        if argn(1) == 2 then
            [x,k] = gsort(x,"lr","d");
            keq = find(and(x(2:$,:) == x(1:$-1,:),"c"))
            if keq<>[] then keq = keq+1;end
            x(keq,:) = [];
            k(keq,:) = [];
            k = k($:-1:1,:);
            x = x($:-1:1,:);
        else
            x = gsort(x,"lr","i");
            x( find(and(x(2:$,:) == x(1:$-1,:),"c")),:) = [];
        end
    elseif  orient==2|orient=="c" then
        if argn(1) == 2 then
            [x,k] = gsort(x,"lc","d");
            keq = find(and(x(:,2:$) == x(:,1:$-1),"r"))
            if keq<>[] then keq = keq+1;end
            x(:,keq) = [];
            k(:,keq) = [];
            k = k(:,$:-1:1);
            x = x(:,$:-1:1);
        else
            x = gsort(x,"lc","i");
            x(:, find(and(x(:,2:$) == x(:,1:$-1),"r")) ) = [];
        end
    else
        msg = _("%s: Argument #%d: Must be in the set {%s}.\n")
        error(msprintf(msg, "unique", 2, "1,""r"",2,""c"""));
    end
endfunction
