function %ar_p(l)
// to provide automatic print of ar objects with armap
// Copyright ENPC 
// 
// We use armp_p as the default function for printing
armap_p(l)

endfunction
function armap(ar,out)
//used for display of armax objects 
//!
[lhs,rhs]=argn(0)
if rhs==1 then out=%io(2),end
deff('[ta]=%cv(x)',['[m,n]=size(x);';
                    'frmt=format();frmt=10**frmt(2)/maxi([1,norm(x)]);';
                    'x=round(frmt*x)/frmt;';
                    't=[];for k=1:m,t=[t;''|''],end;';
                    'ta=t;for k=1:n,';
                    '        aa=string(x(:,k)),';
                    '        for l=1:m,';
                    '           if part(aa(l),1)<>''-'' then ';
                    '               aa(l)='' ''+aa(l),';
                    '           end,';
                    '        end,';
                    '        n=maxi(length(aa)),';
                    '        aa=part(aa+blank,1:n),';
                    '        ta=ta+aa+part(blank,1),';
                    'end;ta=ta+t;'])

// D(x)=Ax + Bu
//-------------
write(out,' ')
if ar(1)<>'ar' then write(%io(2)," This is not an ARMAX");return;end;
//
[ny,vid]=size(ar(2));
if ar(3) == [] then 
	write(out,"  A(z^-1)y= D(z^-1) e(t)");
else 	
	write(out,"  A(z^-1)y=B(z^-1)u + D(z^-1) e(t)");
end 

write(out,"  ");
M=['A','B','D'];
lll=lines();
lll=lll(1);
if ar(3)==[] then I=[1,3];else I=1:3 ;end 
for i=I;
  [a]=ar(i+1)
  [na,lna]=size(a)
  nli=int((lna/na));
  if i==2,nli=int((lna/ar(6)));end;
  blank=[];for k=1:na,blank=[blank;'           '],end
  blank1=part(blank,1:7)
//  blank1([na/2,na/2+1])=['  '+M(i)+'(s)= ']
  blank1([int(na/2)+1])=['  '+M(i)+'(x)=']
  t=blank1;
  nna=na;
  blank1=part(blank,1:6)
  if i==2;nna=ar(6);end
  for j=1:nli;
    ta=a(:,1+(j-1)*nna:j*nna);
    if t==[],t=blank+%cv(ta);else t=t+%cv(ta);end;
    str='x^'+string(j-1)
    if length(str)==3,str=str+' ';end
    if j<>nli;str=str+'+ ';else str=str+'  ';end
//    blank1([na/2,na/2+1])=['        ';str];
    blank1([int(na/2)+1])=[str];
    t=t+blank1
    if length(t(1))>=lll,write(out,t),write(out," ");t=[];end
  end
write(out,t);
write(out," ");
end
write(%io(2),'  e(t)=Sig*w(t); w(t) '+string(ny)+'-dim white noise');
write(out," ");
  [a]=ar(7)
  [na,lna]=size(a)
  blank=[];for k=1:na,blank=[blank;'           '],end
  blank1=part(blank,1:8)
//  blank1([na/2,na/2+1])=['        ';'  Sig=  ']
  blank1([int(na/2)+1])=['  Sig=  ']
  t=blank1;
  ta=a;
  t=t+%cv(ta);
write(out,t);
write(out," ");

endfunction
function armap_p(ar,out)
//used for display of armax objects 
//use Scilan Polynomial matrix display
//!
[lhs,rhs]=argn(0)
if rhs==1 then out=%io(2),end
deff('[ta]=%cv(x)',['[m,n]=size(x);';
                    'frmt=format();frmt=10**frmt(2)/maxi([1,norm(x)]);';
                    'x=round(frmt*x)/frmt;';
                    't=[];for k=1:m,t=[t;''|''],end;';
                    'ta=t;for k=1:n,';
                    '        aa=string(x(:,k)),';
                    '        for l=1:m,';
                    '           if part(aa(l),1)<>''-'' then ';
                    '               aa(l)='' ''+aa(l),';
                    '           end,';
                    '        end,';
                    '        n=maxi(length(aa)),';
                    '        aa=part(aa+blank,1:n),';
                    '        ta=ta+aa+part(blank,1),';
                    'end;ta=ta+t;'])

// D(x)=Ax + Bu
//-------------
write(out,' ')
if ar(1)<>'ar' then write(%io(2)," This is not an ARMAX");return;end;
//
[ny,vid]=size(ar(2));
if ar(3) == [] then 
	write(out,"  A(z^-1)y= D(z^-1) e(t)");
else 	
	write(out,"  A(z^-1)y=B(z^-1)u + D(z^-1) e(t)");
end 

write(out,"  ");
M=['A','B','D'];
lll=lines();
lll=lll(1);
if ar(3)==[] then I=[1,3];else I=1:3 ;end 
A=inv_coeff(ar(2));
//print(out,A);
disp(A,'A(x) =');
write(out,"  ");
if ar(3)<>[] then 
	[mb,nb]=size(ar(3));
	B=inv_coeff(ar(3),(nb/ar('nu'))-1);
	//print(out,B);
	disp(B, 'B(x) =');
	write(out,"  ");
end
D=inv_coeff(ar(4));
//print(out,D);
disp(D, 'D(x)');
write(out," ");
write(%io(2),'  e(t)=Sig*w(t); w(t) '+string(ny)+'-dim white noise');
write(out," ");
  [a]=ar(7)
  [na,lna]=size(a)
  blank=[];for k=1:na,blank=[blank;'           '],end
  blank1=part(blank,1:8)
  //  blank1([na/2,na/2+1])=['        ';'  Sig=  ']
  blank1([int(na/2)+1])=['  Sig=  ']
  t=blank1;
  ta=a;
  t=t+%cv(ta);
write(out,t);
write(out," ");
endfunction
