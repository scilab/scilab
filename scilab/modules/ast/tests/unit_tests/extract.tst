// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - ESI Group - Cedric Delamarre
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//

//---------------------------//
// Implicit list extraction  //
//---------------------------//

invalid_index = msprintf(_("Invalid index.\n"));

result = 0:9;
assert_checkequal((0:9)(1), result(1));
assert_checkequal((0:9)(2), result(2));
assert_checkequal((0:9)(3), result(3));
assert_checkequal((0:9)(4), result(4));
assert_checkequal((0:9)($), result($));
assert_checkerror("(0:9)($+1)", invalid_index);
assert_checkerror("(0:9)($+1,$)", invalid_index);
assert_checkerror("(0:9)($-1,$)", invalid_index);
assert_checkequal((0:9)($-1), result($-1));
assert_checkerror("(0:9)(42)", invalid_index);
assert_checkequal((0:9)(:), result(:));
assert_checkequal((0:9)(1:$), result(1:$));
assert_checkequal((0:9)($:-1:1), result($:-1:1));
assert_checkequal((0:9)([1,2,2,1]), result([1, 2, 2, 1]));
assert_checkerror("(0:9)(1,2,2,1)", invalid_index);
assert_checkequal((0:9)(1,1), result(1,1));

assert_checkequal((0:$)(1), 0);
assert_checkequal((0:$)(2), 1);
assert_checkequal((0:$)(3), $);
assert_checkerror("(0:$)(4)", invalid_index);
assert_checkequal((0:$)($), $);
assert_checkerror("(0:$)($+1)", invalid_index);
assert_checkerror("(0:$)($+1,$)", invalid_index);
assert_checkerror("(0:$)($-1,$)", invalid_index);
assert_checkequal((0:$)($-1), 1);
assert_checkerror("(0:$)(42)", invalid_index);
assert_checkequal((0:$)(:), [0; 1; $]);
assert_checkequal((0:$)(1:$), [0, 1, $]);
assert_checkequal((0:$)($:-1:1), [$, 1, 0]);
assert_checkequal((0:$)([1,2,2,1]), [0, 1, 1, 0]);
assert_checkerror("(0:$)(1,2,2,1)", invalid_index);
assert_checkequal((0:$)(1,1), 0);