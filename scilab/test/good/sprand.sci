function a=sprand(m,n,density,typ)
// Copyright INRIA
  
//Sample code to test the validity
//  n=20;m=30;R=[];C=zeros(m,n);
//  for k=1:10000,
//    M=sprand(20,20,0.01);
//    NZ=find(M<>0);
//    R=[R size(NZ,'*')/(m*n)];
//    C(NZ)=C(NZ)+1;
//  end;
  
  //---- Check arguments----------------------------------------------
  if argn(1)<4 then typ='uniform',end
  if type(typ)<>10 then
    error('typ argument of sprand must be ''uniform'' or ''normal''')
  end
  if part(typ,1)<>'u'&part(typ,1)<>'n' then
    error('typ argument of sprand must be ''uniform'' or ''normal''')
  end 
  density=maxi(mini(density,1),0);
  
  nel=m*n*density; //the objective number of non zero elements
  if nel==0 then a=sparse([],[],[m,n]);return,end
  
  //---- generate a sequence of increments----------------------------
  mdist=1/density //the mean distance between to consecutive index
  nel1=(2.2-density)*nel; //generate more increments than requested nnz elements
 // ij=round(1+2*(mdist-1)*rand(1,nel1,'uniform'));
  ij= round(1+grand(nel1,1,'exp',(mdist-1)))
//  disp(ij)
  //---- sum the increments to get the index--------------------------
  ij=cumsum(ij);

  //---- eliminate the index with exceed the maximum matrix index
  ij(find(ij>m*n))=[];nel1=size(ij,'*');
  if nel1==0 then a=sparse([],[],[m,n]);return,end
  //---- generate the row and column indices from the 1D index--------
  
  ij=ind2sub([m,n],ij);
 
  //----  generates the random non zeros elements --------------------
  //according to the requested law and create the sparse matrix
  a=sparse(ij,rand(nel1,1,typ),[m,n]);
endfunction

function [C,R]=testsprand(m,n,density,typ)
  C=uint32(zeros(m,n));//counter of non zero positions
  R=[];//to memorize the actual density
  for k=1:10000
    M=sprand(m,n,density,typ);
    NZ=find(M<>0);
    R=[R size(NZ,'*')/(m*n)];
    C(NZ)=C(NZ)+uint32(1);
  end;
endfunction
