// =============================================================================
// Copyright (C) 2010 - 2012 - INRIA - Michael BAUDIN
// =============================================================================
// <-- CLI SHELL MODE -->
// =============================================================================
// <-- Non-regression test for bug 281 -->
//
// <-- URL -->
//  http://forge.scilab.org/index.php/p/csv-readwrite/issues/281/
//
// <-- Short Description -->
// The substitute option does not work.
// =============================================================================
path = SCI+"/modules/spreadsheet/tests/unit_tests/";

//
filename = fullfile(TMPDIR,"ticket281.csv");
content = [
"Infinity"
];
substitute = [
"Infinity" "Inf"
"Not-A-Number" "Nan"
];
mputl(content,filename);
M = csvRead(filename,[],[],"string",substitute);
expected = "Inf";
assert_checkequal ( M , expected );
M = csvRead(filename,[],[],"double",substitute);
expected=%inf;
assert_checkequal ( M , expected );
//
// Try with 3 replacements
filename = fullfile(TMPDIR,"ticket281.csv");
content = [
"Infinity"
];
substitute = [
"Infinity" "Inf"
"Not-A-Number" "Nan"
"Youwillnotfind" "NotAChance"
];
mputl(content,filename);
M = csvRead(filename,[],[],"string",substitute);
expected = "Inf";
assert_checkequal ( M , expected );
M = csvRead(filename,[],[],"double",substitute);
expected=%inf;
assert_checkequal ( M , expected );
//
//

// Create a data file with particular Infinities and complex numbers
Astr = [
"6+3*I" "13-7*I" "20+4*I" "27-1.5*I" "34+3.14*I" "41-3*I" "48+3*I" "55-7*I"
"+0" "-0" "Infinity" "-Infinity" "Nan" "1.D+308" "1.e-308" "1.e-323"
];
expected = [
"6+3*%i" "13-7*%i" "20+4*%i" "27-1.5*%i" "34+3.14*%i" "41-3*%i" "48+3*%i" "55-7*%i"
"+0" "-0" "%inf" "-%inf" "%nan" "1.D+308" "1.e-308" "1.e-323"
];
filename = fullfile(TMPDIR , "foo.csv");
sep = ",";
fd = mopen(filename,'wt');
for i = 1 : size(Astr,"r")
    mfprintf(fd,"%s\n",strcat(Astr(i,:),sep));
end
mclose(fd);
//
// Customize the replace map.

substitute=["Nan","%nan";"Infinity","%inf";"I","%i"];
M = csvRead ( filename , [] , [] , "string" , substitute );
assert_checkequal ( M , expected );
// =============================================================================

