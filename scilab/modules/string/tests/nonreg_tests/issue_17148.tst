// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - UTC - Stéphane MOTTELET
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 17148 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/17148
//
// <-- Short Description -->
// timeseries and table: misleading completion

T = datetime(["2022-12-01 00:00:00"; "2022-12-01 06:00:00"; "2022-12-01 12:00:00"; ...
"2022-12-01 18:00:00"; "2022-12-02 00:00:00"]);
WindDir = ["NW"; "NW"; "W"; "W"; "NW"];
WindSpeed = [8; 10; 7; 9.5; 10.2];
ts1 = timeseries(T, WindDir, WindSpeed, "VariableNames", ["Time", "WindDirection", "WindSpeed"])
ts1([2 4], :)
ts1("WindDirection")
ts1.WindDirection
ts1(3, 2) = {11.5}
ts1(datetime(2022,12,1), :) = {"W", 12.8}

assert_checkequal(fieldnames(ts1),["Time", "WindDirection", "WindSpeed", "Properties", "Variables"]);

names = ["a"; "b"; "c"];
id = [1;2;3];
scores = [50; 89; 65];
t = table(names, id, scores, "VariableNames", ["names", "id", "scores"])
t.names
t("names")

assert_checkequal(fieldnames(t),["names", "id", "scores", "Properties", "Variables", "Row"]);
