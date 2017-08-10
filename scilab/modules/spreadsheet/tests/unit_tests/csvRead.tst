// =============================================================================
// Copyright (C) 2011 - INRIA - Michael Baudin
// Copyright (C) 2010 - 2011 - INRIA - Allan CORNET
// =============================================================================
// <-- CLI SHELL MODE -->
path = SCI+"/modules/spreadsheet/tests/unit_tests/";
// =============================================================================
Mstr = [
"1" "0" "0" "0" "0"
"0" "1" "0" "0" "0"
"0" "0" "1" "0" "0"
];
//
r = csvRead(fullfile(path,"M_1.csv"),[],[],"string");
assert_checkequal (r,Mstr);
//
r = csvRead(fullfile(path,"M_2.csv"), ascii(9), [], "string");
assert_checkequal (r,Mstr);
//
r = csvRead(fullfile(path,"M_3.csv"), " ", ",", "string");
assert_checkequal (r,Mstr);
//
r = csvRead(fullfile(path,"M_4.csv"), ";", ",", "string");
assert_checkequal (r,Mstr);
// =============================================================================
Nstr = [
"Nan"   "1"    "Nan"
"-Inf"  "Inf"  "4"
];
//
r = csvRead(fullfile(path,"N_1.csv"), [], [], "string");
assert_checkequal (r,Nstr);
//
r = csvRead(fullfile(path,"N_2.csv"), ascii(9), [], "string");
assert_checkequal (r,Nstr);
//
r = csvRead(fullfile(path,"N_3.csv"), " ", ",", "string");
assert_checkequal (r,Nstr);
//
r = csvRead(fullfile(path,"N_4.csv"), ";", ",", "string");
assert_checkequal (r,Nstr);
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
r = csvRead(fullfile(path,"K_1.csv"), [], [], "string");
assert_checkequal (r,Kstr);
//
r = csvRead(fullfile(path,"K_2.csv"), ascii(9), [], "string");
assert_checkequal (r,Kstr);
//
r = csvRead(fullfile(path,"K_3.csv"), " ", ",", "string");
assert_checkequal (r,Kstr);
//
r = csvRead(fullfile(path,"K_4.csv"), ";", ",", "string");
assert_checkequal (r,Kstr);
//
r = csvRead(fullfile(path,"K_4.csv"), ";", [], "string");
assert_checkequal (r,Kstr2);
// =============================================================================
S = [
  "Allan",                  "2", "CORNET";
  "csv read/write toolbox", "3", "for scilab"
];
//
r = csvRead(fullfile(path,"S_1.csv"), "|", [], "string");
assert_checkequal (r,S);
//
r = csvRead(fullfile(path,"S_2.csv"), ascii(9), [], "string");
assert_checkequal (r,S);
//
r = csvRead(fullfile(path,"S_3.csv"), "!", ",", "string");
assert_checkequal (r,S);
//
r = csvRead(fullfile(path,"S_4.csv"), ";", ",", "string");
assert_checkequal (r,S);
//
r = csvRead(fullfile(path,"S_1.csv"), "|", ".", "string");
assert_checkequal (r,S);
//
r = csvRead(fullfile(path,"S_1.csv"), "|", ".", "double");
ref = [%nan , 2, %nan; %nan, 3, %nan];
assert_checkequal ( r , ref);
// =============================================================================
filename = fullfile(path,"data.csv");
N = csvRead(filename,",",".","double",["Nan", "0"]);
expected = [%inf, 0];
assert_checkequal ( N , expected );
// =============================================================================

