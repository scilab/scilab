//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// ============================================================================
// Tests for riccati() function
// Added : 25/06/2007
// Thanks to Sabine GAUZERE
// ============================================================================

n = 10;
A = rand(n,n);
B = rand(n,n);
C = rand(n,n);
C = C*C';
R = rand(n,n);
R = R*R'+eye();
B = B*inv(R)*B';

// Test de l'équation en temps continu
X = riccati(A,B,C,'c','eigen');

// Vérification
C_cont = A'*X+X*A-X*B*X;

// Test de l'équation en temps discret
F      = A;
B      = rand(n,n);
G1     = B;
G2     = R;
G      = G1/G2*G1';
H      = C;
[X1,X2]= riccati(F,G,H,'d','schur');

// Vérification
X      = X1/X2;
C_disc = F'*X*F-(F'*X*G1/(G2+G1'*X*G1))*(G1'*X*F)-X;

// Comparaison des différents résultats obtenus
if norm(C_cont+C,1) > (10000*%eps) then pause,end
if norm(C_disc+H,1) > (10000*%eps) then pause,end
