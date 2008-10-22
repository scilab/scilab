//--------------------------------------------------------------------------------------------------
// Tests Interface TCL
// Allan CORNET
//--------------------------------------------------------------------------------------------------
test=TCL_SetVar("ScalarA",1.1112)
if (test==%F) then pause,end

test=TCL_GetVar("ScalarA")
if (test<>"1.1112000000") then pause,end
//--------------------------------------------------------------------------------------------------
test=TCL_SetVar("StringB","Test")
if (test==%F) then pause,end

test=TCL_GetVar("StringB")
if (test<>"Test") then pause,end
//--------------------------------------------------------------------------------------------------
b=[6.1,4.1,1.1;2.1,3.1,5.1];
stringb=["6.1000000000","4.1000000000","1.1000000000";"2.1000000000","3.1000000000","5.1000000000"];

test=TCL_SetVar("B",b)
if (test==%F) then pause,end

test=TCL_GetVar('B(1,1)')
if (test<>stringb(1,1)) then pause,end

test=TCL_GetVar('B(1,2)')
if (test<>stringb(1,2)) then pause,end

test=TCL_GetVar('B(1,3)')
if (test<>stringb(1,3)) then pause,end

test=TCL_GetVar('B(2,1)')
if (test<>stringb(2,1)) then pause,end

test=TCL_GetVar('B(2,2)')
if (test<>stringb(2,2)) then pause,end

test=TCL_GetVar('B(2,3)')
if (test<>stringb(2,3)) then pause,end

test=TCL_GetVar("B")
if (test<>stringb) then pause,end

//--------------------------------------------------------------------------------------------------
a=['A','B','C';'D','E','F']

test=TCL_SetVar("A",a)
if (test==%F) then pause,end

test=TCL_GetVar('A(1,1)')
if (test<>a(1,1)) then pause,end

test=TCL_GetVar('A(1,2)')
if (test<>a(1,2)) then pause,end

test=TCL_GetVar('A(1,3)')
if (test<>a(1,3)) then pause,end

test=TCL_GetVar('A(2,1)')
if (test<>a(2,1)) then pause,end

test=TCL_GetVar('A(2,2)')
if (test<>a(2,2)) then pause,end

test=TCL_GetVar('A(2,3)')
if (test<>a(2,3)) then pause,end

test=TCL_GetVar("A")
if (test<>a) then pause,end
//--------------------------------------------------------------------------------------------------

