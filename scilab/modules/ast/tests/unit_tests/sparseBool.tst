//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Antoine ELIAS
// Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
//

// <-- CLI SHELL MODE -->

// boolean sparse function


//from dense form
s=[ %t,%f,%f,%f,%f,%f,%f;
%f,%t,%f,%f,%f,%f,%f;
%f,%f,%t,%f,%f,%f,%f;
%f,%f,%t,%t,%f,%f,%f;
%f,%f,%t,%t,%t,%f,%f;
%f,%f,%t,%t,%f,%t,%f;
%f,%f,%t,%t,%f,%t,%t];


sp=sparse(s);

assert_checkequal(size(s), size(sp));
assert_checkequal(full(sp), s);

//from coord
sp = sparse([1 1;2 2;3 3;4 3;4 4;5 3;5 4;5 5;6 3;6 4;6 6;7 3;7 4;7 6;7 7], [%t %t %t %t %t %t %t %t %t %t %t %t %t %t %t]);
assert_checkequal(size(s), size(sp));
assert_checkequal(full(sp), s);

//with size
newsp = sparse([], [], [10,20]);
assert_checkequal(size(newsp), [10,20]);

//concatenation
spc = [sp [%t; %f; %t; %f; %t; %f; %t]];
sc = [s [%t; %f; %t; %f; %t; %f; %t]];
assert_checkequal(full(spc), sc);

spc = [sp ;[%t %f %t %f %t %f %t]];
sc = [s ;[%t %f %t %f %t %f %t]];
assert_checkequal(full(spc), sc);

spc = [[%t; %f; %t; %f; %t; %f; %t] sp];
sc = [[%t; %f; %t; %f; %t; %f; %t] s];
assert_checkequal(full(spc), sc);

spc = [[%t %f %t %f %t %f %t]; sp];
sc = [[%t %f %t %f %t %f %t]; s];
assert_checkequal(full(spc), sc);

sc = [s s];
spc = [sp sp];
assert_checkequal(full(spc), sc);

sc = [s; s];
spc = [sp; sp];
assert_checkequal(full(spc), sc);

sc = [s s];
spc = [sp sp];
assert_checkequal(full(spc), sc);

A = sparse(%t);
A = [A %f];
A = [A %f];
A = [A %f];

assert_checkequal(full(A), [%t %f %f %f]);
assert_checkequal(size(A, "c"), 4);

//extraction
assert_checkequal(full(sp(:)), s(:));
assert_checkequal(size(sp(:)), size(s(:)));
assert_checkequal(full(sp(1:7, 1:7)), s(1:7, 1:7));
assert_checkequal(size(sp(1:7, 1:7)), size(s(1:7, 1:7)));
assert_checkequal(full(sp(1:4, :)), s(1:4, :));
assert_checkequal(size(sp(1:4, :)), size(s(1:4, :)));
assert_checkequal(full(sp(:, 2:6)), s(:, 2:6));
assert_checkequal(size(sp(:, 2:6)), size(s(:, 2:6)));
assert_checkequal(full(sp(:, $)), s(:, $));
assert_checkequal(size(sp(:, $)), size(s(:, $)));




