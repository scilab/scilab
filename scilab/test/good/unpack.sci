function MM=unpack(M,blck_szs)
//utility function (for use with semidef)
// Copyright INRIA
MM=[]
[mM,nM]=size(M)
n=sum(blck_szs)
for j=1:nM
ptr=1
Mu=[]
for ni=blck_szs
  Mui=[]
  for l=1:ni
    Mui(l:ni,l)=M(ptr:ptr+ni-l,j)
    Mui(l,l:ni)=M(ptr:ptr+ni-l,j)'
    ptr=ptr+ni-l+1
  end
  Mu=[Mu;matrix(Mui,ni*ni,1)]
end
  MM=[MM,Mu]
end
endfunction
