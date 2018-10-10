// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function hdl = %h_set(varargin)
    s = size(varargin);
    if s == 2
        str = varargin(1)
        val = varargin(2)
        hdl=[]
        set(str, val)

    elseif s > 2
        hdl = varargin(1)
        Hsize = size(hdl);
        nh = prod(Hsize);
        for i = 2:2:s   // Loop on ("property",value) pairs
            prop = varargin(i);
            rhs = varargin(i+1);
            if nh==1
                // scalar handle => direct assignment
                hdl(prop) = rhs;
            else
                // array of handles
                RHSsize = size(rhs);
                for j = 1:nh  // Loop on handles
                    h = hdl(j)  // works around http://bugzilla.scilab.org/15802
                    lhs = h(prop);     // Current handle's content
                    LHSsize = size(lhs);
                    if and(RHSsize==LHSsize) | lhs==[]  // => repeated assignment
                        h(prop) = rhs;
                    elseif size(lhs,"*")==1 & and(RHSsize==Hsize)
                        // => element-wise assignments
                        h(prop) = rhs(j);
                    elseif LHSsize(2)==1 & LHSsize(1)==RHSsize(1) & RHSsize(2)==nh
                        // column-wise assignments
                        h(prop) = rhs(:,j);
                    elseif LHSsize(1)==1 & LHSsize(2)==RHSsize(2) & RHSsize(1)==nh
                        // row-wise assignments
                        h(prop) = rhs(j,:);
                    elseif prod(LHSsize)==prod(RHSsize)
                        // Repeated assignment after RHS reformating
                        h(prop) = matrix(rhs,LHSsize);
                    elseif RHSsize(1)==nh & RHSsize(2)==prod(LHSsize)
                        // example: axes.data_bounds is a 2x2 matrix,
                        //          but a 1x4 assigment is possible
                        h(prop) = rhs(j,:);
                    else
                        msg = _("%s: Unsupported assignment for property ''%s''.\n")
                        error(msprintf(msg, "%h_set", prop))
                    end
                    hdl(j) = h;
                end
            end
        end
    end
endfunction
