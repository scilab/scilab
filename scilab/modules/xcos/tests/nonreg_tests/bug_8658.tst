// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 8658 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8658
//
// <-- Short Description -->
// Modelica compiler fail for a diagram with accentuated filename.

diagram = TMPDIR + "/Schema-@-electrique.zcos";
[status,msg] = copyfile(SCI + "/modules/xcos/demos/ModelicaBlocks/RLC_Modelica.zcos", diagram);
if ~status then pause, end

if ~importXcosDiagram(diagram) then pause, end
if ~isempty(strstr(scs_m.props.title, "@")) then pause, end

deletefile(diagram);

