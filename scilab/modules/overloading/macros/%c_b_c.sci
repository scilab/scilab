// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
function r=%c_b_c(varargin)
    if  size(varargin)==2 then
        if type(varargin(1))==10 & type(varargin(2))==10 then
            r=asciimat(asciimat(varargin(1)):asciimat(varargin(2)))
        end
    elseif size(varargin)==3 then
        if type(varargin(1))==10 & type(varargin(2))==10 & type(varargin(3))==10 then
            r=asciimat(asciimat(varargin(1)):asciimat(varargin(2)):asciimat(varargin(3)))
        end
    else
        error(43)
    end
endfunction
