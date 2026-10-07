// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2025 - Dassault Systèmes S.E. - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for issue 17431 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/17431
//
// <-- Short Description -->
// LegendLabels option of stackedplot() was not taken into account when DisplayLabels option is applied.

t = datetime(2022, 1, 1):datetime(2022, 8, 31);
n = size(t, "*");
y1 = floor(10 * rand(n, 3)) + 10;
ts1 = timeseries(t, y1(:, 1), y1(:, 2), y1(:,3), "VariableNames", ["Time", "Result_1", "Result_2", "Result_3"]);

y2 = floor(10 * rand(n, 3)) + 20;
ts2 = timeseries(t, y2(:, 1), y2(:, 2), y2(:, 3), "VariableNames", ["Time", "Result_1", "Result_2", "Result_3"]);
a = stackedplot(ts1, ts2, {"Result_1", ["Result_2", "Result_3"]}, "LegendLabels", ["t1", "t2"], "DisplayLabels", ["res1", "res2 - res3"]);
l = a.children(a.children.type == "Legend");

assert_checkequal(l(1).text, ["t1 - Result_2"; "t1 - Result_3"; "t2 - Result_2"; "t2 - Result_3"]);
assert_checkequal(l(2).text, ["t1 - Result_1"; "t2 - Result_1"]);

a = stackedplot(ts1, ts2, ["Result_1" "Result_3"], "LegendLabels", ["ts1", "ts2"], "CombineMatchingNames", %f, "DisplayLabels", ["r1", "r3"]);
l = a.children(a.children.type == "Legend");
assert_checkequal(l.text, ["ts2"; "ts2"; "ts1"; "ts1"]);

msg = msprintf(_("%s: Wrong value for input argument #%d: a valid LineSpec or VariableName expected.\n"), "stackedplot", 3);
assert_checkerror("stackedplot(ts1, ts2, [""Result_1"" ""Result3""])", msg);

