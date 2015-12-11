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

function [xi,w] = setint()
    // la fonction fournit les point d'int�gration x(i) et les poids      //
    // pour la formule quadratique gaussienne.                            //
    //  Sorties:                                                          //
    //     - x(4,4):  x(:,i) is the Gaussian points of order i.           //
    //     - w(4,4):  w(:,i) is the weights of quadrature of order i.     //
    //                                                                    //
    // Reference: Finite element, An introduction, Vol. 1 by E.Becker,    //
    // G.Carey, and J.Oden, pp. 94.                                       //
    //--------------------------------------------------------------------//
    xi(1,1) = 0;
    w(1,1) = 2;		// Gaussian quadrature of order 1

    xi(1,2) = -1/sqrt(3);
    xi(2,2) = -xi(1,2);
    w(1,2) = 1;
    w(2,2) = w(1,2);		// Gaussian quadrature of order 2

    xi(1,3) = -sqrt(3/5);
    xi(2,3) = 0;
    xi(3,3) = -xi(1,3);
    w(1,3) = 5/9;
    w(2,3) = 8/9;
    w(3,3) = w(1,3);	        // Gaussian quadrature of order 3

    xi(1,4) = - 0.861136311594053;
    xi(2,4) = - 0.339981043584856;
    xi(3,4) = -xi(2,4);
    xi(4,4) = -xi(1,4);
    w(1,4) = 0.347854845137454;
    w(2,4) = 0.652145154862646;
    w(3,4) = w(2,4);
    w(4,4) = w(1,4);		// Gaussian quadrature of order 4

endfunction
//--------------------------- FIN DE SETINT --------------------------//

function [gk,gf]=formkf(nelem,kind,nint,nodes,x,xi,w,nnode,a6,operi,kbc,vbc)
    // d�velopp� par EADS-CCR
    //   la fonction formkf, construit le syst�me discret de l'element        //
    //   finis en appelant la fonction elem pour avoir la matrices locales    //
    //   ek, ef et la fonction assemb pour les  ajout�es aux matrices          //
    //   globales gk et le second membre gf.                                  //
    //   Sorties :                                                            //
    //      - gk (Double) : matrice globale                                   //
    //      - gf (Double) : vecteur qui correspond au scond membre            //
    //   Entr�es :                                                            //
    //      - nelem (Entier) : est le nombre d'�l�ments                       //
    //      - kind(i) (Entier) : ordre des fonctions de test                  //
    //      - ninit(i) (Entier) :ordre d'integration Gaussian                 //
    //      - x (Double):  vecteur des cordonn�es des points nodales          //
    //      - xi, w (Doubles) : les points Gausse et leurs poids obtenu       //
    //        de setint()                                                     //
    //      - a6 (String) : coefficient a(x) de l'op�rateur pour lequel nous  //
    //        calculons ca forme variationelle.                               //
    //      - operi (Entier) : l'op�rateur concern�                           //
    //      - kbc (Entier) : vecteur types des conditions au limites          //
    //      - vbc (String) : vecteur des conditions aux limites en a et b     //
    //------------------------------------------------------------------------//

    // syst�me discr�tis�

    gk = spzeros(nnode,nnode);
    gf = zeros(nnode,1);

    for nel = 1:nelem
        n = kind(nel) + 1;
        i1 = nodes(1,nel);
        i2 = nodes(n,nel);
        i3 = nint(nel);

        //  Prendre le i3-�me ordre de la quadrature Gaussienne: 1, ordre 1; 2, ordre 2, ...

        xic = xi(:,i3);
        wc = w(:,i3);
        [ek,ef] = elemoper(x(i1),x(i2),n,i3,xic,wc,operi,a6);
        // assemblage
        [gk,gf]=assemb(gk,gf,ek,ef,nel,n,nodes);
    end

    // ajustement par apport aux conditions aux limites.
    if (kbc(1) == 0 ) then
        gk(1,:)=0;
        gf(1)=0;
    end
    if (kbc(2) == 0 ) then
        gk(nnode,:)=0;
        gf(nnode)=0;
    end
endfunction

//--------------------------- FIN DE FORMKF -----------------------------//

function [gk,gf]=assemb(gk,gf,ek,ef,nel,n,nodes)
    // d�velopp� par EADS-CCR
    //  la fonction assemb assemble la matrice de regidit� gk et le second   //
    //  membre gf en bouclant sur les nel- elements.                         //
    //   Sorties :                                                           //
    //      - gk (Double) E/S : matrice globale                              //
    //      - gf (Double) E/S : vecteur qui correspond au scond membre       //
    //   Entr�es :                                                           //
    //      - ek (Double) : matrice �l�mentaire                              //
    //      - ef (Double) : vecteur �l�mentaire du scond membre              //
    //      - nel (Entier) : le nombre d'element.                            //
    //      - n (Entier) :   nombre de points nodales dans un element,       //
    //        e.g. linear n=2; quadrtic, n=3; cubic: n=4.                    //
    //      - nodes (Entier) : nombre des points nodales du noeud dans les   //
    //        nel elements, nodes(1,nel), nodes(2,nel), ..., nodes(n,nel).   //
    //-----------------------------------------------------------------------//

    for i=1:n
        ig = nodes(i,nel);
        // Assemblage global du vecteur gf
        gf(ig) = gf(ig) + ef(i);

        for j=1:n
            jg = nodes(j,nel);
            // Assemblage global de la matrice de regidit� gk
            gk(ig,jg) = gk(ig,jg) + ek(i,j);
        end
    end
endfunction
//--------------------------- FIN DE ASSEMB -----------------------------//

function [psi,dpsi]=shape(xi,n);
    // d�velopp� par EADS-CCR
    // la fonction ``shape'' evalue les valeurs des fonction de base et      //
    // d�riv�es en un point xi.                                              //
    // Sorties :                                                             //
    //    - psi (Double) :  valeur de la fonction de base a xi.              //
    //    - dpsi (Double) : valeur de la d�riv�e de la fonction de base a xi.//
    // Entr�es :                                                             //
    //    - xi (Entier) : le point ou la fonction de base est �valu�e.       //
    //    - n (Entier) : la base des functions. n=2,linear,n=3,quadratic,    //
    //      n=4, cubic.                                                      //
    //   Reference: Finite element. An introduction y E.Becker, G.Carey,     //
    //   and J.Oden, Vol.1., pp. 95-96.                                      //
    //-----------------------------------------------------------------------//

    select n
    case 2 then
        // Linear base function
        psi(1) = (1-xi)/2;
        psi(2) = (1+xi)/2;
        dpsi(1) = -0.5;
        dpsi(2) = 0.5;
        return

    case 3 then
        // quadratic base function
        psi(1) = xi*(xi-1)/2;
        psi(2) = 1-xi*xi;
        psi(3) = xi*(xi+1)/2;
        dpsi(1) =  xi-0.5;
        dpsi(2) = -2*xi;
        dpsi(3) = xi + 0.5;
        return

    case 4 then
        // cubic  base function
        psi(1) = 9*(1/9-xi*xi)*(xi-1)/16;
        psi(2) = 27*(1-xi*xi)*(1/3-xi)/16;
        psi(3) = 27*(1-xi*xi)*(1/3+xi)/16;
        psi(4) = -9*(1/9-xi*xi)*(1+xi)/16;

        dpsi(1) = -9*(3*xi*xi-2*xi-1/9)/16;
        dpsi(2) = 27*(3*xi*xi-2*xi/3-1)/16;
        dpsi(3) = 27*(-3*xi*xi-2*xi/3+1)/16;
        dpsi(4) = -9*(-3*xi*xi-2*xi+1/9)/16;
    else
        break
    end
endfunction
//--------------------------- FIN DE SHAPE ---------------------------//

function [ek,ef] = elemoper(x1,x2,n,nl,xi,w,operi,a6)
    // d�velopp� par EADS-CCR
    //  la fonction elem evalue la matrice gk et le second memebre gf     //
    //  Sorties :                                                         //
    //    - ek (Double) : matrice �l�mentaire                             //
    //    - ef (Double) : vecteur �l�mentaire du scond membre             //
    //  Entr�es :                                                         //
    //    - x1,x2 (Doubles): cordonn�es x1 et x2 des limites de l'element //
    //    - n (Entier) : Nombre des points nodals (et la fonction test)   //
    //      dans l'element                                                //
    //    - nl (Entier) : ordre d'integration Gaussian, 1,2,3,4           //
    //    - xi(l) (Double) : la valeur de la cordonn�e globale en         //
    //      un point d'integration.                                       //
    //    - w(l) (Double) : le poids d'integration.                       //
    //    - a6 (String) : coefficient a(x) de l'op�rateur pour lequel     //
    //      nous calculons ca forme variationelle.                        //
    //    - operi (Entier) : l'op�rateur concern�                         //
    //  Fonction appell�e: shape(xi(l),n)                                 //
    //  Reference: Finite element. An introduction by E.Becker, G.Carey,  //
    //  and J.Oden, Vol.1., pp. 97-99.                                    //
    //--------------------------------------------------------------------//

    dx = (x2-x1)/2;

    // Initialisation des matrices �l�mentaires
    ef = zeros(n,1);
    ek = zeros(n,n);
    // Evaluation des coefficients des op�rateurs
    a6_x=strindex(a6,"x");
    if (operi ==2) then
        if (a6_x ~=[]) then
            // cas o� a6 depend de x, on calcul A=a(j)*dpsj+a(j+1)*dps(j+1)
            [psi1,dpsi1] = shape(x1,n);
            for i=1:n
                x=x1;
                ax1=evstr(a6);
                x=x1+(x2-x1)/(n-1);
                ax2=evstr(a6);
                if (n == 2) then
                    ax=(ax1*dpsi1(1)+ ax2*dpsi1(2))/dx;
                elseif (n == 3) then
                    x=x1+2*(x2-x1)/(n-1);
                    ax3=evstr(a6);
                    ax=(ax1*dpsi1(1)+ ax2*dpsi1(2)+ ax3*dpsi1(3))/dx;
                end
            end
        end
    end

    // D�but de la boucle d'it�gration
    for l=1:nl
        x = x1 + (1.0 + xi(l))*dx;
        [psi,dpsi] = shape(xi(l),n);
        ev_a6=evstr(a6);
        // Assemblage de la matrice �l�mentaire et le vecteur charge.
        for i=1:n,
            if (operi == 7) then
                ef(i) = ef(i) + psi(i)*ev_a6*w(l)*dx;
            end
            for j=1:n
                if ( find([1,3,6] == operi) ~= []) then
                    // psi*psi (oper = 1, 3, 6)
                    ek(i,j)=ek(i,j)+(ev_a6*psi(i)*psi(j) )*w(l)*dx;
                elseif (operi == 5) then
                    // psi*dpsi (oper =5)
                    ek(i,j)=ek(i,j)+(ev_a6*psi(i)*dpsi(j) )*w(l);
                elseif (operi == 4) then
                    // dpsi*psi (oper =4)
                    ek(i,j)=ek(i,j)+(ev_a6*dpsi(i)*psi(j) )*w(l);
                elseif (operi == 2) then
                    // dpsi*dpsi (oper =2)
                    if (a6_x ==[]) then
                        // cas o� a6 ne depend pas de x
                        ek(i,j)=ek(i,j)+(ev_a6*dpsi(i)*dpsi(j) )*w(l)/dx;
                    else
                        // cas o� a6 depend de x, on calcul A=a(j)*dpsj+a(j+1)*dps(j+1)
                        ek(i,j)=ek(i,j)+(ev_a6*dpsi(i)*dpsi(j)/dx + ax*dpsi(i)*psi(j))*w(l);
                    end
                end
            end
        end
    end

endfunction

