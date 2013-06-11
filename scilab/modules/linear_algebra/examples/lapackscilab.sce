//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.
//

//
// A example to call some lapack routines
// dgemm, zgemm, dsyev

// WARNING:these gateways use old scilab api and it does not work with Scilab 6*/
// see help api_scilab for more information.


ilib_verbose(0);
setenv("__USE_DEPRECATED_STACK_FUNCTIONS__","YES");

curdir = pwd();
file_path = get_file_path("lapackscilab.sce");
chdir(file_path);

files_functions = ["intdgemm.c";
"intzgemm.c";
"intdsyev.c"];

table_functions = ["dgemm", "intdgemm";
"zgemm", "intzgemm";
"dsyev", "intdsyev"];

ilib_build("lapackexample", table_functions, files_functions, []);
exec loader.sce;

Alfa = 2;
Beta = 3;
m = 3;
n = 4;
C = ones(m, n);
k = 2;
A = ones(m, k);
B = ones(k, n);
C1 = dgemm(Alfa, A, B, Beta, C);
if norm(C1 - (Alfa * A * B + Beta * C)) > %eps then pause,end

A = [1/2^10, 1/2^10; 2^10, 2^10];
A = rand(3,3);
if norm(max(real(dsyev(A))) - max(real(spec(A))) ) > %eps * 1e15 then pause,end

alfa = 1 + %i;
betha = alfa;
A = rand(2,2) + %i * rand(2,2);
B = A;
C = A;
C = zgemm(alfa, A, B, betha, C)

cd(curdir);