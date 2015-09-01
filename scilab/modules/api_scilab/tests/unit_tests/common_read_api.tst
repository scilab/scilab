// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2010 - DIGITEO 
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/common_read_api"));
cd(pathconvert(TMPDIR+"/common_read_api"));
copyfile(SCI+"/modules/api_scilab/tests/unit_tests/common_read_api.c",pathconvert(TMPDIR+"/common_read_api/common_read_api.c",%F));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("gw_common_read",["common_read","common_read"],"common_read_api.c",[],"","",cflags);
exec("loader.sce");

             
function read_all()
d = [1,2,3;4,5,6;7,8,9];common_read(d);
s=poly(0,"x");p=1+s+2*s^2;p = [(p * 2),(p * s + 3);(p * 2 * s ** 2 - 6),(12 - 4 * p * (- s) ** 2)];common_read(p);
b = [%t,%f;%t,%f;%f,%t];common_read(b);
sp=sparse([2,-1,0,0,0;-1,2,-1,0,0;0,-1,2,-1,0;0,0,-1,2,-1;0,0,0,-1,2]);common_read(sp);
bsp=sparse([1,2;4,5;3,10],[%t,%t,%t]);common_read(bsp);
i8 = int8([1,2,3]);common_read(i8);
ui32 = uint32([3;2;1]);common_read(ui32);
str = ["may", "the", "puffin"; "be", "with","you"];common_read(str);
l = list(list(d, p, list(b, sp)), list(i8, bsp), list(ui32, str));
common_read(l)
endfunction
read_all;
