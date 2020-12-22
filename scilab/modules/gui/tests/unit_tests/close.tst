// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Short Description -->
// Unitary tests for close()

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
help
sleep(200)
close help
jimport org.scilab.modules.gui.helpbrowser.ScilabHelpBrowser
hb = ScilabHelpBrowser.getHelpBrowserWithoutCreation()
sleep(100)
assert_checkequal(hb, [])

// Editvar
// -------
a = 1;
editvar a
sleep(200)
close editvar
jimport org.scilab.modules.ui_data.variableeditor.ScilabVariableEditor
ve = ScilabVariableEditor.getVariableEditor();
assert_checkequal(ve, []);

// Browsevar
// ---------
browsevar
sleep(200)
close browsevar
jimport org.scilab.modules.ui_data.variablebrowser.ScilabVariableBrowser
assert_checkfalse(ScilabVariableBrowser.isBrowseVarOpened())

// Xcos
// ----
xcos
sleep(600)
close xcos
// 
javaclasspath(SCI + "/modules/xcos/jar/org.scilab.modules.xcos.jar");
jimport org.scilab.modules.xcos.Xcos
inst = Xcos.getInstance();
od = inst.openedDiagrams();
assert_checkequal(od.size(), int32(0));

xcos
sleep(600)
close xcos!
//
inst = Xcos.getInstance();
od = inst.openedDiagrams();
assert_checkequal(od.size(), int32(0));
