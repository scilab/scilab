// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [S,Series]=sysfact(Sys,Gain,flag)
    //For Sys=[A,B,C,D],
    //if flag=='post' returns S = [A+B*Gain, B , Gain, I]
    //                and  Series = minss(Sys*S)
    //if flag=='pre' returns  S=[A+Gain*C, Gain , C, I]
    //                and  Series = minss(S*Sys)
    if flag=="post" then
        S=syslin(Sys("dt"),Sys("A")+Sys("B")*Gain, Sys("B"), Gain, eye(Gain*Sys("B")));
        //Series=minss(Sys*S);
        Series= syslin("c",Sys("A")+Sys("B")*Gain, Sys("B"), Sys("C")+Sys("D")*Gain, Sys("D"));
    end
    if flag=="pre" then
        S=syslin(Sys("dt"),Sys("A")+Gain*Sys("C"), Gain, Sys("C"), eye(Sys("C")*Gain));
        //Series=minss(S*Sys);
        Series=syslin("c",Sys("A")+Gain*Sys("C"), Sys("B")+Gain*Sys("D"), Sys("C"), Sys("D"));
    end


    //Example:
    //Sys=ssrand(3,2,4);Sys('D')=rand(3,2);
    //S=sysfact(Sys,lqr(Sys),'post');
    //ww=minss(Sys*S);
    //ss2tf(gtild(ww)*ww),Sys('D')'*Sys('D')

    //Syst=Sys';
    //S1=sysfact(Syst,lqe(Syst),'pre');
    //ww=minss(S1*Syst);
    //ss2tf(ww*gtild(ww)),Syst('D')*Syst('D')'


    //Sys=ssrand(2,3,4);
    //[X,d,F,U,k,Z]=abinv(Sys);
    //ss2tf(Sys*Z)
    //ss2tf(Sys*sysfact(Sys,F,'post')*U)
endfunction
