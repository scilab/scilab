// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 1620 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1620
//
// <-- Short Description -->
//    Lorsque j'essaie de lire une matrice de taille 500x148, soit:
//    -le programme me charge la matrice mais en decalant les elements (le
//    premier de la deuxième ligne devient le dernier de la premiere) et en
//    donnant la taille 500x149 -> il ne remonte aucun probleme, ce qui est
//    pervers!
//    - soit scilab me dit que la fonction fscanfMat ne dispose pas de
//    memoire suffisante pour lire la matrice
//    - soit scilab plante avec le message décrit ci-dessous.
//
//    Lorsque je charge une matrice beaucoup plus grosse en nombre
//    d'éléments
//    (mais avec moins de colones), je n'ai aucun problème. Il  ne s'agit
//    donc pas vraiment d'un problème de taille mémoire.
//    J'ai essayé d'augmenter la taille de la pile avec la commande
//    gstacksize
//    (n), mais cela n'a pas eu d'effet. Qu'en pensez vous?
//    La commande clear nettoie-t-elle efficacement la mémoire?
//    Merci de votre aide.

a=rand(500,148);
fprintfMat(TMPDIR+"/bug1620.txt",a);
b=fscanfMat(TMPDIR+"/bug1620.txt");

if or(size(b)<>[500 148]) then pause,end
