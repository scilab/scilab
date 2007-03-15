// Non-regression test file for bug 2333
// Vincent COUVERT - Scilab Project
// Copyright INRIA
// 15/03/2007

mode(-1);
clear;

load("SCI/modules/m2sci/macros/kernel/lib");
r=exists("mfile_path");

affich_result(r,2322);
clear
