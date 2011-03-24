// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- JVM NOT MANDATORY -->

//
// assert_close --
//   Returns 1 if the two real matrices computed and expected are close,
//   i.e. if the relative distance between computed and expected is lesser than epsilon.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_close ( computed, expected, epsilon )
  if expected==0.0 then
    shift = norm(computed-expected);
  else
    shift = norm(computed-expected)/norm(expected);
  end
  if shift < epsilon then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction
//
// assert_equal --
//   Returns 1 if the two real matrices computed and expected are equal.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_equal ( computed , expected )
  if computed==expected then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction

function flag = assert_true ( computed )
  if and(computed) then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction

rtol = 1.e-2;

// Set the seed to always get the same random numbers
grand("setsd",0);

//
// 1. Permute some vectors, and check that the output is basically correct.
//
X = (2:10)';
P = grand(1,"prm",X);
assert_equal ( typeof(P) , "constant" );
assert_equal ( size(P) , size(X) );
assert_equal ( gsort(P,"g","i") , X );
//
X = (10:100)';
P = grand(1,"prm",X);
assert_equal ( typeof(P) , "constant" );
assert_equal ( size(P) , size(X) );
assert_equal ( gsort(P,"g","i") , X );
//
X = [-12 4 9 365]';
P = grand(1,"prm",X);
assert_equal ( typeof(P) , "constant" );
assert_equal ( size(P) , size(X) );
assert_equal ( gsort(P,"g","i") , X );
//
X = (2:11)';
P = grand(5,"prm",X);
assert_equal ( typeof(P) , "constant" );
assert_equal ( size(P) , [10 5] );
assert_equal ( gsort(P,"r","i") , X * ones(1,5) );
//
// 2. Check that the produced permutations are uniformly chosen in the 
// set of all possible permutations.
//
X = [-12 4 9 365]';
S = perms(X);
// Total number of permutations : F = 4! = 24
F = size(S,"r");
// Number of random permutations : N
N = 10000;
P = grand(N,"prm",X);
// Set in R(k), k=1,2,...,N the index i of the permutation, with i=1,2,...,F.
// R must be a random variable uniformly distributed in the interval [1,2,...,F].
R = zeros(N,1);
for k = 1:N
  permk = P(:,k);
  // Search the index i of the permutation permk
  for i = 1 : F
    permi = S(i,:);
    if ( permi'==permk ) then
      R(k) = i;
      break
    end
  end
end
assert_close ( mean(R) , (1+F)/2 , 0.01 );
assert_close ( variance(R) , (F^2-1)/12 , 0.1 );
//
// Check the distribution of R
function p = mycdfuin (X,A,B)
  p = (floor(X)-A+1)/(B-A+1)
endfunction
X = (1:F)';
for k = 1 : size(X,"*")
    EmpicicalPDF(k) = length(find(R==X(k)));
end
EmpicicalPDF = EmpicicalPDF./N;
CDF = mycdfuin(X,1,F);
TheoricPDF=[CDF(1);diff(CDF)];
assert_true( abs(EmpicicalPDF-TheoricPDF) < rtol );
  if ( %f ) then
    plot(X,EmpicicalPDF,"bo-"); // Empirical PDF
    plot(X,TheoricPDF,"rox-"); // Theoretical PDF
  end


