// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//==================================
// unit tests printf
//==================================
printf("printf test 1:%s\n", "simple string");
//==================================
printf("printf test 2:%d\n", 42);
//==================================
printf("printf test 3:%f\n", 10.0/3);
//==================================
printf("printf test 4:%.10f\n", 10.0/3);
//==================================
printf("printf test 5:%-10.2f\n", 2.5);
//==================================
printf("printf test 6:%-010.2f\n", 2.5);
//==================================
printf("printf test 7:%010.2f\n", 2.5);
//==================================
printf("printf test 8:<%20s>\n", "foo");
//==================================
printf("printf test 9:<%-20s>\n", "bar");
//==================================
printf("printf test 10: 123456789012345\n");
//==================================
printf("printf test 11:<%15s>\n", "høyesterettsjustitiarius");
//==================================
printf("printf test 12: 123456789012345678901234567890\n");
//==================================
printf("printf test 13:<%30s>\n", "høyesterettsjustitiarius");
//==================================
printf("printf test 14:%5.2f\n", -12.34);
//==================================
printf("printf test 15:%5d\n", -12);
//==================================
printf("printf test 16:%x\n", 170);
//==================================
printf("printf test 17:%X\n", 170);
//==================================
printf("printf test 18:%.5s\n", "abcdefghij");
//==================================
printf("printf test 19:%-2s\n", "gazonk");
//==================================
