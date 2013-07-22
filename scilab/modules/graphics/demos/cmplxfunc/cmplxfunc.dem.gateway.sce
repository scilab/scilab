// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

demopath = get_absolute_file_path("cmplxfunc.dem.gateway.sce");

subdemolist = ["log"       ,"demo_log.dem.sce"    ; ..
"exp"       ,"demo_exp.dem.sce"    ; ..
"tan"       ,"demo_tan.dem.sce"    ; ..
"atan"      ,"demo_atan.dem.sce"   ; ..
"sin"       ,"demo_sin.dem.sce"    ; ..
"asin"      ,"demo_asin.dem.sce"   ; ..
"cos"       ,"demo_cos.dem.sce"    ; ..
"sinh"      ,"demo_sinh.dem.sce"   ; ..
"asinh"     ,"demo_asinh.dem.sce"  ; ..
"cosh"      ,"demo_cosh.dem.sce"   ; ..
"acosh"     ,"demo_acosh.dem.sce"  ; ..
"tanh"      ,"demo_tanh.dem.sce"   ; ..
"atanh"     ,"demo_atanh.dem.sce"  ; ..
"custom"    ,"demo_custom.dem.sce" ];

subdemolist(:,2) = demopath + subdemolist(:,2);
clear demopath;
