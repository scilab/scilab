// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// unit tests for parquetWrite function
// =============================================================================

for extension = [".parquet", ".arrow"]
    // double
    path = fullfile(TMPDIR, "double" + extension);
    t = table(rand(3,1), [1; 2; 3], [4.5; 9.33; 5.6784], "VariableNames", ["a", "b", "c"]);
    parquetWrite(path, t)

    expected = parquetRead(path);
    assert_checkequal(expected, t);

    // int8 - int16 - int32 - int64
    path = fullfile(TMPDIR, "int" + extension);
    t = table(int8([0:10]'), int16([11:21]'), int32([22:32]'), int64([33:43]'), "VariableNames", ["int8", "int16", "int32", "int64"]);
    parquetWrite(path, t)

    expected = parquetRead(path);
    assert_checkequal(expected, t);

    // uint8 - uint16 -uint32 - uint64
    path = fullfile(TMPDIR, "uint" + extension);
    t = table(uint8([0:10]'), uint16([11:21]'), uint32([22:32]'), uint64([33:43]'), "VariableNames", ["uint8", "uint16", "uint32", "uint64"]);
    parquetWrite(path, t)

    expected = parquetRead(path);
    assert_checkequal(expected, t);

    // bool
    path = fullfile(TMPDIR, "bool" + extension);
    t = table([1;0;1;0],[%t;%f;%t;%f], [%T;%F;%T;%F], "VariableNames", ["d", "bool1", "bool2"]);
    parquetWrite(path, t)

    expected = parquetRead(path);
    assert_checkequal(expected, t);

    // string
    path = fullfile(TMPDIR, "string" + extension);
    t = table(string([1;2;3]), ["a";"b"; "c"], "VariableNames", ["string1", "string2"]);
    parquetWrite(path, t)

    expected = parquetRead(path);
    assert_checkequal(expected, t);

    // datetime
    path = fullfile(TMPDIR, "datetime" + extension);
    t = table([datetime(); datetime("today"); datetime("tomorrow")], [-1; 0; 1], "VariableNames", ["date", "double"]);
    parquetWrite(path, t)

    expected = parquetRead(path);
    assert_checkequal(expected, t);

    ts = timeseries([datetime(); datetime("today"); datetime("tomorrow")], [-1; 0; 1], "VariableNames", ["date", "double"]);
    parquetWrite(path, ts);

    expected = parquetRead(path, "OutputType", "timeseries");
    assert_checktrue(istimeseries(expected));

    // duration
    path = fullfile(TMPDIR, "duration" + extension);
    t = table([datetime(); datetime("today"); datetime("tomorrow"); datetime("yesterday")], [hours(1); minutes(1); seconds(1); milliseconds(1)], "VariableNames", ["date", "duration"]);
    parquetWrite(path, t)

    expected = parquetRead(path);
    assert_checkequal(expected, t);

    ts = timeseries([datetime(); datetime("today"); datetime("tomorrow"); datetime("yesterday")], [hours(1); minutes(1); seconds(1); milliseconds(1)], "VariableNames", ["date", "duration"]);
    parquetWrite(path, ts);

    expected = parquetRead(path, "OutputType", "timeseries");
    assert_checktrue(istimeseries(expected));

    expected = parquetRead(path, "RowTimes", "duration");
    assert_checkequal(expected.Properties.variableNames, ["duration", "date"]);
end

// errors
msg = msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "parquetWrite", 2);
assert_checkerror("parquetWrite()", msg);
msg = msprintf(_("%s: Wrong type for input argument #%d: Must be in %s.\n"), "parquetWrite", 1, sci2exp("string"));
assert_checkerror("parquetWrite(1,1)", msg);
msg = msprintf(_("%s: Wrong type for input argument #%d: Must be in %s.\n"), "parquetWrite", 2, sci2exp(["table", "timeseries"]));
assert_checkerror("parquetWrite(""1"",1)", msg);
msg = msprintf(_("%s: Wrong extension for input argument #%d: Supported extensions are %s.\n"), "parquetWrite", 1, sci2exp(["parquet", "arrow"]));
assert_checkerror("parquetWrite(fullfile(TMPDIR, ""test.csv""), t)", msg);