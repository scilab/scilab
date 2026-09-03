// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2022 - Dassault Systèmes S.E. - Adeline CARNIS
// Copyright (C) 2022 - Dassault Systèmes S.E. - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

function checkdatetime1(dt, d, t)
    assert_checkequal(dt.date, d);
    assert_checkequal(dt.time, t);
endfunction

function checkdatetime2(dt, y, m, d, t)

    dexptected = datenum(y, m, d);
    assert_checkequal(dt.date, dexptected);

    ti = dt.time;
    assert_checkequal(ti, t.duration/1000)

endfunction

function checkstring(d, v)
    assert_checkequal(%datetime_string(d), v);
endfunction

d = datetime();
assert_checktrue(modulo(d.time, 1) >= 0);
d = datetime("now");
assert_checktrue(modulo(d.time, 1) >= 0);
expected = floor(datenum());
checkdatetime1(datetime("today"), expected, 0);
checkdatetime1(datetime("yesterday"), expected - 1, 0);
checkdatetime1(datetime("tomorrow"), expected + 1, 0);

d0 = duration(0,0,0);
d1 = d0 * zeros(1, 5);
d2 = d1';
d3 = d0 * zeros(2, 2);
d10 = d0 * zeros(1, 10);
d20 = d10';

d4 = duration(12, 30, 45);
d5 = d4 * ones(1,5);
d6 = d5';
d7 = d4 * ones(2, 2);

// datetime(datestrings)
checkdatetime2(datetime("2022-10-06"), 2022, 10, 6, d0);
checkdatetime2(datetime(["2022-10-06" "2022-10-07" "2022-10-08" "2022-10-09" "2022-10-10"]), 2022, 10, 6:10, d1);
checkdatetime2(datetime(["2022-10-06" "2022-10-07" "2022-10-08" "2022-10-09" "2022-10-10"]'), 2022, 10, (6:10)', d2);
checkdatetime2(datetime(["2022-10-06" "2022-10-07"; "2022-10-08" "2022-10-09"]), 2022 * ones(2, 2), 10 * ones(2, 2), [6 7; 8 9], d3);

checkdatetime2(datetime("2022-10-06 12:30:45"), 2022, 10, 6, d4);
checkdatetime2(datetime(["2022-10-06 12:30:45" "2022-10-07 12:30:45" "2022-10-08 12:30:45" "2022-10-09 12:30:45" "2022-10-10 12:30:45"]), 2022, 10, 6:10, d5);
checkdatetime2(datetime(["2022-10-06 12:30:45" "2022-10-07 12:30:45" "2022-10-08 12:30:45" "2022-10-09 12:30:45" "2022-10-10 12:30:45"]'), 2022, 10, (6:10)', d6);
checkdatetime2(datetime(["2022-10-06 12:30:45" "2022-10-07 12:30:45"; "2022-10-08 12:30:45" "2022-10-09 12:30:45"]), 2022 * ones(2, 2), 10 * ones(2, 2), [6 7; 8 9], d7);

// datetime(datestrings, "InputFormat", "infmt")
stry = ["yyyy", "yy"];
strm = ["MMMM", "MMM", "MM", "M"];
strd = ["dd", "d"];
valy = ["2022", "22"];
valm = ["October", "Oct", "10", "10"];
vald = ["06" "6"];

for i = 1:2
    y = stry(i);
    vy = valy(i);
    for j = 1:4
        m = strm(j);
        vm = valm(j);
        for k = 1:2
            d = strd(k);
            vd = vald(k);

            fmt = y + "-" + m + "-" + d;
            str = vy + "-" + vm + "-" + vd;
            checkdatetime2(datetime(str, "InputFormat", fmt), 2022, 10, 6, d0);

            fmt = d + "/" + m + "/" + y;
            str = vd + "/" + vm + "/" + vy;
            checkdatetime2(datetime(str, "InputFormat", fmt), 2022, 10, 6, d0);

            fmt = d + "." + m + "." + y;
            str = vd + "." + vm + "." + vy;
            checkdatetime2(datetime(str, "InputFormat", fmt), 2022, 10, 6, d0);

            fmt = m + " " + d + ", " + y;
            str = vm + " " + vd + ", " + vy;
            checkdatetime2(datetime(str, "InputFormat", fmt), 2022, 10, 6, d0);
        end
    end
end

strTime = " " + ["HH:mm:ss", "HH:mm:ss.SSS", "H:mm:ss", "H:mm:ss.SSS", "hh:mm:ss a" "h:mm:ss a"];
valTime = " " + ["09:45:30", "09:45:30.000", "9:45:30", "9:45:30.000", "09:45:30 am", "9:45:30 am"];

dura = duration(9, 45, 30);
for i = 1:2
    y = stry(i);
    vy = valy(i);
    for j = 1:4
        m = strm(j);
        vm = valm(j);
        for k = 1:2
            d = strd(k);
            vd = vald(k);
            for l = 1:4
                fmt = y + "-" + m + "-" + d + strTime(l);
                str = vy + "-" + vm + "-" + vd + valTime(l);
                checkdatetime2(datetime(str, "InputFormat", fmt), 2022, 10, 6, dura);

                fmt = d + "/" + m + "/" + y + strTime(l);
                str = vd + "/" + vm + "/" + vy + valTime(l);
                checkdatetime2(datetime(str, "InputFormat", fmt), 2022, 10, 6, dura);

                fmt = d + "." + m + "." + y + strTime(l);
                str = vd + "." + vm + "." + vy + valTime(l);
                checkdatetime2(datetime(str, "InputFormat", fmt), 2022, 10, 6, dura);

                fmt = m + " " + d + ", " + y + strTime(l);
                str = vm + " " + vd + ", " + vy + valTime(l);
                checkdatetime2(datetime(str, "InputFormat", fmt), 2022, 10, 6, dura);
            end
        end
    end
end

//dura6 = dura1 + milliseconds(300);
//dura7 = dura6 * ones(5, 1);
//dura8 = dura6 * ones(1, 5);
// datetime(datevectors)
checkdatetime2(datetime([2022 10 06]), 2022, 10, 6, d0);
checkdatetime2(datetime([2022 10 06; 2022 10 07; 2022 10 08; 2022 10 09; 2022 10 10]), 2022, 10, (6:10)', d2);

checkdatetime2(datetime([2022 10 06 12 30 45]), 2022, 10, 6, d4);
checkdatetime2(datetime([2022 10 06 12 30 45; 2022 10 07 12 30 45; 2022 10 08 12 30 45; 2022 10 09 12 30 45; 2022 10 10 12 30 45]), 2022, 10, (6:10)', d6);

// datetime(Y, M, D), datetime(Y, M, D, H, MI, S), datetime(Y, M, D, H, MI, S, MS)
checkdatetime2(datetime(2022, 10, 6), 2022, 10, 6, d0);
checkdatetime2(datetime(2022, 10, 6:10), 2022, 10, 6:10, d1);
checkdatetime2(datetime(2022, 10, (6:10)'), 2022, 10, (6:10)', d2);
checkdatetime2(datetime(2022, 10, [6 7; 8 9]), 2022 * ones(2, 2), 10 * ones(2, 2), [6 7; 8 9], d3);

checkdatetime2(datetime(2022, 10, 6, 12, 30, 45), 2022, 10, 6, d4);
checkdatetime2(datetime(2022, 10, 6:10, 12, 30, 45), 2022, 10, (6:10), d5);
checkdatetime2(datetime(2022, 10, (6:10)', 12, 30, 45), 2022, 10, (6:10)', d6);
checkdatetime2(datetime(2022, 10, [6 7; 8 9], 12, 30, 45), 2022 * ones(2, 2), 10 * ones(2, 2), [6 7; 8 9], d7);

//checkdatetime2(datetime(2022, 10, 6, 12, 30, 45, 300), 2022, 10, 6, dura6);
//checkdatetime2(datetime(2022, 10, 6:10, 12, 30, 45, 300), 2022, 10, (6:10), dura8);
//checkdatetime2(datetime(2022, 10, (6:10)', 12, 30, 45, 300), 2022, 10, (6:10)', dura7);
//checkdatetime2(datetime(2022, 10, [6 7; 8 9], 12, 30, 45, 300), 2022, 10, [6 7; 8 9], dura6 * ones(2, 2));

// datetime(x, "ConvertFrom", dateType)
d = datetime(datenum(), "ConvertFrom", "datenum", "OutputFormat", "yyyy-MM-dd HH:mm:ss");
dexpected = datetime("now", "OutputFormat", "yyyy-MM-dd HH:mm:ss");
assert_checktrue(string(d) == string(dexpected));
assert_checktrue(modulo(d.time, 1) >= 0);
checkstring(datetime([44819.3834418981 44819.3834418981;44819.3834418981 44819.3834418981], "ConvertFrom", "excel"), ["2022-09-15 09:12:09.380" "2022-09-15 09:12:09.380"; "2022-09-15 09:12:09.380" "2022-09-15 09:12:09.380"]);

// datetime([1663226303.936;1663226303.936], "ConvertFrom", "posixtime") == ["2022-09-15 09:18:23.936"; "2022-09-15 09:18:23.936"]
// linked to timeZone
dt = datetime([1663226303.936;1663226303.936], "ConvertFrom", "posixtime");
assert_checkequal(dt.Year, [2022; 2022]);
assert_checkequal(dt.Month, [9; 9]);
assert_checkequal(dt.Day, [15; 15]);
checkstring(datetime([20221006, 20221007; 20221008 20221009], "ConvertFrom", "yyyymmdd"), ["2022-10-06" "2022-10-07"; "2022-10-08" "2022-10-09"]);

// Operations
// datetime + val, val + datetime
checkdatetime2(datetime("2022-10-06") + 2, 2022, 10, 8, d0);
checkdatetime2(2 + datetime("2022-10-06"), 2022, 10, 8, d0);
checkdatetime2(datetime("2022-10-06") + [1:10], 2022, 10, 6 + (1:10), d10);
checkdatetime2([1:10] + datetime("2022-10-06"), 2022, 10, 6 + (1:10), d10);
checkdatetime2(datetime("2022-10-06") + [1:10]', 2022, 10, 6 + (1:10)', d20);
checkdatetime2([1:10]' + datetime("2022-10-06"), 2022, 10, 6 + (1:10)', d20);
checkdatetime2(datetime(2022, 10, 6:10) + 2, 2022, 10, (6:10) + 2, d1);
checkdatetime2(datetime(2022, 10, (6:10)') + 2, 2022, 10, (6:10)' + 2, d2);
checkdatetime2(2 + datetime(2022, 10, 6:10), 2022, 10, (6:10) + 2, d1);
checkdatetime2(2 + datetime(2022, 10, (6:10)'), 2022, 10, (6:10)' + 2, d2);
checkdatetime2(datetime(2022, 10, 6:10) + (6:10), 2022, 10, (6:10) + (6:10), d1);
checkdatetime2(datetime(2022, 10, (6:10)') + (6:10)', 2022, 10, (6:10)' + (6:10)', d2);
checkdatetime2((6:10) + datetime(2022, 10, 6:10), 2022, 10, (6:10) + (6:10), d1);
checkdatetime2((6:10)' + datetime(2022, 10, (6:10)'), 2022, 10, (6:10)' + (6:10)', d2);

// datetime + duration, duration + datetime
dh = hours(2);
dh5 = dh * ones(1, 5);
dh6 = dh5';
dh1 = hours(1:10);
dh2 = dh1';
dhv = hours(6:10);
dhvt = dhv';
checkdatetime2(datetime("2022-10-06") + dh, 2022, 10, 6, dh);
checkdatetime2(dh + datetime("2022-10-06"), 2022, 10, 6, dh);
checkdatetime2(datetime("2022-10-06") + dh1, 2022, 10, 6 * ones(1, 10), dh1);
checkdatetime2(dh1 + datetime("2022-10-06"), 2022, 10, 6 * ones(1, 10), dh1);
checkdatetime2(datetime("2022-10-06") + dh2, 2022, 10, 6 * ones(10, 1), dh2);
checkdatetime2(dh2 + datetime("2022-10-06"), 2022, 10, 6 * ones(10, 1), dh2);
checkdatetime2(datetime(2022, 10, 6:10) + dh, 2022, 10, (6:10), dh5);
checkdatetime2(datetime(2022, 10, (6:10)') + dh, 2022, 10, (6:10)', dh6);
checkdatetime2(dh + datetime(2022, 10, 6:10), 2022, 10, (6:10), dh5);
checkdatetime2(dh + datetime(2022, 10, (6:10)'), 2022, 10, (6:10)', dh6);
checkdatetime2(datetime(2022, 10, 6:10) + dhv, 2022, 10, (6:10), dhv);
checkdatetime2(datetime(2022, 10, (6:10)') + dhvt, 2022, 10, (6:10)', dhvt);
checkdatetime2(dhv + datetime(2022, 10, 6:10), 2022, 10, (6:10), dhv);
checkdatetime2(dhvt + datetime(2022, 10, (6:10)'), 2022, 10, (6:10)', dhvt);

// datetime + calendarDuration, calendarDuration + datetime
checkdatetime2(datetime("2022-10-06") + caldays(2), 2022, 10, 8, d0);
checkdatetime2(caldays(2) + datetime("2022-10-06"), 2022, 10, 8, d0);
checkdatetime2(datetime("2022-10-06") + caldays([1:10]), 2022, 10, 6 + (1:10), d10);
checkdatetime2(caldays([1:10]) + datetime("2022-10-06"), 2022, 10, 6 + (1:10), d10);
checkdatetime2(datetime("2022-10-06") + caldays([1:10]'), 2022, 10, 6 + (1:10)', d20);
checkdatetime2(caldays([1:10]') + datetime("2022-10-06"), 2022, 10, 6 + (1:10)', d20);
checkdatetime2(datetime(2022, 10, 6:10) + caldays(2), 2022, 10, (6:10) + 2, d1);
checkdatetime2(datetime(2022, 10, (6:10)') + caldays(2), 2022, 10, (6:10)' + 2, d2);
checkdatetime2(caldays(2) + datetime(2022, 10, 6:10), 2022, 10, (6:10) + 2, d1);
checkdatetime2(caldays(2) + datetime(2022, 10, (6:10)'), 2022, 10, (6:10)' + 2, d2);
checkdatetime2(datetime(2022, 10, 6:10) + caldays(6:10), 2022, 10, (6:10) + (6:10), d1);
checkdatetime2(datetime(2022, 10, (6:10)') + caldays(6:10)', 2022, 10, (6:10)' + (6:10)', d2);
checkdatetime2(caldays(6:10) + datetime(2022, 10, 6:10), 2022, 10, (6:10) + (6:10), d1);
checkdatetime2(caldays(6:10)' + datetime(2022, 10, (6:10)'), 2022, 10, (6:10)' + (6:10)', d2);

checkstring(datetime(2023, 1, 29) + caldays(29), "2023-02-27");
checkstring(datetime(2023, 1, 29) + caldays(30), "2023-02-28");

checkstring(datetime(2023, 1, 30) + caldays(29), "2023-02-28");
checkstring(datetime(2023, 1, 30) + caldays(30), "2023-03-01");

checkstring(datetime(2023, 1, 31) + caldays(29), "2023-03-01");
checkstring(datetime(2023, 1, 31) + caldays(30), "2023-03-02");

checkstring(datetime(2024, 1, 29) + caldays(29), "2024-02-27");
checkstring(datetime(2024, 1, 29) + caldays(30), "2024-02-28");

checkstring(datetime(2024, 1, 30) + caldays(29), "2024-02-28");
checkstring(datetime(2024, 1, 30) + caldays(30), "2024-02-29");

checkstring(datetime(2024, 1, 31) + caldays(29), "2024-02-29");
checkstring(datetime(2024, 1, 31) + caldays(30), "2024-03-01");

checkstring(datetime(2023, 3, 1) - caldays(29), "2023-01-31");
checkstring(datetime(2023, 3, 1) - caldays(30), "2023-01-30");

checkstring(datetime(2024, 3, 1) - caldays(29), "2024-02-01");
checkstring(datetime(2024, 3, 1) - caldays(30), "2024-01-31");

// datetime - datetime
computed = datetime("2022-10-6") - datetime("2022-10-5");
assert_checktrue(computed == days(1));
computed = datetime("2022-10-5") - datetime("2022-10-6");
assert_checktrue(computed == days(1));

computed = datetime(2022, 10, 10:-1:6) - datetime("2022-10-5");
assert_checktrue(computed == days(5:-1:1));
computed = datetime(2022, 10, (10:-1:6)') - datetime("2022-10-5");
assert_checktrue(computed == days(5:-1:1)');
computed = datetime(2022, 10, 6) - datetime(2022, 10, 1:5);
assert_checktrue(computed == days(5:-1:1));
computed = datetime(2022, 10, 6) - datetime(2022, 10, 1:5)';
assert_checktrue(computed == days(5:-1:1)');
computed = datetime(2022, 10, 6:10) - datetime(2022, 10, 1:5);
assert_checktrue(computed == days(5 * ones(1, 5)));
computed = datetime(2022, 10, 6:10)' - datetime(2022, 10, 1:5)';
assert_checktrue(computed == days(5 * ones(1, 5))');

// datetime - val, val - datetime
checkdatetime2(datetime("2022-10-06") - 2, 2022, 10, 4, d0);
checkdatetime2(2 - datetime("2022-10-06"), 2022, 10, 4, d0);
checkdatetime2(datetime("2022-10-06") - [1:5], 2022, 10, 6 - (1:5), d1);
checkdatetime2([1:5] - datetime("2022-10-06"), 2022, 10, 6 - (1:5), d1);
checkdatetime2(datetime("2022-10-06") + [1:5]', 2022, 10, 6 + (1:5)', d2);
checkdatetime2([1:5]' + datetime("2022-10-06"), 2022, 10, 6 + (1:5)', d2);
checkdatetime2(datetime(2022, 10, 6:10) - 2, 2022, 10, (6:10) - 2, d1);
checkdatetime2(datetime(2022, 10, (6:10)') - 2, 2022, 10, (6:10)' - 2, d2);
checkdatetime2(2 - datetime(2022, 10, 6:10), 2022, 10, (6:10) - 2, d1);
checkdatetime2(2 - datetime(2022, 10, (6:10)'), 2022, 10, (6:10)' - 2, d2);
checkdatetime2(datetime(2022, 10, 6:10) - (1:5), 2022, 10, (6:10) - (1:5), d1);
checkdatetime2(datetime(2022, 10, (6:10)') - (1:5)', 2022, 10, (6:10)' - (1:5)', d2);
checkdatetime2((1:5) - datetime(2022, 10, 6:10), 2022, 10, (6:10) - (1:5), d1);
checkdatetime2((1:5)' - datetime(2022, 10, (6:10)'), 2022, 10, (6:10)' - (1:5)', d2);

// datetime - duration
checkdatetime2(datetime("2022-10-06") - hours(2), 2022, 10, 5, hours(22));
checkdatetime2(datetime("2022-10-06") - hours(1:10), 2022, 10, 5 * ones(1, 10), hours(24 - (1:10)));
checkdatetime2(datetime("2022-10-06") - dh2, 2022, 10, 5 * ones(10, 1), hours(24 - (1:10)'));
checkdatetime2(datetime(2022, 10, 6:10) - hours(2), 2022, 10, (6:10)-1, hours(22) * ones(1, 5));
checkdatetime2(datetime(2022, 10, (6:10)') - hours(2), 2022, 10, (6:10)'-1, hours(22) * ones(5, 1));
checkdatetime2(datetime(2022, 10, 6:10) - hours(1:5), 2022, 10, (6:10)-1, hours(24 - (1:5)));
checkdatetime2(datetime(2022, 10, (6:10)') - hours(1:5)', 2022, 10, (6:10)'-1, hours(24 - (1:5)'));

// datetime - calendarDuration
checkdatetime2(datetime("2022-10-06") - caldays(2), 2022, 10, 4, d0);
checkdatetime2(datetime("2022-10-06") - caldays([1:5]), 2022, 10, 6 - (1:5), d1);
checkdatetime2(datetime("2022-10-06") - caldays([1:5]'), 2022, 10, 6 - (1:5)', d2);
checkdatetime2(datetime(2022, 10, 6:10) - caldays(2), 2022, 10, (6:10) - 2, d1);
checkdatetime2(datetime(2022, 10, (6:10)') - caldays(2), 2022, 10, (6:10)' - 2, d2);
checkdatetime2(datetime(2022, 10, 6:10) - caldays(1:5), 2022, 10, (6:10) - (1:5), d1);
checkdatetime2(datetime(2022, 10, (6:10)') - caldays(1:5)', 2022, 10, (6:10)' - (1:5)', d2);

// datetime:duration:datetime
assert_checktrue((datetime("2022-10-06"):hours(5):datetime("2022-10-07")) == datetime(2022, 10, 06, 0:5:24, 0, 0));
assert_checktrue((datetime("2022-10-06"):hours(5):datetime("2022-10-07"))' == datetime(2022, 10, 06, (0:5:24)', 0, 0));

// datetime:caldendarDuration:datetime
assert_checktrue((datetime("2022-10-06"):caldays(5):datetime("2022-10-31")) == datetime(2022, 10, 6:5:31));
assert_checktrue((datetime("2022-10-06"):caldays(5):datetime("2022-10-31"))' == datetime(2022, 10, (6:5:31)'));

// [datetime datetime]
assert_checktrue([datetime("2022-10-06"), datetime("2022-10-07")] == datetime(2022, 10, 6:7));
assert_checktrue([datetime(2022, 10, 1), datetime(2022, 10, 2:10)] == datetime(2022, 10, 1:10));
assert_checktrue([datetime(2022, 10, 1:9), datetime(2022, 10, 10)] == datetime(2022, 10, 1:10));
assert_checktrue([datetime(2022, 10, 1:10), datetime(2022, 10, 11:20)] == datetime(2022, 10, 1:20));
assert_checktrue([datetime(2022, 10, 1:10)', datetime(2022, 10, 11:20)'] == datetime(2022, 10, [(1:10)' (11:20)']));

// [datetime; datetime]
assert_checktrue([datetime("2022-10-06"); datetime("2022-10-07")] == datetime(2022, 10, (6:7)'));
assert_checktrue([datetime(2022, 10, 1); datetime(2022, 10, 2:10)'] == datetime(2022, 10, (1:10)'));
assert_checktrue([datetime(2022, 10, 1:9)'; datetime(2022, 10, 10)] == datetime(2022, 10, (1:10)'));
assert_checktrue([datetime(2022, 10, 1:10); datetime(2022, 10, 11:20)] == datetime(2022, 10, [(1:10); (11:20)]));
assert_checktrue([datetime(2022, 10, 1:10)'; datetime(2022, 10, 11:20)'] == datetime(2022, 10, (1:20)'));

// ==
assert_checktrue(datetime(2022, (1:12)', 1) == datetime("01/" + string(1:12)' + "/2022", "InputFormat", "dd/M/yyyy"));
assert_checktrue(datetime(2022, (1:12)', 1) == "2022-" + msprintf("%02d\n", (1:12)') + "-01");
assert_checktrue("2022-" + msprintf("%02d\n", (1:12)') + "-01" == datetime(2022, (1:12)', 1));
assert_checktrue(datetime(2022, 1, 1) == datetime("01/01/2022", "InputFormat", "dd/MM/yyyy"));
assert_checktrue(datetime(2022, 1, 1) == "2022-01-01");
assert_checktrue("2022-01-01" == datetime(2022, 1, 1));

// <>
assert_checktrue(datetime(2022, (1:6)', 1) <> datetime("01/" + string(7:12)' + "/2022", "InputFormat", "dd/M/yyyy"));
assert_checktrue(datetime(2022, (1:6)', 1) <> datetime("01/07/2022", "InputFormat", "dd/M/yyyy"));
assert_checktrue(datetime(2022, 1, 1) <> datetime("01/" + string(7:12)' + "/2022", "InputFormat", "dd/M/yyyy"));
assert_checktrue(datetime(2022, (7:12)', 1) <> "2022-" + msprintf("%02d\n", (1:6)') + "-01");
assert_checktrue(datetime(2022, 1, 3) <> datetime("01/01/2022", "InputFormat", "dd/MM/yyyy"));
assert_checktrue(datetime(2022, 1, 3) <> "2022-01-01");

// >
assert_checktrue(datetime(2022, (7:12)', 1) > datetime("01/" + string(1:6)' + "/2022", "InputFormat", "dd/M/yyyy"));
assert_checktrue(datetime(2022, (2:6)', 1) > datetime("01/01/2022", "InputFormat", "dd/M/yyyy"));
assert_checktrue(datetime(2022, 12, 1) > datetime("01/" + string(3:9)' + "/2022", "InputFormat", "dd/M/yyyy"));
assert_checktrue(datetime(2022, (7:12)', 1) > "2022-" + msprintf("%02d\n", (1:6)') + "-01");
assert_checktrue(datetime(2022, 1, 3) > datetime("01/01/2022", "InputFormat", "dd/MM/yyyy"));
assert_checktrue(datetime(2022, 1, 3) > "2022-01-01");

// >=
assert_checktrue(datetime(2022, (7:12)', 1) >= datetime("01/" + string(1:6)' + "/2022", "InputFormat", "dd/M/yyyy"));
assert_checktrue(datetime(2022, (2:6)', 1) >= datetime("01/01/2022", "InputFormat", "dd/M/yyyy"));
assert_checktrue(datetime(2022, 12, 1) >= datetime("01/" + string(3:9)' + "/2022", "InputFormat", "dd/M/yyyy"));
assert_checktrue(datetime(2022, (7:12)', 1) >= "2022-" + msprintf("%02d\n", (1:6)') + "-01");
assert_checktrue(datetime(2022, 1, 3) >= datetime("01/01/2022", "InputFormat", "dd/MM/yyyy"));
assert_checktrue(datetime(2022, 1, 3) >= "2022-01-01");

// <
assert_checktrue(datetime(2022, (1:6)', 1) < datetime("01/" + string(7:12)' + "/2022", "InputFormat", "dd/M/yyyy"));
assert_checktrue(datetime(2022, (2:6)', 1) < datetime("01/08/2022", "InputFormat", "dd/M/yyyy"));
assert_checktrue(datetime(2022, 2, 1) < datetime("01/" + string(3:9)' + "/2022", "InputFormat", "dd/M/yyyy"));
assert_checktrue(datetime(2022, (1:6)', 1) < "2022-" + msprintf("%02d\n", (7:12)') + "-01");
assert_checktrue(datetime(2022, 1, 3) < datetime("15/01/2022", "InputFormat", "dd/MM/yyyy"));
assert_checktrue(datetime(2022, 1, 3) < "2022-01-15");

// <=
assert_checktrue(datetime(2022, (1:6)', 1) <= datetime("01/" + string(5:10)' + "/2022", "InputFormat", "dd/M/yyyy"));
assert_checktrue(datetime(2022, (2:6)', 1) <= datetime("01/08/2022", "InputFormat", "dd/M/yyyy"));
assert_checktrue(datetime(2022, 3, 1) <= datetime("01/" + string(3:9)' + "/2022", "InputFormat", "dd/M/yyyy"));
assert_checktrue(datetime(2022, (1:6)', 1) <= "2022-" + msprintf("%02d\n", (5:10)') + "-01");
assert_checktrue(datetime(2022, 1, 3) <= datetime("15/01/2022", "InputFormat", "dd/MM/yyyy"));
assert_checktrue(datetime(2022, 1, 3) <= "2022-01-15");


// datetime(____, "OutputFormat", oufmt)

// extraction
dt = datetime(2022, 6, 15, 12, 45, 30);
assert_checkequal(dt.Year, 2022);
assert_checkequal(dt.Month, 6);
assert_checkequal(dt.Day, 15);
assert_checkequal(dt.Hour, 12);
assert_checkequal(dt.Minute, 45);
assert_checkequal(dt.Second, 30);

// ymd
dt = datetime(2022, 6, 15, 12, 45, 30);
[y,m,d] = ymd(dt);
assert_checkequal([y,m,d], [2022, 6, 15]);

dt = datetime(2022, 6, 1:15, 12, 35:49, 30);
[y,m,d] = ymd(dt);
assert_checkequal([y;m;d], [2022*ones(1,15); 6*ones(1,15); 1:15]);

// datetime("") => NaT
assert_checkequal(datetime(""), NaT());
checkstring(datetime(["2022-12-12" ""]), ["2022-12-12", "NaT"]);
checkstring(datetime(["" "2022-12-12"]), ["NaT" "2022-12-12"]);
checkstring(datetime(["2022-12-12"; ""]), ["2022-12-12"; "NaT"]);
checkstring(datetime([""; "2022-12-12"]), ["NaT"; "2022-12-12"]);
checkstring(datetime(["2022-12-12" "" "2022-12-12" ]), ["2022-12-12", "NaT", "2022-12-12"]);
checkstring(datetime(["2022-12-12"; ""; "2022-12-12"]), ["2022-12-12"; "NaT"; "2022-12-12"]);
checkstring(datetime(["12-Dec-2022" "" "12-Dec-2022" ]), ["2022-12-12", "NaT", "2022-12-12"]);
checkstring(datetime(["12-Dec-2022"; ""; "12-Dec-2022"]), ["2022-12-12"; "NaT"; "2022-12-12"]);
checkstring(datetime(["12/12/2022" "" "12/12/2022" ], "InputFormat", "dd/MM/yyyy"), ["2022-12-12", "NaT", "2022-12-12"]);
checkstring(datetime(["12/12/2022"; ""; "12/12/2022"], "InputFormat", "dd/MM/yyyy"), ["2022-12-12"; "NaT"; "2022-12-12"]);

clear t;
t = []; t($+1) = datetime();
assert_checkequal(size(t, "*"), 1);

dt = datetime(2022,2,1,0,0,0);
c = caldays(0:28);
computed = dt + c;
expected = string(datetime(2022,2,1:29,0,0,0));
checkstring(computed, expected);

assert_checkequal(dt(1):dt($), dt(1));

dt = datetime("2/3/24", "InputFormat", "M/d/yy");
assert_checkequal(string(dt), "2024-02-03");
dt = datetime("12/3/24", "InputFormat", "M/d/yy");
assert_checkequal(string(dt), "2024-12-03");
dt = datetime("2/13/24", "InputFormat", "M/d/yy");
assert_checkequal(string(dt), "2024-02-13");
dt = datetime("12/13/24", "InputFormat", "M/d/yy");
assert_checkequal(string(dt), "2024-12-13");

h = sprintf("%02d\n", [12 1:11]')';
mn = sprintf("%02d\n", [0:15:45]')';
res = string(12:23);
fmt = "MM/dd/yyyy hh:mm:ss a";
for i = h
    for j = mn
        d = datetime("01/18/2022 " + i + ":" + j + ":00 AM", "InputFormat", fmt);
        if i == "12"
            if j == "00" then
                assert_checkequal(string(d), "2022-01-18");
            else
                assert_checkequal(string(d), "2022-01-18 00:" + j + ":00");
            end
        else
            assert_checkequal(string(d), "2022-01-18 " + i + ":" + j + ":00");
        end
    end
end
for i = 1:size(h, "*")
    for j = mn
        d = datetime("01/18/2022 " + h(i) + ":" + j + ":00 PM", "InputFormat", fmt);
        assert_checkequal(string(d), "2022-01-18 " + res(i) + ":" + j + ":00");
    end
end

for ampm = ["AM" "PM"]
    for i = h
        for j = mn
            str = "01/18/2022 " + i + ":" + j + ":00 "+ ampm;
            d = datetime(str, "InputFormat", fmt, "OutputFormat", fmt);
            assert_checkequal(string(d), str);
        end
    end
end

d = datetime("01/18/2022 00:13:00 AM", "InputFormat", fmt, "OutputFormat", fmt);
assert_checkequal(string(d), "01/18/2022 12:13:00 AM");
d = datetime("01/18/2022 00:13:00 PM", "InputFormat", fmt, "OutputFormat", fmt);
assert_checkequal(string(d), "01/18/2022 12:13:00 PM");

dt = datetime("4/5/2024", "InputFormat", "M/d/yyyy", "OutputFormat", "dd-MM-yy");
assert_checkequal(string(dt), "05-04-24");

dt = datetime("14.07.1789 14:37:54.123", "InputFormat", "dd.MM.yyyy HH:mm:ss.SSS");
assert_checkequal(string(dt), "1789-07-14 14:37:54.123");

d = ["2024-04-10 14:48"; "2024-04-10 14:49"];
dt = datetime(d, "OutputFormat", "MM/dd/yyyy hh:mm a");
expected = ["04/10/2024 02:48 PM"; "04/10/2024 02:49 PM"];
assert_checkequal(string(dt), expected);

d = "2024-04-10 14";
dt = datetime(d, "InputFormat", "yyyy-MM-dd HH");
expected = "2024-04-10 14:00:00";
assert_checkequal(string(dt), expected);

d = datetime(["April"; "May"; "June"], "InputFormat", "MMMM");
assert_checkequal(d.Month, [4; 5; 6]);

d = datetime(["Apr"; "May"; "June"], "InputFormat", "MMMM");
assert_checkequal(d.Month, [%nan; 5; 6]);

d = datetime(["Apr", "May", "January"], "InputFormat", "MMM");
assert_checkequal(d.Month, [4 5 %nan]);

d = datetime(2024,1,31);
c = calmonths([1:12]');
expected = datetime([2024*ones(11,1); 2025], [2:12 1]', [29 31 30 31 30 31 31 30 31 30 31 31]');
assert_checkequal(d+c, expected);

expected = datetime(2023, [12:-1:1]', [31 30 31 30 31 31 30 31 30 31 28 31]');
assert_checkequal(d-c, expected);

// check error
msg = msprintf(_("%s: Wrong number of input argument: %d to %d expected, except %d, %d and %d.\n"), "datetime", 0, 7, 2, 4, 5);
assert_checkerror("datetime(1, 2, 3, 4, 5, 6, 7, 8)", msg);
assert_checkerror("datetime(1, 2)", msg);
assert_checkerror("datetime(1, 2, 3, ""OutputFormat"")", msg);
msg = msprintf(_("%s: Wrong number of input argument: %d to %d expected, except %d, %d and %d.\n"), "datetime", 0, 9, 2, 4, 5);
assert_checkerror("datetime(1, 2, 3, 4, 5, 6, 7, 8, ""OutputFormat"", ""dd-MM-yyyy"")", msg);

msg = msprintf(_("%s: Wrong value for input argument #%d: %s, %s or %s expected.\n"), "datetime", 4, """InputFormat""", """OutputFormat""", """ConvertFrom""");
assert_checkerror("datetime(1, 2, 3, ""toto"", ""dd-MM-yyyy"")", msg);
msg = msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "datetime", 5);
assert_checkerror("datetime(1, 2, 3, ""OutputFormat"", 1)", msg);

msg = msprintf(_("%s: Wrong size for input argument #%d: A %d-by-%d or %d-by-%d matrix expected.\n"), "datetime", 1, 1, 3, 1, 6);
assert_checkerror("datetime([1 2])", msg);
assert_checkerror("datetime([1 2 3 4])", msg);
assert_checkerror("datetime([1 2 3 4 5])", msg);
assert_checkerror("datetime([1 2 3 4 5 6 7])", msg);

msg = msprintf(_("%s: Wrong type for input argument #%d: A real matrix or a string expected.\n"), "datetime", 1);
assert_checkerror("datetime(%t)", msg);
assert_checkerror("datetime(%s)", msg);
assert_checkerror("datetime(sparse([1 2], 2))", msg);

msg = msprintf(_("%s: Wrong type for input argument #%d: A real matrix or a string expected.\n"), "datetime", 1);
assert_checkerror("datetime(%t, 1, 3)", msg);
msg = msprintf(_("%s: Wrong type for input argument #%d: A real matrix or a string expected.\n"), "datetime", 2);
assert_checkerror("datetime(1, %t, 3)", msg);
msg = msprintf(_("%s: Wrong type for input argument #%d: A real matrix or a string expected.\n"), "datetime", 3);
assert_checkerror("datetime(1, 2, %t)", msg);
msg = msprintf(_("%s: Wrong value for input argument #%d: %s or %s expected.\n"), "datetime", 2, """InputFormat""", """ConvertFrom""");
assert_checkerror("datetime(1, ""toto"", 2, ""OutputFormat"", ""dd-MM-yyyy"")", msg);
msg = msprintf(_("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "datetime", 3);
assert_checkerror("datetime(1, 1, ""toto"")", msg);
msg = msprintf(_("%s: Wrong type for input argument #%d: A string expected.\n"), "datetime", 3);
assert_checkerror("datetime(1, ""InputFormat"", 1, ""OutputFormat"", ""dd-MM-yyyy"")", msg);

msg = msprintf(_("%s: Wrong size for input arguments: Same size expected.\n"), "datetime");
assert_checkerror("datetime(1, [2 2], [3; 3])", msg);
assert_checkerror("datetime([1 1], 2, [3; 3])", msg);
assert_checkerror("datetime([1 1], [2; 2], 3)", msg);

msg = msprintf(_("%s: Wrong size for input arguments: Same size expected.\n"), "datetime");
assert_checkerror("datetime(1, [2 2], [3; 3], 4, 5, 6)", msg);
assert_checkerror("datetime(1, [2 2], 3, [4; 4], 5, 6)", msg);
assert_checkerror("datetime(1, [2 2], 3, 4, [5; 5], 6)", msg);
assert_checkerror("datetime(1, [2 2], 3, 4, 5, [6; 6])", msg);

msg = msprintf(_("%s: Wrong type for input arguments #%d, #%d, #%d, #%d, #%d and #%d: Matrix of reals expected.\n"), "datetime", 1, 2, 3, 4, 5, 6);
assert_checkerror("datetime(""toto"", 2, 3, 4, 5, 6)", msg);
assert_checkerror("datetime(1, ""toto"", 3, 4, 5, 6)", msg);
assert_checkerror("datetime(1, 2, ""toto"", 4, 5, 6)", msg);
assert_checkerror("datetime(1, 2, 3, ""toto"", 5, 6)", msg);
assert_checkerror("datetime(1, 2, 3, 4, 5, ""toto"")", msg);
assert_checkerror("datetime(""toto"", 2, 3, 4, 5, 6, ""OutputFormat"", ""dd-MM-yyy"")", msg);
assert_checkerror("datetime(1, ""toto"", 3, 4, 5, 6, ""OutputFormat"", ""dd-MM-yyy"")", msg);
assert_checkerror("datetime(1, 2, ""toto"", 4, 5, 6, ""OutputFormat"", ""dd-MM-yyy"")", msg);
assert_checkerror("datetime(1, 2, 3, ""toto"", 5, 6, ""OutputFormat"", ""dd-MM-yyy"")", msg);
assert_checkerror("datetime(1, 2, 3, 4, ""toto"", 6, ""OutputFormat"", ""dd-MM-yyy"")", msg);
assert_checkerror("datetime(1, 2, 3, 4, 5, ""toto"", ""OutputFormat"", ""dd-MM-yyy"")", msg);

msg = msprintf(_("%s: Wrong or missing ""InputFormat"" to be applied.\n"), "datetime");
assert_checkerror("datetime(""toto"")", msg);

str = _("%s: Wrong size for input argument #%d: Must be of the same dimensions of #%d or scalar.\n");
msg = msprintf(str, "%datetime_a_s", 2, 1);
assert_checkerror("datetime(2022, 10, [1 2]) + [1; 2]", msg);
assert_checkerror("datetime(2022, 10, [1; 2]) + [1 2]", msg);
msg = msprintf(str, "%s_a_datetime", 2, 1);
assert_checkerror("[1; 2] + datetime(2022, 10, [1 2])", msg);
assert_checkerror("[1 2] + datetime(2022, 10, [1; 2])", msg);

msg = msprintf(str, "%datetime_a_duration", 2, 1);
assert_checkerror("datetime(2022, 10, [1 2]) + hours([1; 2])", msg);
assert_checkerror("datetime(2022, 10, [1; 2]) + hours([1 2])", msg);
msg = msprintf(str, "%duration_a_datetime", 2, 1);
assert_checkerror("hours([1; 2]) + datetime(2022, 10, [1 2])", msg);
assert_checkerror("hours([1 2]) + datetime(2022, 10, [1; 2])", msg);

msg = msprintf(str, "%datetime_a_calendarDuration", 2, 1);
assert_checkerror("datetime(2022, 10, [1 2]) + caldays([1; 2])", msg);
assert_checkerror("datetime(2022, 10, [1; 2]) + caldays([1 2])", msg);
assert_checkerror("caldays([1; 2]) + datetime(2022, 10, [1 2])", msg);
assert_checkerror("caldays([1 2]) + datetime(2022, 10, [1; 2])", msg);

msg = msprintf(str, "%datetime_s_datetime", 2, 1);
assert_checkerror("datetime(2022, 10, [1 2]) - datetime(2022, 10, [1; 2])", msg);
assert_checkerror("datetime(2022, 10, [1; 2]) - datetime(2022, 10, [1 2])", msg);
assert_checkerror("datetime(2022, 10, [1; 2]) - datetime(2022, 10, [1 2])", msg);
assert_checkerror("datetime(2022, 10, [1 2]) - datetime(2022, 10, [1; 2])", msg);

msg = msprintf(str, "%datetime_s_s", 2, 1);
assert_checkerror("datetime(2022, 10, [1 2]) - [1; 2]", msg);
assert_checkerror("datetime(2022, 10, [1; 2]) - [1 2]", msg);
msg = msprintf(str, "%s_s_datetime", 2, 1);
assert_checkerror("[1; 2] - datetime(2022, 10, [1 2])", msg);
assert_checkerror("[1 2] - datetime(2022, 10, [1; 2])", msg);

msg = msprintf(str, "%datetime_s_duration", 2, 1);
assert_checkerror("datetime(2022, 10, [1 2]) - hours([1; 2])", msg);
assert_checkerror("datetime(2022, 10, [1; 2]) - hours([1 2])", msg);

msg = msprintf(str, "%datetime_s_calendarDuration", 2, 1);
assert_checkerror("datetime(2022, 10, [1 2]) -caldays([1; 2])", msg);
assert_checkerror("datetime(2022, 10, [1; 2]) - caldays([1 2])", msg);

msg = msprintf(_("%s: Wrong type for input argument #%d: Must be a scalar.\n"), "%datetime_b_duration", 2);
assert_checkerror("(datetime(2022, 10, 06):hours(5:10):datetime(2022, 10, 7))", msg);
msg = msprintf(_("%s: Wrong type for input argument #%d: Must be a scalar.\n"), "%datetime_b_duration", 3);
assert_checkerror("(datetime(2022, 10, 06):hours(5):datetime(2022, 10, 07:10))", msg);

msg = msprintf(_("%s: Wrong type for input argument #%d: Must be a scalar.\n"), "%datetime_b_calendarDuration", 2);
assert_checkerror("(datetime(2022, 10, 06):caldays(5:10):datetime(2022, 10, 7))", msg);
msg = msprintf(_("%s: Wrong type for input argument #%d: Must be a scalar.\n"), "%datetime_b_calendarDuration", 3);
assert_checkerror("(datetime(2022, 10, 06):caldays(5):datetime(2022, 10, 07:10))", msg);

msg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "ymd", 1);
assert_checkerror("ymd()", msg);
msg = msprintf(_("%s: Wrong type for input argument #%d: Must be in %s.\n"), "ymd", 1, sci2exp("datetime"));
assert_checkerror("ymd(1)", msg);

msg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "hms", 1);
assert_checkerror("hms()", msg);
msg = msprintf(_("%s: Wrong type for input argument #%d: Must be in %s.\n"), "hms", 1, sci2exp(["datetime", "duration"]));
assert_checkerror("hms(1)", msg);
