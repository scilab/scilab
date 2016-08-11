// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// The first example
a=rand(100,10,"n");
[lambda,facpr,comprinc] = pca(a);
show_pca(lambda,facpr);

// The second example
// Source :
// http://cermics.enpc.fr/scilab_new/site/Tp/Statistique/acp/index.htm
// Analyse en composantes principales
// Jean-François DELMAS et Saad SALAM
// Weight of several parts of 23 cows
// X1: weight (alive)
// X2: skeleton weight
// X3: first grade meat weight
// X4: total meat weight
// X5: fat weight
// X6: bones weight
x = [
395     224     35.1     79.1     6.0     14.9
410     232     31.9     73.4     8.7     16.4
405     233     30.7     76.5     7.0     16.5
405     240     30.4     75.3     8.7     16.0
390     217     31.9     76.5     7.8     15.7
415     243     32.1     77.4     7.1     18.5
390     229     32.1     78.4     4.6     17.0
405     240     31.1     76.5     8.2     15.3
420     234     32.4     76.0     7.2     16.8
390     223     33.8     77.0     6.2     16.8
415     247     30.7     75.5     8.4     16.1
400     234     31.7     77.6     5.7     18.7
400     224     28.2     73.5     11.0     15.5
395     229     29.4     74.5     9.3     16.1
395     219     29.7     72.8     8.7     18.5
395     224     28.5     73.7     8.7     17.3
400     223     28.5     73.1     9.1     17.7
400     224     27.8     73.2     12.2     14.6
400     221     26.5     72.3     13.2     14.5
410     233     25.9     72.3     11.1     16.6
402     234     27.1     72.1     10.4     17.5
400     223     26.8     70.3     13.5     16.2
400     213     25.8     70.4     12.1     17.5
];
[lambda,facpr,comprinc] = pca(x);
scf();
show_pca(lambda,facpr);

