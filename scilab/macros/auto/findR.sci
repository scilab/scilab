function [R,n,sval,rcnd]=findR(s,y,u,meth,alg,jobd,tol,printw)
R=[];n=[];sval=[];rcnd=[];
[nargout,nargin] = argn(0)
//FINDR   Preprocesses the input-output data for estimating the matrices 
//of a linear time-invariant dynamical system, using Cholesky or
//(fast) QR factorization and subspace identification techniques 
//(MOESP or N4SID), and estimates the system order.
// 
//[R,N] = FINDR(S,Y,U,METH,ALG,JOBD,TOL,PRINTW)  returns the processed
//upper triangular factor  R  of the concatenated block-Hankel matrices 
//built from the input-output data, and the order  N  of a discrete-time
//realization. The model structure is:
// 
//             x(k+1) = Ax(k) + Bu(k) + w(k),   k >= 1,
//             y(k)   = Cx(k) + Du(k) + e(k).
// 
//The vectors y(k) and u(k) are transposes of the k-th rows of Y and U,
//respectively.
// 
//S is the number of block rows in the block-Hankel matrices.
// 
//METH is an option for the method to use:
//METH = 1 :  MOESP method with past inputs and outputs;
//     = 2 :  N4SID method.
//Default:    METH = 1.
// 
//ALG is an option for the algorithm to compute the triangular factor of
//the concatenated block-Hankel matrices built from the input-output data:
//ALG = 1 :   Cholesky algorithm on the correlation matrix;
//    = 2 :   fast QR algorithm;
//    = 3 :   standard QR algorithm.
//Default:    ALG = 1.
// 
//JOBD is an option to specify if the matrices B and D should later
//be computed using the MOESP approach:
//JOBD = 1 :  the matrices B and D should later be computed using
//            the MOESP approach;
//     = 2 :  the matrices B and D should not be computed using
//            the MOESP approach.
//Default:    JOBD = 2.
//This parameter is not relevant for METH = 2.
// 
//TOL is a vector of length 2 containing tolerances: 
//TOL(1) is the tolerance for estimating the rank of matrices.
//If  TOL(1) > 0,  the given value of  TOL(1)  is used as a
//lower bound for the reciprocal condition number.
//Default:    TOL(1) = prod(size(matrix))*epsilon_machine where
//            epsilon_machine is the relative machine precision.
//TOL(2) is the tolerance for estimating the system order.
//If  TOL(2) >= 0,  the estimate is indicated by the index of
//the last singular value greater than or equal to  TOL(2). 
//(Singular values less than  TOL(2) are considered as zero.)
//When  TOL(2) = 0,  then  S*epsilon_machine*sval(1)  is used instead
//TOL(2),  where  sval(1)  is the maximal singular value.
//When  TOL(2) < 0,  the estimate is indicated by the index of the
//singular value that has the largest logarithmic gap to its successor.
//Default:    TOL(2) = -1.
// 
//PRINTW is a select for printing the warning messages.
//PRINTW = 1: print warning messages;
//       = 0: do not print warning messages.
//Default:    PRINTW = 0.
// 
//[R,N,SVAL,RCND] = FINDR(S,Y,U,METH,ALG,JOBD,TOL,PRINTW)  also returns
//the singular values SVAL, used for estimating the order, as well as,
//if meth = 2, the vector RCND of length 2 containing the reciprocal
//condition numbers of the matrices involved in rank decisions or least
//squares solutions.
// 
//[R,N] = FINDR(S,Y)  assumes U = [] and default values for the
//remaining input arguments.
// 
//See also FINDABCD, FINDAC, FINDBD, FINDBDK, ORDER, SIDENT
// 
 
//        V. Sima 18-01-2000.
// 
//        Revisions:
//        V. Sima, July 2000.
//   
 
nin = nargin;
// 
// Assumes one batch only.
batch = 4;
conct = 2;
// 
if nin<8 then
  printw = 0;
end
if nin<7 then
  tol(1:2) = [0,-1] 
end
if nin<6 then jobd = 2; end
if jobd==[] then jobd = 2,end

if nin<5 then alg = 1;end
if alg==[] then alg = 1;end

if nin<4 then meth = 1;end
if meth==[] then meth = 1;end
if nin<3 then
  u = [];
end
// 
if meth==1 then
  [R,n,sval] = sorder(meth,alg,jobd,batch,conct,s,y,u,tol,printw);
else
  [R,n,sval,rcnd] = sorder(meth,alg,jobd,batch,conct,s,y,u,tol,printw);
end
// 
// end findR
endfunction
