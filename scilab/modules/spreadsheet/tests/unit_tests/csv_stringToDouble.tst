// =============================================================================
// Copyright (C) 2011 - INRIA - Michael Baudin
// Copyright (C) 2010 - 2011 - INRIA - Allan CORNET
// =============================================================================
// <-- CLI SHELL MODE -->
// =============================================================================
r = csvStringToDouble("-Nani");
assert_checkequal ( r , complex(0,%nan) );
// =============================================================================
r = csvStringToDouble("Nani");
assert_checkequal ( r , complex(0,%nan) );
// =============================================================================
r = csvStringToDouble("+Nani");
assert_checkequal ( r , complex(0,%nan) );
// =============================================================================
r = csvStringToDouble("-Infi");
assert_checkequal ( r , complex(0,-%inf) );
// =============================================================================
r = csvStringToDouble("Infi");
assert_checkequal ( r , complex(0,%inf) );
// =============================================================================
r = csvStringToDouble("+Infi");
assert_checkequal ( r , complex(0,%inf) );
// =============================================================================
r = csvStringToDouble("Nan-Nani");
assert_checkequal ( r , complex(%nan,%nan) );
// =============================================================================
r = csvStringToDouble("Nan-2i");
assert_checkequal ( r , complex(%nan,-2) );
// =============================================================================
r = csvStringToDouble("-Nan-2i");
assert_checkequal ( r , complex(%nan,-2) );
// =============================================================================
r = csvStringToDouble("+Nan-2i");
assert_checkequal ( r , complex(%nan,-2) );
// =============================================================================
r = csvStringToDouble("Nan-Infi");
assert_checkequal ( r , complex(%nan,-%inf) );
// =============================================================================
r = csvStringToDouble("-Inf-2i");
assert_checkequal ( r , complex(-%inf,-2) );
// =============================================================================
r = csvStringToDouble("+Inf-2i");
assert_checkequal ( r , complex(%inf,-2) );
// =============================================================================
r = csvStringToDouble("Inf-2i");
assert_checkequal ( r , complex(%inf,-2) );
// =============================================================================
r = csvStringToDouble("Inf-Infi");
assert_checkequal ( r , complex(%inf,-%inf) );
// =============================================================================
r = csvStringToDouble("Inf+Infi");
assert_checkequal ( r , complex(%inf,%inf) );
// =============================================================================
r = csvStringToDouble("1+Nani");
assert_checkequal ( r , complex(1,%nan) );
// =============================================================================
r = csvStringToDouble("1-Nani");
assert_checkequal ( r , complex(1,%nan) );
// =============================================================================
r = csvStringToDouble("-1-Nani");
assert_checkequal ( r , complex(-1,%nan) );
// =============================================================================
r = csvStringToDouble("1+Infi");
assert_checkequal ( r , complex(1,%inf) );
// =============================================================================
r = csvStringToDouble("1-Infi");
assert_checkequal ( r , complex(1,-%inf) );
// =============================================================================
assert_checkequal ( csvStringToDouble("Nan") , %nan );
assert_checkequal ( csvStringToDouble("-Nan"), %nan );
assert_checkequal ( csvStringToDouble("+Nan"), %nan );
assert_checkequal ( csvStringToDouble("-Inf") , -%inf );
assert_checkequal ( csvStringToDouble("Inf") , %inf );
assert_checkequal ( csvStringToDouble("+Inf") , %inf );
assert_checkequal ( csvStringToDouble("+i") , %i  );
assert_checkequal ( csvStringToDouble("-i") , -%i  );
assert_checkequal ( csvStringToDouble("+j") , %i  );
assert_checkequal ( csvStringToDouble("-j") , -%i  );
assert_checkequal ( csvStringToDouble("i") , %i  );
assert_checkequal ( csvStringToDouble("j") , %i  );
assert_checkequal ( csvStringToDouble(" 3 + 2i") , complex(3,2) );
assert_checkequal ( csvStringToDouble(" 3 - 2i") , complex(3,- 2) );
assert_checkequal ( csvStringToDouble(" -3 + 2i") , complex(-3,2) );
assert_checkequal ( csvStringToDouble(" 3 - 2i") , complex(3,-2) );
assert_checkequal ( csvStringToDouble("- 3 - 2i") , complex(-3,-2) );
assert_checkequal ( csvStringToDouble("+3 - 2i") , complex(+3,-2) );
assert_checkequal ( csvStringToDouble("+3 + 2i") , complex(3,2) );
assert_checkequal ( csvStringToDouble("+3 - 2i") , complex(3,-2) );
assert_checkequal ( csvStringToDouble("4+i") , complex(4,1) );
assert_checkequal ( csvStringToDouble("4-i") , complex(4,-1) );
assert_checkequal ( csvStringToDouble("-4+i") , complex(-4,1) );
assert_checkequal ( csvStringToDouble("-4-i") , complex(-4,-1) );
assert_checkequal ( csvStringToDouble("+4+i") , complex(4,1) );
assert_checkequal ( csvStringToDouble("+4-i") , complex(4,-1) );
assert_checkequal ( csvStringToDouble("10")  , 10 );
assert_checkequal ( csvStringToDouble("+10") , 10 );
assert_checkequal ( csvStringToDouble("-10") , -10 );
assert_checkequal ( csvStringToDouble("10.") , 10 );
assert_checkequal ( csvStringToDouble("+10.") , 10 );
assert_checkequal ( csvStringToDouble("-10.") , -10 );
assert_checkequal ( csvStringToDouble(".4") , .4 );
assert_checkequal ( csvStringToDouble("+.4") , .4 );
assert_checkequal ( csvStringToDouble("-.4") , -.4 );
assert_checkequal ( csvStringToDouble("10.4") , 10.4 );
assert_checkequal ( csvStringToDouble("+10.4") , 10.4 );
assert_checkequal ( csvStringToDouble("-10.4") , -10.4 );
assert_checkequal ( csvStringToDouble("10+10j") , complex(10, 10) );
assert_checkequal ( csvStringToDouble("+10+10j") , complex(10, 10) );
assert_checkequal ( csvStringToDouble("-10+10j") , complex(-10, 10) );
assert_checkequal ( csvStringToDouble("10.+10j") , complex(10, 10) );
assert_checkequal ( csvStringToDouble("+10.+10j") , complex(10, 10) );
assert_checkequal ( csvStringToDouble("-10.+10j") , complex(-10, 10) );
assert_checkequal ( csvStringToDouble("10.4+10j") , complex(10.4, 10) );
assert_checkequal ( csvStringToDouble("+10.4+10j") , complex(10.4, 10) );
assert_checkequal ( csvStringToDouble("-10.4+10j") , complex(-10.4, 10) );
assert_checkequal ( csvStringToDouble(".4+10j") , complex(.4, 10) );
assert_checkequal ( csvStringToDouble("+.4+10j") , complex(.4, 10) );
assert_checkequal ( csvStringToDouble("-.4+10j") , complex(-.4, 10) );
assert_checkequal ( csvStringToDouble("10+10.4j") , complex(10, 10.4) );
assert_checkequal ( csvStringToDouble("10.4+10.4j") , complex(10.4, 10.4) );
assert_checkequal ( csvStringToDouble("+10.4+10.4j") , complex(10.4, 10.4) );
assert_checkequal ( csvStringToDouble("+10.4-10.4j") , complex(10.4, -10.4) );
assert_checkequal ( csvStringToDouble("1i") , %i );
assert_checkequal ( csvStringToDouble("1j") , %i );
assert_checkequal ( csvStringToDouble("+1j") , %i );
assert_checkequal ( csvStringToDouble("+1i") , %i );
assert_checkequal ( csvStringToDouble("-1j") , complex(0,-1) );
assert_checkequal ( csvStringToDouble("-1i") , complex(0,-1) );
assert_checkequal ( csvStringToDouble("10j") , complex(0,10) );
assert_checkequal ( csvStringToDouble("+10j") , complex(0,10) );
assert_checkequal ( csvStringToDouble("-10j") , complex(0, -10) );
assert_checkequal ( csvStringToDouble("10.j") , complex(0,10) );
assert_checkequal ( csvStringToDouble("+10.j") , complex(0,10) );
assert_checkequal ( csvStringToDouble("-10.j") , complex(0,-10) );
assert_checkequal ( csvStringToDouble(".4j") , complex(0,.4) );
assert_checkequal ( csvStringToDouble("+.4j") , complex(0,.4) );
assert_checkequal ( csvStringToDouble("-.4j") , complex(0,-.4) );
assert_checkequal ( csvStringToDouble("10.4j") , complex(0,10.4) );
assert_checkequal ( csvStringToDouble("+10.4j") , complex(0,10.4) );
assert_checkequal ( csvStringToDouble("-10.4j") , complex(0,-10.4) );
assert_checkequal ( csvStringToDouble("4e2") , 400 );
assert_checkequal ( csvStringToDouble("-4e2") , -400 );
assert_checkequal ( csvStringToDouble("+4e2") , 400 );
assert_checkequal ( csvStringToDouble("0.4e2") , 40 );
assert_checkequal ( csvStringToDouble("3e2 + 2e3i") , complex(300, 2000) );
assert_checkequal ( csvStringToDouble("3e2 - 2e3i") , complex(300, -2000) );
if ( getos() == "Windows" ) then
    assert_checkequal ( csvStringToDouble("3e2 - 2d3i") , complex(300, -2000) );
    assert_checkequal ( csvStringToDouble("3e2 + 2d3i") , complex(300, 2000) );
    assert_checkequal ( csvStringToDouble("3d2 + 2e3i") , complex(300, 2000) );
    assert_checkequal ( csvStringToDouble("3d2 - 2e3i") , complex(300, -2000) );
    assert_checkequal ( csvStringToDouble("3d2 + 2d3i") , complex(300, 2000) );
    assert_checkequal ( csvStringToDouble("3d2 - 2d3i") , complex(300, -2000) );
end
// =============================================================================
r = csvStringToDouble("3e2 - blabli");
assert_checkequal ( r , %nan);
// =============================================================================
r = csvStringToDouble("3e2 - blabli", %t);
assert_checkequal ( r , %nan);
// =============================================================================
assert_checkequal ( execstr("r = csvStringToDouble(""3e2 - blabli"", %f);", "errcatch") , 999 );
assert_checkequal ( lasterror() , msprintf(_("%s: can not convert data.\n"), "csvStringToDouble") );
// =============================================================================
r = csvStringToDouble("-Nan*%i");
assert_checkequal ( isnan(imag(r)) , %t);
assert_checkequal ( csvStringToDouble("+%i") , %i  );
assert_checkequal ( csvStringToDouble("-%i") , -%i  );
assert_checkequal ( csvStringToDouble("%i") , %i  );
assert_checkequal ( csvStringToDouble(" 3 + 2*%i") , complex(3,2) );
assert_checkequal ( csvStringToDouble(" 3 - 2*%i") , complex(3,- 2) );
assert_checkequal ( csvStringToDouble(" -3 + 2*%i") , complex(-3,2) );
assert_checkequal ( csvStringToDouble(" 3 - 2*%i") , complex(3,-2) );
assert_checkequal ( csvStringToDouble("- 3 - 2*%i") , complex(-3,-2) );
assert_checkequal ( csvStringToDouble("+3 - 2*%i") , complex(+3,-2) );
assert_checkequal ( csvStringToDouble("+3 + 2*%i") , complex(3,2) );
assert_checkequal ( csvStringToDouble("+3 - 2*%i") , complex(3,-2) );
assert_checkequal ( csvStringToDouble("4+%i") , complex(4,1) );
assert_checkequal ( csvStringToDouble("4-%i") , complex(4,-1) );
assert_checkequal ( csvStringToDouble("-4+%i") , complex(-4,1) );
assert_checkequal ( csvStringToDouble("-4-%i") , complex(-4,-1) );
assert_checkequal ( csvStringToDouble("+4+%i") , complex(4,1) );
assert_checkequal ( csvStringToDouble("+4-%i") , complex(4,-1) );
assert_checkequal ( csvStringToDouble("1*%i") , %i );
assert_checkequal ( csvStringToDouble("+1*%i") , %i );
assert_checkequal ( csvStringToDouble("-1*%i") , complex(0,-1) );
assert_checkequal ( csvStringToDouble("3e2 + 2e3*%i") , complex(300, 2000) );
assert_checkequal ( csvStringToDouble("3e2 - 2e3*%i") , complex(300, -2000) );
// =============================================================================
assert_checkequal ( csvStringToDouble('a'), %nan );
assert_checkequal ( csvStringToDouble('e'), %nan );
assert_checkequal ( csvStringToDouble('d'), %nan );
assert_checkequal ( csvStringToDouble('E'), %nan );
assert_checkequal ( csvStringToDouble('e'), %nan );
// =============================================================================
assert_checkequal ( csvStringToDouble("%pi") , %pi );
assert_checkequal ( csvStringToDouble("-%pi") , -%pi );
assert_checkequal ( csvStringToDouble("%nan") , %nan );
assert_checkequal ( csvStringToDouble("%inf") , %inf );
assert_checkequal ( csvStringToDouble("-%inf") , -%inf );
assert_checkequal ( csvStringToDouble("+%inf") , %inf );
// =============================================================================
