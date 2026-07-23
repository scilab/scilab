// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
// Copyright (C) 2026 - Dassault Systèmes S.E. - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Short Description -->
// Unitary tests for close()

// helper to execute a Scilab command after all pending AWT events have been processed
jimport org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement

// Figures
// -------
close(winsid())
scf(7);
scf(3);
scf(10);
scf(6);
close(6)
assert_checkequal(winsid(), [7 3 10]);
close()
assert_checkequal(winsid(), [3 10]);
close([10 3])
assert_checkequal(winsid(), []);

// Help
// ----
jimport org.scilab.modules.gui.helpbrowser.ScilabHelpBrowser

doc
ScilabInterpreterManagement.executeAndWaitForEDT("close doc")
hb = ScilabHelpBrowser.getHelpBrowserWithoutCreation()
assert_checkequal(hb, [])

// Editvar
// -------
jimport org.scilab.modules.ui_data.variableeditor.ScilabVariableEditor

a = 1;
editvar a
ScilabInterpreterManagement.executeAndWaitForEDT("close editvar")
ve = ScilabVariableEditor.getVariableEditor();
assert_checkequal(ve, []);

// Browsevar
// ---------
jimport org.scilab.modules.ui_data.variablebrowser.ScilabVariableBrowser

browsevar
ScilabInterpreterManagement.executeAndWaitForEDT("close browsevar")
assert_checkfalse(ScilabVariableBrowser.isBrowseVarOpened())

// Xcos
// ----
javaclasspath(SCI + "/modules/xcos/jar/org.scilab.modules.xcos.jar");
jimport org.scilab.modules.xcos.Xcos

// // this is INTERACTIVE, as it requires user confirmation on canClose()
// xcos
// ScilabInterpreterManagement.executeAndWaitForEDT("close xcos")
// 
// inst = Xcos.getInstance();
// od = inst.openedDiagrams();
// assert_checkequal(od.size(), int32(0));

xcos
ScilabInterpreterManagement.executeAndWaitForEDT("close xcos!")

//
inst = Xcos.getInstance();
od = inst.openedDiagrams();
assert_checkequal(od.size(), int32(0));
