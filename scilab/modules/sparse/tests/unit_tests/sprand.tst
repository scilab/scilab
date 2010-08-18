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

// Check default "typ" = "uniform"
rand("seed",0);
grand("setsd",0);
nrows = 1000;
ncols = 2000;
density = 1/100;
s=sprand(nrows,ncols,density);
assert_equal ( size(s) , [nrows,ncols] );
nnzs=nnz(s);
[ij,values]=spget(s);
assert_equal ( min(values) >= 0 , %t );
assert_equal ( max(values) <= 1 , %t );
assert_close ( mean(values) , 0.5 , 1.e-2 );

// Get empty matrix
s=sprand(0,0,0.01);
assert_equal ( s , [] );

// Test the scientific part
// In the following script, we check that the entries of the matrix have the expected distribution. 
// We use the spget function in order to get the nonzero entries. 
// Then we compute the min, mean and max of the entries and compare them with the limit values.

rand("seed",0);
grand("setsd",0);
typ = "normal";
nrows = 1000;
ncols = 2000;
density = 1/100;
s=sprand(nrows,ncols,density,typ);
assert_equal ( size(s) , [nrows,ncols] );
nnzs=nnz(s);
[ij,values]=spget(s);
assert_equal ( nnzs > 10000 , %t );
assert_equal ( min(values) < -3 , %t );
assert_close ( mean(values) , 0 , 1.e-2 );
assert_equal ( max(values) > 3 , %t );
assert_close ( variance(values) , 1 , 1.e-3 );

rand("seed",0);
grand("setsd",0);
typ = "uniform";
nrows = 1000;
ncols = 2000;
density = 1/100;
s=sprand(nrows,ncols,density,typ);
assert_equal ( size(s) , [nrows,ncols] );
nnzs=nnz(s);
[ij,values]=spget(s);
assert_equal ( nnzs > 10000 , %t );
assert_close ( min(values) , 0 , 1.e-2 );
assert_close ( mean(values) , 0.5 , 1.e-2 );
assert_close ( max(values) , 1 , 1.e-2 );
assert_close ( variance(values) , 1/12 , 1.e-2 );

// In the following script, we check that the entry indices, 
// which are also chosen at random, have the correct distribution. 
// We generate kmax sparse random matrices with uniform distribution. 
// For each matrix, we consider the indices of the nonzero entries 
// which were generated, i.e. we see if the event Aij = {the entry (i,j) is nonzero}
// occured for each i and j, for i=1,2,...,nrows and j=1,2,...,ncols. 
// The matrix C(i,j) stores the number of times that the event Aij occured. 
// The matrix R(k) stores the actual density of the try number k, where k=1,2,...,kmax.

rand("seed",0);
grand("setsd",0);
kmax = 1000;
ncols=100;
nrows=100;
density=0.01;
typ="uniform";
C=zeros(nrows,ncols);
R=[];
for k=1:kmax
  M=sprand(nrows,ncols,density,typ);
  NZ=find(M<>0);
  NZratio = size(NZ,"*")/(nrows*ncols);
  R=[R NZratio];
  C(NZ)=C(NZ)+1;
end
// Now that this algorithm has been performed (which may require some time), 
// we can compute elementary statistics to check that the algorithm performed well.

// The average number should be close to the expectation.
assert_close ( density*kmax , mean(C) , 1.e-2 );
// The density should be close to expected density
assert_close ( density , mean(R) , 1.e-2 );

// More deeper tests should involve the particular distribution of 
// C, which follows a binomial law.
// May be a chi-square test should be used for this.


