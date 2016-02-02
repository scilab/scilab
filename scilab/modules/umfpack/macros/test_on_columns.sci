//   Copyright Bruno Pinçon, ESIAL-IECN, Inria CORIDA project
//   <bruno.pincon@iecn.u-nancy.fr>
//
// This set of scilab 's macros provide a few sparse utilities.
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [X] = test_on_columns(X,Xold)
    //
    //  X and Xold are (n,t) matrix , Xold being an optional argument
    //  verify and force all columns of the new X to be non paralleles
    //  and also to be non paralleles to the columns of Xold if Xold
    //  is given
    //  2 columns j1 and j2 are // if  | Cj1'*Cj2 | = n because
    //  all elements are in {+1,-1}
    //
    [lhs,rhs] = argn()
    [n,t] = size(X)
    if rhs == 1 then
        for j=2:t
            while %t
                res_test = abs(X(:,j)'*X(:,1:j-1))
                if or(res_test == n) then
                    X(:,j) = fsign(rand(n,1)-0.5)
                else
                    break
                end
            end
        end
    else   // rhs = 2
        for j=1:t
            while %t
                res_test = abs([X(:,j)'*Xold  X(:,j)'*X(:,1:j-1)])
                if or(res_test == n) then
                    X(:,j) = fsign(rand(n,1)-0.5)
                else
                    break
                end
            end
        end
    end
endfunction
