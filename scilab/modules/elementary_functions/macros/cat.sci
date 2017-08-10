// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Farid BELAHCENE
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

function  y = cat(dims, varargin)
    //
    // CAT function concatenates the inputs arguments (included in varargin) according to dims
    // if dims=1 then concatenation is done according to the rows of the input arguments, if dims=2 then concatenation is done according to the columns of the input arguments,...
    // Output
    // -y : a (multi) array, the result of the concatenation
    // Input
    // -dims : a scalar, the dimension chosen for the concatenation
    // -varargin : scalars, matrices, hypermatrices to concatenate
    // F.B


    rhs = argn(2);
    if rhs < 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"cat", 1));
    end

    // dims must be a positive real
    if type(dims)==1 & dims>=0 & size(dims,"*")==1
        dims = max(1,round(dims));
    else
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A positive real expected.\n"),"cat",1));
    end

    // verify if dims value is superior to the dimension of the input arguments
    dimssupvar = %t;
    for i = 1:size(varargin)
        if ndims(varargin(i))>=dims then
            dimssupvar = %f;
        end
    end

    // delete all empty input arguments
    for i = size(varargin):-1:2
        if isempty(varargin(i)) then
            varargin(i) = null();
        end
    end

    // if just one input argument to concatenate then return this argument
    if size(varargin) == 1 then
        y = varargin(1);
        return
    elseif isempty(varargin(1)) then
        varargin(1) = null();
    end

    // create 2 lists sizevar and sizevarless which contains (respectively) the size of all input arguments (sizevar(i) = size(varargin(i))), and the size of all input arguments excluding the size of the dimension number dims
    onedims = ones(1,dims);
    sizevarless = list();
    sizevar = list();

    for i=1:size(varargin)
        var = onedims;
        var(1:ndims(varargin(i))) = size(varargin(i));
        sizevar(i) = var;
        var(dims) = [];
        sizevarless(i) = var;
    end

    for i=2:size(varargin)
        if or(sizevarless(i-1)<> sizevarless(i))
            error(msprintf(gettext("%s: Wrong size for input arguments: Same size expected.\n"),"cat"));
        end
    end

    // case : input arguments are cells arrrays
    if typeof(varargin(1))=="ce"
        ytemp = cell();
    else   // case : input arguments are arrays of doubles, strings, characters,...
        ytemp = [];
    end

    vartype = typeof(varargin(1));

    for j=2:size(varargin)
        if  typeof(varargin(j)) ==vartype
            vartype = typeof(varargin(j));
        else
            error(msprintf(gettext("%s: Wrong type for input arguments: Same types expected.\n"),"cat"));
        end
    end

    // permorder is the order of the permutation
    permuteorder = 1:size(sizevarless(1),"*")+1;
    if ~dimssupvar then
        permuteorder(dims) = [];
        permuteorder = [dims permuteorder];
    end

    // permutevar is a list which contains the permuted input arguments arrays
    permutevar = list();
    for j=1:size(varargin)
        permutevar(j) = permute(varargin(j),permuteorder);
    end

    for i=1:prod(sizevarless(1))
        for j=1:size(varargin)
            permutevarj = permutevar(j);
            lj = size(varargin(j),"*")/prod(sizevarless(j));
            if typeof(permutevarj)=="ce" then
                for k=1+lj*(i-1):lj*i
                    ytemp{size(ytemp,"*")+1} = permutevarj{k};
                end
            else
                ytemp= [ytemp (permutevarj(1+lj*(i-1):lj*i)).'];
            end
        end
    end
    ytemp = matrix(ytemp,1,-1);

    sizevar = sizevar(1);
    prodxdims = prod(sizevar(1:dims));
    ny = prod(sizevar)/prodxdims;
    ydimsize = size(ytemp,2)/(prod(sizevarless(1)));
    prodxdimless = prod(sizevar(1:dims-1));
    ind = 1;
    for j=1:ydimsize-1
        ind = [ind 1+prodxdimless*j];
    end
    for i=0:ny-1
        index = [];
        for j=0:prodxdimless-1
            index = [index ind+j+i*(prodxdimless)*ydimsize];
        end
        if typeof(ytemp)=="ce"
            ceindex = (1:ydimsize*prodxdimless)+ydimsize*prodxdimless*(i);
            for k=1:size(index,"*")
                y{index(k)} = ytemp{ceindex(k)};
            end
        else
            y(index) = ytemp((1:ydimsize*prodxdimless)+ydimsize*prodxdimless*(i));
        end
    end

    // redimension of y
    ysize = sizevarless(1);
    if dimssupvar then
        ysize($+1) = size(varargin);
    else
        if dims == size(sizevarless(1))+1
            ysize(dims) = size(ytemp,2)/prod(sizevarless(1));
        else
            ysize = [ysize(1:dims-1) size(ytemp,2)/prod(sizevarless(1)) ysize(dims:$)];
        end
    end
    y = matrix(y,ysize);

endfunction
