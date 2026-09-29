// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - UTC - Stephane Mottelet
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 17563 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/17563
//
// <-- Short Description -->
// printMatrix error while displaying a cell containing strings wider
// than the console

// Pin the console width so the test is deterministic regardless of the
// actual terminal size it runs in.
lines(0, 80);

c = {"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx","yyyyyyyyyyyyyyyyyyyyyyyy"}
