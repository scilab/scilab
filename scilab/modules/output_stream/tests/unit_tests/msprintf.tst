// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// unit tests for msprintf function
// =============================================================================

// format '%f'
// =============================================================================

assert_checkequal(msprintf("%f",-35), "-35.000000");

assert_checkequal(msprintf("%f",35.55), "35.550000");
assert_checkequal(msprintf("%f",0.00433), "0.004330");
assert_checkequal(msprintf("%f",0.0000000345456), "0.000000");
assert_checkequal(msprintf("%f",1112423453), "1112423453.000000");

assert_checkequal(msprintf("%15f",-35), "     -35.000000");
assert_checkequal(msprintf("%15f",0.00433), "       0.004330");
assert_checkequal(msprintf("%15f",0.0000000345456), "       0.000000");
assert_checkequal(msprintf("%15f",1112423453), "1112423453.000000");

assert_checkequal(msprintf("%.1f" ,-35), "-35.0");
assert_checkequal(msprintf("%.0f" ,-35), "-35");
assert_checkequal(msprintf("%#.0f",-35), "-35.");
assert_checkequal(msprintf("%.1f" ,0.00433), "0.0");
assert_checkequal(msprintf("%.15f",0.0000000345456), "0.000000034545600");
assert_checkequal(msprintf("%.1f" ,11124234534545), "11124234534545.0");

// format '%g'
// =============================================================================

assert_checkequal(msprintf("%g" ,-35), "-35");
assert_checkequal(msprintf("%g" ,35.55), "35.55");
assert_checkequal(msprintf("%g" ,35.551234567890), "35.5512");
assert_checkequal(msprintf("%+g",35.551234567890), "+35.5512");
assert_checkequal(msprintf("%g" ,0.00433), "0.00433");

assert_checkequal(msprintf("%g" ,0.0000000345456), "3.45456e-08");

assert_checkequal(msprintf("%g" ,11124234534545), "1.11242e+13");

assert_checkequal(msprintf("%15g",-35), "            -35");
assert_checkequal(msprintf("%15g",0.00433), "        0.00433");

assert_checkequal(msprintf("%15g",0.0000000345456), "    3.45456e-08");

assert_checkequal(msprintf("%15g",11124234534545), "    1.11242e+13");

assert_checkequal(msprintf("%.1g",-35.1), "-4e+01");

assert_checkequal(msprintf("%.0g",-35.1), "-4e+01");

assert_checkequal(msprintf("%#.0g",-35.1), "-4.e+01");

assert_checkequal(msprintf("%#.0G",-35.1), "-4.E+01");

assert_checkequal(msprintf("%.1g",0.00433), "0.004");

assert_checkequal(msprintf("%.15g",0.0000000345456), "3.45456e-08");

assert_checkequal(msprintf("%.1g",11124234534545), "1e+13");

// format '%e'
// =============================================================================

assert_checkequal(msprintf("%e",-35), "-3.500000e+01");

assert_checkequal(msprintf("%e",35.55), "3.555000e+01");

assert_checkequal(msprintf("%+e",35.55), "+3.555000e+01");

assert_checkequal(msprintf("%e",35.551234567890), "3.555123e+01");

assert_checkequal(msprintf("%e",0.00433), "4.330000e-03");

assert_checkequal(msprintf("%e",0.0000000345456), "3.454560e-08");

assert_checkequal(msprintf("%e",11124234534545), "1.112423e+13");

assert_checkequal(msprintf("%E",11124234534545), "1.112423E+13");

assert_checkequal(msprintf("%15e",-35), "  -3.500000e+01");

assert_checkequal(msprintf("%15e",0.00433), "   4.330000e-03");

assert_checkequal(msprintf("%15e",0.0000000345456), "   3.454560e-08");

assert_checkequal(msprintf("%+15e",0.0000000345456), "  +3.454560e-08");

assert_checkequal(msprintf("%15e",11124234534545), "   1.112423e+13");

assert_checkequal(msprintf("%.1e",-35), "-3.5e+01");

assert_checkequal(msprintf("%.0e",-35.1), "-4e+01");

assert_checkequal(msprintf("%.1e",0.00433), "4.3e-03");

assert_checkequal(msprintf("%.15e",0.0000000345456), "3.454560000000000e-08");

assert_checkequal(msprintf("%.1e",11124234534545), "1.1e+13");

// format '%c'
// =============================================================================

assert_checkequal(msprintf("%c","t"), "t");
assert_checkequal(msprintf("%10c","t"), "         t");
assert_checkequal(msprintf("%10.3c","t"), "         t");
assert_checkequal(msprintf("%-10c","t"), "t         ");

// format '%s'
// =============================================================================

assert_checkequal(msprintf("%s","text"), "text");
assert_checkequal(msprintf("%10s","text"), "      text");
assert_checkequal(msprintf("%10.3s","text"), "       tex");
assert_checkequal(msprintf("%-10s","text"), "text      ");
assert_checkequal(msprintf("%s","t"), "t");

// format '%x'
// =============================================================================

assert_checkequal(msprintf("%x",123), "7b");
assert_checkequal(msprintf("%.10x",123), "000000007b");
assert_checkequal(msprintf("%x",-123), "ffffff85");
assert_checkequal(msprintf("%X",-123), "FFFFFF85");
assert_checkequal(msprintf("%#.3X",12), "0X00C");

// format '%o'
// =============================================================================
assert_checkequal(msprintf("%015o",-12), "000037777777764");

// Vectorisation
// =============================================================================

nb_row = 10000;

// test 1

A = "row "+string(1:nb_row)';
B = 100*rand(nb_row,3);
C = msprintf("%10s => %08.4f %08.4f %08.4f\n",A,B);

assert_checkequal(size(C), [nb_row,1]);

for i=1:nb_row
    assert_checkequal(length(C(i)), 40);
end

// test 2
B = 100*rand(nb_row,2);
C = 100*rand(nb_row,1);
D = msprintf("%10s => %08.4f %08.4f %08.4f\n",A,B,C);

assert_checkequal(size(D), [nb_row,1]);

for i=1:nb_row
    assert_checkequal(size(D), [nb_row,1]);
end

// test 3
B = 100*rand(nb_row,1);
C = 100*rand(nb_row,1);
D = 100*rand(nb_row,1);
E = msprintf("%10s => %08.4f %08.4f %08.4f\n",A,B,C,D);

assert_checkequal(size(E), [nb_row,1]);

for i=1:nb_row
    assert_checkequal(size(E), [nb_row,1]);
end

// test 4
B = 100*rand(nb_row,1);

assert_checkfalse(execstr("msprintf(""%10s => %08.4f %08.4f %08.4f\n"",A,B,B);","errcatch")     == 0);
refMsg = msprintf(_("%s: Wrong number of input arguments: data doesn''t fit with format.\n"), "msprintf");
assert_checkerror("msprintf(""%10s => %08.4f %08.4f %08.4f\n"",A,B,B);", refMsg);

// No arg
assert_checkfalse(execstr("msprintf();","errcatch")     == 0);
refMsg = msprintf(_("%s: Wrong number of input arguments: at least %d expected.\n"), "msprintf", 1);
assert_checkerror("msprintf();", refMsg);

// overload: Arg not managed
s=poly(0,"s");
p=1+s+2*s^2;

assert_checkfalse(execstr("msprintf(""plop"",p);","errcatch") <> 999);

assert_checkfalse(execstr("msprintf(""%s %s"",""plop"");","errcatch")     <> 999);
refMsg = msprintf(_("%s: Wrong number of input arguments: data doesn''t fit with format.\n"), "msprintf");
assert_checkerror("msprintf(""%s %s"",""plop"");", refMsg);

