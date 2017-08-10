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

function s=makecell(varargin)
    // Equivalent for Matlab {}

    if lstsize(varargin)==0 then
        dims=[0 0]
        s=mlist(["ce","dims","entries"],int32(dims),list())
        return
    end

    dims=varargin(1)
    if prod(dims)<>lstsize(varargin)-1 then
        error(msprintf(gettext("%s: Wrong input arguments: Dimensions given as first argument do not match specified cell contents.\n"),"makecell"));
    end

    if prod(dims)==1 then
        s=mlist(["ce","dims","entries"],int32(dims),varargin(2))
    else
        listentries=varargin
        listentries(1)=null()
        s=mlist(["ce","dims","entries"],int32(dims($:-1:1)),listentries)'
    end
endfunction
