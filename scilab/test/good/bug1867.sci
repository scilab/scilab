// Non-regression test file for bug 1867
// A.C
// Copyright INRIA 2006

mode(-1);
clear

str1 = "1 2 3 4 5 6 7 8 9 10";
L1 = msscanf(str1,"%d %d %d %d %d %d %d %d %d %d");
Test1=and(L1==[1:10]);

str2 = ""+..
"1 2 3 4 5 6 7 8 9 10 "+..
"11 12 13 14 15 16 17 18 19 20" +..
"21 22 23 24 25 26 27 28 29 30" +..
"31 32 33 34 35 36 37 38 39 40" +..
"41 42 43 44 45 46 47 48 49 50";

L2 = msscanf(str2,""+..
"%d %d %d %d %d %d %d %d %d %d "+ ..
"%d %d %d %d %d %d %d %d %d %d "+..
"%d %d %d %d %d %d %d %d %d %d "+..
"%d %d %d %d %d %d %d %d %d %d "+..
"%d %d %d %d %d %d %d %d %d %d");
Test2=and(L2==[1:50]);

str3 = " "+..
"1 2 3 4 5 6 7 8 9 10 "+..
"11 12 13 14 15 16 17 18 19 20 "+..
"21 22 23 24 25 26 27 28 29 30 "+..
"31 32 33 34 35 36 37 38 39 40 "+..
"41 42 43 44 45 46 47 48 49 50 "+..
"51 52 53 54 55 56 57 58 59 60"+..
"";

StrExec=" "+..
"L3 = msscanf(str3,"""+..
"%d %d %d %d %d %d %d %d %d %d "+..
"%d %d %d %d %d %d %d %d %d %d "+..
"%d %d %d %d %d %d %d %d %d %d "+..
"%d %d %d %d %d %d %d %d %d %d "+..
"%d %d %d %d %d %d %d %d %d %d "+..
"%d %d %d %d %d %d %d %d %d %d"+..
""");";
errmsg=execstr(StrExec,"errcatch");

Test3=(errmsg==998);

if (Test1 & Test2 & Test3) then
    affich_result(%T,1867);
else
    affich_result(%F,1867);
end

clear
