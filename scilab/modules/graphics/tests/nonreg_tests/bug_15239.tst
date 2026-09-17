// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 15239 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/15239
//
// <-- Short Description -->
// The console's properties .tag and .userdata were not displayed
// Test must be run interactively because the console does not exist
// in adv-cli mode

c = get(0,"children");
c.userdata = [1 2 ; 3 4];
c.tag = "myConsoleName";

str = ["parent: [Root]"
"children = []"
"toolbar_visible = ""on"""
"user_data = [1,2;3,4]"
"tag = ""myConsoleName"""];
assert_checkequal(%l_string(c),str);
