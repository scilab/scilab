// =============================================================================
// Copyright (C) 2010 - 2012 - INRIA - Allan CORNET
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// =============================================================================
assert_checkequal (csvIsnum('%pi'), %t);
assert_checkequal (csvIsnum('1'), %t);
assert_checkequal (csvIsnum('1.2'), %t);
assert_checkequal (csvIsnum('+1.2'), %t);
assert_checkequal (csvIsnum('-1.2'), %t);
assert_checkequal (csvIsnum('1e2'), %t);
if getos() == "Windows" then
   assert_checkequal (csvIsnum('1d2'), %t);
end
assert_checkequal (csvIsnum('1E2'), %t);
if getos() == "Windows" then
   assert_checkequal (csvIsnum('1D2'), %t);
end
assert_checkequal (csvIsnum('1e+2'), %t);
assert_checkequal (csvIsnum('1e-2'), %t);
assert_checkequal (csvIsnum('+1e+2'), %t);
assert_checkequal (csvIsnum('-1e+2'), %t);
assert_checkequal (csvIsnum('-12e+3'), %t);
assert_checkequal (csvIsnum('-1.2e+3'), %t);
assert_checkequal (csvIsnum('-1.2e+345'), %t);
assert_checkequal (csvIsnum('a'), %f);
assert_checkequal (csvIsnum('e'), %f);
assert_checkequal (csvIsnum('d'), %f);
assert_checkequal (csvIsnum('E'), %f);
assert_checkequal (csvIsnum('e'), %f);
assert_checkequal (csvIsnum('-1.2a+345'), %f);
assert_checkequal (csvIsnum('-1.2e+3+45'), %f);
assert_checkequal (csvIsnum('-1.2e+3-45'), %f);
// =============================================================================



