//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4073 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4073
//
// <-- Short Description -->
// mprintf does not display accent correctly
// =============================================================================
tab_ref = [
"世界您好",
"азеазея",
"ハロー・ワールド",
"เฮลโลเวิลด์",
"حريات وحقوق",
"תוכנית",
'àéùç'];
// =============================================================================
for i = 1 : size(tab_ref,'*')
  if tab_ref(i) <> sprintf(tab_ref(i)) then bugmes();quit;end
  if tab_ref(i) <> sprintf('%s',tab_ref(i)) then bugmes();quit;end
end
// =============================================================================
for i = 1 : size(tab_ref,'*')
  disp(tab_ref(i));
end
// =============================================================================
for i = 1 : size(tab_ref,'*')
  mprintf(tab_ref(i));
  mprintf('\n');
  mprintf('%s\n',tab_ref(i));
end
// =============================================================================
for i = 1 : size(tab_ref,'*')
  printf(tab_ref(i));
  printf('\n');
  printf('%s\n',tab_ref(i));
end
// =============================================================================
