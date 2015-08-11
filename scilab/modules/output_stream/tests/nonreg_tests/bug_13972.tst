// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 13972 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=13972
//
// <-- Short Description -->
// use of wildcard '*' in printf expressions was not managed
//

//with double
printf("|%*f|\n", 20, rand());
printf("|%*f|\n", -20, rand());
printf("|%.*f|\n", 8, rand());
printf("|%.*f|\n", -8, rand());
printf("|%*.*f|\n", 20, 8, rand());
printf("|%*.*f|\n", 20, -8, rand());
printf("|%*.*f|\n", -20, 8, rand());
printf("|%*.*f|\n", -20, -8, rand());

//with string
printf("|%s|\n", "test");
printf("|%*s|\n", -10, "test");
printf("|%*s|\n", -5, "test");
printf("|%*s|\n", 0, "test");
printf("|%*s|\n", 5, "test");
printf("|%*s|\n", 10, "test");

//multiple
printf("|%*s|%*s|%*s|%*s|\n", -10, "test", -5, "test", 5, "test", 10, "test");
printf("|%*s|\n", -10, ["t";"tt";"ttt";"tttt";"ttttt";"tttttt";"ttttttt";"tttttttt";"ttttttttt";"tttttttttt"]);
printf("|%*s|\n", 10, ["t";"tt";"ttt";"tttt";"ttttt";"tttttt";"ttttttt";"tttttttt";"ttttttttt";"tttttttttt"]);

printf("|%*d|%*d|%*d|%*d|\n", -10, -10, -5, -5, 5, 5, 10, 10);
printf("|%*d|\n", -8, [1;22;333;4444;55555;666666;7777777;88888888]);
printf("|%*d|\n", 8, [1;22;333;4444;55555;666666;7777777;88888888]);
printf("|%0*d|\n", 8, [1;22;333;4444;55555;666666;7777777;88888888]);
printf("|%.*d|\n", -8, [1;22;333;4444;55555;666666;7777777;88888888]);
printf("|%.*d|\n", 8, [1;22;333;4444;55555;666666;7777777;88888888]);
