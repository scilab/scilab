// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 5456 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5456
//
// <-- Short Description -->
// TCL_UpVar(source,alias) returns %T even if the TCL variable source did not exist and then aliasing had not been performed.
//
// =============================================================================
if TCL_ExistVar("tmp") <> %f then pause,end
if TCL_UpVar("tmp","tmp2") <> %f then pause,end
if TCL_ExistVar("tmp2") <> %f then pause,end
// =============================================================================
if TCL_SetVar("tmp","OK") <> %t then pause,end
if TCL_ExistVar("tmp") <> %t then pause,end
if TCL_UpVar("tmp","tmp2") <> %t then pause,end
if TCL_ExistVar("tmp2") <> %t then pause,end
if TCL_GetVar("tmp2") <> "OK" then pause,end
// =============================================================================
if TCL_SetVar("Scilab","OK") <> %t then pause,end
if TCL_UpVar("Scilab","ScilabBis") <> %t then pause,end
if TCL_GetVar("ScilabBis") <> "OK" then pause,end
if TCL_SetVar("Scilab","NOK") <> %t then pause,end
if TCL_GetVar("ScilabBis")<> "NOK" then pause,end
if TCL_SetVar("ScilabBis","modified") <> %t then pause,end
if TCL_GetVar("ScilabBis") <> "modified" then pause,end
if TCL_GetVar("Scilab") <>  "modified" then pause,end
if TCL_GetVar("Scilab") <> TCL_GetVar("ScilabBis") then pause,end
// =============================================================================
if TCL_CreateSlave('InterpBis') <> 'InterpBis' then pause,end
if TCL_SetVar("Scilab","Good",'InterpBis') <> %t then pause,end
if TCL_UpVar("Scilab","ScilabBis",'InterpBis') <> %t then pause,end
if TCL_GetVar("ScilabBis",'InterpBis') <> 'Good' then pause,end
if TCL_SetVar("Scilab","Not good",'InterpBis') <> %t then pause,end
if TCL_GetVar("ScilabBis",'InterpBis') <> "Not good" then pause,end
if TCL_SetVar("ScilabBis","modified again",'InterpBis') <> %t then pause,end
if TCL_GetVar("ScilabBis",'InterpBis') <> "modified again" then pause,end
if TCL_GetVar("Scilab",'InterpBis') <> TCL_GetVar("ScilabBis",'InterpBis') then pause,end
TCL_DeleteInterp('InterpBis')
// =============================================================================
