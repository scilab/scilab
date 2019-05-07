// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - ESI Group - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Short Description -->
// from RFC4180:
//  Fields containing line breaks (CRLF), double quotes, and commas
//  should be enclosed in double-quotes.
//

path = SCI+"/modules/spreadsheet/tests/unit_tests/";

M = csvRead(fullfile(path, "csvRead_multi_line.csv"));
assert_checkequal(M, %nan + zeros(2,5));

M = csvRead(fullfile(path, "csvRead_multi_line.csv"), [], [], "string");
assert_checkequal(M(1, 1), "hello");
assert_checkequal(M(1, 2), "this");
assert_checkequal(M(1, 3), "is");
assert_checkequal(M(1, 4), strcat(["C", "S", "V"], ascii(10)));
assert_checkequal(M(1, 5), "!");
assert_checkequal(M(2, 1), "on");
assert_checkequal(M(2, 2), "a");
assert_checkequal(M(2, 3), strcat(["m", "u", "l", "t", "i"], ascii(10)));
assert_checkequal(M(2, 4), "line");
assert_checkequal(M(2, 5), "sheet");


