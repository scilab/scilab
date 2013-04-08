// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Jérôme Picard
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 7770 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7770
//
// <-- Short Description -->
// It was not possible to compile a xcos diagram
// containing Modelica blocks

funcprot(0);
// Stubbing the messagebox method
function [btn] = messagebox(msg, msgboxtitle, msgboxicon, buttons, ismodal)
    btn=1;
endfunction

ilib_verbose(0);
// import a diagram of a Modelica demo in Scilab
result= importXcosDiagram(SCI+"/modules/xcos/demos/ModelicaBlocks/RLC_Modelica.zcos");
if ~result then pause,end

// compile the diagram
[%cpr,ok] = xcos_compile(scs_m);

if ~ok then pause,end
