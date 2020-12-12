// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Short Description -->
// Display of lists unitary tests

%l_p(list())
%l_p(list(1,2,3))
%l_p(list(list(1,list()),2))
[p, q] = (poly(rand(5,1),"z"), poly(rand(6,1),"z"));
%l_p(list(rlist(q,p), 1.23))
%l_p(list(1,1:$))
%l_p(list(list(1:$)))
%l_p(list(list(1,$),list()))

//
films = struct("Title", "A.I. Artificial Intelligence",..
               "Year", 2001, ..
               "Director", "Steven Spielberg", ..
               "Duration", 140);
L = list([%t %f %f]',                ..
         $:-1:3,                    ..
         int8([1 2 3 ; 4 5 6]),     ..
         [-%inf, -1, 0, %i, 7.1, 3.3e20, %inf, %nan], ..
         sparse([0 1 2; 3 0 4]),    ..
         ["This" "is" "some" "text"], ..
         ,              ..
         sin,           ..
         logspace,      ..
         iolib,         ..
         {%pi, "abcd" ;
          list(,)(1),(%i-%s)^3},    ..
          {},           ..
          list(["Hello" "Allo" "Hé"], uint64(12345678)^(1:3), ,grand(2,4,"uin",0,10)), ..
          list(), ..
         films);
%l_p(L)


// Tlist overloaded display
TL = tlist("x",1,2)
deff("%x_p(x)","disp(''Mydisplay'',x(2),x(3))")
TL
list(TL)
list(list(TL))
TL = tlist("x",1:$,2)
list(TL)
list(list(TL))
TL(2) = TL
