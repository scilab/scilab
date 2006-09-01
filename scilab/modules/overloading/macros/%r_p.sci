function %r_p(h)
//used to display rational fraction with complex coefficients
//The real case is hard coded
// Copyright INRIA
[m,n]=size(h)
del='!'
blank=' '
if m*n==1 then del=' ',end
height=zeros(m,1)  // to store "height" of each row do be displayed
width=zeros(1,n) // to store "width" of each column do be displayed
T=list() // to store display of each entry of the rational
for k=1:n
  for l=1:m
    tlk=r2str(h(l,k))
    height(l)=max(size(tlk,1),height(l))
    width(k)=max(max(length(tlk)),width(k))
    T($+1)=tlk
  end
end
ll=lines()
k0=0

//manage column display
while %t
  // find how many columns can be displayed simultaneously
  last=find(cumsum(width+2)<ll(1)-3);last=last($);
  if last==[] then last=1,end
  // form display of these columns
  txt=[]
  for l=1:m
    txtr=emptystr(height(l),1)
    for k=1:last
      txtr=txtr+part(blank(ones(height(l),1)),1:2)
      tlk=T(l+(k0+k-1)*m)
      txtr=txtr+[part(tlk,1:width(k));emptystr(height(l)-size(tlk,1),1)]
    end
    txt=[txt;txtr]
  end
  // add matrix delimiter and columns title and display
  nt=size(txt,1)
  txt=part(txt,1:max(length(txt)))
  if k0==0&last==n then
    write(%io(2),del(ones(nt,1))+txt+blank(ones(nt,1))+del(ones(nt,1)))
  else
    if last==1 then
      leg='column '+string(k0+1)
    else
      leg='column '+string(k0+1)+' to '+string(k0+last)
    end
    write(%io(2),[' ';
                  leg;
                  ' '; 
                  del(ones(nt,1))+txt+blank(ones(nt,1))+del(ones(nt,1))])
  end
  width(1:last)=[]
  k0=last
  if width==[] then break,end
end

endfunction
function txt=p2str(p)
//form display of a single polynomial with complex coefficients
lparen='('
rparen=')'
blank=' '
if type(p)==1 then p=poly(p,'s','c'),end
d=degree(p)
v=stripblanks(varn(p))

c=strsubst(string(coeff(p)),'%i','i')
// find coefficients with displays as "0" (deleted later)
kz=find(c=='0') 
// find coefficients with displays as "1"
k1=find(c=='1');if k1(1)==1 then k1(1)=[],end
if k1<>[] then c(k1)=emptystr(1,size(k1,'*')),end
// find coefficients with real AND imaginary part (to be parenthezied)
kc=find(imag(coeff(p))<>0&real(coeff(p))<>0)
w=ones(1,size(kc,'*'))
if kc<>[] then c(kc)=lparen(w)+c(kc)+rparen(w),end
// add formal variable name
c=c+[emptystr(),v(ones(1:d))]

// form exponents
expo1=[' ',' ',string(2:d)]

//delete coeffiecients and exponents corresponding to "0"s
c(kz)=[]
expo1(kz)=[]
if c==[] then
  c='0'
  expo1=emptystr()
end

// change coefficients sign display and adjust length of exponents
le=0
expo=emptystr(c)
for kc=1:size(c,'*')
  if kc>1 then
    if part(c(kc),1)<>'-' then 
      c(kc)=' + '+c(kc),
    else
      c(kc)=' - '+part(c(kc),2:length(c(kc)))
    end
  end
  expo(kc)=part(blank,ones(1,length(c(kc))-le))
  le=length(expo1(kc))
end
expo=expo+expo1(1:size(c,'*'))

//Handle long lines
ll=lines()
nn=size(expo,'*')
txt=[]
count=0
while %t
  L=cumsum(length(expo))
  last=find(L<ll(1)-3);last=last($)
  txt=[txt;
       part(blank,ones(1,count))+strcat(expo(1:last));
       strcat(c(1:last))]
  expo(1:last)=[]
  c(1:last)=[]
  if c==[] then break,end
  count=count+1
end

endfunction
function txt=r2str(h)
//form display of a single rational with complex coefficients
dash='-'
blank=' '

t1=p2str(h('num')) //display of numerator
t2=p2str(h('den')) //display of denominator

//add fraction bar and center 
l1=max(length(t1))
l2=max(length(t2))
if l1>l2 then
  ll1=int((l1-l2)/2)
  ll2=l1-l2-ll1
  b=blank(ones(size(t2,'*'),1))
  txt=[t1;
      part(dash,ones(1,l1));
      part(b,ones(1,ll1))+t2+part(b,ones(1,ll2))]
elseif l1<l2 then
  ll1=int((l2-l1)/2)
  ll2=l2-l1-ll1
  b=blank(ones(size(t1,'*'),1))
  txt=[part(b,ones(1,ll1))+t1+part(b,ones(1,ll2));
       part(dash,ones(1,l2));
       t2]
else
  txt=[t1;part(dash,ones(1,l1));t2]
end
endfunction
