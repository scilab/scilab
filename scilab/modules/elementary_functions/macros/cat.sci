// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2017 - Samuel GOUGEON : cat() rewritten: http://bugzilla.scilab.org/8297
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function  y = cat(dim, varargin)
    // dim: scalar = dimension # along which input arrays are stacked
    // varargin: input arrays: matrices, hypermatrices, cells arrays, struct
    //           arrays, custom mlists,.. For custom arrays, [,] and [;] may be
    //           not defined. Must be all of the same sizes, except along dim.
    // y : concatenated array

    rhs = argn(2);
    if rhs < 2 then
        msg = gettext("%s: Wrong number of input argument(s): %d expected.\n")
        error(msprintf(msg,"cat", 1));
    end

    // dim must be a positive real
    if type(dim)==1 & dim>=0 & size(dim,"*")==1
        dim = max(1,round(dim));
    else
        msg = gettext("%s: Wrong type for input argument #%d: A positive real expected.\n");
        error(msprintf(msg, "cat", 1));
    end

    // CHECK INPUT ARRAYS
    //  - empty arrays are ignored
    for i = size(varargin):-1:1
        if isempty(varargin(i)) then
            varargin(i) = null();
        end
    end
    // - cat(m,[]), cat(m)  => []
    if isempty(varargin) then
        y = [];
        return
    end
    // - if just one input argument to concatenate, then return this argument
    if size(varargin) == 1 then
        y = varargin(1);
        return
    end
    //  - they must all have the same sizes (except along dim)
    v = varargin(1);
    S = size(v);
    if length(S) < dim then
        S = [S ones(1, dim-length(S))];
    end
    S(dim) = -1;
    for i = 2:size(varargin)
        s = size(varargin(i));
        s(dim) = -1;
        s(find(s==0)) = 1;
        if ~and(s==S)
            msg = gettext("%s: Wrong size for input arguments: Same size expected.\n");
            error(msprintf(msg, "cat"));
        end
    end
    // They must be all of the same type: cell, struct, or other
    T = typeof(v)
    for i = 2:size(varargin)
        v = varargin(i)
        if typeof(v)~=T
            msg = gettext("%s: Wrong type for input arguments: Same types expected.\n");
            error(msprintf(msg, "cat"));
        end
    end

    // PROCESSING
    // ----------
    y = varargin(1);
    [vcat, hcat] = (%t, %t);
    if type(y)>10
        t = typeof(y, "overload");
        if ~or(t==["ce" "st"])
            [vcat, hcat] = (isdef("%"+t+"_f_"+t,"n"), isdef("%"+t+"_c_"+t,"n"));
        end
    end
    if dim==1 & vcat then
        for i = 2:length(varargin)
            y = [y ; varargin(i)];
        end
    elseif dim==2 & hcat
        for i = 2:length(varargin)
            y = [y, varargin(i)];
        end
    else
        nd = max(ndims(y),dim);
        st = emptystr(1,nd) + ":";
        st(dim) = "%d:%d";
        st = strcat(st,",");
        Cmd = "y("+st+") = varargin(i);"
        sy = size(y,dim);
        for i = 2:length(varargin)
            s = size(varargin(i), dim);
            cmd = msprintf(Cmd,sy+1,sy+s);
            execstr(cmd);
            sy = sy + s;
        end
    end
endfunction
