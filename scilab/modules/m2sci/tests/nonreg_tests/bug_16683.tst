// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2021 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 16683 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16683

// <-- Short Description -->
// m2sci: series of fix and improvements

Syntaxes = [
    "% NEW TYPES AND PROPERTIES DEFINED:"
    "a = @sin"                       // Function, with property Macro|Builtin|?
    "%m2scideclare a|1 1|Function|Builtin"      // must not warn
    "%m2scideclare b|1 1|Function|Macro"        // must not warn
    "%m2scideclare c|1 1|Polynomial|Real"       // must not warn
    "%m2scideclare d|1 1|Polynomial|Complex"    // must not warn
    "%m2scideclare e|1 1|Polynomial|?"          // must not warn
    "% NEW PROPERTIES DEFINED:  No warning must be yielded"
    "%m2scideclare s|2 3|String|String"     // property Char <> String
    "%m2scideclare s2|2 3|String|Char"      //  differentiates both (only for Matlab)
    "%m2scideclare t|2 3|Boolean|Boolean"   // Default Boolean property was "Real"
    "%m2scideclare u|2 3|Boolean|Sparse"    // Sparse Boolean introduced
    "%m2scideclare h|1 1|Handle|Figure" // Handle => Figure property introduced
    "%m2scideclare k|1 1|Handle|Axes"   // Handle => Axes property introduced
    "%m2scideclare v1|1 1|Int|int8"     // Int=>"int8"  property introduced
    "%m2scideclare v2|1 1|Int|int16"    // Int=>"int16" property introduced
    "%m2scideclare v3|1 1|Int|int32"    // Int=>"int32" property introduced
    "%m2scideclare v4|1 1|Int|int64"    // Int=>"int64" property introduced
    "%m2scideclare v5|1 1|Int|uint8"    // Int=>"uint8" property introduced
    "%m2scideclare v6|1 1|Int|uint16"   // Int=>"uint16" property introduced
    "%m2scideclare v7|1 1|Int|uint32"   // Int=>"uint32" property introduced
    "%m2scideclare v8|1 1|Int|uint64"   // Int=>"uint64" property introduced
    "%m2scideclare v9|1 1|Int|?"
    ""
    "% SYNTAX AND OPERATORS"
    "% --------------------"
    "% CONVERSIONS FIXED:"
    "a=1, b=2"              // was converted into `a = 1b = 2`
    "c = [''abc'';''def'']" // was of property Unknown instead of Char
    "%m2scideclare c|2 3|String|Char"   // checking: must yield no warning
    "d = [1 2]==1"                  // yielded a type==1 instead of 4
    "%m2scideclare d|1 2|Boolean|Boolean" // checking: must yield no warning
    "funh = @sin"                         // was converted into funh = 'sin'
    "%m2scideclare funh|1 1|Function|?" // checking: must yield no warning
    "%"
    "% CONVERSIONS IMPROVED:"
    "g = 2*3*4*5"   // was converted into ((2*3)*4)*5  instead of the same
    "h =  2*3/4"    // was converted into (2*3)/4  instead of the same
    "k = 2/3+4/5-3"
    "m = 1; n = 2"  // was converted into m = 1;n = 2  without separating space
    "for i=1:2, disp(i), end"  // a parasitic semi-colon was appended to end (by instruction2code)
    "%"
    "% Scilab 5 => 6 improvements"
    "p = true || false" // yielded `p = %t;if ~%t then p = %f;end` instead of the same
    "q = true && false" // yielded `q = %f;if %t then q = %f;end` instead of the same
    ""
    "% FUNCTIONS"
    "% ---------"
    "% CONVERSIONS FIXED:"
    "a = full([true false])" // yielded `full(bool2s([true false]))` instead of the same.
    "%m2scideclare spb|2 3|Boolean|Sparse"
    "a = full(spb)"   // yielded an error in convert2double
    "a = full(''hello'')" // yielded `mtlb_full('hello')` instead of the same.
    ""
    "%m2scideclare x|? ?|?|?"
    "a = isa(x,''logical'')"         // yielded `a = type(x)==4` instead of `a = or(type(x)==[4,6])`
    "a = isa(x,''function_handle'')" // yielded `a = type(x)==13` instead of `a = or(type(x)==[13 130])`
    ""
    "ishandle(@sin)"  // yielded type("sin")==9  instead of type(sin)==9
    ""
    "a = isvector(rand(1,2,3))"  // yielded an expression equivalent to %T
    ""
    "a = isreal(complex(1,0))"  // was `a = isreal(complex(1,0),0)` instead of the same
    ""
    "sparse([true false])" // yielded a `convert2double` error, instead of `sparse([%t,%f])`
    ""
    "% CONVERSIONS IMPROVED or UPDATED:"
    "a = isa(x,''integer'')"  // was not converted
    "a = isa(x,''float'')"    // was not converted
    ""
    "% rem() was using a formula instead of modulo()"
    "a = rem(-10, 3)"
    "a = rem([-10 4], -3)"
    "a = rem([-10 4], [-3 2])"
    "a = rem(-10, [-3 2])"
    ];
Ref = [
  "// NEW TYPES AND PROPERTIES DEFINED:"
  "a = sin"
  ""
  ""
  ""
  ""
  ""
  "// NEW PROPERTIES DEFINED:  No warning must be yielded"
  ""
  ""
  ""
  ""
  ""
  ""
  ""
  ""
  ""
  ""
  ""
  ""
  ""
  ""
  ""
  ""
  "// SYNTAX AND OPERATORS"
  "// --------------------"
  "// CONVERSIONS FIXED:"
  "a = 1, b = 2"
  "c = [""abc"";""def""]"
  ""
  "d = [1,2]==1"
  ""
  "funh = sin"
  ""
  "// "
  "// CONVERSIONS IMPROVED:"
  "g = 2*3*4*5"
  "h = 2*3/4"
  "k = 2/3+4/5-3"
  "m = 1; n = 2"
  "for i = 1:2"
  "  disp(i)"
  "end"
  "// "
  "// Scilab 5 => 6 improvements"
  "p = %t || %f"
  "q = %t && %f"
  ""
  "// FUNCTIONS"
  "// ---------"
  "// CONVERSIONS FIXED:"
  "a = full([%t,%f])"
  ""
  "a = full(spb)"
  "a = full(""hello"")"
  ""
  ""
  "a = or(type(x)==[4,6])"
  "a = or(type(x)==[13,130])"
  ""
  "type(sin)==9"
  ""
  "%v0 = rand(1,2,3); a = ndims(%v0)==2 & min(size(%v0))==1"
  ""
  "// !! L.65: Matlab function complex not yet converted, original calling sequence used."
  "isreal(complex(1,0))"
  ""
  "sparse([%t,%f])"
  ""
  "// CONVERSIONS IMPROVED or UPDATED:"
  "a = type(x)==8"
  "a = type(x)==1"
  ""
  "// rem() was using a formula instead of modulo()"
  "a = modulo(-10,3)"
  "a = modulo([-10,4],-3)"
  "a = modulo([-10,4],[-3,2])"
  "a = modulo(-10,[-3,2])"
    ];

mfile = TMPDIR + "/_gcf";
mputl(Syntaxes, mfile + ".m");
mfile2sci(mfile + ".m", TMPDIR)
Res = mgetl(mfile + ".sci");

assert_checkequal(Res, ["";Ref]);
