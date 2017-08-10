//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2010 - DIGITEO - Clément DAVID <clement.david@scilab.org>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
//

function version = getDiagramVersion(diagram)
    // Return the current diagram version
    //
    // Syntax
    //  version = getDiagramVersion(diagram)
    //
    // Parameters
    //  diagram:    the diagram instance
    //  version:    the version of the diagram
    //
    // Example
    //  loadXcosLibs();
    //  version = getDiagramVersion(scicos_diagram())

    [lhs,rhs] = argn(0);
    version = get_scicos_version();
    if rhs <> 1 then
        error(999,  msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "getDiagramVersion", 1));
    end

    if typeof(diagram) <> "diagram" then //check inputs
        error(999 ,msprintf(_("%s: Wrong type for argument #%d: diagram structure expected"), "getDiagramVersion", 1));
    end

    // check version
    if find(getfield(1, diagram) == "version") <> [] & diagram.version <> "" then
        version=diagram.version;
    else
        version=find_scicos_version(diagram);
    end
endfunction
