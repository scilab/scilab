function [svm] = svplot(Sl,w)
//SVPLOT singular-value sigma-plot.
// SVM = SVPLOT(SL,W) computes for the system
// SL=(A,B,C,D), the singular values of its transfer function matrix:
//                              -1
//             G(jw) = C(jw*I-A)  B+D
//
//           or
//                                        -1
//             G(exp(jw)) = C(exp(jw)*I-A)  B+D
//
// evaluated over the frequency range specified by W.
// SL is a sylin list (see syslin) representing the system
// [A,B,C,D] in state-space form.
// The i-th column of the output matrix SVM contains the singular
// values of G(exp(jw)) for the i-th frequency value.
// SVM = SVPLOT(Sl) is equivalent to
// SVM = SVPLOT(Sl,LOGSPACE(-3,3))  (continuous) or
// SVM = SVPLOT(Sl,LOGSPACE(-3,PI)) (discrete).
//!
// Copyright INRIA
[nargout,nargin]=argn(0);
//
[a,b,c,d]=abcd(Sl);
// Reduce a to Hessenberg form
[q,a] = hess(a); b = q'*b; c = c*q;
// Compute the singular values of the frequency response
select Sl(7)
case []
   warning('svplot: time domain not defined-->assumed continuous');
if nargin == 1
   w = logspace(-3,3);
end
nf = maxi(size(w)); nsv = mini(size(d)); j = sqrt(-1);
svm(nsv,nf) = 0;
for i = 1:nf
    svm(:,i) = svd(c*((j*w(i)*eye()-a)\b)+d);
end
case 'c'
if nargin == 1
   w = logspace(-3,3);
end
nf = maxi(size(w)); nsv = mini(size(d)); j = sqrt(-1);
svm(nsv,nf) = 0;
for i = 1:nf
    svm(:,i) = svd(c*((j*w(i)*eye()-a)\b)+d);
end
case 'd'
if nargin == 1
   w = logspace(-3,%pi);
end
nf = maxi(size(w)); nsv = mini(size(d)); j = sqrt(-1);
svm(nsv,nf) = 0;
for i = 1:nf
    svm(:,i) = svd(c*((exp(j*w(i))*eye()-a)\b)+d);
  end
else T=Sl('dt');
if nargin == 1
   w = logspace(-3,%pi);
end
nf = maxi(size(w)); nsv = mini(size(d)); j = sqrt(-1);
svm(nsv,nf) = 0;
for i = 1:nf
    svm(:,i) = svd(c*((exp(j*w(i)*T)*eye()-a)\b)+d);
  end
  
end
endfunction
