// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


// <-- CLI SHELL MODE -->

ilib_verbose(0);
funcprot(0);
function make_compiled_function(name, ext, code)
    filename=name+ext;
    mputl(code, filename);
    ilib_for_link(name,filename,[],"c");
    exec loader.sce;
    mdelete(filename);
endfunction

g_fun_name="test_fun";
c_prog=["#include  <math.h>"
"void "+g_fun_name+"(void const* const* args, void *const* res) {"
"*((double*)*res)= 2.*(((double*)args[0])[0]);"
"}"];

make_compiled_function(g_fun_name, ".c",c_prog);

args=[1,2];
res=parallel_run(args,g_fun_name,1);
if res<>[2, 4] then pause,end

function a= g(arg1); a=2*arg1; endfunction;

res=parallel_run(args, g);
if res<>[2, 4] then pause,end

fun_name="test_fun_2_1";
c_prog=["#include  <math.h>"
"void "+fun_name+"(double const* const* args, double ** res) {"
"res[0][0]= .5*args[0][0]; res[0][1]= .25*args[0][1];res[1][0]= 2*args[1][0];"
"}"];
make_compiled_function(fun_name, ".c",c_prog);

arg1=[1,2;4,5]; arg2=[2,3];
[res1, res2] =parallel_run(arg1, arg2,fun_name,"constant", [2;1]);
if res1<>[.5,1;1,1.25]  then pause,end
if res2 <>[4,6]  then pause,end

function [a,b]= f(arg1, arg2); a=arg1'.*[.5,.25]; b=2*arg2; endfunction;
[res1, res2] =parallel_run(arg1, arg2,"f","constant", [2;1]);
if res1<>[.5,1;1,1.25]  then pause,end
if res2 <>[4,6]  then pause,end

arg1=[1,2;4,5]; arg2=[2];
[res1, res2] =parallel_run(arg1, arg2,fun_name,"constant", [2;1]);
if res1<>[.5,1;1,1.25]  then pause,end
if res2 <>[4,4]  then pause,end

function [a,b]= f(arg1, arg2); a=arg1'.*[.5,.25]; b=2*arg2; endfunction;
[res1, res2] =parallel_run(arg1, arg2,"f","constant", [2;1]);
if res1<>[.5,1;1,1.25]  then pause,end
if res2 <>[4,4]  then pause,end


function [r_min, r_med, r_max]= min_med_max(a, b, c)
    r_min=min(a,b,c); r_med=median([a,b,c]); r_max=max(a,b,c);
endfunction

N=10;

A=rand(1:N);B=rand(1:N);C=rand(1:N);
Min=zeros(N); Med=Min; Max=Min;
for i =1:N
    [Min(i), Med(i), Max(i)]= min_med_max(A(i), B(i), C(i));
end;

[Min_mc,Med_mc,Max_mc]=parallel_run(A,B,C,"min_med_max");

if max(max(Min-Min_mc', Med-Med_mc',Max-Max_mc')) > %eps  then pause,end


fun_name="min_med_max_cxx";
c_prog=["#include <algorithm>"
"extern ""C""{"
"void "+fun_name+"(void const*const* args, void*const* res) {"
"    double a(((double const*const*)args)[0][0]),b(((double const*const*)args)[1][0]),c(((double const*const*)args)[2][0]);"
"    if(b<a){ std::swap(a,b);}"
"    if(c<a){ std::swap(a,c);}"
"    if(c<b){ std::swap(b,c);}"
"    ((double*const*)res)[0][0]=a; ((double*const*)res)[1][0]=b; ((double*const*)res)[2][0]= c;"
"    return;"
"  }"
"}"];
make_compiled_function(fun_name, ".cxx",c_prog);
[Min_mc,Med_mc,Max_mc]=parallel_run(A,B,C, fun_name);

if max(max(Min-Min_mc', Med-Med_mc',Max-Max_mc')) > %eps  then pause,end

function r= quantiles(data, ranks)
    tmp=gsort(data,"g","i");
    r=tmp(ranks);
endfunction

data=matrix(rand(1:N*N),N,N);

R4_6_7=matrix(zeros(1:3*N),N,3);
for i =1:N
    R4_6_7(i,:)= quantiles(data(i,:),[4,6,7]);
end;

R4_6_7_mc=parallel_run(data',[4,6,7]',"quantiles", 3)';

if max(R4_6_7-R4_6_7_mc)  > %eps  then pause,end

fun_name="quantiles_cxx";

c_prog=["#include<algorithm>"
"extern ""C""{"
" void "+fun_name+ "(void const*const* args, void*const* res){"
"   double * data(((double *const*)args)[0]);"
"   int data_size(static_cast<int>(((double const*const*)args)[1][0]));"
"   double const* ranks(((double *const*)args)[2]);"
"   int ranks_size(static_cast<int>(((double *const*)args)[3][0]));"
"   std::sort(data,data+data_size);"
"   for(int i(0); i != ranks_size; ++i)"
"     { ((double*const*)res)[0][i]= data[static_cast<int>(ranks[i])-1]; }"
"   return;"
" }"
"}"];
make_compiled_function(fun_name, ".cxx",c_prog);

R4_6_7_mc=parallel_run(data',N,[4,6,7]',3,fun_name, 3)';

if max(R4_6_7-R4_6_7_mc)  > %eps  then pause,end


N=5; // size of a block
L=10; // number of blocks


data=spzeros(N*L,N*L);
for k=1:L
    kk=(k-1)*N+1:k*N;
    data(kk,kk)=matrix(rand(1:N*N),N,N);
end

function r= quantiles_sparse(indices_i, indices_j, ranks)
    tmp=gsort(full(data(indices_i, indices_j)),"g","i");
    r=tmp(ranks);
endfunction

R3_6_9=matrix(zeros(1:3*L),L,3);
for k=1:L
    kk=(k-1)*N+1:k*N;
    R3_6_9(k,:)=quantiles_sparse(kk,kk, [3,6,9])';
end;

idx=matrix(1:(L*N), N,L);
R3_6_9_mc=parallel_run(idx,idx,[3,6,9]',"quantiles_sparse",3)';

if max(R3_6_9-R3_6_9_mc) > %eps  then pause,end

function r=block_invert(indices_i, indices_j)
    r=inv(full(data(indices_i, indices_j)));
endfunction

sp_inv=spones(data);

for k=1:L
    kk=(k-1)*N+1:k*N;
    sp_inv(kk,kk)=block_invert(kk,kk);
end;


sp_inv_1=sp_inv;

function r= invert_one_block(block_size,k)
    kk=(k-1)*block_size+1:k*block_size;
    r=full(inv(data(kk,kk)));// A from global scope
endfunction;

function blocks_to_sparse(blocks)
    s=size(blocks);
    block_size=sqrt(s(1));
    nb_blocks=s(2);
    for k=1:nb_blocks
        kk=(k-1)*block_size+1:k*block_size;
        sp_inv(kk,kk)= matrix(data(1:s(1),k),block_size,block_size);
    end
endfunction

blocks_to_sparse(parallel_run(N,1:L,"invert_one_block",[N,N]));


if max(full(sp_inv-sp_inv_1)) > %eps then pause,end


// testing configuration
// nb_workers only effective on Linux for now :(
if(getos() <> "Windows")

    args=[1,2,3,4,5];
    function a= g(arg1); a=2*arg1; endfunction;
    conf= init_param("nb_workers",2);
    parallel_run(args, "g", conf);
    parallel_run(args, g_fun_name, conf);

    conf=add_param(conf,"shared_memory",1);
    parallel_run(args, g_fun_name, conf);

    function init_rand(n)
        rand("seed",n);
    endfunction;

    function res= rand_macro(nb)
        res= rand(1, nb);
    endfunction
    nb= 5;
    conf= init_param("nb_workers", 2);
    // We use conf to force nb_workers = 2 even on monocore
    res= parallel_run([nb,nb],"rand_macro",nb,conf);
    // without seeding the 2 workers have the same rng.
    if max(abs(res(:,1)-res(:,2))) > %eps then pause,end

    conf=add_param(conf, "prologue","init_rand");
    res= parallel_run([nb,nb],"rand_macro",nb,conf);
    // when setting the seed, they should have different random numbers
    if max(abs(res(:,1)-res(:,2))) < %eps then pause,end

    many_args= 1:10e3;
    many_workers= 80;
    for sched=0:1
        res= parallel_run(many_args, "g", init_param("dynamic_scheduling", sched,"nb_workers", many_workers ));
        if res<>g(many_args) then pause,end;
    end;

    // test scheduling: sleeping is embarrassingly parallel even on uniprocessors ! :)
    function r=do_sleep(t);sleep(t*1000);r=1;endfunction
    many_workers= 80;
    sleep_time=2;
    args=ones(1,many_workers)*sleep_time;
    for sched=0:1
        tic();
        res= parallel_run(args, "do_sleep", init_param("dynamic_scheduling", sched, "nb_workers", many_workers ));
        if toc()>sleep_time+2 then pause,end;
    end;
    // test dynamic versus static scheduling : dynamic scheduling should be better on tasks of varying length.
    args=(1:20).^2/100;
    for sched=0:1
        tic();
        res= parallel_run(args, "do_sleep", init_param("dynamic_scheduling", sched, "nb_workers", 5 ));
        elapsed(sched+1)= toc();
    end;
    if elapsed(2)>elapsed(1) then pause,end;

end