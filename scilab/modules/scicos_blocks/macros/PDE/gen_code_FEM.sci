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

function [equations,impl_type]=gen_code_FEM(A,B1,B2,C1,C2,C3,F3,oper,N,..
    a,b,b1,b2,b3,b4,b5,b6,b7,vbc,kbc)
    // Cette fonction est pour la g�n�ration des �quations DAE du bloc        //
    // sorties :                                                              //
    //    - equations (String) : vecteur qui contient le code C des �quations //
    //      d'etat (DAE)                                                      //
    //    - impl_type (Entier) : indique si le type des �tats                 //
    //      (diff�rentiels 1 ou alg�brique -1)                                //
    // entr�es :                                                              //
    //    - Ai, Bi (Doubles) : matrices d'assemblage A, B (B1 (oper 3) et     //
    //      B2(oper 4), C (C1 pour oper 2, C2 pour oper 5 et C3 pour oper 6)  //
    //      et F3 pour oper 7) pour le syst�me: A*d2u/dt2 + B*du/dt + C*u = F //
    //    - oper (Entier) : vecteur des op�rateurs selectionnes de 1 � 7      //
    //    - N (Entier) : est le nombre de noeuds                              //
    //    - a, b (Double) : limites du domaine [a b]                          //
    //    - ai, bi (String) : les differents coeficients des op�rateurs       //
    //      (ai(x) et bi(t))                                                  //
    //    - vbc (String) : vecteur des conditions aux limites en a et b       //
    //    - kbc (Entier) : vecteur types des conditions au limites            //
    //------------------------------------------------------------------------//
    Cla2=[];
    Clb2=[];
    Cla4=[];
    Clb4=[];
    lambda=spzeros(N,N);
    impl_type=1; // 1 pour syst�me d'�tat, -1 pour le syst�me alg�brique
    sep=[",","*","/"];

    // prise en compte des conditions aux limites par dualisation
    if (kbc(1) == 1) then
        x=a;
        if (find(oper == 4) ~= []) then
            Cla4="-"+mulf3v(b4,msprintfv(evstr(a4)),"xd[0]");
        end
        if (find(oper == 2) ~= []) then
            Cla2=mulf3v(msprintfv(evstr(a2)),b2,vbc(1));
        end
    end
    if (kbc(2) == 1) then
        x=b;
        if (find(oper == 4) ~= []) then
            Clb4=mulf3v(b4,msprintfv(evstr(a4)),"xd["+string(N)+"]");
        end
        if (find(oper == 2) ~= []) then
            Clb2=mulf3v(msprintfv(evstr(a2)),b2,vbc(2));
        end
    end
    //*******************************************************

    if ( find(oper == 1) ~= []) then
        // cas d2u/dt2 ==> implicite
        equations=emptystr(2*N,1);
        vec2=equations;
        dvec2=vec2;
        for i=1:2*N
            vec2(i)="x["+string(i-1)+"]";
            dvec2(i)="xd["+string(i-1)+"]";
        end

        for i=1:N
            equations(i)="   res["+string(i-1)+"]="+vec2(i+N)+"-"+dvec2(i)+";";
            F=mulfv(msprintfv(F3(i)),b7);
            if (i == 1) then
                // prise en compte des conditions aux limites de type Dirichlet par dualisation en a
                if (kbc(1) == 0) then
                    F=vbc(1);
                    lambda(1,1)=1;
                else
                    F=subfv(F,addfv(mulfv(Cla2,b2),mulfv(Cla4,b4)));
                end
            elseif (i == N) then
                // prise en compte des conditions aux limites de type Dirichlet par dualisation en b
                if (kbc(2) == 0) then
                    F=vbc(2);
                    lambda(N,N)=1;
                else
                    F=subfv(F,addfv(mulfv(Clb2,b2),mulfv(Clb4,b4)));
                end
            end
            B=mulfstring(addf_mat(multVectStr(B1(i,:),b3),multVectStr(-B2(i,:),b4)),vec2(N+1:$));
            C=mulfstring(addf_mat(multVectStr(-C1(i,:),b2),addf_mat(addf_mat(multVectStr(C2(i,:),b5),..
            multVectStr(C3(i,:),b6)),msprintfv(full(lambda(i,:))')')),vec2(1:N));
            Ai=mulfstring(multVectStr(A(i,:),b1),dvec2(N+1:$));
            equations(i+N)="   res["+string(i+N-1)+"]="+subfv(subfv(subfv(F,Ai),B),C)+";";
        end

    elseif ( find(oper == 3) ~= [] | find(oper == 4) ~= []) then
        // cas du/dt (oper 3 ou 4) ==> implicite
        equations=emptystr(N,1);
        vec2=equations;
        dvec2=vec2;
        for i=1:N
            vec2(i)="x["+string(i-1)+"]";
            dvec2(i)="xd["+string(i-1)+"]";
        end

        for i=1:N
            F=mulfv(msprintfv(F3(i)),b7);
            if (i == 1) then
                // prise en compte des conditions aux limites de type Dirichlet par dualisation en a
                if (kbc(1) == 0) then
                    F=vbc(1);
                    lambda(1,1)=1;
                else
                    F=subfv(F,addfv(mulfv(Cla2,b2),mulfv(Cla4,b4)));
                end
            elseif (i == N) then
                // prise en compte des conditions aux limites de type Dirichlet par dualisation en b
                if (kbc(2) == 0) then
                    F=vbc(2);
                    lambda(N,N)=1;
                else
                    F=subfv(F,addfv(mulfv(Clb2,b2),mulfv(Clb4,b4)));
                end
            end
            B=mulfstring(addf_mat(multVectStr(B1(i,:),b3),multVectStr(-B2(i,:),b4)),dvec2(:));
            C=mulfstring(addf_mat(multVectStr(-C1(i,:),b2),addf_mat(addf_mat(multVectStr(C2(i,:),b5),..
            multVectStr(C3(i,:),b6)),msprintfv(full(lambda(i,:))')')),vec2(:));

            equations(i)="   res["+string(i-1)+"]="+subfv(subfv(F,B),C)+";";
        end
    else
        // cas alg�brique ==> implicite
        impl_type=-1;
        equations=emptystr(N,1);
        vec2=equations;
        for i=1:N
            vec2(i)="x["+string(i-1)+"]";
        end

        for i=1:N
            F=mulfv(msprintfv(F3(i)),b7);
            if (i == 1) then
                // prise en compte des conditions aux limites de type Dirichlet par dualisation en a
                if (kbc(1) == 0) then
                    F=vbc(1);
                    lambda(1,1)=1;
                else
                    F=subfv(F,addfv(mulfv(Cla2,b2),mulfv(Cla4,b4)));
                end
            elseif (i == N) then
                // prise en compte des conditions aux limites de type Dirichlet par dualisation en b
                if (kbc(2) == 0) then
                    F=vbc(2);
                    lambda(N,N)=1;
                else
                    F=subfv(F,addfv(mulfv(Clb2,b2),mulfv(Clb4,b4)));
                end
            end
            C=mulfstring(addf_mat(multVectStr(-C1(i,:),b2),addf_mat(addf_mat(multVectStr(C2(i,:),b5),..
            multVectStr(C3(i,:),b6)),msprintfv(full(lambda(i,:))')')),vec2(:));

            equations(i)="   res["+string(i-1)+"]="+subfv(F,C)+";";
        end
    end

endfunction

