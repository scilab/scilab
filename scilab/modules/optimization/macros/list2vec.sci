function [bigVector,varsizes]=list2vec(li)
//li=list(X1,...Xk) is a list of matrices
//bigVector: big vector [X1(:);...;Xk(:)] (stacking of matrices in li)
//varsizes: k x 2 matrix, with varsiz(i,:)=size(Xi)
// Copyright INRIA
bigVector=[];varsizes=[];
li=aplat(li)
for mati=li
  sm=size(mati);
  varsizes=[varsizes;sm];
  bigVector=[bigVector;matrix(mati,prod(sm),1)];
//  bigVector=[bigVector;mati(:)];        
end
endfunction
