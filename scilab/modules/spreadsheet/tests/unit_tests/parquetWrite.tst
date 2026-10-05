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

    // compression options (scalar: gzip, snappy, zstd, uncompressed) for parquet files
    if extension == ".parquet" then
        for comp = ["gzip", "snappy", "zstd", "uncompressed"]
            path_comp = fullfile(TMPDIR, "comp_" + comp + extension);
            parquetWrite(path_comp, t, comp);
            expected = parquetRead(path_comp);
            assert_checkequal(expected, t);
        end

        // per-variable compression (vector of strings)
        t_multi = table(rand(3,1), ["a"; "b"; "c"], int32([1; 2; 3]), "VariableNames", ["v1", "v2", "v3"]);
        path_multi = fullfile(TMPDIR, "comp_multi" + extension);
        parquetWrite(path_multi, t_multi, ["gzip", "snappy", "uncompressed"]);
        expected_multi = parquetRead(path_multi);
        assert_checkequal(expected_multi, t_multi);

        parquetWrite(path_multi, t_multi, ["zstd"; "gzip"; "snappy"]);
        expected_multi = parquetRead(path_multi);
        assert_checkequal(expected_multi, t_multi);
    end
end

// errors
msg = msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"), "parquetWrite", 2, 3);
assert_checkerror("parquetWrite()", msg);
msg = msprintf(_("%s: Wrong type for input argument #%d: Must be in %s.\n"), "parquetWrite", 1, sci2exp("string"));
assert_checkerror("parquetWrite(1,1)", msg);
msg = msprintf(_("%s: Wrong type for input argument #%d: Must be in %s.\n"), "parquetWrite", 2, sci2exp(["table", "timeseries"]));
assert_checkerror("parquetWrite(""1"",1)", msg);
msg = msprintf(_("%s: Wrong extension for input argument #%d: Supported extensions are %s.\n"), "parquetWrite", 1, sci2exp(["parquet", "arrow"]));
assert_checkerror("parquetWrite(fullfile(TMPDIR, ""test.csv""), t)", msg);
msg = msprintf(_("%s: Wrong value for input argument #%d: Must be in %s.\n"), "parquetWrite", 3, sci2exp(["gzip", "snappy", "zstd", "uncompressed"]));
assert_checkerror("parquetWrite(path, t, ""invalid"")", msg);
assert_checkerror("parquetWrite(path, t, 123)", msg);
msg_size = msprintf(_("%s: Wrong size for input argument #%d: A vector of %d compression types expected.\n"), "parquetWrite", 3, 2);
assert_checkerror("parquetWrite(path, t, [""gzip"", ""snappy"", ""uncompressed""])", msg_size);
msg_val = msprintf(_("%s: Wrong value for input argument #%d: Elements must be in %s.\n"), "parquetWrite", 3, sci2exp(["gzip", "snappy", "zstd", "uncompressed"]));
assert_checkerror("parquetWrite(path, t, [""gzip"", ""invalid""])", msg_val);