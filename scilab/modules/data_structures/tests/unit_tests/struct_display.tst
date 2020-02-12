// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019, 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Unitary tests of default display of scalar (nested) structures
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

oldDepth = xmlGetValues("//general/body/environment","container_disp_max_depth");
xmlSetValues("//general/body/environment", ["container_disp_max_depth"; "3"]);

// ----------------------------------------------------------------
clear s
s.f = [%i/%s, %s/(1-%s), 1/(1-%s)^2];
s.c = {1 3 ; "abc" %z};
s.ec = {};
s.b = rand(1,5)<0.5;
s.iL = $-4:2:$;
s.fun = linspace;
s.L = list(rand(2,3),{},(1-%z)^3,,%pi, %t, "abcd");
//s.lib = corelib;
s.tlist = tlist(["myTlist" "bool" "int" "num" "txt" "list"],..
          rand(3,4,2)<0.5, int8((rand(3,5)-0.5)*200), ..
          grand(3,4,"uin",-100,100), ["This" "is" "a" "test"], list(%pi,"number"));
s.mlist = mlist(["myMlist" "spb" "fun" "pol" "txt"],..
           sparse(rand(3,4)<0.5), cosd, (1-%z).^(0:4),["Scilab" "6.1"]);
s.estruct = struct();
s.withVoid = struct("num",%pi,"v",,"t","text");
s.struct = struct("a",rand(1,3),"L",list(%e,,"ABC"),"c",{{"GHI",1234}});
s

// ----------------------------------------------------------------
clear s
s.root = "Hello 0";
s.L = list(2,"bonjour", list(%i, list(%z,3)));
s.t.a = "Level 1";
s.t.r = %pi;
s.t.tl = tlist(["myTlist" "a" "b" "c"], rand(2,3), %t, list("abc", 123));
s.t.b.a = "Level 2";
s.t.b.r = %e;
s.t.L2 = list("Hello", list(1-%z+%z^2, list(-4,%t)));
s.t.b.s.a = "Level 3";
s.t.b.s.p = (1-%z)^3;

xmlSetValues("//general/body/environment", ["container_disp_max_depth";"0"]);
s
xmlSetValues("//general/body/environment", ["container_disp_max_depth";"1"]);
s
xmlSetValues("//general/body/environment", ["container_disp_max_depth";"2"]);
s
xmlSetValues("//general/body/environment", ["container_disp_max_depth";"3"]);
s
// ----------------------------------------------------------------

xmlSetValues("//general/body/environment", ["container_disp_max_depth"; oldDepth]);
