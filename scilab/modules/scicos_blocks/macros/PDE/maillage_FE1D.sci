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

function [xvec,nelem,nodes,kind,nint]=maillage_FE1D(a,b,inf_ele,nnode,CLa_type,..
    CLa_exp,CLb_type,CLb_exp)
    // rentre les donn�es des partitions (les elements)                               //
    // sorties :                                                                      //
    //   - xvec (Double) : vecteur des cordonn�es de tout les noeuds de 1:nnode       //
    //   - nelem (Entier) = (nnode-1)/inf_ele; c'est le nombre d'�l�ment.             //
    //   - nodes(j,i) (Entier): Matrice de connection des element, nodes(nnode,nelem),//
    //     nodes(j,i) est le jeme indice du noeud dans le ieme element. Pour le       //
    //     lin�aire j=1,2 (deuxnoeuds dans chaque element). Pour le quadratique,      //
    //     j=1,2,3 (deux noeuds et un point auxiliare.                                //
    //   - kind(i) (Entier), i=1, ..., nelem, = 1, ou 2, ou 3. les fonctions de base  //
    //     dans le i-eme element. Pour la triangulation uniforme, kind(i) = inf_ele.  //
    //   - nint(i) (Entier), i=1, ..., nelem = 1, ou 2, ou 3, ou 4. Pour l'ordre du   //
    //     Gaussian quadratique dans le i-eme element.                                //
    // entr�es :                                                                      //
    //   - a, b (Doubles) :correspondent aux deux points limites (segment [a,b])      //
    //   - inf_ele (Entier) : 1, 2 ou 3. Les fonctions de base : lin�aires,           //
    //     quadratique ou cubique                                                     //
    //   - nnode (Entier) : nombre de noeuds. Si inf_ele=1, nnode est un nombre >= 2. //
    //     Si inf_ele=2, nnode doit �tre 2k+1, k>=1. Si inf_ele = 3,                  //
    //     nnode doit �tre 3k+1.                                                      //
    //   - CLa_type, CLb_type(Entiers):types des conditions aux limites (0:Dirichlet, //
    //     1 : Neumann)                                                               //
    //   - CLb_exp,CLa_exp(String):expressions des conditions aux limites en a et b   //
    //--------------------------------------------------------------------------------//

    xvec=zeros(nnode,1);
    nelem = (nnode-1)/inf_ele;

    //  load data.sci	// n1,n2,x1,x2
    [data] = datain(a,b,nnode,nelem);
    [nrec,junk]=size(data);

    for nr=1:nrec
        n1=data(nr,1);
        n2=data(nr,2);
        x1=data(nr,3);
        x2=data(nr,4);
        if (n2 == 0) then
            xvec(n1) = x1;
        else
            dn = n2 - n1;
            dx = (x2-x1)/dn;
            xx = x1 - dx;
            // debut de la boucle de d�finition des cordonn�es des points nodales.
            for i=n1:n2
                xx = xx + dx;
                xvec(i) = xx;
            end
        end
    end

    // Definition du type de la fonction de base.
    for i=1:nelem
        kind(i) = inf_ele;	   // on utilise les elements lineaires
        nint(i) = 4;        // l'ordre d'int�gration
        // connection des ddls:
        for j=1:kind(i)+1
            nodes(j,i) = j + kind(i)*(i-1);   // nodal point numbers of nodes
        end		           // dans l'element
    end

endfunction


function [data] = datain(a,b,nnode,nelem)
    // d�velopp� par EADS-CCR
    //   La fonction datain fournit les information des partitions  pour  //
    //   la m�thode des elements finis 1D. On choisissant les donn�es     //
    //   sohait�es, nous pouvons g�n�r� les noeuds avec un espacement     //
    //   uniforme dans les diff�rentes sections du maillage.              //
    // sorties :                                                          //
    //   - data (Double) : Matrice de donn�es                             //
    // entr�es :                                                          //
    //   - a, b (Doubles) :correspondent aux deux points limites          //
    //   - nnode (Entier) : nombre de noeuds.                             //
    //   - nelem (Entier) = (nnode-1)/inf_ele; c'est le nombre d'�l�ment. //
    //   Reference: Finite element. An introduction by E.Becker, G.Carey, //
    //   and J.Oden, Vol.1., pp. 101-102.                                 //
    //--------------------------------------------------------------------//

    h=(b-a)/(nnode-1);
    x = zeros(nnode,1);
    for i=1:nnode
        x(i) = a +(i-1)*h;
    end

    for i=1:nnode
        data(i,1) = i;
        data(i,2) = 0;
        data(i,3) = x(i);
        data(i,4) = x(i);
    end

endfunction

//--------------------------- END OF DATAIN -----------------------------

