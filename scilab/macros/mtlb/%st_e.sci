function s=%st_e(varargin)
//extraction from struct
w=varargin($);
if type(varargin(1))==15
//x(i,j,k).f or x.f(i,j,k)
   index=varargin($-1);
   if type(index($))==10 then  //x(i,j,k).f
   f=index($);
   //ijk=list2vec(index(1));
   [indx,I]=convertindex(double(w.dims),index(1)(:));
   // I<->ijk
   all_fields=getfield(1,w);all_fields=all_fields(3:$);
   fieldnum=find(f==all_fields);
   if fieldnum==[] then error("invalid field name");end
   good_list=getfield(2+fieldnum,w);
   if size(I,'*')==1 then
   s=good_list(I)  //list(good_list(I)) ?
   else
   s=list();
   for kk=1:size(I,"*") 
       s(kk)=good_list(I(kk));end
   end
   return;
else //x.f(i,j,k)
   f=index(1);
   li=w(f);
   [indx,I]=convertindex(double(w.dims),index($)(:));
   s=li(I);
   return;
end
end

if type(varargin(1))==10 then
//field  x.f
  fields=getfield(1,w);nu=find(fields(3:$)==varargin(1));
  if nu==[] then
    error(' '+varargin(1)+' is not a valid field name');return;
  end
  s=getfield(nu+2,w);
  if prod(getfield(2,w))==1 then s=s(1);end
  return; 
end
//substruct x(i,j,k...)
sz=size(getfield(1,w),'*');
[indx,I]=convertindex(double(w.dims),varargin(1:$-1));  //indx = v below ??
for kk=1:size(varargin)-1
  if size(varargin(kk),1)~=-1 then 
  v(kk)=size(varargin(kk),'*')
  else v(kk)=double(w.dims(kk));
  end
end

s=mlist(getfield(1,w),int32(v));
for k=3:sz
    ww=getfield(k,w);
//S(2,3).f1=12  -> k=3;I=6;ww(I)=12;s=mlist(["st","dims","f1"],int32([1;1]));
    if prod(v)==1
       setfield(k,ww(I),s); else setfield(k,list(ww(I)),s);
    end
end
endfunction
