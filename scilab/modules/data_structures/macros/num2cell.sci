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

function  y = num2cell(x, dimens)
    //
    // x : regular array to be converted into a cells one.
    // dimens : vector of integers > 0: indices of dimensions along
    //          which the cells array must be concatenated.
    // y : cells vector or array

    rhs = argn(2)
    // check the number of input arguments
    if rhs < 1 | rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d or %d expected.\n"), "num2cell", 1, 2))
    end
    // case : x is empty, returns an empty cell
    xsize = size(x)
    if prod(xsize)==0 then
        y = cell()
        return
    end

    if argn(2)==1 then
        for i = 1:size(x,"*")
            y{i} = x(i)
        end
        y = matrix(y, xsize)
        return
    end

    // check the second input argument is a scalar (or a vector) of
    // positive integers
    if type(dimens)<>1 | or(dimens<=0) | or(dimens-floor(dimens)<>0)  then
        error(msprintf(gettext("%s: Wrong argument #%d: Positive integer expected.\n"),"num2cell",2));
    end
    dimens = matrix(dimens,1,-1)


    // xdimens : vector which contains the sizes of x dimensions which are in the vectordimens
    // nxdimens : vector which contains the sizes of x dimensions which are not in the vector dimens
    inddims=find(dimens<=ndims(x))
    xdimens=dimens(inddims)
    prodxdimens=prod(xsize(xdimens))
    // ydims : dimension of y cell (it's the new size of x after reduction to one of the dimensions contained in 'dimens' vector)
    // ycpdims : dimension of y components (equal to dimensions of x contained in 'dimens' vector)
    ycpdims=ones(1:ndims(x))
    ydims=ones(1:ndims(x))
    ycpdims(xdimens)=xsize(xdimens)
    nxdimens=1:ndims(x)
    nxdimens(xdimens)=[]
    ydims(nxdimens)=xsize(nxdimens)
    y=cell()
    xdimens=[xdimens,nxdimens]
    x=permute(x,xdimens)

    // delete the value of 'ycpdims' vector equal to one which are in last positions
    // if ycpdims=[2 3 1 4 1 1]  -> ycpdims=[2 3 1 4]
    i=prod(size(ycpdims))
    while i>2 & ycpdims(i)==1
        ycpdims(i)=[]
        i=i-1
    end
    // delete the value of 'ydims' vector equal to one which are in last positions
    // if ydims=[2 3 1 4 1 2 1 1]  -> ydims=[2 3 1 4 1 2]
    i=prod(size(ydims))
    while i>2 & ydims(i)==1
        ydims(i)=[]
        i=i-1
    end

    // fill the cell y
    for i=1:prod(ydims)
        y{i}=matrix(x(1+(i-1)*prodxdimens:prodxdimens*i),ycpdims)
    end
    y=matrix(y,ydims)

endfunction
