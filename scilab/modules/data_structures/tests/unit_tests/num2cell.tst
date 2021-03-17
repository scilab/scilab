// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// unit tests for num2cell()
// -----------------------------------------------------------------------------

assert_checkequal(num2cell([]), {});

// -------------------
// For decimal numbers
// -------------------
assert_checkequal(num2cell(1:4), {1,2,3,4});
assert_checkequal(num2cell((1:4)'), {1,2,3,4}');
assert_checkequal(num2cell([1 2 ; 3 4]), {1, 2 ; 3, 4});
h = matrix(1:8,2,2,2);
assert_checkequal(num2cell(h), cat(3,{1,3;2,4}, {5,7;6,8}));

// Concatenating some directions
assert_checkequal(num2cell(h,1), cat(3,{[1;2],[3;4]}, {[5;6],[7;8]}));
assert_checkequal(num2cell(h,2), cat(3,{[1 3]; [2 4]}, {[5 7]; [6 8]}));
assert_checkequal(num2cell(h,3), ..
                     {cat(3,1,5), cat(3,3,7); cat(3,2,6), cat(3,4,8)});
assert_checkequal(num2cell(h,[1 2]), cat(3,{[1 3; 2 4]},{[5 7; 6 8]}));
assert_checkequal(num2cell(h,1:3), {h});

// --------
// For text
// --------
assert_checkequal(num2cell(""), {""});
assert_checkequal(num2cell(["" ""]), {"",""});
assert_checkequal(num2cell(["" "";"" ""]), {"","";"" ""});

assert_checkequal(num2cell(["1" "2" "3" "4"]), {"1","2","3","4"});
assert_checkequal(num2cell(["1" "2" "3" "4"]'), {"1","2","3","4"}');
assert_checkequal(num2cell(["1" "2" ; "3" "4"]), {"1", "2" ; "3", "4"});
h = matrix(["1" "2" "3" "4" "5" "6" "7" "8"], 2,2,2);
assert_checkequal(num2cell(h), cat(3,{"1","3"; "2","4"}, {"5","7"; "6","8"}));

// Concatenating some directions
assert_checkequal(num2cell(["" "";"" ""], 2), {["" ""] ; ["" ""]});

assert_checkequal(num2cell(h,1), cat(3,{["1";"2"],["3";"4"]}, {["5";"6"],["7";"8"]}));
assert_checkequal(num2cell(h,2), cat(3,{["1" "3"];["2" "4"]}, {["5" "7"];["6" "8"]}));
assert_checkequal(num2cell(h,3), ..
              {cat(3,"1","5"), cat(3,"3","7"); cat(3,"2","6"), cat(3,"4","8")});
assert_checkequal(num2cell(h,[1 2]), cat(3,{["1" "3";"2" "4"]},{["5" "7";"6" "8"]}));
assert_checkequal(num2cell(h,1:3), {h});
