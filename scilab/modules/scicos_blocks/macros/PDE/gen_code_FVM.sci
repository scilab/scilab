//  Scicos
//
//  Copyright (C) INRIA - Author : EADS-CCR
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function [equations,flag_type,impl_type]=gen_code_FVM(a1,b1,a2,b2,a3,b3,a4,b4,..
    a5,b5,a6,b6,a7,b7,N,oper,vbc,xn,xw)
    // Cette fonction est pour la g�n�ration des �quations ODE ou DAE du bloc    //
    // sorties :                                                                 //
    //    - equations (String) : vecteur qui contient le code C des �quations    //
    //      d'etat (DAE)                                                         //
    //    - impl_type (Entier) : indique si le type des �tats                    //
    //      (diff�rentiels 1 ou alg�brique -1)                                   //
    //    - flag_type (Entier) : 1 : explicie, 2 : implicite                     //
    // entr�es :                                                                 //
    //    - ai, bi (String) : les differents coeficients des op�rateurs          //
    //      (ai(x) et bi(t))                                                     //
    //    - a, b (Double) : limites du domaine [a b]                             //
    //    - N (Entier) : est le nombre de noeuds                                 //
    //    - oper (Entier) : vecteur des op�rateurs selectionnes de 1 � 7         //
    //    - vbc (String) : vecteur des conditions aux limites en a et b          //
    //    - xn (Double) : vecteur colonne representant les noeuds                //
    //    - xw (Double) : vecteur colonne representant les cellules              //
    //----------------------------------------------------------------------- ---//

    flag_type=1; // 1 : explicie, 2 : implicite
    impl_type=1; // 1 pour syst�me d'�tat, -1 pour le syst�me alg�brique
    sep=[",","*","/"];
    coef51=[];
    coef52=[];
    coef1=[];
    coef2=[];
    cla=[];
    clb=[];
    clua=[];
    club=[];

    for i=1:size(xn,1)
        x=xn(i);
        a1_ev(i)=evstr(a1);
        a2_ev(i)=evstr(a2);
        a3_ev(i)=evstr(a3);
        a4_ev(i)=evstr(a4);
        a5_ev(i)=evstr(a5);
        a6_ev(i)=evstr(a6);
        a7_ev(i)=evstr(a7);
    end
    if ((find(oper == 2) ==[]) & (find(oper == 4) ==[]) ) then
        // cas oper5 sans oper2 et oper4
        if (find(oper == 5) ~=[] ) then
            [coef51,coef52,coef21,coef22]=Disc2_5_FVM1d(h,kbc,5,N,xn,xw,a5);
            cla=mulfv(msprintfv(full(coef52(1)/h)),vbc(1));
            clb=mulfv(msprintfv(full(coef52(N)/h)),vbc(2));
        end
        if ( find(oper == 1) ~= []) then
            // cas d2u/dt2
            equations=emptystr(2*N,1);
            vec2=equations;
            for i=1:2*N
                vec2(i)="x["+string(i-1)+"]";
            end

            for i=1:N
                equations(i)="   xd["+string(i-1)+"]= "+vec2(i+N)+";";
                F=mulfv(msprintfv(a7_ev(i)),b7);
                if (i == 1) then
                    F=subfv(F,mulfv(cla,b5));
                elseif (i == N) then
                    F=subfv(F,mulfv(clb,b5));
                end
                B=mulfv(mulfv(msprintfv(a3_ev(i)),b3),vec2(i+N));
                C=addfv(mulfv(multMatVect(coef51(i,:)/h,vec2(1:N)),b5),mulf3v(msprintfv(a6_ev(i)),b6,vec2(i)));
                A=mulfv(msprintfv(a1_ev(i)),b1);
                equations(i+N)="   xd["+string(i+N-1)+"]=("+subfv(subfv(F,B),C)+")/"+A+";";
            end
        elseif ( find(oper == 3) ~= []) then
            // cas du/dt (oper 3)
            equations=emptystr(N,1);
            vec2=equations;
            for i=1:N
                vec2(i)="x["+string(i-1)+"]";
            end

            for i=1:N
                F=mulfv(msprintfv(a7_ev(i)),b7);
                if (i == 1) then
                    F=subfv(F,mulfv(cla,b5));
                elseif (i == N) then
                    F=subfv(F,mulfv(clb,b5));
                end
                B=mulfv(msprintfv(a3_ev(i)),b3);
                C=addfv(mulfv(multMatVect(coef51(i,:)/h,vec2(1:N)),b5),mulf3v(msprintfv(a6_ev(i)),b6,vec2(i)));
                equations(i)="   xd["+string(i+N-1)+"]=("+subfv(F,C)+")/"+B+";";
            end
        else
            // cas alg�brique ==> implicite
            flag_type=2;
            impl_type=-1;
            equations=emptystr(N,1);
            vec2=equations;
            for i=1:N
                vec2(i)="x["+string(i-1)+"]";
            end

            if ( find(oper == 5) ~= []) then
                cla=mulfv(msprintfv(full(coef52(1))),vbc(1));
                clb=mulfv(msprintfv(full(coef52(N))),vbc(2));
            end
            for i=1:N
                F=mulfv(msprintfv(h*a7_ev(i)),b7);
                if (i == 1) then
                    F=subfv(F,mulfv(cla,b5));
                elseif (i == N) then
                    F=subfv(F,mulfv(clb,b5));
                end
                C=addfv(mulfv(multMatVect(coef51(i,:),vec2(1:N)),b5),mulf3v(msprintfv(h*a6_ev(i)),b6,vec2(i)));
                equations(i)="   res["+string(i-1)+"]="+subfv(F,C)+";";
            end
        end

    else
        // cas oper2 ou oper4
        if (find(oper == 2) ~=[] ) then
            [coef1,coef2,coef21,coef22]=Disc2_5_FVM1d(h,kbc,2,N,xn,xw,a2);
            clua=mulfv(msprintfv(full(coef22(1))),vbc(1));
            club=mulfv(msprintfv(full(coef22(N))),vbc(2));
        end

        if ( find(oper == 1) ~= []) then
            // cas d2u/dt2
            flag_type=2;
            impl_type=0;
            equations=emptystr(3*N,1);
            vec2=equations; dvec2=vec2;
            for i=1:3*N
                vec2(i)="x["+string(i-1)+"]";
                dvec2(i)="xd["+string(i-1)+"]";
            end

            for i=1:N
                F=mulfv(vec2(i),msprintfv(h));
                if (i == 1) then
                    F=subfv(F,mulfv(clua,b2));
                elseif (i == N) then
                    F=subfv(F,mulfv(club,b2));
                end
                C=mulfv(multMatVect(coef21(i,:),vec2(N+1:2*N)),b2);
                equations(i)="   res["+string(i-1)+"]="+subfv(F,C)+";";
                equations(i+N)="   res["+string(i+N-1)+"]="+subfv(vec2(i+2*N),dvec2(i+N))+";";
                F=mulfv(msprintfv(h*a7_ev(i)),b7);
                if (i ~= 1 & i ~= N) then
                    B=addfv(mulfv(mulfv(msprintfv(h*a1_ev(i)),b1),dvec2(i+2*N)),mulfv(mulfv(msprintfv(h*a4_ev(i)),b4),dvec2(i)));
                    C=addfv(addfv(mulfv(multMatVect(coef1(i,:),vec2(1:N)),b2),mulf3v(msprintfv(h*a5_ev(i)),b5,vec2(i))),..
                    mulf3v(msprintfv(h*a6_ev(i)),b6,vec2(i+N)));
                    equations(i+2*N)="   res["+string(i+2*N-1)+"]="+subfv(subfv(F,B),C)+";";
                end
            end
        elseif ( find(oper == 3) ~= [] | find(oper == 4) ~= []) then
            // cas du/dt (oper 3)
            flag_type=2;
            impl_type=0;
            equations=emptystr(2*N,1);
            vec2=equations; dvec2=vec2;
            for i=1:2*N
                vec2(i)="x["+string(i-1)+"]";
                dvec2(i)="xd["+string(i-1)+"]";
            end

            for i=1:N
                F=mulfv(vec2(i),msprintfv(h));
                if (i == 1) then
                    F=subfv(F,mulfv(clua,b2));
                elseif (i == N) then
                    F=subfv(F,mulfv(club,b2));
                end
                C=mulfv(multMatVect(coef21(i,:),vec2(N+1:2*N)),b2);
                equations(i)="   res["+string(i-1)+"]="+subfv(F,C)+";";
                F=mulfv(msprintfv(h*a7_ev(i)),b7);
                if (i ~= 1 & i ~= N) then
                    B=addfv(mulfv(mulfv(msprintfv(h*a3_ev(i)),b3),dvec2(i+N)),mulfv(mulfv(msprintfv(h*a4_ev(i)),b4),dvec2(i)));
                    C=addfv(addfv(mulfv(multMatVect(coef1(i,:),vec2(1:N)),b2),mulf3v(msprintfv(h*a5_ev(i)),b5,vec2(i))),..
                    mulf3v(msprintfv(h*a6_ev(i)),b6,vec2(i+N)));
                    equations(i+N)="   res["+string(i+N-1)+"]="+subfv(subfv(F,B),C)+";";
                end
            end
        else
            // cas alg�brique ==> implicite
            flag_type=2;
            impl_type=-1;
            equations=emptystr(2*N,1);
            vec2=equations;
            for i=1:2*N
                vec2(i)="x["+string(i-1)+"]";
            end

            for i=1:N
                F=mulfv(vec2(i),msprintfv(h));
                if (i == 1) then
                    F=subfv(F,mulfv(clua,b2));
                elseif (i == N) then
                    F=subfv(F,mulfv(club,b2));
                end
                C=mulfv(multMatVect(coef21(i,:),vec2(N+1:2*N)),b2);
                equations(i)="   res["+string(i-1)+"]="+subfv(F,C)+";";
                F=mulfv(msprintfv(h*a7_ev(i)),b7);
                if (i ~= 1 & i ~= N) then
                    C=addfv(addfv(mulfv(multMatVect(coef1(i,:),vec2(1:N)),b2),mulf3v(msprintfv(h*a5_ev(i)),b5,vec2(i))),..
                    mulf3v(msprintfv(h*a6_ev(i)),b6,vec2(i+N)));
                    equations(i+N)="   res["+string(i+N-1)+"]="+subfv(F,C)+";";
                end
            end
        end
    end
endfunction

