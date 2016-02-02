// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [cout,grad,ind]=iirlp(x,ind,p,flag,lambda,omega,ad,wa,td,wt)
    //
    //optimization of IIR filters IIR with  Lp criterium for magnitude
    //                                      and/or group delay
    //     -cf Rabiner & Gold pp270-273-
    //
    //auteur : G. Le Vey
    //
    //   p ===> critere Lp
    //
    //r=module des poles et zeros des filtres
    //theta=argument des  "    "   "    "    "
    //omega=frequences ou sont donnees les specifications des filtres
    //wa,wt=fonctions de ponderation pour l'amplitude et le
    //retard de groupe ad,td=amplitudes et retard de groupe desires
    //!
    r=x(:,1);theta=x(:,2);
    [m,n]=size(ad);if m>n,ad=ad';end
    [m,n]=size(td);if m>n,td=td';end
    [m,n]=size(omega);if m>n,omega=omega';end;
    [m,n]=size(r);if n>m,r=r';m=n;end;
    [m,n]=size(theta);if n>m,theta=theta';m=n;end;
    //
    select flag
    case "a"
        //AMPLITUDE
        [cout,grad]=iirmod(p,r,theta,omega,wa,ad);
        //
    case "gd"
        //RETARD DE GROUPE
        [cout,grad]=iirgroup(p,r,theta,omega,wt,td);
        //
    else
        //AMPLITUDE ET RETARD DE GROUPE
        [la,ga]=iirmod(p,r,theta,omega,wa,ad);
        [lt,gt]=iirgroup(p,r,theta,omega,wt,td);
        cout=lambda*la+(1-lambda)*lt;
        grad=lambda*ga+(1-lambda)*gt;
    end;
endfunction
