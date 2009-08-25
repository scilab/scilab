// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Scilab Consortium Operational Team
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

ilib_verbose(0);
mkdir(pathconvert(TMPDIR+"/commom_read_api"));
cd(pathconvert(TMPDIR+"/commom_read_api"));
cflags = "-I"+SCI+"/modules/localization/includes";
ilib_build("commom_read",["commom_read","commom_read"],SCI+"/modules/core/tests/unit_tests/commom_read_api.c",[],[],"",cflags);
exec("loader.sce");

            
function read_all()
d = [1,2,3;4,5,6;7,8,9];commom_read(d);
s=poly(0,"x");p=1+s+2*s^2;p = [(p * 2),(p * s + 3);(p * 2 * s ** 2 - 6),(12 - 4 * p * (- s) ** 2)];commom_read(p);
b = [%t,%f;%t,%f;%f,%t];commom_read(b);
sp=sparse([2,-1,0,0,0;-1,2,-1,0,0;0,-1,2,-1,0;0,0,-1,2,-1;0,0,0,-1,2]);commom_read(sp);
bsp=sparse([1,2;4,5;3,10],[%t,%t,%t]);commom_read(bsp);
i8 = int8([1,2,3]);commom_read(i8);
ui32 = uint32([3;2;1]);commom_read(ui32);
str = ["may", "the", "puffin"; "be", "with","you"];commom_read(str);
if with_module('umfpack') then
    Cp = taucs_chfact(sp);
    l = list(list(d, p, list(b, sp)), list(i8, bsp), list(ui32, str), Cp);
else
    l = list(list(d, p, list(b, sp)), list(i8, bsp), list(ui32, str));
end
commom_read(l)
endfunction
read_all;
            
        
