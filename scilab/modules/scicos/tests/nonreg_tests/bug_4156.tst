// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//
// <-- Non-regression test for bug 4156 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4156
//
// <-- Short Description -->
// 
//scicos_pal , %scicos_menu ,  %scicos_short ,
//%scicos_help, %scicos_display_mode, scicos_pal_libs ,
//%scicos_lhb_list, %CmenuTypeOneVector , %scicos_gif
//%scicos_contrib  variables should not be defined at scilab startup but only when you launch scicos

if isdef("scicos_pal") then pause,end
if isdef("%scicos_menu") then pause,end
if isdef("%scicos_short") then pause,end
if isdef("%scicos_help") then pause,end
if isdef("%scicos_display_mode") then pause,end
if isdef("modelica_libs") then pause,end
if isdef("scicos_pal_libs") then pause,end
if isdef("%scicos_lhb_list") then pause,end
if isdef("%CmenuTypeOneVector") then pause,end
if isdef("%scicos_gif") then pause,end
if isdef("%scicos_contrib") then pause,end
if isdef("%scicos_libs") then pause,end
