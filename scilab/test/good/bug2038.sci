// Non-regression test file for bug 2038
// Author : Scilab Project - Allan CORNET
// Copyright INRIA
// Date : 3 july 2006

mode(-1);
clear;

mkdir("bug2038_dir");
cd("bug2038_dir");
mkdir("bug2038_subdir");
cd("..")

a=dir("bug2038_dir");
b=mtlb_dir("bug2038_dir");

x=getdate(a.date);
cx=x(1:4);

y=getdate();
cy=y(1:4);

T1=(a.isdir & b.isdir);
T2=(cx == cy);

if (T1 & T2) then
    affich_result(%T,2038);
else
    affich_result(%F,2038);
end

clear
