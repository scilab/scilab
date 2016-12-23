// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// =============================================================================
// unit tests length
// =============================================================================


// = Empty matrix ==============================================================
l = length([]);
if (l <> 0) then pause,end;


// = integer ===================================================================
N = 321;
l = length(N);
if (l <> 1) then pause,end;


// = Matrix of integer =========================================================
N = 321;
M = 32;
PARAMIN = [N,M;M,N];
l = length(PARAMIN);
if (l <> size(PARAMIN,"*") ) then pause,end;


// = String ====================================================================
STRING = "Scilab";
l = length(STRING);
if (l <> 6 ) then pause,end;


// = Matrix of Strings =========================================================
STRING1 = "Scilab";
STRING2 = "Strings";
PARAMIN = [STRING1,STRING2;STRING2,STRING1];
l = length(PARAMIN);
LS1 = length(STRING1);
LS2 = length(STRING2);
if (LS1 <> 6 ) then pause,end;
if (LS2 <> 7 ) then pause,end;
if (l <> [ LS1, LS2; LS2 , LS1 ]) then pause,end


// = List (of strings) =========================================================
STRING1 = "Scilab";
STRING2 = "5.x";
m=list();
m(1)=STRING1;
m(2)=STRING2;
if length(m) <> 2  then pause,end


// = List (mixed) ==============================================================
l = list(1,["a" "b"],[1 2 3 4],%T);
if length(l) <> 4  then pause,end


// = mlist =====================================================================
M = mlist(["V","name","value"],["a","b","c"],[1 2 3]);
if length(M) <> 3  then pause,end


// = tlist =====================================================================
Sys=tlist(["lss";"A";"B";"C";"D";"X0";"dt"],1,2,3,4,5,"c");
if length(Sys) <> 7  then pause,end


// = Matrix of Strings =========================================================
STRING1 = "Scilab";
STRING2 = "5.x";
LS1 = length(STRING1);
LS2 = length(STRING2);
if length([STRING1,STRING2]) <> [LS1,LS2] then pause,end


// = Matrix of Strings =========================================================
if length("abd")<>3 then pause,end
if length(emptystr())<>0 then pause,end
if or(length(["abd";emptystr()])<>[3;0]) then pause,end
if or(length(string(ones(10,10)))<>1) then pause,end


// = with a file ===============================================================
fd = mopen(SCI+"/modules/string/tests/unit_tests/text.txt","r");
txt = mgetl( fd );
mclose( fd );
if length(txt(1)) <> 280  then pause,end
S = size(txt);
if length(txt(S(1)-1)) <> 106  then pause,end
if length(txt(S(1))) <> 0  then pause,end


// = an hypermatrix (double) ===================================================
A = rand(2,5,2,3,2);
assert_checkequal(length(A), size(A,"*"));
A = matrix(1:24, [2,3,4]);
assert_checkequal(length(A), size(A,"*"));

// = an hypermatrix (string) ===================================================
clear S;
S(:,:,1) = ["a","ab";"abc","abcd"];
S(:,:,2) = ["dcba", "cba"; "ba", "a"];
ref(:,:,1) = [1 2;3 4];
ref(:,:,2) = [4 3;2 1];
assert_checkequal(length(S), ref);

// = an sparse ===================================================
sp = sparse([1 2 3;4 5 6]);
assert_checkequal(length(sp), 6);
sp = sparse(eye(12,2));
assert_checkequal(length(sp), 24);
sp = sparse(eye(2,12));
assert_checkequal(length(sp), 24);
