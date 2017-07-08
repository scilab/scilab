// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// Current date
assert_checkequal(execstr("weekday()", "errcatch"),0);
assert_checkequal(execstr("weekday(""long"")", "errcatch"),0);
assert_checkequal(execstr("weekday(""short"")", "errcatch"),0);
assert_checkequal(execstr("weekday(""long"",""ru_RU"")", "errcatch"),0);
assert_checkequal(execstr("weekday(""ru_RU"")", "errcatch"),0);
assert_checkequal(execstr("weekday(""ru_RU"", ""long"")", "errcatch"),0);

// Checking output sizes
[n, t] = weekday();
assert_checkequal(size(n), [1 1]);
assert_checkequal(size(t), [1 1]);
[n, t] = weekday(740000);
assert_checkequal(size(n), [1 1]);
assert_checkequal(size(t), [1 1]);
[n, t] = weekday([740000 ; 740008]);
assert_checkequal(size(n), [2 1]);
assert_checkequal(size(t), [2 1]);
[n, t] = weekday([740000  740008]);
assert_checkequal(size(n), [1 2]);
assert_checkequal(size(t), [1 2]);
[n, t] = weekday([740000 ; 740008], "long");
assert_checkequal(size(n), [2 1]);
assert_checkequal(size(t), [2 1]);
[n, t] = weekday([740000  740008], "long");
assert_checkequal(size(n), [1 2]);
assert_checkequal(size(t), [1 2]);
[n, t] = weekday([740000  740008], "ru_RU");
assert_checkequal(size(n), [1 2]);
assert_checkequal(size(t), [1 2]);
[n, t] = weekday([740000  740008], "ru_RU", "long");
assert_checkequal(size(n), [1 2]);
assert_checkequal(size(t), [1 2]);

assert_checkequal(size(weekday([2015 6 11])), [1 1]);
assert_checkequal(size(weekday([2015 2015]', [2 10]', [21 3]')), [2 1]);
assert_checkequal(size(weekday([2015 2015], [2 10], [21 3])), [1 2]);

[n0, t0] = weekday(["9/7/2017" "17/7/2017" "25/7/2017"]);
assert_checkequal(n0, [1 2 3]);
[n, t] = weekday(["09/7/2017" "17/07/2017" "25/07/2017"]);
assert_checkequal(n, n0);
assert_checkequal(t, t0);
[n0, t0] = weekday("en"+["7/9/2017" "7/17/2017" "7/25/2017"]);
assert_checkequal(n0, [1 2 3]);
[n, t] = weekday("en"+["7/09/2017" "07/17/2017" "07/25/2017"]);
assert_checkequal(n, n0);
assert_checkequal(t, t0);
[n, t] = weekday(["2017-07-09" "2017-07-17" "2017-07-25"]);
assert_checkequal(n, n0);
assert_checkequal(t, t0);
[n, t] = weekday(["2017-07-09 14:27:08.2" "2017-07-17 abcd" "2017-07-25"]);
assert_checkequal(n, n0);
assert_checkequal(t, t0);
[n, t] = weekday(["9-Jul-2017" "17-Jul-2017" "25-Jul-2017"]);
assert_checkequal(n, n0);
assert_checkequal(t, t0);
d = [ 2017  7  9
      2017  7  17
      2017  7  25
    ];
[n, t] = weekday(d);
assert_checkequal(n, n0(:));
assert_checkequal(t, t0(:));
[n, t] = weekday([736885 736893 736901]);
assert_checkequal(n, n0);
assert_checkequal(t, t0);

// Options
// -------
[n0, t] = weekday(["9/7/2017" "17/7/2017" ; "25/7/2017" "31/07/2017"],"fr_FR");
assert_checkequal(n0, [1 2 ; 3 2]);
assert_checkequal(t, ["Dim." "Lun." ; "Mar." "Lun."]);
[n, t] = weekday(["9/7/2017" "17/7/2017" ; "25/7/2017" "31/07/2017"],"fr_FR","long");
assert_checkequal(n, n0);
assert_checkequal(t, ["Dimanche" "Lundi" ; "Mardi" "Lundi"]);
[n, t] = weekday(["9/7/2017" "17/7/2017" ; "25/7/2017" "31/07/2017"],"long","fr_FR");
assert_checkequal(n, n0);
assert_checkequal(t, ["Dimanche" "Lundi" ; "Mardi" "Lundi"]);
lang = getlanguage();
[n0, t0] = weekday(["9/7/2017" "17/7/2017" ; "25/7/2017" "31/07/2017"]);
[n, t] = weekday(["9/7/2017" "17/7/2017" ; "25/7/2017" "31/07/2017"], lang);
assert_checkequal(n, n0);
assert_checkequal(t, t0);
[n, t] = weekday(["9/7/2017" "17/7/2017" ; "25/7/2017" "31/07/2017"],"en_US","long");
assert_checkequal(t, ["Sunday" "Monday" ; "Tuesday" "Monday"]);
[n, t] = weekday(["9/7/2017" "17/7/2017" ; "25/7/2017" "31/07/2017"],"long","ru_RU");
assert_checkequal(t, ["Воскресенье" "Понедельник" ; "Вторник" "Понедельник"]);

