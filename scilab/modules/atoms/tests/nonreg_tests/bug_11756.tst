// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 11756 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11756
//
// <-- Short Description -->
// In the ATOMS GUI, listing modules of a selected category C : When a module
// registered in C is installed, after using atomsSystemUpdate(), all modules
// remaining in C but uninstalled were no longer listed.

// Let's consider the "Documentation" category:

// 1) Be sure that no module of this category is installed

[x, ierr] = fileinfo("SCI/.atoms/packages");
if ierr then
    packages = struct();
else
    load("SCI/.atoms/packages");
end

pnames = fieldnames(packages)';
pdoc = [];
for p = pnames
    v = fieldnames(packages(p));
    categories = packages(list(p, v(1), "Category"));
    if or(categories == "Documentation")
        pdoc = [pdoc p];
    end
end
// If there are some installed modules, uninstall them:
if pdoc~=[] then
    for p = pdoc
        atomsRemove(p);
    end
end
// 2) update
atomsSystemUpdate()

// 3) Then quit and restart Scilab
exit(0)

// 4) open the ATOMS manager:
atomsGui

// 5) click on the "Documentation" category
//    CHECK: you should see the whole list. For Scilab 6.0, there is at least
//           "uman" and "removed"

// 6) Install "uman" by hand (for instance)

// 7) Click on "All categories", to list the categories

// 8) update:
atomsSystemUpdate()

// 9) Click on the "Documentation" category
//    CHECK: you must still see the WHOLE list, with at least "uman" and "removed",
//    NOT ONLY THE "uman" INSTALLED ENTRY.
//    Without the patch, the "removed" module is no longer listed.

