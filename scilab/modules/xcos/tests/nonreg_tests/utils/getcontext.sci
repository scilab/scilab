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

function [context, status] = getcontext(testName)

    //  Description:
    //
    //    Get context data from a text file.
    //    Result can be assigned to %scicos_context
    //
    //  Input argument(s):
    //
    //    testName - MANDATORY - string
    //      Path to folder storing diagrams to check
    //
    //  Output argument(s):
    //
    //    context - MANDATORY - structure
    //      Context of the diagram <testName>.zcos
    //
    //    status - MANDATORY - structure
    //      Feedback about success or failure of the operation
    //
    //  Usage:
    //
    //    [context, status] = getcontext('./Linear/GAINBLK_001_001')
    //
    //  Algorithm: (grep "^\s*//--" getcontext.sci | awk -F "//-- " '{print "//  " $1 $2}')
    //
    //    Initializations
    //    Open file containing context data
    //    Read contents and convert to <variable name / value> pairs
    //    Set up context
    //
    //  Notes:
    //
    //    @TODO: Use XML to store context in a cleaner and more efficient way
    //
    //  2008/03/31  Laurent Vaylet    Creation
    //  YYYY/MM/DD  ???               Modification: ???
    //

    //-- Initializations
    context = []
    status.ok = %t
    status.details = []

    //-- Open file containing context data
    try
        filename = testName + ".cxt"
        fid = mopen(filename)
    catch
        context = []
        status.ok = %f
        status.details = mprintf(gettext("%s: Cannot open file ''%s'' for reading.\n"), ..
        "getcontext", filename)
        return
    end
    strData = mgetl(fid)
    mclose(fid)

    //-- Read contents and convert to <variable name / value> pairs
    // !! ASSUME THERE IS NO SPACE IN THE 'VALUE' PART OF THE STRING !!
    try
        strData = msscanf(-1, strData, "%s %s")
    catch
        context = []
        status.ok = %f
        status.details = mprintf(gettext("%s: Error while trying to read file contents from ''%s''.\n"), ..
        "getcontext", filename)
        return
    end

    //-- Set up context
    for k = 1:size(strData,1)
        cmd = "context." + strData(k,1) + " = " + strData(k,2)
        try
            execstr(cmd)
        catch
            context = []
            status.ok = %f
            status.details = mprintf(gettext("%s: Syntax error while trying to set up context from"..
            + " file ''%s'' (at line %d).\n"), "getcontext", filename, k)
            return
        end
    end
endfunction

