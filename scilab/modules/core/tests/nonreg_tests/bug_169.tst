//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 169 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=169
//
// <-- Short Description -->
//    L'utilisation de variables globales entre ma fonction et la fenetre de commande occassionne inmanquablement des plantages de scilab a un endroit precis, lors de l'execution de la fonction

function [] = crash()
    global S big1 Z big2 X Y

    N=1000;
    S=1:50;
    X=ones(N,1)*(1:N); Y=  (1:N)' * ones(1,N);

    big1=(X.* X - Y.* Y);
    big2=(big1==0);
    Z=sum(big1(S,:),"r");
    Z=sum(1.*big2(S,:),"r");
endfunction

global S big1 Z big2 X Y
crash()
