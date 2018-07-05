// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 14606 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14606
//
// <-- Short Description -->
// "who" doesn"t return memory size

bug_14606_double = ones(10,10);
bug_14606_uint8 = uint8(ones(10,10));
bug_14606_uint16 = uint16(ones(10,10));
bug_14606_uint32 = uint32(ones(10,10));
bug_14606_uint64 = uint64(ones(10,10));
bug_14606_int8 = int8(ones(10,10));
bug_14606_int16 = int16(ones(10,10));
bug_14606_int32 = int32(ones(10,10));
bug_14606_int64 = int64(ones(10,10));
bug_14606_bool = eye(10,10)==1;
bug_14606_poly = [%s 1+%s;1 1];
bug_14606_rat = 1./[%s 1+%s;1 1];
bug_14606_list = list(bug_14606_double, bug_14606_uint64, bug_14606_poly);
deff("out=%tlist_14606_size(x)","out=%nan");
deff("out=%mlist_14606_size(x)","out=%nan");
bug_14606_tlist = tlist(["tlist_14606","field1","field2"], 1:3, ["Scilab" "is" "not" "Basile"]);
bug_14606_mlist = mlist(["mlist_14606","field1","field2"], 1:3, ["Scilab" "is" "not" "Basile"]);
bug_14606_syslin=syslin('c',[0,1;0,0],[1;1],[1,1]); 
bug_14606_sparse= sparse([1 1;2 2;3 3],1:3,[3 3]);
bug_14606_sparseboolean= sparse([1 1;2 2;3 3],1:3,[3 3]) > 1;
bug_14606_struct=struct("list1",list("testlist",1,2), "matrix1", [1 2 3; 4 5 6], "boolean1", %t);
bug_14606_struct=struct("list1",list("testlist",1,2), "matrix1", [1 2 3; 4 5 6], "boolean1", %t);
bug_14606_cell={bug_14606_struct, bug_14606_sparseboolean, bug_14606_poly};
bug_14606_handle=[sdf(),sda()];
bug_14606_pointer=lufact(bug_14606_sparse);
deff("out=bug_14606_macro(x,y)","out=x+y");
libdir=TMPDIR+"/bug_14606";
mkdir(libdir);
mputl(["function out=bug_14606_macro_file(x,y)","out=x+y","endfunction"],libdir+"/bug_14606_macro_file.sci");
genlib("bug_14606_lib",libdir,%f,%t);
clear bug_14606_lib
bug_14606_lib=lib(libdir); // output variable is compulsory otherwise Scilab crashes after next line
bug_14606_macro_file(1,1);
whos -name bug_14606
ludel(bug_14606_pointer)
bug_14606_var=zeros(10,10);
global bug_14606_var
whos -name bug_14606_var
clear bug_14606_var
whos -name bug_14606_var

