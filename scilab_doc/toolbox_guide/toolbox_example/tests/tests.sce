// Test the macros scilab: foo1
 B=[1 2 3 4; 5 -6 7 8; 9 10 -11 12 ;-13 14 -15 16]
 Xtest=[1;16]
 X=foo1(B)
 if norm(X-Xtest) > %eps then pause,end

// Test the primitive: scisum
 a=[1,2,3];b=[4,5,6]
 c=scisum(a,b)
 if norm(c-(a+b)) > %eps then pause,end

// Test the primitivre: scifoo
 A=[2 -4 6 8 9 -7 10 -3]
 [X,Y]=scifun1(A)
 Xtest=[2 6 8 9 10]
 Ytest=35
 if (norm(X-Xtest) > %eps | norm(Y-Ytest)) then pause,end
 [Z]=scifun1(A)
 Ztest=[2 6 8 9 10]
 if norm(Z-Ztest) > %eps then pause,end
