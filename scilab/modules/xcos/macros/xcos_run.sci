//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//
//

//
// Simulate the diagram
// @xcosFile String Path to the diagram
//
function xcos_run(xcosFile)

    // Check that the Scicos libs are loaded
    if isdef("haltscicos") <> %t then
        error(msprintf(gettext("The Scicos libraries are not loaded")));
        return;
    end

    // import the file
    result = importXcosDiagram(xcosFile);
    if result <> %t then
        error(msprintf(gettext("Unable to simulate %s"), xcosFile));
        return;
    end

    // simulate
    scicos_simulate(scs_m, list(), []);

endfunction

