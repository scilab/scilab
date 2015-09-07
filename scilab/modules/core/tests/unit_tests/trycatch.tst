// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- NOT FIXED -->
//interactive mode
clear a b
try
    a=1;
catch
    b=2+1;
end
if exists("a")==0|exists("b")==1 then pause,end
if a<>1 then pause,end

clear a b
try  a=1+1;
catch
    b=2;
end
if exists("a")==0|exists("b")==1 then pause,end
if a<>2 then pause,end

clear a b
try  a=1;
catch  b=2;
end
if exists("a")==0|exists("b")==1 then pause,end
if a<>1 then pause,end

clear a b
try  a=1;catch  b=2;end
if exists("a")==0|exists("b")==1 then pause,end
if a<>1 then pause,end

clear a b
try,  a=1;catch,  b=2;end
if exists("a")==0|exists("b")==1 then pause,end
if a<>1 then pause,end


clear a b xxxx
try
    a=xxxx;
catch
    b=2;
end
if exists("a")==1|exists("b")==0 then pause,end
if b<>2 then pause,end

clear a b xxxx
try   a=xxxx;
catch
    b=2;
end
if exists("a")==1|exists("b")==0 then pause,end
if b<>2 then pause,end

clear a b xxxx
try
    a=xxxx;
catch b=2;
end
if exists("a")==1|exists("b")==0 then pause,end
if b<>2 then pause,end

clear a b xxxx
try   a=xxxx;
catch b=2;
end
if exists("a")==1|exists("b")==0 then pause,end
if b<>2 then pause,end


clear a b xxxx
try   a=xxxx;catch b=2;end
if exists("a")==1|exists("b")==0 then pause,end
if b<>2 then pause,end

clear a b xxxx
if execstr("try a=xxxx catch b=2;end","errcatch")<>0 then pause,end
if exists("a")==1|exists("b")==0 then pause,end


clear a b xxxx
if execstr("try a=1 catch b=2;end","errcatch")<>0 then pause,end
if exists("a")==0|exists("b")==1 then pause,end


clear a b xxxx

try,  a=xxxx;catch, b=2;end
if exists("a")==1|exists("b")==0 then pause,end
if b<>2 then pause,end

clear a  xxxx
try,  a=xxxx;catch;end
if exists("a")==1 then pause,end

clear a  xxxx
try,  a=xxxx;end
if exists("a")==1 then pause,end

clear a b xxxx
if execstr("try;catch, b=2;end", "errcatch") == 0 then pause, end
if exists("b")==1 then pause,end

if execstr("try,end", "errcatch") == 0 then pause, end

clear a b xxxx
u=1;try,  a=xxxx;catch, b=2;end
if exists("a")==1|exists("b")==0 then pause,end
if b<>2 then pause,end

clear a b xxxx
try, if %t then  a=xxxx,end;catch, b=2;end
if exists("a")==1|exists("b")==0 then pause,end
if b<>2 then pause,end

//nested try catch
clear a b xxxx
try
    a=xxxx;
catch
    try
        b=xxx,
    catch
        b=2;
    end;
    b=b+1;
end
if exists("a")==1|exists("b")==0 then pause,end
if b<>3 then pause,end

clear a b xxxx
try   a=xxxx;catch try b=xxx,catch b=2;end;end
if exists("a")==1|exists("b")==0 then pause,end
if b<>2 then pause,end


clear a b xxxx
try a=2;try a=xxxx; catch a=a+1,end;catch;b=2;end
if exists("a")==0|exists("b")==1 then pause,end
if a<>3 then pause,end


clear a b xxxx
try a=2;try a=xxxx; catch a=a+1,end;b=2;end
if exists("a")==0|exists("b")==0 then pause,end
if a<>3|b<>2 then pause,end

//catch in  functions
funcprot(0);
clear a b
deff("r=test()",[
"try"
"  a=1;"
"catch "
"  b=2+1;"
"end"
"r=exists(''a'')==0|exists(''b'')==1 "
"r=r|a<>1"],"n")
if test() then pause,end

deff("r=test()",[
"try  a=1;"
"catch "
"  b=2;"
"end"
"r=exists(''a'')==0|exists(''b'')==1"
"r=r|a<>1"],"n")
if test() then pause,end

deff("r=test()",[
"try  a=1;"
"catch  b=2;"
"end"
"r=exists(''a'')==0|exists(''b'')==1"
"r=r|a<>1"],"n")
if test() then pause,end

deff("r=test()",[
"try  a=1;catch  b=2;end"
"r=exists(''a'')==0|exists(''b'')==1"
"r=r|a<>1"],"n")
if test() then pause,end

deff("r=test()",[
"try,  a=1;catch,  b=2;end"
"r=exists(''a'')==0|exists(''b'')==1"
"r=r|a<>1"],"n")
if test() then pause,end

clear a b xxxx
deff("r=test()",[
"try"
"  a=xxxx;"
"catch "
"  b=2;"
"end"
"r=exists(''a'')==1|exists(''b'')==0"
"r=r|b<>2"],"n")
if test() then pause,end

deff("r=test()",[
"try a=xxxx;"
"catch "
"  b=2;"
"end"
"r=exists(''a'')==1|exists(''b'')==0"
"r=r|b<>2"],"n")
if test() then pause,end

deff("r=test()",[
"try"
"  a=xxxx;"
"catch b=2;"
"end"
"r=exists(''a'')==1|exists(''b'')==0"
"r=r|b<>2"],"n")
if test() then pause,end

deff("r=test()",[
"try a=xxxx;"
"catch b=2;"
"end"
"r=exists(''a'')==1|exists(''b'')==0"
"r=r|b<>2"],"n")
if test() then pause,end

deff("r=test()",[
"try a=xxxx;catch b=2;end"
"r=exists(''a'')==1|exists(''b'')==0"
"r=r|b<>2"],"n")
if test() then pause,end

deff("r=test()",[
"if execstr(''try a=xxxx catch b=2;end'',''errcatch'')<>0 then pause,end"
"r=exists(''a'')==1|exists(''b'')==0 "],"n")
if test() then pause,end

deff("r=test()",[
"try a=xxxx catch b=2;end"
"r=exists(''a'')==1|exists(''b'')==0"
"r=r|b<>2"],"n")
if execstr("test()","errcatch")<>0 then pause,end

deff("r=test()",[
"if execstr(''try a=1 catch b=2;end'',''errcatch'')<>0 then pause,end"
"r=exists(''a'')==0|exists(''b'')==1 "],"n")
if test() then pause,end

deff("r=test()",[
"try a=1, catch b=2;end"
"r=exists(''a'')==0|exists(''b'')==1"
"r=r|a<>1"],"n")
if execstr("r=test()","errcatch")<>0 then pause,end
if r then pause,end

deff("r=test()",[
"try b=xxxx, catch a=1;end"
"r=exists(''a'')==0|exists(''b'')==1"
"r=r|a<>1"],"n")
if execstr("r=test()","errcatch")<>0 then pause,end
if r then pause,end

deff("r=test()",[
"try a=1 catch b=2;end"
"r=exists(''a'')==0|exists(''b'')==1"
"r=r|a<>1"],"n")
if execstr("test()","errcatch")<>0 then pause,end



deff("r=test()",[
"try,  a=xxxx;catch, b=2;end"
"r=exists(''a'')==1|exists(''b'')==0"
"r=r|b<>2"],"n")
if test() then pause,end

deff("r=test()",[
"try,  a=xxxx;catch;end"
"r=exists(''a'')==1"],"n")
if test() then pause,end

deff("r=test()",[
"try,  a=1;end"
"r=exists(''a'')==0"
"r=r|a<>1"],"n")
if test() then pause,end


deff("r=test()",[
"try,  a=xxxx;end"
"r=exists(''a'')==1"],"n")
if test() then pause,end


if execstr("deff(""r=test()"",[""try;catch, b=2;end"" ""r=exists(""""b"""")==1""])" ,"errcatch") == 0 then pause, end
if execstr("deff(""r=test()"",[""try;end"" ""r=%f""])", "errcatch") == 0 then pause, end


deff("r=test()",[
"try, if %t then  a=xxxx,end;catch, b=2;end"
"r=exists(''a'')==1|exists(''b'')==0"
"r=r|b<>2"],"n")
if test() then pause,end

//nested try catch
deff("r=test()",[
"try   "
"  a=xxxx;"
"catch "
"  try "
"    b=xxx,"
"  catch "
"    b=2;"
"  end;"
"  b=b+1;"
"end"
"r=exists(''a'')==1|exists(''b'')==0"
"r=r|b<>3"],"n")
if test() then pause,end

deff("r=test()",[
"try   a=xxxx;catch try b=xxx,catch b=2;end;end"
"r=exists(''a'')==1|exists(''b'')==0"
"r=r|b<>2"],"n")
if test() then pause,end

deff("test()",["try a=2; try a=xxxx; catch a=a+1,end; catch;b=2;end"])
macr2tree(tree2code(test))

deff("r=test()",[
"try a=2;try a=xxxx+33; catch a=a+1,end;catch;b=2;end"
"r=exists(''a'')==0|exists(''b'')==1"
"r=r|a<>3"],"n")
if test() then pause,end


deff("r=test()",[
"try a=2;try a=xxxx; catch a=a+1,end;b=2;end"
"r=exists(''a'')==0|exists(''b'')==0"
"r=r|a<>3|b<>2"],"n")
if test() then pause,end

deff("r=test()",[
"try a=2;if %t then try a=xxxx; catch a=a+1,end;end;b=2;end"
"r=exists(''a'')==0|exists(''b'')==0"
"r=r|a<>3|b<>2"],"n")
if test() then pause,end

deff("r=test()",[
"try a=2;if %t then try a=xxxx; catch a=a+1,end;b=2;end;end"
"r=exists(''a'')==0|exists(''b'')==0"
"r=r|a<>3|b<>2"],"n")
if test() then pause,end

deff("r=test()",[
"try a=2;try a=xxxx; catch if %t then a=a+1,end,end;b=2;end"
"r=exists(''a'')==0|exists(''b'')==0"
"r=r|a<>3|b<>2"],"n")
if test() then pause,end

deff("r=test()",[
"if %t then try a=2;try a=xxxx; catch a=a+1,end;b=2;end;end"
"r=exists(''a'')==0|exists(''b'')==0"
"r=r|a<>3|b<>2"],"n")
if test() then pause,end

deff("r=test()",[
"try a=2;try a=xxxx; catch for k=1:2,a=a+1,end,end;b=2;end"
"r=exists(''a'')==0|exists(''b'')==0"
"r=r|a<>4|b<>2"],"n")
if test() then pause,end

deff("r=test()",[
"for k=1:3,try a=2;if %t then try a=xxxx; catch a=a+1,end;end;b=2;end,end"
"r=exists(''a'')==0|exists(''b'')==0"
"r=r|a<>3|b<>2"],"n")
if test() then pause,end

deff("r=test()",[
"for k=1:3,try a=2;if %t then try a=xxxx; catch for k=1:2,a=a+1,end,end;end;b=2;end,end"
"r=exists(''a'')==0|exists(''b'')==0"
"r=r|a<>4|b<>2"],"n")
if test() then pause,end



deff("r=test()",[
"b=0;while b==0,try a=2;if %t then try a=xxxx; catch a=a+1,end;end;b=2;end,end"
"r=exists(''a'')==0|exists(''b'')==0"
"r=r|a<>3|b<>2"],"n")
if test() then pause,end


deff("b=test1()",[
"  try "
"    b=xxx,"
"  catch "
"    b=2;"
"  end;"],"n")

deff("r=test()",[
"try"
"  a=xxxx;"
"catch"
"  b=test1()"
"  b=b+1"
"end"
"r=exists(''b'')==0"
"r=r|b<>3"],"n")
if test() then pause,end

