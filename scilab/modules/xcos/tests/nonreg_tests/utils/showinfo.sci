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

function showinfo(scs_m)

    //  Description:
    //
    //    Show basic info on given diagram structure
    //
    //  Input argument(s):
    //
    //    scs_m - MANDATORY - structure
    //      Diagram structure, typically obtained after a 'load' instruction
    //
    //  Output argument(s):
    //
    //    -
    //
    //  Usage:
    //
    //    showinfo(scs_m)
    //
    //  Algorithm: (grep "^\s*//--" showinfo.sci | awk -F "//-- " '{print "//  " $1 $2}')
    //
    //    Display number of objects in the diagram
    //    For each object
    //      Determine and display object type + some info
    //        Block
    //        Link
    //        Text
    //        Unknown
    //
    //  Notes:
    //
    //    -
    //
    //  2008/03/31  Laurent Vaylet    Creation
    //  YYYY/MM/DD  ???               Modification: ???
    //

    //-- Display number of objects in the diagram
    nbObjs = length(scs_m.objs);
    mprintf("\n\n %d objects in diagram %s:\n\n", nbObjs, "test.cos")

    //-- For each object
    for k = 1:nbObjs

        //-- Determine and display object type + some info
        select typeof(scs_m.objs(k))

            //-- Block
        case "Block" then
            mprintf(" - Object #%d is a block whose simulation function is ""%s""\n", ..
            k, scs_m.objs(k).model.sim)

            //-- Link
        case "Link" then
            mprintf(" - Object #%d is a link from block #%d to block #%d\n", k, ..
            scs_m.objs(k).from(1), scs_m.objs(k).to(1))

            //-- Text
        case "Text" then
            mprintf(" - Object #%d is some text\n", k)

            //-- Unknown
        else
            mprintf(" - Object #%d is of unknown type\n", k)
        end
    end
endfunction

