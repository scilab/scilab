// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// Create a default figure
function closeRequestFcn()
    btn = messagebox("Do you want to close the figure?", "Close?", "question",  ["Yes", "No"], "modal")
    if btn then
        delete(gcf());
    else
        disp("Figure is still opened.");
    end
endfunction

f = scf();
assert_checkequal(f.closerequestfcn, "");
f.closerequestfcn = "closeRequestFcn()";
assert_checkequal(f.closerequestfcn, "closeRequestFcn()");

// Try with wrong values
refMsg = msprintf(_("Wrong type for ''%s'' property: String expected.\n"), "closerequestfcn");
assert_checkerror("f = gcf();f.closerequestfcn = 10;", refMsg);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A single string expected.\n"), "set", 3);
assert_checkerror("f = gcf();f.closerequestfcn = [""resizeMe()"" ""resizeMe()""];", refMsg);

// Test for other handles
refMsg = msprintf(_("''%s'' property does not exist for this handle.\n"), "closerequestfcn");
assert_checkerror("a = gca();a.closerequestfcn = ""closeRequestFcn()""", refMsg); // set
assert_checkerror("a = gca();v = a.closerequestfcn;", refMsg); // get


// Save/Load (SOD)
save(TMPDIR + "/figure.closerequestfcn.bin", "f");
delete(f);
clear f
load(TMPDIR + "/figure.closerequestfcn.bin");
assert_checkequal(f.closerequestfcn, "closeRequestFcn()");
