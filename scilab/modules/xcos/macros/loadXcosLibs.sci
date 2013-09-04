//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//
//
function loadXcosLibs()
    // Extracted from initial_scicos_tables
    scicos_pal_libs = ["Branching","Events","Misc","Sinks","Threshold","Linear","MatrixOp","NonLinear","Sources","Electrical","Hydraulics","PDE","IntegerOp"];

    // list of scicos libraries that we need at xcos launch
    listlibsname = [];

    for theLib = scicos_pal_libs
        if isfile("SCI/modules/scicos_blocks/macros/" + theLib + "/lib") then
            load("SCI/modules/scicos_blocks/macros/" + theLib + "/lib");
            listlibsname = [listlibsname, theLib];
        end
    end
    clear theLib;

    if isfile("SCI/modules/scicos/macros/scicos_scicos/lib") then
        load("SCI/modules/scicos/macros/scicos_scicos/lib");
        listlibsname = [listlibsname, "scicos_scicos"];
    end

    // name and libname
    listlibsname(listlibsname == "MatrixOp") = "Matrixop";
    listlibsname(listlibsname == "NonLinear") = "Nonlinear";
    listlibsname(listlibsname == "IntegerOp") = "Integerop";

    if listlibsname <> [] then
        resumedLibs = listlibsname + "lib";
    else
        resumedLibs = string([]);
    end

    // Compatibility interface functions
    function [x,y,typ]=COMPAT_BLOCK(job,arg1,arg2)
        // Throw an error on block access
        x=[];y=[];typ=[]
        if ~exists("arg1") then
            arg1 = mlist(["Block", "gui"], "COMPAT_BLOCK");
        end
        error(msprintf(gettext("%s: the block ""%s"" is no more available, please update the diagram with a compatible one."), "loadXcosLibs", arg1.gui));
    endfunction

    // removed blocks
    removed = [
    "AFFICH_f"
    "RFILE"
    "WFILE"];
    prot = funcprot();
    funcprot(0);
    execstr(strcat(removed + "=COMPAT_BLOCK; "));
    funcprot(prot);

    resumedBlocks = removed';

    // Put all resumed symbols into the parent scope
    //    Take care: resume() will also return to the parent scope
    execstr("[" + strcat([resumedLibs resumedBlocks], ", ") + "] = resume(" + strcat([resumedLibs resumedBlocks], ", ") + ");");
endfunction

