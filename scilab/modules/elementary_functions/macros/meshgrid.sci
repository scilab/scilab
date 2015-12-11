// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function  varargout=meshgrid(varargin)

    // Syntax:
    // [X,Y]=meshgrid(x)
    // [X,Y]=meshgrid(x,y)
    // [X,Y,Z]=meshgrid(x,y,z)
    //
    // MESHGRID function creates matrices or 3-D arrays
    // Input
    // x,y,z : vectors
    // Output
    // X,Y,Z : matrices or 3-D arrays
    // F.B

    [lhs,rhs]=argn();
    if lhs>3 then
        error(msprintf(gettext("%s: Wrong number of output arguments: At most %d expected.\n"),"meshgrid",3));
    elseif rhs>3 then
        error(msprintf(gettext("%s: Wrong number of input arguments: At most %d expected.\n"),"meshgrid",3));
    end
    if or(lhs==[1 2]) & rhs==1 then
        [x,y]=ndgrid(varargin(1),varargin(1));
        varargout(1)=x';
        if lhs==2
            varargout(2)=y';
        end
    elseif or(lhs==[1 2]) & rhs==2 then
        [x,y]=ndgrid(varargin(:));
        varargout(1)=x';
        if lhs==2
            varargout(2)=y';
        end
    elseif lhs==3 & rhs==3 then
        [x,y,z]=ndgrid(varargin(:));
        varargout(1)=permute(x,[2,1,3]);
        if lhs>1
            varargout(2)=permute(y,[2,1,3]);
        end
        if lhs>2
            varargout(3)=permute(z,[2,1,3]);
        end
    else
        error(msprintf(gettext("%s: Wrong number of output arguments vs. input arguments: Same number expected.\n"),"meshgrid"));
    end
endfunction
