// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// unit tests for between function
// =============================================================================

// same datetime
b = between(datetime(2026,1,1), datetime(2026,1,1));
expected = caldays(0);
assert_checkequal(b, expected);

// one day forward
b = between(datetime(2026,1,1), datetime(2026,1,2));
expected = caldays(1);
assert_checkequal(b, expected);

// one day backward
b = between(datetime(2026,1,2), datetime(2026,1,1));
expected = caldays(-1);
assert_checkequal(b, expected);

// one month forward
b = between(datetime(2026,1,1), datetime(2026,2,1));
expected = calmonths(1);
assert_checkequal(b, expected);

// one month backward
b = between(datetime(2026,2,1), datetime(2026,1,1));
expected = calmonths(-1);
assert_checkequal(b, expected);

// one year forward
b = between(datetime(2026,1,1), datetime(2027,1,1));
expected = calyears(1);
assert_checkequal(b, expected);

// one year backward
b = between(datetime(2027,1,1), datetime(2026,1,1));
expected = calyears(-1);
assert_checkequal(b, expected);


// 30-Apr-2026 -> 31-July-2026
d1 = datetime(2026,4,30);
d2 = datetime(2026,7,31);
res = between(d1,d2);
assert_checkequal(res, calendarDuration(0, 3, 1));
assert_checktrue(d1+res == d2);

// 31-Jan -> 30-Mar
d1 = datetime(2026,1,31);
d2 = datetime(2026,3,30);
res = between(d1, d2);
assert_checkequal(res, calendarDuration(0, 1, 30));
assert_checktrue(d1+res == d2);

// 30-Mar -> 31-Jan
res = between(d2, d1);
assert_checkequal(res, calendarDuration(0, -1, -28));
assert_checktrue(d2+res == d1);

// 31-Jan -> 28-Feb
d1 = datetime(2026,1,31);
d2 = datetime(2026,2,28);
res = between(d1, d2);
assert_checkequal(res, calmonths(1));
assert_checktrue(d1+res == d2);

// 28-Feb -> 31-Jan
res = between(d2, d1);
assert_checkequal(res, caldays(-28));
assert_checktrue(d2+res == d1);

// 30-Jan -> 28-Feb
d1 = datetime(2026,1,30);
d2 = datetime(2026,2,28);
res = between(d1, d2);
assert_checkequal(res, calmonths(1));
assert_checktrue(d1+res == d2);

// 28-Feb -> 30-Jan
res = between(d2, d1);
assert_checkequal(res, caldays(-29));
assert_checktrue(d2+res == d1);

// 31-Jan -> 31-Mar
d1 = datetime(2026,1,31);
d2 = datetime(2026,3,31);
res = between(d1, d2);
assert_checkequal(res, calmonths(2));
assert_checktrue(d1+res == d2);

// 31-Mar -> 31-Jan
res = between(d2, d1);
assert_checkequal(res, calmonths(-2));
assert_checktrue(d2+res == d1);

// 31-Jan -> 30-Apr
d2 = datetime(2026,4,30);
res = between(d1, d2);
assert_checkequal(res, calmonths(3));
assert_checktrue(d1+res == d2);

// 30-Apr -> 31-Jan
res = between(d2, d1);
assert_checkequal(res, calendarDuration(0,-2,-28));
assert_checktrue(d2+res == d1);

// "31-Mar -> 30-Apr"
d1 = datetime(2026,3,31);
res = between(d1, d2);
assert_checkequal(res, calmonths(1));
assert_checktrue(d1+res == d2);

// "30-Apr -> 31-Mar"
res = between(d2, d1);
assert_checkequal(res, caldays(-30));
assert_checktrue(d2+res == d1);

// 31-May -> 30-Jun
d1 = datetime(2026,5,31);
d2 = datetime(2026,6,30);
res = between(d1, d2);
assert_checkequal(res, calmonths(1));
assert_checktrue(d1+res == d2);

// 30-Jun -> 31-May
res = between(d2, d1);
assert_checkequal(res, caldays(-30));
assert_checktrue(d2+res == d1);

// 31-Jul -> 30-Sep
d1 = datetime(2026,7,31);
d2 = datetime(2026,9,30);
res = between(d1, d2);
assert_checkequal(res, calmonths(2));
assert_checktrue(d1+res == d2);

// 30-Sep -> 31-Jul
res = between(d2, d1);
assert_checkequal(res, calendarDuration(0,-1,-30));
assert_checktrue(d2+res == d1);

// 29-Feb-2020 -> 28-Feb-2021
d1 = datetime(2020,2,29);
d2 = datetime(2021,2,28);
res = between(d1, d2);
assert_checkequal(res, calyears(1));
assert_checktrue(d1+res == d2);

// 28-Feb-2021 -> 29-Feb-2020
res = between(d2, d1);
assert_checkequal(res, calendarDuration(0, -11, -28));
assert_checktrue(d2+res == d1);

// 29-Feb-2020 -> 29-Feb-2024
d1 = datetime(2020,2,29);
d2 = datetime(2024,2,29);
res = between(d1, d2);
assert_checkequal(res, calyears(4));
assert_checktrue(d1+res == d2);

// 29-Feb-2024 -> 29-Feb-2020
res = between(d2, d1);
assert_checkequal(res, calyears(-4));
assert_checktrue(d2+res == d1);

// 28-Feb-2019 -> 29-Feb-2020
d1 = datetime(2019,2,28);
d2 = datetime(2020,2,29);
res = between(d1, d2);
assert_checkequal(res, calyears(1)+caldays(1));
assert_checktrue(d1+res == d2);

// 29-Feb-2020 -> 28-Feb-2019
res = between(d2, d1);
assert_checkequal(res, calyears(-1));
assert_checktrue(d2+res == d1);

// 28-Feb-2099 -> 28-Feb-2100
d1 = datetime(2099,2,28);
d2 = datetime(2100,2,28);
res = between(d1, d2);
assert_checkequal(res, calyears(1));
assert_checktrue(d1+res == d2);

// 28-Feb-1999 -> 29-Feb-2000
d1 = datetime(1999,2,28);
d2 = datetime(2000,2,29);
res = between(d1, d2);
assert_checkequal(res, calyears(1)+caldays(1));
assert_checktrue(d1+res == d2);

// 31-Dec -> 1-Jan
d1 = datetime(2025,12,31);
d2 = datetime(2026,1,1);
res = between(d1, d2);
assert_checkequal(res, caldays(1));
assert_checktrue(d1+res == d2);

// 1-Jan -> 31-Dec
res = between(d2, d1);
assert_checkequal(res, caldays(-1));
assert_checktrue(d2+res == d1);

// 30-Nov -> 28-Feb
d1 = datetime(2025,11,30);
d2 = datetime(2026,2,28);
res = between(d1, d2);
assert_checkequal(res, calmonths(3));
assert_checktrue(d1+res == d2);

// 28-Feb -> 30-Nov"
res = between(d2, d1);
assert_checkequal(res, calendarDuration(0,-2,-28));
assert_checktrue(d2+res == d1);

// 31-Dec-2024 -> 31-Dec-2025
d1 = datetime(2024,12,31);
d2 = datetime(2025,12,31);
res = between(d1, d2);
assert_checkequal(res, calyears(1));
assert_checktrue(d1+res == d2);

// 31-Dec-2025 -> 31-Dec-2024
res = between(d2, d1);
assert_checkequal(res, calyears(-1));
assert_checktrue(d2+res == d1);

b = between(datetime(2026,1,1,0,0,0), datetime(2026,1,1,12,0,0));
assert_checkequal(b, calendarDuration(0,0,0,hours(12)));

b = between(datetime(2026,1,1,12,0,0), datetime(2026,1,1,0,0,0));
assert_checkequal(b, calendarDuration(0,0,0,hours(-12)));

b = between(datetime(2026,1,1,10,30,0), datetime(2026,1,2,14,45,30));
assert_checkequal(b, calendarDuration(0,0,1,duration(4, 15, 30)));

b = between(datetime(2026,1,2,14,45,30), datetime(2026,1,1,10,30,0));
assert_checkequal(b, calendarDuration(0,0,-1,duration(-4, -15, -30)));

b = between(datetime(2026,1,1,23,59,59), datetime(2026,1,2,0,0,0));
assert_checkequal(b, calendarDuration(0,0,0,seconds(1)));

b = between(datetime(2026,1,2,0,0,0), datetime(2026,1,1,23,59,59));
assert_checkequal(b, calendarDuration(0,0,0,seconds(-1)));

b = between(datetime(2026,3,30,10,0,0), datetime(2026,1,31,9,0,0));
assert_checkequal(b, calendarDuration(0,-1,-28,hours(-1)));

b = between(datetime(2026,1,31,9,0,0), datetime(2026,3,30,10,0,0));
assert_checkequal(b, calendarDuration(0,1,30,hours(1)));


// years mode
d1 = datetime(2020,1,1);
d2 = datetime(2022,4,1);
b = between(d1, d2, "years");
assert_checkequal(b, calyears(2));

// months mode
b = between(d1, d2, "months");
assert_checkequal(b, calmonths(27));

// days mode
d1 = datetime(2026,1,1);
d2 = datetime(2026,1,31);
b = between(d1, d2, "days");
assert_checkequal(b, caldays(30));

// Case-insensitivity of c
d1 = datetime(2026,1,1);
d2 = datetime(2026,2,1);
b = between(d1, d2, "YEARS");
assert_checkequal(b, calyears(0));

b = between(d1, d2, "MoNths");
assert_checkequal(b, calmonths(1));

b = between(d1, d2, "DAYS");
assert_checkequal(b, caldays(31));

// Scalar expansion
d1 = datetime(2026,1,1);
d2 = [datetime(2026,2,1), datetime(2026,3,1), datetime(2026,4,1)];

res = between(d1,d2);
assert_checktrue(d1 + res == d2);

d1 = [datetime(2026,2,1), datetime(2026,3,1), datetime(2026,4,1)];
d2 = datetime(2026,1,1);

res = between(d1,d2);
assert_checktrue(d1 + res == d2);

//  Matrix input
d1 = [datetime(2026,1,1), datetime(2026,2,1);
      datetime(2026,3,1), datetime(2026,4,1)];

d2 = [datetime(2026,2,1), datetime(2026,3,1);
      datetime(2026,4,1), datetime(2026,5,1)];

res = between(d1,d2);
assert_checktrue(d1 + res == d2);

// NaT
d1 = NaT();
d2 = datetime(2026,2,1);

res = between(d1,d2);
assert_checkequal(res, calendarDuration(%nan));

// large intervals
b = between(datetime(1900,1,1), datetime(2100,1,1));
assert_checkequal(b, calyears(200));

b = between(datetime(2100,1,1), datetime(1900,1,1));
assert_checkequal(b, calyears(-200));

b = between(datetime(1900,2,28),datetime(2100,2,28));
assert_checkequal(b, calyears(200));

// error
msg = msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"), "between", 2, 3);
assert_checkerror("between(1)", msg);

msg = msprintf(_("%s: Wrong type for input argument #%d: Must be in %s.\n"), "between", 1, sci2exp("datetime"));
assert_checkerror("between(1,1)", msg);

msg = msprintf(_("%s: Wrong type for input argument #%d: Must be in %s.\n"), "between", 2, sci2exp("datetime"));
assert_checkerror("between(d2,1)", msg);

msg = msprintf(_("%s: Wrong size for input argument #%d: Must be of the same dimensions of #%d or scalar.\n"), "between", 2, 1);
assert_checkerror("between([d2, d2],[d2 d2 d2])", msg);
