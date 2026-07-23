// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// unit tests for parquetRead function
// =============================================================================

dt = datetime(2026, 1, 1, 9, 30, 45):calmonths(1):datetime(2026, 12, 31);
dt = dt';
d = [1:12]';
i = int32(d);
str = "A" + string(d);
b = [%f; %t; %t; %f; %f; %t; %f; %f; %f; %t; %t; %f];
t = table(dt, d, i, str, b, "VariableNames", ["date", "double", "int32", "string", "bool"]);

// Extension: "parquet"
path = fullfile(TMPDIR, "table.parquet");
parquetWrite(path, t);

expected = parquetRead(path);
assert_checkequal(expected, t);

// Extension: "arrow"
path2 = fullfile(TMPDIR, "table.arrow");
parquetWrite(path2, t);

expected = parquetRead(path2);
assert_checkequal(expected, t);

// OutputType == "timeseries"
ts = parquetRead(path, "OutputType", "timeseries");
assert_checktrue(istimeseries(ts));

dura = hours(d);
t = table(d, i, dt, str, dura, b, "VariableNames", ["double", "int32", "date", "string", "hours", "bool"]);
parquetWrite(path, t);

expected = parquetRead(path);
assert_checkequal(expected, t);

ts = parquetRead(path, "OutputType", "timeseries");
assert_checktrue(istimeseries(ts));
assert_checkequal(ts.Properties.VariableNames, ["date", "double", "int32", "string", "hours", "bool"]);

// Rowtimes
ts = parquetRead(path, "RowTimes", "hours");
assert_checktrue(istimeseries(ts));
assert_checkequal(ts.Properties.VariableNames, ["hours", "double", "int32", "date", "string", "bool"]);

ts = parquetRead(path, "RowTimes", seconds(d));
assert_checktrue(istimeseries(ts));
assert_checkequal(ts.Properties.VariableNames, ["Time", "double", "int32", "date", "string", "hours", "bool"]);
assert_checktrue(isduration(ts.Time));

ts = parquetRead(path, "RowTimes", datetime()+ caldays(d));
assert_checktrue(istimeseries(ts));
assert_checkequal(ts.Properties.VariableNames, ["Time", "double", "int32", "date", "string", "hours", "bool"]);
assert_checktrue(isdatetime(ts.Time));

// TimeStep - StartTime - SampleRate
ts = parquetRead(path, "TimeStep", seconds(1));
assert_checktrue(istimeseries(ts));
assert_checkequal(ts.Properties.VariableNames, ["Time", "double", "int32", "date", "string", "hours", "bool"]);
assert_checktrue(isduration(ts.Time));

ts = parquetRead(path, "TimeStep", seconds(1), "StartTime", hours(1));
assert_checktrue(istimeseries(ts));
assert_checkequal(ts.Properties.VariableNames, ["Time", "double", "int32", "date", "string", "hours", "bool"]);
assert_checktrue(isduration(ts.Time));

ts = parquetRead(path, "TimeStep", caldays(1), "StartTime", datetime(2026, 6, 1));
assert_checktrue(istimeseries(ts));
assert_checkequal(ts.Properties.VariableNames, ["Time", "double", "int32", "date", "string", "hours", "bool"]);
assert_checktrue(isdatetime(ts.Time));

ts = parquetRead(path, "SampleRate", 1/3600);
assert_checktrue(istimeseries(ts));
assert_checkequal(ts.Properties.VariableNames, ["Time", "double", "int32", "date", "string", "hours", "bool"]);
assert_checktrue(isduration(ts.Time));

ts = parquetRead(path, "SampleRate", 1/3600, "StartTime", hours(1));
assert_checktrue(istimeseries(ts));
assert_checkequal(ts.Properties.VariableNames, ["Time", "double", "int32", "date", "string", "hours", "bool"]);
assert_checktrue(isduration(ts.Time));


// errors
msg = msprintf(_("%s: Wrong number of input argument(s): at least %d expected.\n"), "parquetRead", 1);
assert_checkerror("parquetRead()", msg);
msg = msprintf(_("%s: Wrong type for input argument #%d: Must be in %s.\n"), "parquetRead", 1, sci2exp("string"));
assert_checkerror("parquetRead(123)", msg);
msg = msprintf(_("%s: Wrong value for input argument #%d: File ""%s"" does not exist.\n"), "parquetRead", 1, "nonexistent.parquet");
assert_checkerror("parquetRead(""nonexistent.parquet"")", msg);