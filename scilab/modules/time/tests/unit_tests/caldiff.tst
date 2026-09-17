// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// unit tests for caldiff function
// =============================================================================

// row vector
dt = datetime(2024, 1, [1 2 4 7]);
c = caldiff(dt);
expected = caldays([1 2 3]);
assert_checkequal(c, expected);

// column vector
dt = datetime(2024, 1, [1; 3; 6; 10]);
c = caldiff(dt);
expected = caldays([2; 3; 4]);
assert_checkequal(c, expected);

// dim = 1
dt = datetime(2024, 1, [1 10; 3 20; 6 40]);
c = caldiff(dt, "days", 1);
expected = caldays([2 10; 3 20]);
assert_checkequal(c, expected);

// dim = 2
dt = datetime(2024, [1 1 1;2 2 2], [1 3 6;10 20 40]);
c = caldiff(dt, "days", 2);
expected = caldays([2 3; 10 20]);
assert_checkequal(c, expected);

// dim = "r" and dim = "c"
dt = datetime(2024, 1, [1 2 4; 10 20 30]);
c1 = caldiff(dt, "days", "r");
c2 = caldiff(dt, "days", 1);
assert_checktrue(c1 == c2);

c1 = caldiff(dt, "days", "c");
c2 = caldiff(dt, "days", 2);
assert_checktrue(c1 == c2);

// dim = "*"
dt = datetime(2024, 1, [1 2; 4 7]);
c = caldiff(dt, "days", "*");
expected = caldays([3 -2 5]);
assert_checkequal(c, expected);

// identical dates
dt = datetime(2024, 1, [1 1 1]);
c = caldiff(dt);
expected = caldays([0 0]);
assert_checkequal(c, expected);

// with "years"
dt = [datetime(2020, 1, 1)
      datetime(2021, 1, 1)
      datetime(2023, 1, 1)];

c = caldiff(dt, "years");
expected = calyears([1; 2]);
assert_checkequal(c, expected);

dt = [datetime(2020, 6, 15)
      datetime(2021, 6, 14)];

c = caldiff(dt, "years");
expected = calyears(0);
assert_checkequal(c, expected);

dt = [datetime(2020, 6, 15)
      datetime(2021, 6, 15)];

c = caldiff(dt, "years");
expected = calyears(1);
assert_checkequal(c, expected);

// with "months"
dt = [datetime(2024, 1, 15)
      datetime(2024, 2, 15)
      datetime(2024, 5, 15)];

c = caldiff(dt, "months");
expected = calmonths([1; 3]);
assert_checkequal(c, expected);

dt = [datetime(2024, 1, 15)
      datetime(2024, 2, 14)];

c = caldiff(dt, "months");
expected = calmonths(0);
assert_checkequal(c, expected);

dt = [datetime(2024, 1, 31); datetime(2024, 2, 29)];
c = caldiff(dt);
expected = calendarDuration(0, 1, 0, duration(0));
assert_checkequal(c, expected);

dt = datetime(2024, [2; 2; 3], [28; 29; 1]);
c = caldiff(dt, "days");
expected = caldays([1; 1]);
assert_checkequal(c, expected);

dt = [datetime(2024, 1, 1, 10, 30, 0);
      datetime(2024, 1, 1, 12, 0, 0)];
c = caldiff(dt, "time");
expected = calendarDuration(0, 0, 0, hours(1.5));
assert_checkequal(c, expected);

dt = [datetime(2020, 1, 15, 10, 30, 0)
      datetime(2021, 3, 20, 12, 45, 0)];
c = caldiff(dt);
expected = calendarDuration(1, 2, 5, hours(2.25));
assert_checkequal(c, expected);

dt = [datetime(2024, 5, 15); datetime(2024, 3, 10)];
c = caldiff(dt);
expected = calendarDuration(0, -2, -5);
assert_checkequal(c, expected);

dt = [datetime(2024, 6, 15); datetime(2022, 6, 15)];
c = caldiff(dt, "years")
expected = calyears(-2);
assert_checkequal(c, expected);

dt = [datetime(2024, 1, 1);
     NaT();
     datetime(2024, 1, 3)];
c = caldiff(dt);
assert_checkequal(c, caldays([%nan; %nan]));

dt = [datetime(2024, 1, 1);
      datetime(2024, 1, 3)];
c1 = caldiff(dt, "DAYS");
c2 = caldiff(dt, "days");
assert_checktrue(c1 == c2);

// with "m"
dt = datetime(2024, 1, [1 2 4; 10 20 40]);
c = caldiff(dt, "days", "m");
expected = caldays([9 18 36]);
assert_checkequal(c, expected);

dt = [datetime(2020, 1, 15); datetime(2021, 3, 20); datetime(2022, 6, 25)];
c = caldiff(dt);
assert_checktrue(dt(1) + c(1) == dt(2));
assert_checktrue(dt(2) + c(2) == dt(3));

dt = [datetime(2024, 1, 1, 12, 0, 0) datetime(2024, 1, 2, 11, 59, 0)];
c = caldiff(dt, "days");
expected = caldays(0);
assert_checkequal(c, expected);

// error
msg = msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"), "caldiff", 1, 3);
assert_checkerror("caldiff()", msg);

msg = msprintf(_("%s: Wrong type for input argument #%d: Must be in %s.\n"), "caldiff", 1, sci2exp("datetime"));
assert_checkerror("caldiff(1)", msg);

msg = msprintf(_("%s: Wrong type for input argument #%d: Must be in %s.\n"), "caldiff", 2, sci2exp("string"));
assert_checkerror("caldiff(dt,1)", msg);
