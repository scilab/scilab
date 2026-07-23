// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2025 - Dassault Systèmes S.E. - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for summary function
// =============================================================================

// table with only doubles
t = table([1 2 3; 4 5 6; 7 8 9]);
summary(t)
s = summary(t)
assert_checkequal(s.Var1.Size, [3 1]);
assert_checkequal(s.Var2.Size, [3 1]);
assert_checkequal(s.Var3.Size, [3 1]);
assert_checkequal(s.Var1.Type, "double");
assert_checkequal(s.Var2.Type, "double");
assert_checkequal(s.Var3.Type, "double");

t.Properties.VariableNames = ["a", "b", "c"];
summary(t)
s = summary(t)
assert_checkequal(fieldnames(s), ["a"; "b"; "c"]);
assert_checkequal(s.a.Size, [3 1]);
assert_checkequal(s.b.Size, [3 1]);
assert_checkequal(s.c.Size, [3 1]);
assert_checkequal(s.a.Type, "double");
assert_checkequal(s.b.Type, "double");
assert_checkequal(s.c.Type, "double");

t.Properties.VariableDescriptions = ["field a", "", "field c"];
summary(t)
s = summary(t)
assert_checkequal(s.a.Description, "field a");
assert_checkequal(s.b.Description, "");
assert_checkequal(s.c.Description, "field c");

t.Properties.VariableUnits = ["h", "mm", "ss"];
summary(t)
s = summary(t)
assert_checkequal(s.a.Units, "h");
assert_checkequal(s.b.Units, "mm");
assert_checkequal(s.c.Units, "ss");

summary(t, ["min", "max", "var"])
s = summary(t, ["min", "max", "var"])
summary(t, "q3")
s = summary(t, "q3")

// table containing doubles, datetimes, durations
t = table([1;2;3], datetime(2025, 12, [10; 11; 12]), hours([1;2;3]));
summary(t)
s = summary(t)
assert_checkequal(s.Var1.Size, [3 1]);
assert_checkequal(s.Var2.Size, [3 1]);
assert_checkequal(s.Var3.Size, [3 1]);

assert_checkequal(s.Var1.Type, "double");
assert_checkequal(s.Var2.Type, "datetime");
assert_checkequal(s.Var3.Type, "duration");

t.Properties.VariableNames = ["a", "b", "c"];
summary(t)
s = summary(t)
assert_checkequal(fieldnames(s), ["a"; "b"; "c"]);
assert_checkequal(s.a.Size, [3 1]);
assert_checkequal(s.b.Size, [3 1]);
assert_checkequal(s.c.Size, [3 1]);
assert_checkequal(s.a.Type, "double");
assert_checkequal(s.b.Type, "datetime");
assert_checkequal(s.c.Type, "duration");

t.Properties.VariableDescriptions = ["", "date time", "hours"];
summary(t)
s = summary(t)
assert_checkequal(s.a.Description, "");
assert_checkequal(s.b.Description, "date time");
assert_checkequal(s.c.Description, "hours");

t.Properties.VariableUnits = ["", "days", "h"];
summary(t)
s = summary(t)
assert_checkequal(s.a.Units, "");
assert_checkequal(s.b.Units, "days");
assert_checkequal(s.c.Units, "h");

summary(t, ["min", "max"])

t = table([%nan; 1; 2; 3], [datetime(2025, 12, [10; 11; 12]); NaT()], hours([1;%nan;2;3]));
summary(t)
s = summary(t)

// table containing doubles and int
t = table([1; 2; 3], int8([1; 2; 3]), uint8([1; 2; 3]));
summary(t)
s = summary(t)
assert_checkequal(s.Var1.Size, [3 1]);
assert_checkequal(s.Var2.Size, [3 1]);
assert_checkequal(s.Var3.Size, [3 1]);

assert_checkequal(s.Var1.Type, "double");
assert_checkequal(s.Var2.Type, "int8");
assert_checkequal(s.Var3.Type, "uint8");

t.Properties.VariableNames = ["a", "b", "c"];
summary(t)
s = summary(t)
assert_checkequal(fieldnames(s), ["a"; "b"; "c"]);
assert_checkequal(s.a.Size, [3 1]);
assert_checkequal(s.b.Size, [3 1]);
assert_checkequal(s.c.Size, [3 1]);
assert_checkequal(s.a.Type, "double");
assert_checkequal(s.b.Type, "int8");
assert_checkequal(s.c.Type, "uint8");

t.Properties.VariableDescriptions = ["double", "integer", "unsigned integer"];
summary(t)
s = summary(t)
assert_checkequal(s.a.Description, "double");
assert_checkequal(s.b.Description, "integer");
assert_checkequal(s.c.Description, "unsigned integer");

summary(t, ["min", "max"])

// table containing booleans and strings
t = table([%t; %f; %t], [%f; %t; %f], ["a"; "b"; "a"], ["b"; "b"; "bb"]);
summary(t)
s = summary(t)
assert_checkequal(s.Var1.Size, [3 1]);
assert_checkequal(s.Var2.Size, [3 1]);
assert_checkequal(s.Var3.Size, [3 1]);
assert_checkequal(s.Var4.Size, [3 1]);

assert_checkequal(s.Var1.Type, "boolean");
assert_checkequal(s.Var2.Type, "boolean");
assert_checkequal(s.Var3.Type, "string");
assert_checkequal(s.Var4.Type, "string");

// timeseries
T = datetime(2022, 12, 1:5)';
AmbientTemperature = [18; 18.5; 20; 20.2; 20.5];
FlowRate = [50; 52; 53; 55; 60];
ts = timeseries(T, AmbientTemperature, FlowRate)
summary(ts)
s = summary(ts)
assert_checkequal(s.Time.Size, [5 1]);
assert_checkequal(s.Var1.Size, [5 1]);
assert_checkequal(s.Var2.Size, [5 1]);
assert_checkequal(s.Time.Type, "datetime");
assert_checkequal(s.Var1.Type, "double");
assert_checkequal(s.Var2.Type, "double");
ts.Properties.VariableNames = ["T", "AmbientTemperature", "FlowRate"];
summary(ts)
s = summary(ts)
assert_checkequal(fieldnames(s), ["T"; "AmbientTemperature"; "FlowRate"]);
assert_checkequal(s.T.Size, [5 1]);
assert_checkequal(s.AmbientTemperature.Size, [5 1]);
assert_checkequal(s.FlowRate.Size, [5 1]);
assert_checkequal(s.T.Type, "datetime");
assert_checkequal(s.AmbientTemperature.Type, "double");
assert_checkequal(s.FlowRate.Type, "double");


// double
x = [1 2 3; 4 5 %nan];
summary(x)
s = summary(x);
assert_checkequal(s.NumMissing, [0 0 1]);
assert_checkequal(s.Min, [1 2 3]);
assert_checkequal(s.Max, [4 5 3]);
assert_checkalmostequal(s.Mean, [2.5 3.5 3]);

summary(x, 1, ["NumMissing", "Min", "Median", "Max", "Mean", "Std"])
s = summary(x, 1, ["NumMissing", "Min", "Median", "Max", "Mean", "Std"]);
assert_checkequal(s.NumMissing, [0 0 1]);
assert_checkequal(s.Min, [1 2 3]);
assert_checkequal(s.Max, [4 5 3]);
assert_checkalmostequal(s.Mean, [2.5 3.5 3]);

summary(x, 2, "Mean")
s = summary(x, 2, "Mean");
assert_checkalmostequal(s.Mean, [2 ; 4.5]);


// integers
x = int32([1 2 3 ; 4 5 6]);
summary(x, 1, ["Min", "Max", "Mean"])
s = summary(x, 1, ["Min", "Max", "Mean"]);
assert_checkequal(s.Min, int32([1 2 3]));
assert_checkequal(s.Max, int32([4 5 6]));
assert_checkequal(s.Mean, int32([2 3 4]));


// booleans
x = [%t %f %t ;
     %t %t %f];

summary(x)
s = summary(x, 1);
assert_checkequal(s.True, [2 1 1]);
assert_checkequal(s.False, [0 1 1]);

summary(x, 2)
s = summary(x, 2);
assert_checkequal(s.True, [2; 2]);
assert_checkequal(s.False, [1; 1]);


// strings
x = ["a" "b" "a" ;
     "b" "b" "c"];

summary(x)
s = summary(x);
assert_checkequal(s.Values, ["a" ; "b" ; "c"]);

x = ["red" "red";
     "blue" "red";
     "red" "green"];

summary(x)
s = summary(x, 1)
assert_checkequal(s.Values, ["blue"; "green"; "red"]);
assert_checkequal(s.Counts, [1 0; 0 1; 2 2]);

summary(x, 2)
s = summary(x, 2)
assert_checkequal(s.Counts, [0 0 2; 1 0 1; 0 1 1]);


// datetime
d = datetime([2025 1 1 ;
              2025 1 2 ;
              2025 1 3]);

summary(d, 1, ["Min", "Median", "Max"])
s = summary(d, 1, ["Min", "Median", "Max"]);

assert_checkequal(s.Min, d(1));
assert_checkequal(s.Median, d(2));
assert_checkequal(s.Max, d(3));

d = [datetime(2025,1,1)
     NaT()
     datetime(2025,1,3)];

summary(d, 1, "NumMissing")
s = summary(d, 1, "NumMissing");
assert_checkequal(s.NumMissing, 1);

// duration
d = hours([1 2 3 4])';

summary(d, 1, "Mean")
s = summary(d, 1, "Mean");
assert_checkequal(hours(s.Mean), 2.5);

d = hours([1 %nan 3]);
summary(d, 1, "NumMissing")
s = summary(d, 1, "NumMissing");
assert_checkequal(s.NumMissing, [0 1 0]);

summary(d, 2, "NumMissing")
s = summary(d, 2, "NumMissing");
assert_checkequal(s.NumMissing, 1);


// mode
x = [1 1 2 3 3 3];
summary(x, 1, "Mode")
s = summary(x, 1, "Mode");
assert_checkequal(s.Mode, x);

summary(x, 2, "Mode")
s = summary(x, 2, "Mode");
assert_checkequal(s.Mode, 3);

// NumUnique
x = [1 1 2 3 3];
summary(x, 2, "NumUnique")
s = summary(x, 2, "NumUnique");
assert_checkequal(s.NumUnique, 3);

// Nnz
x = [0 1 0 ;
     2 0 3];
summary(x, 1, "Nnz")
s = summary(x, 1, "Nnz");
assert_checkequal(s.Nnz, [1 1 1]);

// Range
x = [1 2 3 ;
     5 8 7];

s = summary(x, 1, ["Range"]);
assert_checkequal(s.Range, [4 6 4]);