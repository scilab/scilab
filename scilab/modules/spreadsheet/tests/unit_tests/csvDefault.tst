//<-- CLI SHELL MODE -->
// =============================================================================
// Copyright (C) 2011 - INRIA - Michael Baudin
// =============================================================================
// See all the available fields
allvalues = csvDefault();
expected = [
    "separator"  ","
    "decimal"    "."
    "conversion" "double"
    "precision"  "%.17lg"
    "regexp" ""
];

if getos() == "Windows" then
  expected = [expected; "eol" "windows"];
else
  expected = [expected; "eol" "linux"];
end
expected = [expected; "encoding" "utf-8"];
expected = [expected; "blank" "on"];

assert_checkequal ( allvalues , expected );
// Get the value of the "precision" field
value = csvDefault("precision");
assert_checkequal ( value , "%.17lg" );
// Set the value of the "precision" field
bRes = csvDefault("precision","%.17e");
assert_checkequal ( bRes , %t );
// Set the value of the "precision" field to
// an impossible value
bRes = csvDefault("precision","Ouch!");
assert_checkequal ( bRes , %f );
// =============================================================================

