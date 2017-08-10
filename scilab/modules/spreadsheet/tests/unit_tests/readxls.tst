//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

ref1 = [ 0 1 2 3 4 5 6 7 8 9 ;
        10 11 12 13 14 15 16 17 18 19 ;
        20 21 22 23 24 25 26 27 28 29 ;
        30 31 32 33 34 35 36 37 38 39 ;
        40 41 42 43 44 45 46 47 48 49 ];

ref2 = 'Test string 1';
ref3 = 'Test string 2';
ref4 = 'Test string 3';


XLS = readxls(SCI+'/modules/spreadsheet/tests/unit_tests/readxls.xls');
if type(XLS) <> 17 then pause,end;
if size(XLS(1)) <> [ 10 10 ] then pause,end;
if size(XLS(2)) <> 0 then pause,end;
if size(XLS(3)) <> 0 then pause,end;

Feuil1 = XLS(1);
if size(Feuil1.value)<> [ 10 10 ] then pause,end;
if size(Feuil1.text)<> [ 10 10 ] then pause,end;        
if Feuil1(1:5,1:10) <> ref1 then pause,end;
if Feuil1(8,1) <> ref2 then pause,end;
if Feuil1(9,2) <> ref3 then pause,end;
if Feuil1(10,3) <> ref4 then pause,end;
// =============================================================================

