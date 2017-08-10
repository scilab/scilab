//<-- CLI SHELL MODE -->
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
// UTF-8 test

// ASCII
a = 42;
txt = "a";

// UTF-8 2 bytes
é = 1
définie = 1
dé = 1
éd = 1
txt = "é"
txt = "définie"
txt = "dé"
txt = "éd"

// UTF-8 3 bytes
€ = 1
d€finie = 1
d€ = 1
€d = 1
txt = "€"
txt = "d€finie"
txt = "d€"
txt = "€d"

豈 = 42
bla豈bla = 42
bla豈 = 42
豈bla = 42
txt = "豈"
txt = "豈bla"
txt = "bla豈"
txt = "豈bla"

// UTF-8 4 bytes

𯿽 = 51
bla𯿽bla = 51
bla𯿽 = 51
𯿽bla = 51

txt = "𯿽"
txt = "bla𯿽bla"
txt = "bla𯿽"
txt = "𯿽bla"

