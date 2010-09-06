// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 7137 -->
// <-- TEST WITH GRAPHIC -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7137
//
// <-- Short Description -->
// In Text uicontrol, fontsize was rounded which was problematic when
// fontUnits was 'normalized'.

ui=uicontrol("style","text","fontUnits","normalized","fontSize",0.1,"string","Hello World !","position",[20,40,170,40]);
if ui.FontSize == 0 then pause, end
