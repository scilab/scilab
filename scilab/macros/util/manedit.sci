function []=manedit(manitem,editor)
// manitem : character string giving a manitem 
//
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<=1, editor ="emacs -geometry 81x50+427+143  -fn 9x15  ";end
fname='fname=`ls $SCI/man/*/'+manitem+'.man 2>/dev/null `;';
unixstr=fname+"if [ $fname ] ; then "+editor+" $fname;else echo No man ; fi";
unix_s(unixstr)
endfunction
