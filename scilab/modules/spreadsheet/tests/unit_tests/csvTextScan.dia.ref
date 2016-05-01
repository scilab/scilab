// =============================================================================
// Copyright (C) 2011 - INRIA - Michael Baudin
// Copyright (C) 2010 - 2011 - INRIA - Allan CORNET
// =============================================================================
// <-- CLI SHELL MODE -->
// =============================================================================
A = "        1;        2;     3";
B = "        4;        5;     6";
C = [A;B];
bbSTR = csvTextScan(C, ';', [], "string");
expected = [
"        1" , "        2" , "     3"
"        4" , "        5" , "     6"
];
assert_checkequal ( bbSTR , expected );
// =============================================================================
bbDouble = csvTextScan(C, ';', [], 'double');
expected = [1 2 3;4 5 6];
assert_checkequal ( bbDouble , expected );
// =============================================================================
path = SCI+"/modules/spreadsheet/tests/unit_tests/";
// =============================================================================
Mstr = [
"1" "0" "0" "0" "0"
"0" "1" "0" "0" "0"
"0" "0" "1" "0" "0"
];
//
r = mgetl(fullfile(path,"M_1.csv"));
b = csvTextScan(r, [], [], "string");
assert_checkequal (b,Mstr);
//
r = mgetl(fullfile(path,"M_2.csv"));
b = csvTextScan(r, ascii(9), [], "string");
assert_checkequal (b,Mstr);
//
r = mgetl(fullfile(path,"M_3.csv"));
b = csvTextScan(r, " ", ",", "string");
assert_checkequal (b,Mstr);
//
r = mgetl(fullfile(path,"M_4.csv"));
b = csvTextScan(r, ";", ",", "string");
assert_checkequal (b,Mstr);
// =============================================================================
Nstr = [
"Nan"   "1"    "Nan"
"-Inf"  "Inf"  "4"
];
//
r = mgetl(fullfile(path,"N_1.csv"));
b = csvTextScan(r, [], [], "string");
assert_checkequal (b,Nstr);
//
r = mgetl(fullfile(path,"N_2.csv"));
b = csvTextScan(r, ascii(9), [], "string");
assert_checkequal (b,Nstr);
//
r = mgetl(fullfile(path,"N_3.csv"));
b = csvTextScan(r, " ", ",", "string");
assert_checkequal (b,Nstr);
//
r = mgetl(fullfile(path,"N_4.csv"));
b = csvTextScan(r, ";", ",", "string");
assert_checkequal (b,Nstr);
// =============================================================================
Kstr = [
"1.1000000000000001" "0.10000000000000001"
"0.10000000000000001" "1.1000000000000001"
"0.10000000000000001" "0.10000000000000001"
];
Kstr2 = [
"1,1000000000000001" "0,10000000000000001"
"0,10000000000000001" "1,1000000000000001"
"0,10000000000000001" "0,10000000000000001"
];
//
r = mgetl(fullfile(path,"K_1.csv"));
b = csvTextScan(r, [], [], "string");
assert_checkequal (b,Kstr);
//
r = mgetl(fullfile(path,"K_2.csv"));
b = csvTextScan(r, ascii(9), [], "string");
assert_checkequal (b,Kstr);
//
r = mgetl(fullfile(path,"K_3.csv"));
b = csvTextScan(r, " ", ",", "string");
assert_checkequal (b,Kstr);
//
r = mgetl(fullfile(path,"K_3.csv"));
b = csvTextScan(r, " ", [], "string");
assert_checkequal (b,Kstr2);
//
r = mgetl(fullfile(path,"K_4.csv"));
b = csvTextScan(r, ";", ",", "string");
assert_checkequal (b,Kstr);
//
r = mgetl(fullfile(path,"K_4.csv"));
b = csvTextScan(r, ";", [], "string");
assert_checkequal (b,Kstr2);
// =============================================================================
S = [
  "Allan",                  "2", "CORNET";
  "csv read/write toolbox", "3", "for scilab"
];
//
r = mgetl(fullfile(path,"S_1.csv"));
b = csvTextScan(r, "|", [], "string");
assert_checkequal (b,S);
//
r = mgetl(fullfile(path,"S_2.csv"));
b = csvTextScan(r, ascii(9), [], "string");
assert_checkequal (b,S);
//
r = mgetl(fullfile(path,"S_3.csv"));
b = csvTextScan(r, "!", ",", "string");
assert_checkequal (b,S);
//
r = mgetl(fullfile(path,"S_4.csv"));
b = csvTextScan(r, ";", ",", "string");
assert_checkequal (b,S);
//
r = mgetl(fullfile(path,"S_1.csv"));
b = csvTextScan(r, "|", ".", "string");
assert_checkequal (b,S);
//
r = mgetl(fullfile(path,"S_1.csv"));
b = csvTextScan(r, "|", ".", "double");
ref = [%nan , 2, %nan; %nan, 3, %nan];
assert_checkequal ( b , ref);
// =============================================================================
r = mgetl(fullfile(path, "double_quotes.csv"), 5);
ref = ['Dummy1', 'Dummy1'; ..
       'Dummy2', 'Dummy2, Dummy2'; ..
       'Dummy3', '(""Dummy3"")'; ..
       '""Dummy4"" Dummy4','Dummy4'; ..
       'Dummy5', 'Dummy5 ""Dummy5""'];
for i=1:5
    b = csvTextScan(r(i), ",", [], "string");
    assert_checkequal(b , ref(i,:));
end
