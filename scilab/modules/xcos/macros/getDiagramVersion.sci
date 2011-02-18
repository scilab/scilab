//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2010 - DIGITEO - Clément DAVID <clement.david@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

function version = getDiagramVersion(diagram)
// Return the current diagram version
//
// Calling Sequence
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
        error(999 ,msprintf(_("%s: Wrong type for argument %d: diagram structure expected"), "getDiagramVersion", 1));
    end

    // check version
    if find(getfield(1, diagram) == "version") <> [] & diagram.version <> "" then
        version=diagram.version;
    else
        version=find_scicos_version(diagram);
    end
endfunction
