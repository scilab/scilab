function M=%s_i_s(varargin)
// Copyright INRIA
[lhs,rhs]=argn(0)
M=varargin(rhs)
N=varargin(rhs-1)//inserted matrix
index=varargin(1) //...
if type(index)==15 then
 if size(index)==2 then
   if type(index(2))==10 then
      if type(index(1))==1 then
      //X(p,q).f = y  : index=list([p,q],f)
      toinsert=varargin(rhs-1)
      Dims=list2vec(index(1));
      M=mlist(["st","dims",index(2)],int32(Dims));
      nmax=prod(Dims);
      li=list();
        for kk=1:nmax-1
          li(kk)=[];
        end
      li(nmax)=toinsert;
      setfield(3,li,M);
      return;
      elseif type(index(1))==15
      //X(p,q).f=z
      Dims=[];
      for kkk=1:lstsize(index(1))
       Dims=[Dims,int32(index(1)(kkk))];
      end
	L=list();
      for lll=1:prod(double(Dims))
      L(lll)=[];
      end
      L($)=varargin(rhs-1);
      M=mlist(["st","dims",index(2)],Dims,L);
      return;
      else
      //X.f.g=y
      M=mlist(["st","dims",index(1)],int32([1,1]),...
        mlist(["st","dims",index(2)],int32([1,1]),varargin(2)));
      return;
      end
   else
   //X.f(p,q)=y : index=list(f,list(p,q))  
   z(index(2)(:))=varargin(2);
   M=mlist(["st","dims",index(1)],int32([1,1]),z);
   return;
   end
 else
 v=varargin(2);
// pause
 if type(index($))==1 then
 w(index($)(:))=v;index($)=null();
 tmp=mlist(["st","dims",index($-1)],int32([1,1]),w);
 else
 tmp=mlist(["st","dims",index($)],int32([1,1]),v);
 end
//pause;
 for k=1:2
 dims=index($-k)(:);
 
 tmp=mlist(["st","dims",index($-k-1)],int32(dims));
 error("not implemented"); 
 end

 M=mlist(["st","dims",index($)],int32([1,1]));
 index($)=null();
 M(index)=tmp;
//pause;
 end
end

if type(index)==10 then
//X.f = y
  M=mlist(["st","dims",varargin(1)],int32([1 1]),N);
  return	
end

//X(i,j,k)=n  hypermatrix
M=mlist(['hm','dims','entries'],size(M),M(:))
dims=M('dims')
v=M('entries');v=v(:)


if rhs-2>size(dims,'*') then
  dims(size(dims,'*')+1:(rhs-2))=0
end
dims1=[]
I=0
iimp=0
for k=rhs-2:-1:1
  ik=varargin(k)
  if type(ik)==2 |type(ik)==129 then // size implicit subscript $...
    ik=horner(ik,dims(k)) // explicit subscript
  elseif type(ik)==4 then // boolean subscript
    ik=find(ik)
  elseif mini(size(ik))<0 then // :
    if dims(k)<>0 then
      ik=1:dims(k)
    else
      iimp=iimp+1
      if iimp<=2 then
	ik=1:size(N,3-iimp)
      else
	ik=1
      end
    end
  end
  dims1(k)=max([max(ik),dims(k)])
  if size(ik,'*')>1 then
    ik=ik(:)
    if size(I,'*')>1 then
      I=(dims1(k)*I).*.ones(ik)+ones(I).*.(ik-1)
    else
      I=dims1(k)*I+ik-1
    end
  else
    I=dims1(k)*I+ik-1
  end
end
//
if prod(dims1)>size(v,'*') then v(prod(dims1))=0,end
v(I+1)=N(:)


while  dims1($)==1 then dims1($)=[],end
select size(dims1,'*')
case 0
  M=v
case 1
  M=v
case 2
  M=matrix(v,dims1(1),dims1(2))
else
  M=mlist(['hm','dims','entries'],matrix(dims1,1,-1),v)
end
endfunction




