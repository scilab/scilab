// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2022 - Dassault Systèmes S.E. - Adeline CARNIS
// Copyright (C) 2022 - Dassault Systèmes S.E. - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// unit tests for timeseries function
// =============================================================================

// -----------------------------------------------------------------------
// ts = timeseries(Time, var1, ..., varN)
// -----------------------------------------------------------------------
function checkstring(t, v)
    assert_checkequal(%timeseries_string(t), v);
endfunction

time = minutes(0:15:60)';
v1 = [1;2;3;4;5];
v2 = [2:2:10]';
v3 = ["a";"b"; "c"; "d"; "e"];
tscomputed = timeseries(time, v1, v2, v3);

checkstring(tscomputed, [%duration_string(time) string(v1) string(v2) v3]);
assert_checkequal(tscomputed.Properties.Description, "");
assert_checkequal(tscomputed.Properties.VariableNames, ["Time", "Var1" "Var2" "Var3"]);
assert_checkequal(tscomputed.Properties.VariableDescriptions, ["" "" "" ""]);
assert_checkequal(tscomputed.Properties.VariableUnits, ["" "" "" ""]);
assert_checkequal(tscomputed.Properties.VariableContinuity, ["" "" "" ""]);
assert_checktrue(tscomputed.Properties.StartTime == duration(0,0,0));
assert_checkequal(tscomputed.Properties.SampleRate, seconds(1)/minutes(15));
assert_checkequal(tscomputed.Properties.TimeStep, minutes(15));


// -----------------------------------------------------------------------
// ts = timeseries(var1, ..., varN, 'RowTimes', Time)
// -----------------------------------------------------------------------
tscomputed = timeseries(v1, v2, v3, 'RowTimes', time);

checkstring(tscomputed, [%duration_string(time) string(v1) string(v2) v3]);
assert_checkequal(tscomputed.Properties.Description, "");
assert_checkequal(tscomputed.Properties.VariableNames, ["Time", "Var1" "Var2" "Var3"]);
assert_checkequal(tscomputed.Properties.VariableDescriptions, ["" "" "" ""]);
assert_checkequal(tscomputed.Properties.VariableUnits, ["" "" "" ""]);
assert_checkequal(tscomputed.Properties.VariableContinuity, ["" "" "" ""]);
assert_checktrue(tscomputed.Properties.StartTime == duration(0,0,0));
assert_checkequal(tscomputed.Properties.SampleRate, seconds(1)/minutes(15));
assert_checkequal(tscomputed.Properties.TimeStep, minutes(15));


// -----------------------------------------------------------------------
// ts = timeseries(var1, ..., varN, 'TimeStep', step)
// -----------------------------------------------------------------------
step = minutes(15);
tscomputed = timeseries(v1, v2, v3, 'TimeStep', step);

checkstring(tscomputed, [%duration_string(time) string(v1) string(v2) v3]);
assert_checkequal(tscomputed.Properties.Description, "");
assert_checkequal(tscomputed.Properties.VariableNames, ["Time", "Var1" "Var2" "Var3"]);
assert_checkequal(tscomputed.Properties.VariableDescriptions, ["" "" "" ""]);
assert_checkequal(tscomputed.Properties.VariableUnits, ["" "" "" ""]);
assert_checkequal(tscomputed.Properties.VariableContinuity, ["" "" "" ""]);
assert_checktrue(tscomputed.Properties.StartTime == duration(0,0,0));
assert_checkequal(tscomputed.Properties.SampleRate, seconds(1)/step);
assert_checkequal(tscomputed.Properties.TimeStep, step);

// -----------------------------------------------------------------------
// ts = timeseries(var1, ..., varN, 'SampleRate', Fs)
// -----------------------------------------------------------------------
Fs = seconds(1)/step;
tscomputed = timeseries(v1, v2, v3, 'SampleRate', Fs);

checkstring(tscomputed, [%duration_string(time) string(v1) string(v2) v3]);
assert_checkequal(tscomputed.Properties.Description, "");
assert_checkequal(tscomputed.Properties.VariableNames, ["Time", "Var1" "Var2" "Var3"]);
assert_checkequal(tscomputed.Properties.VariableDescriptions, ["" "" "" ""]);
assert_checkequal(tscomputed.Properties.VariableUnits, ["" "" "" ""]);
assert_checkequal(tscomputed.Properties.VariableContinuity, ["" "" "" ""]);
assert_checkequal(tscomputed.Properties.StartTime, duration(0,0,0));
assert_checkequal(tscomputed.Properties.SampleRate, Fs);
assert_checkequal(tscomputed.Properties.TimeStep, step);


// -----------------------------------------------------------------------
// ts = timeseries(Time, var1, ..., varN, Name, Value) where Name =
// * 'VariableNames'
// * 'StarTime'
// -----------------------------------------------------------------------
tscomputed = timeseries(v1, v2, v3, 'TimeStep', minutes(15), 'StartTime', minutes(0), 'VariableNames', ["Time", "a", "b", "c"]);

checkstring(tscomputed, [%duration_string(time) string(v1) string(v2) v3]);
assert_checkequal(tscomputed.Properties.Description, "");
assert_checkequal(tscomputed.Properties.VariableNames, ["Time", "a", "b", "c"]);
assert_checkequal(tscomputed.Properties.VariableDescriptions, ["" "" "" ""]);
assert_checkequal(tscomputed.Properties.VariableUnits, ["" "" "" ""]);
assert_checkequal(tscomputed.Properties.VariableContinuity, ["" "" "" ""]);
assert_checkequal(tscomputed.Properties.StartTime, minutes(0));
assert_checkequal(tscomputed.Properties.SampleRate, Fs);
assert_checkequal(tscomputed.Properties.TimeStep, minutes(15));


// -----------------------------------------------------------------------
// Test case-sensitivity on options
// -----------------------------------------------------------------------
assert_checktrue(execstr("timeseries(v1, v2, v3, ""roWtiMeS"", time)", "errcatch") == 0);
assert_checktrue(execstr("timeseries(v1, v2, v3, ""TiMeSTeP"", step)", "errcatch") == 0);
assert_checktrue(execstr("timeseries(v1, v2, v3, ""sAmplErAtE"", Fs)", "errcatch") == 0);
assert_checktrue(execstr("timeseries(v1, v2, v3, ""timestep"", minutes(15), ""StaRTTime"", minutes(0), ""variableNAMES"", [""Time"", ""a"", ""b"", ""c""])", "errcatch") == 0);

// -----------------------------------------------------------------------
// Extraction - Insertion
// -----------------------------------------------------------------------
checkstring(tscomputed(:), [%duration_string(time) string(v1) string(v2) v3]);
checkstring(tscomputed(2), [%duration_string(time(2)) string(v1(2)) string(v2(2)) v3(2)]);
checkstring(tscomputed(:, 2), [%duration_string(time) string(v2)]);
checkstring(tscomputed(:, $), [%duration_string(time) v3]);
checkstring(tscomputed(2:3), [%duration_string(time(2:3)) string(v1(2:3)) string(v2(2:3)) v3(2:3)]);
checkstring(tscomputed(2, 2:3), [%duration_string(time(2)) string(v2(2)) v3(2)]);

n = 10;
t = datetime(2000, 1, 1) + caldays(1:n)';
y = floor(10 * rand(n, 3)) + 10;
ts = timeseries(t, y(:, 1), y(:, 2), y(:, 3), "VariableNames", ["Time", "Var 1", "Var 2", "Var3"]);
ts(t, :) = 1;
checkstring(ts, [%datetime_string(t) string(ones(10,3))]);
ts(datetime(2000, 1, 2), "Var 2") = 2;
checkstring(ts(:,2), [%datetime_string(t) string([2; ones(9,1)])]);

t = seconds(1:n)';
ts = timeseries(t, y(:, 1), "VariableNames", ["seconds", "Var 1"]);
ts(t, :) = 1;
checkstring(ts, [%duration_string(t) string(ones(10,1))]);
ts(seconds(1:2:n), :) = [2;3;4;5;6];
checkstring(ts, [%duration_string(t) string([2;1;3;1;4;1;5;1;6;1])])


// -----------------------------------------------------------------------
// Others tests
// -----------------------------------------------------------------------
AmbientTemperature = [18; 18.5; 20; 20.2; 20.5];
FlowRate = [50; 52; 53; 55; 60];
timestep = hours(1);
ts = timeseries(AmbientTemperature, FlowRate, 'TimeStep', timestep);
assert_checktrue(ts.Properties.StartTime == duration(0));
assert_checkequal(ts.Properties.SampleRate, seconds(1)/timestep);
assert_checkequal(ts.Properties.TimeStep, timestep);
assert_checkequal(ts("Time"), [hours(0):timestep:hours(4)]');

ts = timeseries(AmbientTemperature, FlowRate, 'TimeStep', caldays(1), 'StartTime', datetime(2022, 1, 1));
assert_checktrue(ts.Properties.StartTime == datetime(2022, 1, 1));
assert_checkequal(ts.Properties.SampleRate, %nan);
assert_checkequal(ts.Properties.TimeStep, caldays(1));
assert_checkequal(ts("Time"), [datetime(2022, 1, 1):caldays(1):datetime(2022, 1, 5)]');

ts = timeseries(datetime(2022, 1:5, 1), AmbientTemperature);
assert_checkequal(ts.Var1, AmbientTemperature);
ts.Properties.VariableNames = ["Time", "AmbTemp"];
assert_checkequal(ts.AmbTemp, AmbientTemperature);
assert_checkequal(cell2mat(ts.Variables), AmbientTemperature);
assert_checktrue(ts.Properties.StartTime == datetime(2022, 1, 1));
assert_checktrue(ts.Properties.TimeStep == calmonths(1));
assert_checkequal(ts.Properties.SampleRate, %nan);
checkstring(ts(datetime(2022, 1, 1), :), [%datetime_string(datetime(2022, 1, 1)), "18"]);

ts = timeseries(datetime(2022, 1:5, 1:5)', (1:5)');
assert_checktrue(ts.Properties.StartTime == datetime(2022, 1, 1));
assert_checkequal(ts.Properties.SampleRate, %nan);
ts.Properties.StartTime = datetime(2019, 6, 25);
assert_checktrue(ts.Time == datetime(2019, 6:10, [25 27 25 26 27])');

// with duration
time = [duration(0,0,0): minutes(15): duration(2,0,0)]';
v1 = ones(length(time), 1);
ts = timeseries(time, v1, "VariableNames", ["time", "v1"]);
ts.time.format = "hh:mm";

time.format = "hh:mm";

assert_checkequal(string(ts.time), string(time));


// -----------------------------------------------------------------------
// [ts; ts], [ts ts]
// -----------------------------------------------------------------------
time1 = datetime(2022, 1:5, 1)';
v1 = floor(18 + rand(5, 1) * 3);
ts1 = timeseries(time1, v1, "VariableNames", ["Time", "Info"]);
time2 = datetime(2022, 8:12, 1)';
v2 = floor(18 + rand(5, 1) * 3);
ts2 = timeseries(time2, v2, "VariableNames", ["Time", "Info"]);
ts = [ts1; ts2];
assert_checkequal(size(ts), [10 1]);
assert_checkequal(ts.Properties.VariableNames, ["Time", "Info"]);

t = timeseries(datetime(2023, 3, 15:20)', [1:6]');
assert_checkequal(t(%f), []);
assert_checkequal(t([%f, %f, %f, %f, %f, %f]), []);
assert_checktrue(string(t(%t)) == string(t(1)));
assert_checktrue(t([%t %f %t]) == t([1 3]));
assert_checktrue(string(t([%t %f; %f %t; %t %t])) == string(t([1 3 5 6])));
assert_checktrue(string(t([%t %f  %f; %t %t %t])) == string(t([1 2 4 6])));

// -----------------------------------------------------------------------
// timeseries with matrices
// -----------------------------------------------------------------------
ts = timeseries(hours(1), [1 2]);
expected = [string(hours(1)), string([1 2])];
checkstring(ts, expected);

ts = timeseries([1 2], "RowTimes", hours(1));
checkstring(ts, expected);

ts = timeseries(hours(1), [1 2], 3, [5 6]);
expected = [string(hours(1)), string([1 2 3 5 6])];
checkstring(ts, expected);

ts = timeseries([1 2], 3, [5 6], "RowTimes", hours(1));
checkstring(ts, expected);

dt = datetime(2023, 1, 1:5)';
ts = timeseries(dt, [1 2 3].*. ones(5,1));
expected = [string(dt), string([1 2 3].*. ones(5,1))];
checkstring(ts, expected);

ts.Time = datetime(2024, 9, 10:14);
expected = [string(datetime(2024, 9, 10:14)'), string([1 2 3].*. ones(5,1))];
checkstring(ts, expected);

ts.Time = hours(1:5);
expected = [string(hours(1:5)'), string([1 2 3].*. ones(5,1))];
checkstring(ts, expected);

dt = datetime(2025, 1:3, 1)';
ts = timeseries(dt, [1; 2; 3]);
ts.Time = datetime(2025, 5, 1:3)';
assert_checkequal(ts.Properties.TimeStep, days(1));
assert_checkequal(ts.Properties.StartTime, ts.Time(1));
assert_checkequal(ts.Properties.SampleRate, seconds(1)/days(1));

// with integers
inttyp = ["int8", "int16", "int32", "int64", "uint8", "uint16", "uint32", "uint64"];
mat = [1 2;3 4];
for i = inttyp
    execstr("m = " + i + "(mat)");
    t = timeseries(hours([1;2]), m);
    assert_checkequal(typeof(t("Var1")), i);
    assert_checkequal(typeof(t.Var2), i);

    execstr("b = t(""Var1"") == " + i + "([1; 3])");
    assert_checktrue(b);
    execstr("b = t.Var2 == " + i + "([2; 4])");
    assert_checktrue(b);
end

AmbientTemperature = [18; 18.5; 20; 20.2; 20.5];
FlowRate = [50; 52; 53; 55; 60];
ts = timeseries(AmbientTemperature, FlowRate, "RowTimes", hours(1:5)', "VariableNames", ["Temp", "FlowRate"]);
expected = [string(hours(1:5)'), string(AmbientTemperature), string(FlowRate)];
checkstring(ts, expected);
assert_checkequal(ts.Properties.VariableNames, ["Time", "Temp", "FlowRate"]);

ts = timeseries(AmbientTemperature, FlowRate, "TimeStep", hours(1), "VariableNames", ["Temp", "FlowRate"]);
expected = [string(hours(0:4)'), string(AmbientTemperature), string(FlowRate)];
checkstring(ts, expected);
assert_checkequal(ts.Properties.VariableNames, ["Time", "Temp", "FlowRate"]);

ts = timeseries(AmbientTemperature, FlowRate, "SampleRate", 1/3600, "VariableNames", ["Temp", "FlowRate"]);
checkstring(ts, expected);
assert_checkequal(ts.Properties.VariableNames, ["Time", "Temp", "FlowRate"]);

ts = timeseries(AmbientTemperature, FlowRate, "TimeStep", caldays(1), "StartTime", datetime(2026, 6, 1), "VariableNames", ["Temp", "FlowRate"]);
expected = [string(datetime(2026,6,1):caldays(1):datetime(2026,6,5))', string(AmbientTemperature), string(FlowRate)];
checkstring(ts, expected);
assert_checkequal(ts.Properties.VariableNames, ["Time", "Temp", "FlowRate"]);

ts = timeseries(AmbientTemperature, FlowRate, "SampleRate", 1/3600, "StartTime", datetime(2026, 6, 1),  "VariableNames", ["Temp", "FlowRate"]);
expected = [string(datetime(2026,6,1)+hours(0:4))', string(AmbientTemperature), string(FlowRate)];
checkstring(ts, expected);
assert_checkequal(ts.Properties.VariableNames, ["Time", "Temp", "FlowRate"]);

// -----------------------------------------------------------------------
// Errors
// -----------------------------------------------------------------------
msg = msprintf(_("%s: Wrong number of input argument: At least %d expected.\n"), "timeseries", 1);
assert_checkerror("timeseries()", msg);


msg = msprintf(_("%s: Wrong type for %s argument #%d: duration or calendarDuration expected"), "timeseries", "TimeStep", 4);
assert_checkerror("timeseries(AmbientTemperature, FlowRate, ""TimeStep"", datetime(2022, 1, 1))", msg);
msg = msprintf(_("%s: Wrong type for %s option: StarTime must be a datetime when TimeStep is a calendarDuration.\n"), "timeseries", "TimeStep");
assert_checkerror("timeseries(AmbientTemperature, FlowRate, ""TimeStep"", caldays(1))", msg);

msg = msprintf(_("%s: Wrong type for %s argument #%d: a real value expected"), "timeseries", "SampleRate", 4);
assert_checkerror("timeseries(AmbientTemperature, FlowRate, ""SampleRate"", hours(0))", msg);

msg = msprintf(_("%s: Wrong type for %s argument #%d: duration or datetime expected"), "timeseries", "StartTime", 6);
assert_checkerror("timeseries(AmbientTemperature, FlowRate, ""TimeStep"", hours(1), ""StartTime"", caldays(1))", msg);

msg = msprintf(_("%s: Wrong type for %s argument #%d: string vector expected"), "timeseries", "VariableNames", 6);
assert_checkerror("timeseries(AmbientTemperature, FlowRate, ""TimeStep"", hours(1), ""VariableNames"", 1)", msg);

msg = msprintf(_("%s: Wrong value for ""%s"" property: ""%s"" can not duplicate.\n"), "timeseries", "VariableNames", "Time");
assert_checkerror("timeseries(AmbientTemperature, FlowRate, ""TimeStep"", hours(1), ""VariableNames"", [""Time"", ""Temp""])", msg);

msg = msprintf(_("%s: Wrong type for %s argument #%d: string vector expected"), "timeseries", "VariableUnits", 6);
assert_checkerror("timeseries(AmbientTemperature, FlowRate, ""TimeStep"", hours(1), ""VariableUnits"", 1)", msg);

msg = msprintf(_("%s: Wrong type for %s argument #%d: string vector expected"), "timeseries", "VariableContinuity", 6);
assert_checkerror("timeseries(AmbientTemperature, FlowRate, ""TimeStep"", hours(1), ""VariableContinuity"", 1)", msg);

msg = msprintf(_("%s: Wrong type for %s argument #%d: %s, %s, %s or %s expected"), "timeseries", "VariableContinuity", 6, "unset", "continuous", "step", "event");
assert_checkerror("timeseries(AmbientTemperature, FlowRate, ""TimeStep"", hours(1), ""VariableContinuity"", [""continuous"", ""1""])", msg);

msg = msprintf(_("%s: %s must be used with %s or %s property.\n"), "timeseries", "StartTime", "TimeStep", "SampleRate");
assert_checkerror("timeseries(AmbientTemperature, FlowRate, ""StartTime"", datetime(2022, 1, 1))", msg);

msg = msprintf(_("%s: Row times vector is missing.\n"), "timeseries");
assert_checkerror("timeseries(AmbientTemperature, FlowRate)", msg);

msg = msprintf(_("%s: Wrong type for %s option: duration or datetime vector expected.\n"), "timeseries", "RowTimes");
assert_checkerror("timeseries(AmbientTemperature, FlowRate, ""RowTimes"", 1)", msg);

// msg = msprintf(_("%s: unknown property"), "timeseries");
// assert_checkerror("timeseries(AmbientTemperature, FlowRate, ""StarTime"", datetime(2022,1,1))", msg);

msg = msprintf(_("%s: Wrong size for input argument #%d: must be the same size of time vector.\n"), "timeseries", 2);
assert_checkerror("timeseries(datetime(2022, 12, 1:4), FlowRate)", msg);