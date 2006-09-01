function txt=%p_string(p)
[m,n]=size(p)
s=stripblanks(varn(p))
txt=emptystr(2*m,n)

for l=1:m
  for k=1:n
    c=coeff(p(l,k))
    knz=find(c<>0)
    if knz==[] then
      txt(2*l-1:2*l,k)=[' ';'0'];
    else
      c=c(knz)
      //special case for i=1
      if isreal(c(1),0) then
	w=real(c(1))
	if w<0 then
	  C(1)=' - '+string(abs(c(1))),
	else
	  C(1)=string(c(1)),
	end
      else
	w=string(c(1));
	if knz(1)<> 1 then w='('+w+')',end
	C(1)=w;
      end
      
      for i=2:size(c,'*'),
	if isreal(c(i),0) then
	  w=real(c(i))
	  if w<0 then
	    C(i)=' - '+string(abs(c(i))),
	  else
	    C(i)=' + '+string(c(i)),
	  end
	else
	  C(i)=' + ('+string(c(i))+')';
	end
      end
      
      if knz(1)>1 then
	C=C+s
      else
	C(2:$)=C(2:$)+s
      end

      i=min(find(knz>2))
      blank=' '
      e=blank(ones(1,i-1))
      if size(knz,'*')>=i then e=[e string(knz(i:$)-1)],end

      lc=cumsum(length(C))
      C=strcat(C)
      E='';
      for i=1:size(c,'*'),E=E+part(' ',1:(lc(i)-length(E)))+e(i);end  
      txt(2*l-1:2*l,k)=[E;C];
    end
  end
end
endfunction
