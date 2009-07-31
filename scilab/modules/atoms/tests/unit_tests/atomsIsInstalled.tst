// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// Install toolbox N°1 for all users (SCI/.atoms)
atomsInstallRegister("toolbox1","1.0","A",%T);
atomsInstallRegister("toolbox1","1.1","A",%T);
atomsInstallRegister("toolbox1","1.2","A",%T);

// Install toolbox N°2 for current user only (SCIHOME/atoms)
atomsInstallRegister("toolbox2","0.1","A",%F);
atomsInstallRegister("toolbox2","0.2","A",%F);

// Check input parameters
if execstr("atomsIsInstalled()","errcatch") == 0 then pause; end
if execstr("atomsIsInstalled([""toolbox1"" ""toolbox2""],[""1.0"";""0.1""]  )","errcatch") == 0 then pause, end
if execstr("atomsIsInstalled([""toolbox1"" ""toolbox1""],[""1.0""]  )","errcatch") == 0 then pause, end

// Check output results

if ~ atomsIsInstalled("toolbox1") then pause, end
if atomsIsInstalled("toolbox1",[],%F) then pause, end

if ~ atomsIsInstalled("toolbox2") then pause, end
if ~ atomsIsInstalled("toolbox2",[],%F) then pause, end

if ~ atomsIsInstalled("toolbox1","1.0") then pause, end
if ~ atomsIsInstalled("toolbox1","1.1") then pause, end
if ~ atomsIsInstalled("toolbox1","1.2") then pause, end

if or( atomsIsInstalled(["toolbox1" "toolbox2" "toolbox1" "toolbox1"],["1.0" "1.1" "1.2" "1.3"] ) <> [%T %F %T %F] )  then pause, end
if or( atomsIsInstalled(["toolbox1" "toolbox2";"toolbox1" "toolbox1"],["1.0" "1.1";"1.2" "1.3"] ) <> [%T %F;%T %F] )  then pause, end
if or( atomsIsInstalled(["toolbox1";"toolbox2";"toolbox1";"toolbox1"],["1.0";"1.1";"1.2";"1.3"] ) <> [%T;%F;%T;%F] )  then pause, end
