function  M=createstruct(index,N)
  //create a struct with field pointed to by index equal to N
if type(index)==15 then
 if size(index)==2 then
   if type(index(2))==10 then
      if type(index(1))==1 then
      //X(p,q).f = y  : index=list([p,q],f)
      Dims=list2vec(index(1));
      if and(size(Dims)==[1 1]) then Dims=[1,Dims];end
      M=mlist(["st","dims",index(2)],int32(Dims));
      nmax=prod(Dims);
      li=list();
        for kk=1:nmax-1
          li(kk)=[];
        end
      li(nmax)=N;
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
      L($)=N;
      M=mlist(["st","dims",index(2)],Dims,L);
      return;
      else
      //X.f.g=y
      M=mlist(["st","dims",index(1)],int32([1,1]),...
        mlist(["st","dims",index(2)],int32([1,1]),N));
      return;
      end
   else
   //X.f(p,q)=y : index=list(f,list(p,q))  
   z(index(2)(:))=N;
   M=mlist(["st","dims",index(1)],int32([1,1]),z);
   return;
   end
 else
   if type(index($-1))==10 then
   M=mlist(["st","dims",index($)],int32([1,1]),N);
   index($)=null();
   else
   dims=[];
   I=index($-1);
   for K=1:lstsize(I)
   dims=[dims,I(K)]
   end
   Li=list();
   Kmax=prod(dims);
   for kl=1:Kmax;
    Li(kl)=[];
   end
   Li($)=N;
   if Kmax>1 then
   M=mlist(["st","dims",index($)],int32(dims),Li);
   else
   M=mlist(["st","dims",index($)],int32(dims),N);
   end
   index($)=null();index($)=null();

   end
   M=createstruct(index,M);
   return;
// pause
 if type(index($))==1 then
 w(index($)(:))=N;index($)=null();
 tmp=mlist(["st","dims",index($-1)],int32([1,1]),w);
 else
 tmp=mlist(["st","dims",index($)],int32([1,1]),N);
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
  M=mlist(["st","dims",index],int32([1 1]),N);
  return	
end
endfunction
