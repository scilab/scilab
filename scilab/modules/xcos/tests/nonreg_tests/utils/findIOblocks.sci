// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Laurent VAYLET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [idxWrite, idxRead] = findIOblocks(scs_m)

    //  Description:
    //
    //    Find index(es) of "Write to File" blocks in given diagram structure
    //
    //  Input argument(s):
    //
    //    scs_m - MANDATORY - structure
    //      Diagram structure, typically obtained after a 'load' instruction
    //
    //  Output argument(s):
    //
    //    idxWrite - MANDATORY - array of integers
    //      Index(es) of "Write to File" blocks in input diagram structure
    //
    //    idxRead - MANDATORY - array of integers
    //      Index(es) of "Read from File" blocks in input diagram structure
    //
    //  Usage:
    //
    //    [idxWrite, idxRead] = findIOblocks(scs_m)
    //
    //  Algorithm: (grep "^\s*//--" findIOblocks.sci | awk -F "//-- " '{print "//  " $1 $2}')
    //
    //    Initialize outputs
    //    For each object in the diagram (scs_m.objs)
    //      Is it a block ?
    //        Is it a "Write to File" block ?
    //          Add its index to the related output list
    //        Is it a "Read from File" block ?
    //          Add its index to the related output list
    //
    //  Notes:
    //
    //    -
    //
    //  2008/03/31  Laurent Vaylet    Creation
    //  YYYY/MM/DD  ???               Modification: ???
    //

    //-- Initialize outputs
    idxWrite = [];
    idxRead  = [];

    //-- For each object in the diagram (scs_m.objs)
    nbObjs = length(scs_m.objs);
    for k = 1:nbObjs
        //-- Is it a block ?
        if typeof(scs_m.objs(k)) == "Block"
            //-- Is it a "Write to File" block ?
            if scs_m.objs(k).model.sim == "writef"
                //-- Add its index to the related output list
                idxWrite($+1) = k;
            end

            if scs_m.objs(k).model.sim == "readf"
                //-- Add its index to the related output list
                idxRead($+1) = k;
            end
        end
    end
endfunction

