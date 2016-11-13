// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
