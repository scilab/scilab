// Copyright Inria/Enpc
//================================ Matrix of double case ===========================================
N=4;P=20;Nrand=100; 

a = matrix(grand(1,'prm',(1:N*P)'),N,P);

//-----Global sort --------------------------------

[a1,ind]=gsort(a,'g');

if norm(a1- matrix(N*P:-1:1,N,P)) > %eps then pause,end;
if norm(a1- matrix(a(ind),N,P)) > %eps then pause,end;

for i=1:Nrand 
  b = int(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'g');
  if norm(b1- matrix(b(ind),N,P)) > %eps then pause,end;
  if or(b1(1:$-1)-b1(2:$) <0) then pause,end
end 

//increasing values 

[a1,ind]=gsort(a,'g','i');

if norm(a1- matrix(1:N*P,N,P)) > %eps then pause,end;
if norm(a1- matrix(a(ind),N,P)) > %eps then pause,end;

for i=1:100
  b = int(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'g','i');
  if norm(b1- matrix(b(ind),N,P)) > %eps then pause,end;
  if or(b1(1:$-1)-b1(2:$) > 0) then pause,end
end 

//----sort each column of a ('r' means that the row indice is used for sorting)

[a1,ind]=gsort(a,'r');
nc=size(a,'c');
test=[];
for i=1:nc ; 
   test= [test, matrix(a(ind(:,i),i),N,1)];
end 
if norm(a1- test) > %eps then pause,end;
test=[];
for i=1:nc ; 
   test= [test, gsort(a(:,i),'g')];
end 
if norm(a1- test) > %eps then pause,end;

for i=1:Nrand
  b = int(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'r');
  if or( b1(2:$,:)-b1(1:$-1,:) > 0 ) then pause,end 
  test=[];
  for j=1:nc ; 
    test= [test, matrix(b(ind(:,j),j),N,1)];
  end 
  if norm(b1- test) > %eps then pause,end;
end 

//increasing values 

[a1,ind]=gsort(a,'r','i');
nc=size(a,'c');
test=[];
for i=1:nc ; 
  test= [test, matrix(a(ind(:,i),i),N,1)];
end 
if norm(a1- test) > %eps then pause,end;
test=[];
for i=1:nc ; 
   test= [test, gsort(a(:,i),'g','i')];
end 
if norm(a1- test) > %eps then pause,end;

//----sort each row of a ('c' means that the column indice is used for sorting)

[a1,ind]=gsort(a,'c')   ;
nr=size(a,'r');
test=[];
for i=1:nr ; 
   test= [test; matrix(a(i,ind(i,:)),1,P)];
end 
if norm(a1- test) > %eps then pause,end;
test=[];
for i=1:nr ; 
   test= [test; gsort(a(i,:),'g')];
end 
if norm(a1- test) > %eps then pause,end;

for i=1:Nrand
  b = int(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'c');
  if or( b1(:,2:$)-b1(:,1:$-1) > 0 ) then pause,end 
  test=[];
  for j=1:nr ; 
    test= [test; b(j,ind(j,:))];
  end 
  if norm(b1- test) > %eps then pause,end;
end 

//increasing 

[a1,ind]=gsort(a,'c','i')   ;
nr=size(a,'r');
test=[];
for i=1:nr ; 
   test= [test; matrix(a(i,ind(i,:)),1,P)];
end 
if norm(a1- test) > %eps then pause,end;
test=[];
for i=1:nr ; 
   test= [test; gsort(a(i,:),'g','i')];
end 
if norm(a1- test) > %eps then pause,end;

//----sort the rows of a in lexicographic order 
//    i.e a(k,:) < a(l,:) if there's a number j 
//    such that a(k,j) < a(l,j) and a(k,p)=a(l,p) for p in [1,j-1];
//
N1=4;P1=3;
alr=[1,2,2;
     1,2,1;
     1,1,2;
     1,1,1];
// a random permutation 
[ax,perm]=sort(rand(1,N1,'u'));
a=alr(perm,:);

[a1,ind]=gsort(a,'lr');                   

if norm(a1- alr) > %eps then pause,end;
if norm(a1- matrix(a(ind,:),N1,P1)) > %eps then pause,end;

[a2,ind2]=gsort(a*[100;10;1],'g');
if norm(ind2-ind) > %eps then pause,end;

for i=1:Nrand
  vmax=4;
  b = int(vmax*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'lr');
  if norm(b1- b(ind,:)) > %eps then pause,end;
  v= b1*((vmax+1)^[P-1:-1:0])';
  if or( v(2:$)-v(1:$-1) > 0 ) then pause,end 
end 

// increasing 

[a1,ind]=gsort(a,'lr','i');                   
if norm(a1- alr(N1:-1:1,:)) > %eps then pause,end;
if norm(a1- matrix(a(ind,:),N1,P1)) > %eps then pause,end;
[a2,ind2]=gsort(a*[100;10;1],'g','i');
if norm(ind2-ind) > %eps then pause,end;

for i=1:Nrand
  vmax=4;
  b = int(vmax*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'lr','i');
  if norm(b1- b(ind,:)) > %eps then pause,end;
  v= b1*((vmax+1)^[P-1:-1:0])';
  if or( v(2:$)-v(1:$-1) < 0 ) then pause,end 
end 

//----sort the columns of a in lexicographic order 

N1=3;P1=4;
alr=alr';
// a random permutation 
[ax,perm]=sort(rand(1,P1,'u'));
a=alr(:,perm);

[a1,ind]=gsort(a,'lc');                   

if norm(a1- alr) > %eps then pause,end;
if norm(a1- matrix(a(:,ind),N1,P1)) > %eps then pause,end;

[a2,ind2]=gsort([100,10,1]*a,'g');
if norm(ind2-ind) > %eps then pause,end;

for i=1:Nrand
  vmax=4;
  b = int(vmax*rand(N1,P1,'u')); 
  [b1,ind]=gsort(b,'lc');
  if norm(b1- b(:,ind)) > %eps then pause,end;
  v= ((vmax+1)^[N1-1:-1:0])*b1;
  if or( v(2:$)-v(1:$-1) > 0 ) then pause,end 
end 

//increasing 

[a1,ind]=gsort(a,'lc','i');                   

if norm(a1- alr(:,P1:-1:1)) > %eps then pause,end;
if norm(a1- matrix(a(:,ind),N1,P1)) > %eps then pause,end;

[a2,ind2]=gsort([100,10,1]*a,'g','i');
if norm(ind2-ind) > %eps then pause,end;

for i=1:Nrand
  vmax=4;
  b = int(vmax*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'lc','i');
  if norm(b1- b(:,ind)) > %eps then pause,end;
  v= ((vmax+1)^[N-1:-1:0])*b1;
  if or( v(2:$)-v(1:$-1) < 0 ) then pause,end 
end 

a = [1,1,1,1,2,2,3,3,2,2,3,3,4,4,4,4,4,4,4,4,5,5,5,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,9,9,9,7,9,10,..
 10,10,10,11,11,11,12,13,13,13,13,14,14,12,12,14,14,14,14,14,14,15,15,15,15,16,17,18;
3,4,4,5,3,8,4,4,7,8,8,8,5,5,8,9,10,10,9,8,6,10,10,10,10,11,8,8,13,13,9,9,13,13,10,10,13,..
 13,14,15,15,12,14,11,11,15,15,15,15,16,17,14,14,17,17,15,15,13,13,17,17,18,18,19,19,16,..
 16,19,19,19,18,19]';
[b,ind]=gsort(a,'lr','i');
t=b(2:$,:)-b(1:$-1,:);
if or(t(:,1)<0) then pause,end
if or(t(find(t(:,1)==0),2)<0) then pause,end
if or(a(ind,:)<>b) then pause,end

for k=1:30
  p=grand(1,'prm',(1:size(a,1))');
  [b,ind]=gsort(a(p,:),'lr','i');	
  t=b(2:$,:)-b(1:$-1,:);
  if or(t(:,1)<0) then pause,end
  if or(t(find(t(:,1)==0),2)<0) then pause,end
  if or(a(p(ind),:)<>b) then pause,end
end

[b,ind]=gsort(a,'lr','d');
t=b(1:$-1,:)-b(2:$,:);
if or(t(:,1)<0) then pause,end
if or(t(find(t(:,1)==0),2)<0) then pause,end
if or(a(ind,:)<>b) then pause,end

for k=1:30
  p=grand(1,'prm',(1:size(a,1))');
  [b,ind]=gsort(a(p,:),'lr','d');	
  t=b(1:$-1,:)-b(2:$,:);
  if or(t(:,1)<0) then pause,end
  if or(t(find(t(:,1)==0),2)<0) then pause,end
  if or(a(p(ind),:)<>b) then pause,end
end


a=b;
a([10 60],:)=a([60 10],:);
[b,ind]=gsort(a,'lr','i');
t=b(2:$,:)-b(1:$-1,:);
if or(t(:,1)<0) then pause,end
if or(t(find(t(:,1)==0),2)<0) then pause,end
if or(a(ind,:)<>b) then pause,end

[b,ind]=gsort(a,'lr','d');
t=b(1:$-1,:)-b(2:$,:);
if or(t(:,1)<0) then pause,end
if or(t(find(t(:,1)==0),2)<0) then pause,end
if or(a(ind,:)<>b) then pause,end




//================================ Matrix of string case ===========================================
a=int(10*rand(N,P,'u'));

//-----Global sort 

[a1,ind]=gsort(string(a),'g');
[a2,ind2]=gsort(a,'g');
if norm(evstr(a1)- matrix(a(ind),N,P)) > %eps then pause,end;
// a and string(a) are in the same order in the previous example 
if norm(evstr(a1)- a2 ) > %eps then pause,end;

//-- rows 
[a1,ind1]=gsort(string(a),'r');   
[a2,ind2]=gsort(a,'r');
if norm(ind1-ind2) > %eps then pause,end;
// a and string(a) are in the same order in the previous example 
if norm(evstr(a1)- a2 ) > %eps then pause,end;

//--columns 
[a1,ind1]=gsort(string(a),'c')   ;                      
[a2,ind2]=gsort(a,'c');
if norm(ind1-ind2) > %eps then pause,end;
// a and string(a) are in the same order in the previous example 
if norm(evstr(a1)- a2 ) > %eps then pause,end;

//----sort the rows of a in lexicographic order 

N1=4;P1=3;
alr=[1,2,2;
     1,2,1;
     1,1,2;
     1,1,1];
// a random permutation 
[ax,perm]=sort(rand(1,N1,'u'));
a=alr(perm,:);
[a1,ind]=gsort(string(a),'lr') ;        
[a2,ind]=gsort(a,'lr') ;
//
if norm(evstr(a1)- matrix(a(ind,:),N1,P1)) > %eps then pause,end;
// a and string(a) are in the same order in the previous example 
if norm(evstr(a1)- a2 ) > %eps then pause,end;

//----sort the columns of a in lexicographic order 
N1=3;P1=4;
alr=alr';
// a random permutation 
[ax,perm]=sort(rand(1,P1,'u'));
a=alr(:,perm);

[a1,ind]=gsort(string(a),'lc');                   
[a2,ind]=gsort(a,'lc') ;
//
if norm(evstr(a1)- matrix(a(:,ind),N1,P1)) > %eps then pause,end;
// a and string(a) are in the same order in the previous example 
if norm(evstr(a1)- a2 ) > %eps then pause,end;

exit
//================================ Matrix of int32 and uint32 case =======================================
// Copyright Inria
N=4;P=20;Nrand=100; 

a = int32(matrix(grand(1,'prm',(1:N*P)'),N,P));

//-----Global sort --------------------------------

[a1,ind]=gsort(a,'g');

if or(a1- matrix(N*P:-1:1,N,P) <>0) then pause,end;
if or(a1- matrix(a(ind),N,P)  <>0) then pause,end;

for i=1:Nrand 
  b = int32(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'g');
  if or(b1- matrix(b(ind),N,P)<>0)  then pause,end;
  if or(b1(1:$-1)-b1(2:$) <0) then pause,end
end 

//increasing values 

[a1,ind]=gsort(a,'g','i');

if or(a1- matrix(1:N*P,N,P) <>0) then pause,end;
if or(a1- matrix(a(ind),N,P)  <>0) then pause,end;


for i=1:100
  b = int32(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'g','i');
  if or(b1- matrix(b(ind),N,P)<>0)  then pause,end;
  if or(b1(1:$-1)-b1(2:$) > 0) then pause,end
end 

//----sort each column of a ('r' means that the row indice is used for sorting)

[a1,ind]=gsort(a,'r');
nc=size(a,'c');
test=[];
for i=1:nc ; 
   test= [test, matrix(a(ind(:,i),i),N,1)];
end 
if or((a1- test)<>0)  then pause,end;
test=[];
for i=1:nc ; 
   test= [test, gsort(a(:,i),'g')];
end 
if or((a1- test)<>0)  then pause,end;


for i=1:Nrand
  b = int32(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'r');
  if or( b1(2:$,:)-b1(1:$-1,:) > 0 ) then pause,end 
  test=[];
  for j=1:nc ; 
    test= [test, matrix(b(ind(:,j),j),N,1)];
  end 
  if or((b1- test)<>0) then pause,end;
end 

//increasing values 

[a1,ind]=gsort(a,'r','i');
nc=size(a,'c');
test=[];
for i=1:nc ; 
  test= [test, matrix(a(ind(:,i),i),N,1)];
end 
if or((a1- test)<>0)  then pause,end;

test=[];
for i=1:nc ; 
   test= [test, gsort(a(:,i),'g','i')];
end 
if or((a1- test)<>0)  then pause,end;

//----sort each row of a ('c' means that the column indice is used for sorting)

[a1,ind]=gsort(a,'c')   ;
nr=size(a,'r');
test=[];
for i=1:nr ; 
   test= [test; matrix(a(i,ind(i,:)),1,P)];
end 
if or((a1- test)<>0)  then pause,end;

test=[];
for i=1:nr ; 
   test= [test; gsort(a(i,:),'g')];
end 
if or((a1- test)<>0)  then pause,end;


for i=1:Nrand
  b = int32(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'c');
  if or( b1(:,2:$)-b1(:,1:$-1) > 0 ) then pause,end 
  test=[];
  for j=1:nr ; 
    test= [test; b(j,ind(j,:))];
  end 
  if or((b1- test)<>0) then pause,end;
end 

//increasing 

[a1,ind]=gsort(a,'c','i')   ;
nr=size(a,'r');
test=[];
for i=1:nr ; 
   test= [test; matrix(a(i,ind(i,:)),1,P)];
end 
if or((a1- test)<>0)  then pause,end;
test=[];
for i=1:nr ; 
   test= [test; gsort(a(i,:),'g','i')];
end 
if or((a1- test)<>0)  then pause,end;

//----sort the rows of a in lexicographic order 
//    i.e a(k,:) < a(l,:) if there's a number j 
//    such that a(k,j) < a(l,j) and a(k,p)=a(l,p) for p in [1,j-1];
//
N1=4;P1=3;
alr=[1,2,2;
     1,2,1;
     1,1,2;
     1,1,1];
// a random permutation 
[ax,perm]=sort(rand(1,N1,'u'));
a=alr(perm,:);

[a1,ind]=gsort(a,'lr');                   

if or((a1- alr) <>0) then pause,end;
if or((a1- matrix(a(ind,:),N1,P1)) <>0) then pause,end;

[a2,ind2]=gsort(a*[100;10;1],'g');
if or((ind2-ind)<>0) then pause,end;
///////////////////////
for i=1:Nrand
  vmax=4;
  b = int32(vmax*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'lr');
  if or((b1- b(ind,:)) <>0) then pause,end;
  v= double(b1)*((vmax+1)^[P-1:-1:0])';
  if or( v(2:$)-v(1:$-1) > 0 ) then pause,end 
end 

// increasing 

[a1,ind]=gsort(a,'lr','i');                   
if or((a1- alr(N1:-1:1,:)) <>0) then pause,end;

if or((a1- matrix(a(ind,:),N1,P1)) <>0) then pause,end;
[a2,ind2]=gsort(a*[100;10;1],'g','i');
if or((ind2-ind) <>0) then pause,end;

for i=1:Nrand
  vmax=4;
  b = int(vmax*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'lr','i');
  if or((b1- b(ind,:)) <>0) then pause,end;
  v= double(b1)*((vmax+1)^[P-1:-1:0])';
  if or( v(2:$)-v(1:$-1) < 0 ) then pause,end 
end 

//----sort the columns of a in lexicographic order 

N1=3;P1=4;
alr=alr';
// a random permutation 
[ax,perm]=sort(rand(1,P1,'u'));
a=alr(:,perm);

[a1,ind]=gsort(a,'lc');                   

if or((a1- alr) <>0) then pause,end;
if or((a1- matrix(a(:,ind),N1,P1)) <>0) then pause,end;

[a2,ind2]=gsort([100,10,1]*a,'g');
if or((ind2-ind) <>0) then pause,end;

for i=1:Nrand
  vmax=4;
  b = int(vmax*rand(N1,P1,'u')); 
  [b1,ind]=gsort(b,'lc');
  if or((b1- b(:,ind)) <>0) then pause,end;
  v= ((vmax+1)^[N1-1:-1:0])*double(b1);
  if or( v(2:$)-v(1:$-1) > 0 ) then pause,end 
end 

//increasing 

[a1,ind]=gsort(a,'lc','i');                   

if or((a1- alr(:,P1:-1:1)) <>0) then pause,end;
if or((a1- matrix(a(:,ind),N1,P1)) <>0) then pause,end;

[a2,ind2]=gsort([100,10,1]*a,'g','i');
if or((ind2-ind) <>0) then pause,end;

for i=1:Nrand
  vmax=4;
  b = int(vmax*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'lc','i');
  if or((b1- b(:,ind)) <>0) then pause,end;
  v= ((vmax+1)^[N-1:-1:0])*b1;
  if or( v(2:$)-v(1:$-1) < 0 ) then pause,end 
end 



a = int32([1,1,1,1,2,2,3,3,2,2,3,3,4,4,4,4,4,4,4,4,5,5,5,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,9,9,9,7,9,10,..
 10,10,10,11,11,11,12,13,13,13,13,14,14,12,12,14,14,14,14,14,14,15,15,15,15,16,17,18;
3,4,4,5,3,8,4,4,7,8,8,8,5,5,8,9,10,10,9,8,6,10,10,10,10,11,8,8,13,13,9,9,13,13,10,10,13,..
 13,14,15,15,12,14,11,11,15,15,15,15,16,17,14,14,17,17,15,15,13,13,17,17,18,18,19,19,16,..
 16,19,19,19,18,19])';
[b,ind]=gsort(a,'lr','i');
t=b(2:$,:)-b(1:$-1,:);
if or(t(:,1)<0) then pause,end
if or(t(find(t(:,1)==0),2)<0) then pause,end
if or(a(ind,:)<>b) then pause,end

for k=1:30
  p=grand(1,'prm',(1:size(a,1))');
  [b,ind]=gsort(a(p,:),'lr','i');	
  t=b(2:$,:)-b(1:$-1,:);
  if or(t(:,1)<0) then pause,end
  if or(t(find(t(:,1)==0),2)<0) then pause,end
  if or(a(p(ind),:)<>b) then pause,end
end

[b,ind]=gsort(a,'lr','d');
t=b(1:$-1,:)-b(2:$,:);
if or(t(:,1)<0) then pause,end
if or(t(find(t(:,1)==0),2)<0) then pause,end
if or(a(ind,:)<>b) then pause,end

for k=1:30
  p=grand(1,'prm',(1:size(a,1))');
  [b,ind]=gsort(a(p,:),'lr','d');	
  t=b(1:$-1,:)-b(2:$,:);
  if or(t(:,1)<0) then pause,end
  if or(t(find(t(:,1)==0),2)<0) then pause,end
  if or(a(p(ind),:)<>b) then pause,end
end


a=b;
a([10 60],:)=a([60 10],:);
[b,ind]=gsort(a,'lr','i');
t=b(2:$,:)-b(1:$-1,:);
if or(t(:,1)<0) then pause,end
if or(t(find(t(:,1)==0),2)<0) then pause,end
if or(a(ind,:)<>b) then pause,end

[b,ind]=gsort(a,'lr','d');
t=b(1:$-1,:)-b(2:$,:);
if or(t(:,1)<0) then pause,end
if or(t(find(t(:,1)==0),2)<0) then pause,end
if or(a(ind,:)<>b) then pause,end
////////////////////////////////////////////////////////////////////////
// test of gsort for uuint32 matrices:
N=4;P=20;Nrand=100; 

a = uint32(matrix(grand(1,'prm',(1:N*P)'),N,P));

//-----Global sort --------------------------------

[a1,ind]=gsort(a,'g');

if or(a1- matrix(N*P:-1:1,N,P) <>0) then pause,end;
if or(a1- matrix(a(ind),N,P)  <>0) then pause,end;

for i=1:Nrand 
  b = uint32(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'g');
  if or(b1- matrix(b(ind),N,P)<>0)  then pause,end;
  if or(b1(1:$-1)-b1(2:$) <0) then pause,end
end 

//increasing values 

[a1,ind]=gsort(a,'g','i');

if or(a1- matrix(1:N*P,N,P) <>0) then pause,end;
if or(a1- matrix(a(ind),N,P)  <>0) then pause,end;


for i=1:100
  b = uint32(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'g','i');
  if or(b1- matrix(b(ind),N,P)<>0)  then pause,end;
  if or(b1(1:$-1)-b1(2:$) > 0) then pause,end
end 

//----sort each column of a ('r' means that the row indice is used for sorting)

[a1,ind]=gsort(a,'r');
nc=size(a,'c');
test=[];
for i=1:nc ; 
   test= [test, matrix(a(ind(:,i),i),N,1)];
end 
if or((a1- test)<>0)  then pause,end;
test=[];
for i=1:nc ; 
   test= [test, gsort(a(:,i),'g')];
end 
if or((a1- test)<>0)  then pause,end;


for i=1:Nrand
  b = uint32(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'r');
  if or( b1(2:$,:)-b1(1:$-1,:) > 0 ) then pause,end 
  test=[];
  for j=1:nc ; 
    test= [test, matrix(b(ind(:,j),j),N,1)];
  end 
  if or((b1- test)<>0) then pause,end;
end 

//increasing values 

[a1,ind]=gsort(a,'r','i');
nc=size(a,'c');
test=[];
for i=1:nc ; 
  test= [test, matrix(a(ind(:,i),i),N,1)];
end 
if or((a1- test)<>0)  then pause,end;

test=[];
for i=1:nc ; 
   test= [test, gsort(a(:,i),'g','i')];
end 
if or((a1- test)<>0)  then pause,end;

//----sort each row of a ('c' means that the column indice is used for sorting)

[a1,ind]=gsort(a,'c')   ;
nr=size(a,'r');
test=[];
for i=1:nr ; 
   test= [test; matrix(a(i,ind(i,:)),1,P)];
end 
if or((a1- test)<>0)  then pause,end;

test=[];
for i=1:nr ; 
   test= [test; gsort(a(i,:),'g')];
end 
if or((a1- test)<>0)  then pause,end;


for i=1:Nrand
  b = uint32(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'c');
  if or( b1(:,2:$)-b1(:,1:$-1) > 0 ) then pause,end 
  test=[];
  for j=1:nr ; 
    test= [test; b(j,ind(j,:))];
  end 
  if or((b1- test)<>0) then pause,end;
end 

//increasing 

[a1,ind]=gsort(a,'c','i')   ;
nr=size(a,'r');
test=[];
for i=1:nr ; 
   test= [test; matrix(a(i,ind(i,:)),1,P)];
end 
if or((a1- test)<>0)  then pause,end;
test=[];
for i=1:nr ; 
   test= [test; gsort(a(i,:),'g','i')];
end 
if or((a1- test)<>0)  then pause,end;

//----sort the rows of a in lexicographic order 
//    i.e a(k,:) < a(l,:) if there's a number j 
//    such that a(k,j) < a(l,j) and a(k,p)=a(l,p) for p in [1,j-1];
//
N1=4;P1=3;
alr=[1,2,2;
     1,2,1;
     1,1,2;
     1,1,1];
// a random permutation 
[ax,perm]=sort(rand(1,N1,'u'));
a=alr(perm,:);

[a1,ind]=gsort(a,'lr');                   

if or((a1- alr) <>0) then pause,end;
if or((a1- matrix(a(ind,:),N1,P1)) <>0) then pause,end;

[a2,ind2]=gsort(a*[100;10;1],'g');
if or((ind2-ind)<>0) then pause,end;
///////////////////////
for i=1:Nrand
  vmax=4;
  b = uint32(vmax*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'lr');
  if or((b1- b(ind,:)) <>0) then pause,end;
  v= double(b1)*((vmax+1)^[P-1:-1:0])';
  if or( v(2:$)-v(1:$-1) > 0 ) then pause,end 
end 

// increasing 

[a1,ind]=gsort(a,'lr','i');                   
if or((a1- alr(N1:-1:1,:)) <>0) then pause,end;

if or((a1- matrix(a(ind,:),N1,P1)) <>0) then pause,end;
[a2,ind2]=gsort(a*[100;10;1],'g','i');
if or((ind2-ind) <>0) then pause,end;

for i=1:Nrand
  vmax=4;
  b = int(vmax*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'lr','i');
  if or((b1- b(ind,:)) <>0) then pause,end;
  v= double(b1)*((vmax+1)^[P-1:-1:0])';
  if or( v(2:$)-v(1:$-1) < 0 ) then pause,end 
end 

//----sort the columns of a in lexicographic order 

N1=3;P1=4;
alr=alr';
// a random permutation 
[ax,perm]=sort(rand(1,P1,'u'));
a=alr(:,perm);

[a1,ind]=gsort(a,'lc');                   

if or((a1- alr) <>0) then pause,end;
if or((a1- matrix(a(:,ind),N1,P1)) <>0) then pause,end;

[a2,ind2]=gsort([100,10,1]*a,'g');
if or((ind2-ind) <>0) then pause,end;

for i=1:Nrand
  vmax=4;
  b = int(vmax*rand(N1,P1,'u')); 
  [b1,ind]=gsort(b,'lc');
  if or((b1- b(:,ind)) <>0) then pause,end;
  v= ((vmax+1)^[N1-1:-1:0])*double(b1);
  if or( v(2:$)-v(1:$-1) > 0 ) then pause,end 
end 

//increasing 

[a1,ind]=gsort(a,'lc','i');                   

if or((a1- alr(:,P1:-1:1)) <>0) then pause,end;
if or((a1- matrix(a(:,ind),N1,P1)) <>0) then pause,end;

[a2,ind2]=gsort([100,10,1]*a,'g','i');
if or((ind2-ind) <>0) then pause,end;

for i=1:Nrand
  vmax=4;
  b = int(vmax*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'lc','i');
  if or((b1- b(:,ind)) <>0) then pause,end;
  v= ((vmax+1)^[N-1:-1:0])*b1;
  if or( v(2:$)-v(1:$-1) < 0 ) then pause,end 
end 



a = uint32([1,1,1,1,2,2,3,3,2,2,3,3,4,4,4,4,4,4,4,4,5,5,5,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,9,9,9,7,9,10,..
 10,10,10,11,11,11,12,13,13,13,13,14,14,12,12,14,14,14,14,14,14,15,15,15,15,16,17,18;
3,4,4,5,3,8,4,4,7,8,8,8,5,5,8,9,10,10,9,8,6,10,10,10,10,11,8,8,13,13,9,9,13,13,10,10,13,..
 13,14,15,15,12,14,11,11,15,15,15,15,16,17,14,14,17,17,15,15,13,13,17,17,18,18,19,19,16,..
 16,19,19,19,18,19]-5)';
[b,ind]=gsort(a,'lr','i');
t=b(2:$,:)<b(1:$-1,:);
if or(t(:,1)) then pause,end
if or(t(b(2:$,1)==b(1:$-1,1),2)) then pause,end
if or(a(ind,:)<>b) then pause,end

for k=1:30
  p=grand(1,'prm',(1:size(a,1))');
  [b,ind]=gsort(a(p,:),'lr','i');
  t=b(2:$,:)<b(1:$-1,:);
  if or(t(:,1)) then pause,end
  if or(t(b(2:$,1)==b(1:$-1,1),2)) then pause,end
  if or(a(p(ind),:)<>b) then pause,end
end

[b,ind]=gsort(a,'lr','d');
t=b(2:$,:)>b(1:$-1,:);
if or(t(:,1)) then pause,end
if or(t(b(2:$,1)==b(1:$-1,1),2)) then pause,end
if or(a(ind,:)<>b) then pause,end


for k=1:30
  p=grand(1,'prm',(1:size(a,1))');
  [b,ind]=gsort(a(p,:),'lr','d');
  t=b(2:$,:)>b(1:$-1,:);
  if or(t(:,1)) then pause,end
  if or(t(b(2:$,1)==b(1:$-1,1),2)) then pause,end
  if or(a(p(ind),:)<>b) then pause,end
end


a=b;
a([10 60],:)=a([60 10],:);
[b,ind]=gsort(a,'lr','i');
t=b(2:$,:)<b(1:$-1,:);
if or(t(:,1)) then pause,end
if or(t(b(2:$,1)==b(1:$-1,1),2)) then pause,end
if or(a(ind,:)<>b) then pause,end



[b,ind]=gsort(a,'lr','d');
t=b(2:$,:)>b(1:$-1,:);
if or(t(:,1)) then pause,end
if or(t(b(2:$,1)==b(1:$-1,1),2)) then pause,end
if or(a(ind,:)<>b) then pause,end

//================================ Matrix of int16 and uint16 case =======================================
// Copyright Inria
// test of gsort for int16 matrices:
N=4;P=20;Nrand=100; 

a = int16(matrix(grand(1,'prm',(1:N*P)'),N,P));

//-----Global sort --------------------------------

[a1,ind]=gsort(a,'g');

if or(a1- matrix(N*P:-1:1,N,P) <>0) then pause,end;
if or(a1- matrix(a(ind),N,P)  <>0) then pause,end;

for i=1:Nrand 
  b = int16(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'g');
  if or(b1- matrix(b(ind),N,P)<>0)  then pause,end;
  if or(b1(1:$-1)-b1(2:$) <0) then pause,end
end 

//increasing values 

[a1,ind]=gsort(a,'g','i');

if or(a1- matrix(1:N*P,N,P) <>0) then pause,end;
if or(a1- matrix(a(ind),N,P)  <>0) then pause,end;


for i=1:100
  b = int16(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'g','i');
  if or(b1- matrix(b(ind),N,P)<>0)  then pause,end;
  if or(b1(1:$-1)-b1(2:$) > 0) then pause,end
end 

//----sort each column of a ('r' means that the row indice is used for sorting)

[a1,ind]=gsort(a,'r');
nc=size(a,'c');
test=[];
for i=1:nc ; 
   test= [test, matrix(a(ind(:,i),i),N,1)];
end 
if or((a1- test)<>0)  then pause,end;
test=[];
for i=1:nc ; 
   test= [test, gsort(a(:,i),'g')];
end 
if or((a1- test)<>0)  then pause,end;


for i=1:Nrand
  b = int16(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'r');
  if or( b1(2:$,:)-b1(1:$-1,:) > 0 ) then pause,end 
  test=[];
  for j=1:nc ; 
    test= [test, matrix(b(ind(:,j),j),N,1)];
  end 
  if or((b1- test)<>0) then pause,end;
end 

//increasing values 

[a1,ind]=gsort(a,'r','i');
nc=size(a,'c');
test=[];
for i=1:nc ; 
  test= [test, matrix(a(ind(:,i),i),N,1)];
end 
if or((a1- test)<>0)  then pause,end;

test=[];
for i=1:nc ; 
   test= [test, gsort(a(:,i),'g','i')];
end 
if or((a1- test)<>0)  then pause,end;

//----sort each row of a ('c' means that the column indice is used for sorting)

[a1,ind]=gsort(a,'c')   ;
nr=size(a,'r');
test=[];
for i=1:nr ; 
   test= [test; matrix(a(i,ind(i,:)),1,P)];
end 
if or((a1- test)<>0)  then pause,end;

test=[];
for i=1:nr ; 
   test= [test; gsort(a(i,:),'g')];
end 
if or((a1- test)<>0)  then pause,end;


for i=1:Nrand
  b = int16(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'c');
  if or( b1(:,2:$)-b1(:,1:$-1) > 0 ) then pause,end 
  test=[];
  for j=1:nr ; 
    test= [test; b(j,ind(j,:))];
  end 
  if or((b1- test)<>0) then pause,end;
end 

//increasing 

[a1,ind]=gsort(a,'c','i')   ;
nr=size(a,'r');
test=[];
for i=1:nr ; 
   test= [test; matrix(a(i,ind(i,:)),1,P)];
end 
if or((a1- test)<>0)  then pause,end;
test=[];
for i=1:nr ; 
   test= [test; gsort(a(i,:),'g','i')];
end 
if or((a1- test)<>0)  then pause,end;

//----sort the rows of a in lexicographic order 
//    i.e a(k,:) < a(l,:) if there's a number j 
//    such that a(k,j) < a(l,j) and a(k,p)=a(l,p) for p in [1,j-1];
//
N1=4;P1=3;
alr=[1,2,2;
     1,2,1;
     1,1,2;
     1,1,1];
// a random permutation 
[ax,perm]=sort(rand(1,N1,'u'));
a=alr(perm,:);

[a1,ind]=gsort(a,'lr');                   

if or((a1- alr) <>0) then pause,end;
if or((a1- matrix(a(ind,:),N1,P1)) <>0) then pause,end;

[a2,ind2]=gsort(a*[100;10;1],'g');
if or((ind2-ind)<>0) then pause,end;
///////////////////////
for i=1:Nrand
  vmax=4;
  b = int16(vmax*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'lr');
  if or((b1- b(ind,:)) <>0) then pause,end;
  v= double(b1)*((vmax+1)^[P-1:-1:0])';
  if or( v(2:$)-v(1:$-1) > 0 ) then pause,end 
end 

// increasing 

[a1,ind]=gsort(a,'lr','i');                   
if or((a1- alr(N1:-1:1,:)) <>0) then pause,end;

if or((a1- matrix(a(ind,:),N1,P1)) <>0) then pause,end;
[a2,ind2]=gsort(a*[100;10;1],'g','i');
if or((ind2-ind) <>0) then pause,end;

for i=1:Nrand
  vmax=4;
  b = int(vmax*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'lr','i');
  if or((b1- b(ind,:)) <>0) then pause,end;
  v= double(b1)*((vmax+1)^[P-1:-1:0])';
  if or( v(2:$)-v(1:$-1) < 0 ) then pause,end 
end 

//----sort the columns of a in lexicographic order 

N1=3;P1=4;
alr=alr';
// a random permutation 
[ax,perm]=sort(rand(1,P1,'u'));
a=alr(:,perm);

[a1,ind]=gsort(a,'lc');                   

if or((a1- alr) <>0) then pause,end;
if or((a1- matrix(a(:,ind),N1,P1)) <>0) then pause,end;

[a2,ind2]=gsort([100,10,1]*a,'g');
if or((ind2-ind) <>0) then pause,end;

for i=1:Nrand
  vmax=4;
  b = int(vmax*rand(N1,P1,'u')); 
  [b1,ind]=gsort(b,'lc');
  if or((b1- b(:,ind)) <>0) then pause,end;
  v= ((vmax+1)^[N1-1:-1:0])*double(b1);
  if or( v(2:$)-v(1:$-1) > 0 ) then pause,end 
end 

//increasing 

[a1,ind]=gsort(a,'lc','i');                   

if or((a1- alr(:,P1:-1:1)) <>0) then pause,end;
if or((a1- matrix(a(:,ind),N1,P1)) <>0) then pause,end;

[a2,ind2]=gsort([100,10,1]*a,'g','i');
if or((ind2-ind) <>0) then pause,end;

for i=1:Nrand
  vmax=4;
  b = int(vmax*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'lc','i');
  if or((b1- b(:,ind)) <>0) then pause,end;
  v= ((vmax+1)^[N-1:-1:0])*b1;
  if or( v(2:$)-v(1:$-1) < 0 ) then pause,end 
end 



a = int16([1,1,1,1,2,2,3,3,2,2,3,3,4,4,4,4,4,4,4,4,5,5,5,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,9,9,9,7,9,10,..
 10,10,10,11,11,11,12,13,13,13,13,14,14,12,12,14,14,14,14,14,14,15,15,15,15,16,17,18;
3,4,4,5,3,8,4,4,7,8,8,8,5,5,8,9,10,10,9,8,6,10,10,10,10,11,8,8,13,13,9,9,13,13,10,10,13,..
 13,14,15,15,12,14,11,11,15,15,15,15,16,17,14,14,17,17,15,15,13,13,17,17,18,18,19,19,16,..
 16,19,19,19,18,19])';
[b,ind]=gsort(a,'lr','i');
t=b(2:$,:)-b(1:$-1,:);
if or(t(:,1)<0) then pause,end
if or(t(find(t(:,1)==0),2)<0) then pause,end
if or(a(ind,:)<>b) then pause,end

for k=1:30
  p=grand(1,'prm',(1:size(a,1))');
  [b,ind]=gsort(a(p,:),'lr','i');	
  t=b(2:$,:)-b(1:$-1,:);
  if or(t(:,1)<0) then pause,end
  if or(t(find(t(:,1)==0),2)<0) then pause,end
  if or(a(p(ind),:)<>b) then pause,end
end

[b,ind]=gsort(a,'lr','d');
t=b(1:$-1,:)-b(2:$,:);
if or(t(:,1)<0) then pause,end
if or(t(find(t(:,1)==0),2)<0) then pause,end
if or(a(ind,:)<>b) then pause,end

for k=1:30
  p=grand(1,'prm',(1:size(a,1))');
  [b,ind]=gsort(a(p,:),'lr','d');	
  t=b(1:$-1,:)-b(2:$,:);
  if or(t(:,1)<0) then pause,end
  if or(t(find(t(:,1)==0),2)<0) then pause,end
  if or(a(p(ind),:)<>b) then pause,end
end


a=b;
a([10 60],:)=a([60 10],:);
[b,ind]=gsort(a,'lr','i');
t=b(2:$,:)-b(1:$-1,:);
if or(t(:,1)<0) then pause,end
if or(t(find(t(:,1)==0),2)<0) then pause,end
if or(a(ind,:)<>b) then pause,end

[b,ind]=gsort(a,'lr','d');
t=b(1:$-1,:)-b(2:$,:);
if or(t(:,1)<0) then pause,end
if or(t(find(t(:,1)==0),2)<0) then pause,end
if or(a(ind,:)<>b) then pause,end
////////////////////////////////////////////////////////////////////////
// test of gsort for uuint16 matrices:
N=4;P=20;Nrand=100; 

a = uint16(matrix(grand(1,'prm',(1:N*P)'),N,P));

//-----Global sort --------------------------------

[a1,ind]=gsort(a,'g');

if or(a1- matrix(N*P:-1:1,N,P) <>0) then pause,end;
if or(a1- matrix(a(ind),N,P)  <>0) then pause,end;

for i=1:Nrand 
  b = uint16(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'g');
  if or(b1- matrix(b(ind),N,P)<>0)  then pause,end;
  if or(b1(1:$-1)-b1(2:$) <0) then pause,end
end 

//increasing values 

[a1,ind]=gsort(a,'g','i');

if or(a1- matrix(1:N*P,N,P) <>0) then pause,end;
if or(a1- matrix(a(ind),N,P)  <>0) then pause,end;


for i=1:100
  b = uint16(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'g','i');
  if or(b1- matrix(b(ind),N,P)<>0)  then pause,end;
  if or(b1(1:$-1)-b1(2:$) > 0) then pause,end
end 

//----sort each column of a ('r' means that the row indice is used for sorting)

[a1,ind]=gsort(a,'r');
nc=size(a,'c');
test=[];
for i=1:nc ; 
   test= [test, matrix(a(ind(:,i),i),N,1)];
end 
if or((a1- test)<>0)  then pause,end;
test=[];
for i=1:nc ; 
   test= [test, gsort(a(:,i),'g')];
end 
if or((a1- test)<>0)  then pause,end;


for i=1:Nrand
  b = uint16(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'r');
  if or( b1(2:$,:)-b1(1:$-1,:) > 0 ) then pause,end 
  test=[];
  for j=1:nc ; 
    test= [test, matrix(b(ind(:,j),j),N,1)];
  end 
  if or((b1- test)<>0) then pause,end;
end 

//increasing values 

[a1,ind]=gsort(a,'r','i');
nc=size(a,'c');
test=[];
for i=1:nc ; 
  test= [test, matrix(a(ind(:,i),i),N,1)];
end 
if or((a1- test)<>0)  then pause,end;

test=[];
for i=1:nc ; 
   test= [test, gsort(a(:,i),'g','i')];
end 
if or((a1- test)<>0)  then pause,end;

//----sort each row of a ('c' means that the column indice is used for sorting)

[a1,ind]=gsort(a,'c')   ;
nr=size(a,'r');
test=[];
for i=1:nr ; 
   test= [test; matrix(a(i,ind(i,:)),1,P)];
end 
if or((a1- test)<>0)  then pause,end;

test=[];
for i=1:nr ; 
   test= [test; gsort(a(i,:),'g')];
end 
if or((a1- test)<>0)  then pause,end;


for i=1:Nrand
  b = uint16(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'c');
  if or( b1(:,2:$)-b1(:,1:$-1) > 0 ) then pause,end 
  test=[];
  for j=1:nr ; 
    test= [test; b(j,ind(j,:))];
  end 
  if or((b1- test)<>0) then pause,end;
end 

//increasing 

[a1,ind]=gsort(a,'c','i')   ;
nr=size(a,'r');
test=[];
for i=1:nr ; 
   test= [test; matrix(a(i,ind(i,:)),1,P)];
end 
if or((a1- test)<>0)  then pause,end;
test=[];
for i=1:nr ; 
   test= [test; gsort(a(i,:),'g','i')];
end 
if or((a1- test)<>0)  then pause,end;

//----sort the rows of a in lexicographic order 
//    i.e a(k,:) < a(l,:) if there's a number j 
//    such that a(k,j) < a(l,j) and a(k,p)=a(l,p) for p in [1,j-1];
//
N1=4;P1=3;
alr=[1,2,2;
     1,2,1;
     1,1,2;
     1,1,1];
// a random permutation 
[ax,perm]=sort(rand(1,N1,'u'));
a=alr(perm,:);

[a1,ind]=gsort(a,'lr');                   

if or((a1- alr) <>0) then pause,end;
if or((a1- matrix(a(ind,:),N1,P1)) <>0) then pause,end;

[a2,ind2]=gsort(a*[100;10;1],'g');
if or((ind2-ind)<>0) then pause,end;
///////////////////////
for i=1:Nrand
  vmax=4;
  b = uint16(vmax*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'lr');
  if or((b1- b(ind,:)) <>0) then pause,end;
  v= double(b1)*((vmax+1)^[P-1:-1:0])';
  if or( v(2:$)-v(1:$-1) > 0 ) then pause,end 
end 

// increasing 

[a1,ind]=gsort(a,'lr','i');                   
if or((a1- alr(N1:-1:1,:)) <>0) then pause,end;

if or((a1- matrix(a(ind,:),N1,P1)) <>0) then pause,end;
[a2,ind2]=gsort(a*[100;10;1],'g','i');
if or((ind2-ind) <>0) then pause,end;

for i=1:Nrand
  vmax=4;
  b = int(vmax*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'lr','i');
  if or((b1- b(ind,:)) <>0) then pause,end;
  v= double(b1)*((vmax+1)^[P-1:-1:0])';
  if or( v(2:$)-v(1:$-1) < 0 ) then pause,end 
end 

//----sort the columns of a in lexicographic order 

N1=3;P1=4;
alr=alr';
// a random permutation 
[ax,perm]=sort(rand(1,P1,'u'));
a=alr(:,perm);

[a1,ind]=gsort(a,'lc');                   

if or((a1- alr) <>0) then pause,end;
if or((a1- matrix(a(:,ind),N1,P1)) <>0) then pause,end;

[a2,ind2]=gsort([100,10,1]*a,'g');
if or((ind2-ind) <>0) then pause,end;

for i=1:Nrand
  vmax=4;
  b = int(vmax*rand(N1,P1,'u')); 
  [b1,ind]=gsort(b,'lc');
  if or((b1- b(:,ind)) <>0) then pause,end;
  v= ((vmax+1)^[N1-1:-1:0])*double(b1);
  if or( v(2:$)-v(1:$-1) > 0 ) then pause,end 
end 

//increasing 

[a1,ind]=gsort(a,'lc','i');                   

if or((a1- alr(:,P1:-1:1)) <>0) then pause,end;
if or((a1- matrix(a(:,ind),N1,P1)) <>0) then pause,end;

[a2,ind2]=gsort([100,10,1]*a,'g','i');
if or((ind2-ind) <>0) then pause,end;

for i=1:Nrand
  vmax=4;
  b = int(vmax*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'lc','i');
  if or((b1- b(:,ind)) <>0) then pause,end;
  v= ((vmax+1)^[N-1:-1:0])*b1;
  if or( v(2:$)-v(1:$-1) < 0 ) then pause,end 
end 



a = uint16([1,1,1,1,2,2,3,3,2,2,3,3,4,4,4,4,4,4,4,4,5,5,5,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,9,9,9,7,9,10,..
 10,10,10,11,11,11,12,13,13,13,13,14,14,12,12,14,14,14,14,14,14,15,15,15,15,16,17,18;
3,4,4,5,3,8,4,4,7,8,8,8,5,5,8,9,10,10,9,8,6,10,10,10,10,11,8,8,13,13,9,9,13,13,10,10,13,..
 13,14,15,15,12,14,11,11,15,15,15,15,16,17,14,14,17,17,15,15,13,13,17,17,18,18,19,19,16,..
 16,19,19,19,18,19]-5)';
[b,ind]=gsort(a,'lr','i');
t=b(2:$,:)<b(1:$-1,:);
if or(t(:,1)) then pause,end
if or(t(b(2:$,1)==b(1:$-1,1),2)) then pause,end
if or(a(ind,:)<>b) then pause,end

for k=1:30
  p=grand(1,'prm',(1:size(a,1))');
  [b,ind]=gsort(a(p,:),'lr','i');
  t=b(2:$,:)<b(1:$-1,:);
  if or(t(:,1)) then pause,end
  if or(t(b(2:$,1)==b(1:$-1,1),2)) then pause,end
  if or(a(p(ind),:)<>b) then pause,end
end

[b,ind]=gsort(a,'lr','d');
t=b(2:$,:)>b(1:$-1,:);
if or(t(:,1)) then pause,end
if or(t(b(2:$,1)==b(1:$-1,1),2)) then pause,end
if or(a(ind,:)<>b) then pause,end


for k=1:30
  p=grand(1,'prm',(1:size(a,1))');
  [b,ind]=gsort(a(p,:),'lr','d');
  t=b(2:$,:)>b(1:$-1,:);
  if or(t(:,1)) then pause,end
  if or(t(b(2:$,1)==b(1:$-1,1),2)) then pause,end
  if or(a(p(ind),:)<>b) then pause,end
end


a=b;
a([10 60],:)=a([60 10],:);
[b,ind]=gsort(a,'lr','i');
t=b(2:$,:)<b(1:$-1,:);
if or(t(:,1)) then pause,end
if or(t(b(2:$,1)==b(1:$-1,1),2)) then pause,end
if or(a(ind,:)<>b) then pause,end



[b,ind]=gsort(a,'lr','d');
t=b(2:$,:)>b(1:$-1,:);
if or(t(:,1)) then pause,end
if or(t(b(2:$,1)==b(1:$-1,1),2)) then pause,end
if or(a(ind,:)<>b) then pause,end
//================================ Matrix of int8 and uint8 case =======================================
// Copyright Inria
// test of gsort for int8 matrices:
N=4;P=20;Nrand=100; 

a = int8(matrix(grand(1,'prm',(1:N*P)'),N,P));

//-----Global sort --------------------------------

[a1,ind]=gsort(a,'g');

if or(a1- matrix(N*P:-1:1,N,P) <>0) then pause,end;
if or(a1- matrix(a(ind),N,P)  <>0) then pause,end;

for i=1:Nrand 
  b = int8(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'g');
  if or(b1- matrix(b(ind),N,P)<>0)  then pause,end;
  if or(b1(1:$-1)-b1(2:$) <0) then pause,end
end 

//increasing values 

[a1,ind]=gsort(a,'g','i');

if or(a1- matrix(1:N*P,N,P) <>0) then pause,end;
if or(a1- matrix(a(ind),N,P)  <>0) then pause,end;


for i=1:100
  b = int8(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'g','i');
  if or(b1- matrix(b(ind),N,P)<>0)  then pause,end;
  if or(b1(1:$-1)-b1(2:$) > 0) then pause,end
end 

//----sort each column of a ('r' means that the row indice is used for sorting)

[a1,ind]=gsort(a,'r');
nc=size(a,'c');
test=[];
for i=1:nc ; 
   test= [test, matrix(a(ind(:,i),i),N,1)];
end 
if or((a1- test)<>0)  then pause,end;
test=[];
for i=1:nc ; 
   test= [test, gsort(a(:,i),'g')];
end 
if or((a1- test)<>0)  then pause,end;


for i=1:Nrand
  b = int8(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'r');
  if or( b1(2:$,:)-b1(1:$-1,:) > 0 ) then pause,end 
  test=[];
  for j=1:nc ; 
    test= [test, matrix(b(ind(:,j),j),N,1)];
  end 
  if or((b1- test)<>0) then pause,end;
end 

//increasing values 

[a1,ind]=gsort(a,'r','i');
nc=size(a,'c');
test=[];
for i=1:nc ; 
  test= [test, matrix(a(ind(:,i),i),N,1)];
end 
if or((a1- test)<>0)  then pause,end;

test=[];
for i=1:nc ; 
   test= [test, gsort(a(:,i),'g','i')];
end 
if or((a1- test)<>0)  then pause,end;

//----sort each row of a ('c' means that the column indice is used for sorting)

[a1,ind]=gsort(a,'c')   ;
nr=size(a,'r');
test=[];
for i=1:nr ; 
   test= [test; matrix(a(i,ind(i,:)),1,P)];
end 
if or((a1- test)<>0)  then pause,end;

test=[];
for i=1:nr ; 
   test= [test; gsort(a(i,:),'g')];
end 
if or((a1- test)<>0)  then pause,end;


for i=1:Nrand
  b = int8(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'c');
  if or( b1(:,2:$)-b1(:,1:$-1) > 0 ) then pause,end 
  test=[];
  for j=1:nr ; 
    test= [test; b(j,ind(j,:))];
  end 
  if or((b1- test)<>0) then pause,end;
end 

//increasing 

[a1,ind]=gsort(a,'c','i')   ;
nr=size(a,'r');
test=[];
for i=1:nr ; 
   test= [test; matrix(a(i,ind(i,:)),1,P)];
end 
if or((a1- test)<>0)  then pause,end;
test=[];
for i=1:nr ; 
   test= [test; gsort(a(i,:),'g','i')];
end 
if or((a1- test)<>0)  then pause,end;

//----sort the rows of a in lexicographic order 
//    i.e a(k,:) < a(l,:) if there's a number j 
//    such that a(k,j) < a(l,j) and a(k,p)=a(l,p) for p in [1,j-1];
//
N1=4;P1=3;
alr=[1,2,2;
     1,2,1;
     1,1,2;
     1,1,1];
// a random permutation 
[ax,perm]=sort(rand(1,N1,'u'));
a=alr(perm,:);

[a1,ind]=gsort(a,'lr');                   

if or((a1- alr) <>0) then pause,end;
if or((a1- matrix(a(ind,:),N1,P1)) <>0) then pause,end;

[a2,ind2]=gsort(a*[100;10;1],'g');
if or((ind2-ind)<>0) then pause,end;
///////////////////////
for i=1:Nrand
  vmax=4;
  b = int8(vmax*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'lr');
  if or((b1- b(ind,:)) <>0) then pause,end;
  v= double(b1)*((vmax+1)^[P-1:-1:0])';
  if or( v(2:$)-v(1:$-1) > 0 ) then pause,end 
end 

// increasing 

[a1,ind]=gsort(a,'lr','i');                   
if or((a1- alr(N1:-1:1,:)) <>0) then pause,end;

if or((a1- matrix(a(ind,:),N1,P1)) <>0) then pause,end;
[a2,ind2]=gsort(a*[100;10;1],'g','i');
if or((ind2-ind) <>0) then pause,end;

for i=1:Nrand
  vmax=4;
  b = int(vmax*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'lr','i');
  if or((b1- b(ind,:)) <>0) then pause,end;
  v= double(b1)*((vmax+1)^[P-1:-1:0])';
  if or( v(2:$)-v(1:$-1) < 0 ) then pause,end 
end 

//----sort the columns of a in lexicographic order 

N1=3;P1=4;
alr=alr';
// a random permutation 
[ax,perm]=sort(rand(1,P1,'u'));
a=alr(:,perm);

[a1,ind]=gsort(a,'lc');                   

if or((a1- alr) <>0) then pause,end;
if or((a1- matrix(a(:,ind),N1,P1)) <>0) then pause,end;

[a2,ind2]=gsort([100,10,1]*a,'g');
if or((ind2-ind) <>0) then pause,end;

for i=1:Nrand
  vmax=4;
  b = int(vmax*rand(N1,P1,'u')); 
  [b1,ind]=gsort(b,'lc');
  if or((b1- b(:,ind)) <>0) then pause,end;
  v= ((vmax+1)^[N1-1:-1:0])*double(b1);
  if or( v(2:$)-v(1:$-1) > 0 ) then pause,end 
end 

//increasing 

[a1,ind]=gsort(a,'lc','i');                   

if or((a1- alr(:,P1:-1:1)) <>0) then pause,end;
if or((a1- matrix(a(:,ind),N1,P1)) <>0) then pause,end;

[a2,ind2]=gsort([100,10,1]*a,'g','i');
if or((ind2-ind) <>0) then pause,end;

for i=1:Nrand
  vmax=4;
  b = int(vmax*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'lc','i');
  if or((b1- b(:,ind)) <>0) then pause,end;
  v= ((vmax+1)^[N-1:-1:0])*b1;
  if or( v(2:$)-v(1:$-1) < 0 ) then pause,end 
end 



a = int8([1,1,1,1,2,2,3,3,2,2,3,3,4,4,4,4,4,4,4,4,5,5,5,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,9,9,9,7,9,10,..
 10,10,10,11,11,11,12,13,13,13,13,14,14,12,12,14,14,14,14,14,14,15,15,15,15,16,17,18;
3,4,4,5,3,8,4,4,7,8,8,8,5,5,8,9,10,10,9,8,6,10,10,10,10,11,8,8,13,13,9,9,13,13,10,10,13,..
 13,14,15,15,12,14,11,11,15,15,15,15,16,17,14,14,17,17,15,15,13,13,17,17,18,18,19,19,16,..
 16,19,19,19,18,19])';
[b,ind]=gsort(a,'lr','i');
t=b(2:$,:)-b(1:$-1,:);
if or(t(:,1)<0) then pause,end
if or(t(find(t(:,1)==0),2)<0) then pause,end
if or(a(ind,:)<>b) then pause,end

for k=1:30
  p=grand(1,'prm',(1:size(a,1))');
  [b,ind]=gsort(a(p,:),'lr','i');	
  t=b(2:$,:)-b(1:$-1,:);
  if or(t(:,1)<0) then pause,end
  if or(t(find(t(:,1)==0),2)<0) then pause,end
  if or(a(p(ind),:)<>b) then pause,end
end

[b,ind]=gsort(a,'lr','d');
t=b(1:$-1,:)-b(2:$,:);
if or(t(:,1)<0) then pause,end
if or(t(find(t(:,1)==0),2)<0) then pause,end
if or(a(ind,:)<>b) then pause,end

for k=1:30
  p=grand(1,'prm',(1:size(a,1))');
  [b,ind]=gsort(a(p,:),'lr','d');	
  t=b(1:$-1,:)-b(2:$,:);
  if or(t(:,1)<0) then pause,end
  if or(t(find(t(:,1)==0),2)<0) then pause,end
  if or(a(p(ind),:)<>b) then pause,end
end


a=b;
a([10 60],:)=a([60 10],:);
[b,ind]=gsort(a,'lr','i');
t=b(2:$,:)-b(1:$-1,:);
if or(t(:,1)<0) then pause,end
if or(t(find(t(:,1)==0),2)<0) then pause,end
if or(a(ind,:)<>b) then pause,end

[b,ind]=gsort(a,'lr','d');
t=b(1:$-1,:)-b(2:$,:);
if or(t(:,1)<0) then pause,end
if or(t(find(t(:,1)==0),2)<0) then pause,end
if or(a(ind,:)<>b) then pause,end
////////////////////////////////////////////////////////////////////////
// test of gsort for uuint8 matrices:
N=4;P=20;Nrand=100; 

a = uint8(matrix(grand(1,'prm',(1:N*P)'),N,P));

//-----Global sort --------------------------------

[a1,ind]=gsort(a,'g');

if or(a1- matrix(N*P:-1:1,N,P) <>0) then pause,end;
if or(a1- matrix(a(ind),N,P)  <>0) then pause,end;

for i=1:Nrand 
  b = uint8(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'g');
  if or(b1- matrix(b(ind),N,P)<>0)  then pause,end;
  if or(b1(1:$-1)-b1(2:$) <0) then pause,end
end 

//increasing values 

[a1,ind]=gsort(a,'g','i');

if or(a1- matrix(1:N*P,N,P) <>0) then pause,end;
if or(a1- matrix(a(ind),N,P)  <>0) then pause,end;


for i=1:100
  b = uint8(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'g','i');
  if or(b1- matrix(b(ind),N,P)<>0)  then pause,end;
  if or(b1(1:$-1)-b1(2:$) > 0) then pause,end
end 

//----sort each column of a ('r' means that the row indice is used for sorting)

[a1,ind]=gsort(a,'r');
nc=size(a,'c');
test=[];
for i=1:nc ; 
   test= [test, matrix(a(ind(:,i),i),N,1)];
end 
if or((a1- test)<>0)  then pause,end;
test=[];
for i=1:nc ; 
   test= [test, gsort(a(:,i),'g')];
end 
if or((a1- test)<>0)  then pause,end;


for i=1:Nrand
  b = uint8(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'r');
  if or( b1(2:$,:)-b1(1:$-1,:) > 0 ) then pause,end 
  test=[];
  for j=1:nc ; 
    test= [test, matrix(b(ind(:,j),j),N,1)];
  end 
  if or((b1- test)<>0) then pause,end;
end 

//increasing values 

[a1,ind]=gsort(a,'r','i');
nc=size(a,'c');
test=[];
for i=1:nc ; 
  test= [test, matrix(a(ind(:,i),i),N,1)];
end 
if or((a1- test)<>0)  then pause,end;

test=[];
for i=1:nc ; 
   test= [test, gsort(a(:,i),'g','i')];
end 
if or((a1- test)<>0)  then pause,end;

//----sort each row of a ('c' means that the column indice is used for sorting)

[a1,ind]=gsort(a,'c')   ;
nr=size(a,'r');
test=[];
for i=1:nr ; 
   test= [test; matrix(a(i,ind(i,:)),1,P)];
end 
if or((a1- test)<>0)  then pause,end;

test=[];
for i=1:nr ; 
   test= [test; gsort(a(i,:),'g')];
end 
if or((a1- test)<>0)  then pause,end;


for i=1:Nrand
  b = uint8(10*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'c');
  if or( b1(:,2:$)-b1(:,1:$-1) > 0 ) then pause,end 
  test=[];
  for j=1:nr ; 
    test= [test; b(j,ind(j,:))];
  end 
  if or((b1- test)<>0) then pause,end;
end 

//increasing 

[a1,ind]=gsort(a,'c','i')   ;
nr=size(a,'r');
test=[];
for i=1:nr ; 
   test= [test; matrix(a(i,ind(i,:)),1,P)];
end 
if or((a1- test)<>0)  then pause,end;
test=[];
for i=1:nr ; 
   test= [test; gsort(a(i,:),'g','i')];
end 
if or((a1- test)<>0)  then pause,end;

//----sort the rows of a in lexicographic order 
//    i.e a(k,:) < a(l,:) if there's a number j 
//    such that a(k,j) < a(l,j) and a(k,p)=a(l,p) for p in [1,j-1];
//
N1=4;P1=3;
alr=[1,2,2;
     1,2,1;
     1,1,2;
     1,1,1];
// a random permutation 
[ax,perm]=sort(rand(1,N1,'u'));
a=alr(perm,:);

[a1,ind]=gsort(a,'lr');                   

if or((a1- alr) <>0) then pause,end;
if or((a1- matrix(a(ind,:),N1,P1)) <>0) then pause,end;

[a2,ind2]=gsort(a*[100;10;1],'g');
if or((ind2-ind)<>0) then pause,end;
///////////////////////
for i=1:Nrand
  vmax=4;
  b = uint8(vmax*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'lr');
  if or((b1- b(ind,:)) <>0) then pause,end;
  v= double(b1)*((vmax+1)^[P-1:-1:0])';
  if or( v(2:$)-v(1:$-1) > 0 ) then pause,end 
end 

// increasing 

[a1,ind]=gsort(a,'lr','i');                   
if or((a1- alr(N1:-1:1,:)) <>0) then pause,end;

if or((a1- matrix(a(ind,:),N1,P1)) <>0) then pause,end;
[a2,ind2]=gsort(a*[100;10;1],'g','i');
if or((ind2-ind) <>0) then pause,end;

for i=1:Nrand
  vmax=4;
  b = int(vmax*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'lr','i');
  if or((b1- b(ind,:)) <>0) then pause,end;
  v= double(b1)*((vmax+1)^[P-1:-1:0])';
  if or( v(2:$)-v(1:$-1) < 0 ) then pause,end 
end 

//----sort the columns of a in lexicographic order 

N1=3;P1=4;
alr=alr';
// a random permutation 
[ax,perm]=sort(rand(1,P1,'u'));
a=alr(:,perm);

[a1,ind]=gsort(a,'lc');                   

if or((a1- alr) <>0) then pause,end;
if or((a1- matrix(a(:,ind),N1,P1)) <>0) then pause,end;

[a2,ind2]=gsort([100,10,1]*a,'g');
if or((ind2-ind) <>0) then pause,end;

for i=1:Nrand
  vmax=4;
  b = int(vmax*rand(N1,P1,'u')); 
  [b1,ind]=gsort(b,'lc');
  if or((b1- b(:,ind)) <>0) then pause,end;
  v= ((vmax+1)^[N1-1:-1:0])*double(b1);
  if or( v(2:$)-v(1:$-1) > 0 ) then pause,end 
end 

//increasing 

[a1,ind]=gsort(a,'lc','i');                   

if or((a1- alr(:,P1:-1:1)) <>0) then pause,end;
if or((a1- matrix(a(:,ind),N1,P1)) <>0) then pause,end;

[a2,ind2]=gsort([100,10,1]*a,'g','i');
if or((ind2-ind) <>0) then pause,end;

for i=1:Nrand
  vmax=4;
  b = int(vmax*rand(N,P,'u')); 
  [b1,ind]=gsort(b,'lc','i');
  if or((b1- b(:,ind)) <>0) then pause,end;
  v= ((vmax+1)^[N-1:-1:0])*b1;
  if or( v(2:$)-v(1:$-1) < 0 ) then pause,end 
end 



a = uint8([1,1,1,1,2,2,3,3,2,2,3,3,4,4,4,4,4,4,4,4,5,5,5,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,9,9,9,7,9,10,..
 10,10,10,11,11,11,12,13,13,13,13,14,14,12,12,14,14,14,14,14,14,15,15,15,15,16,17,18;
3,4,4,5,3,8,4,4,7,8,8,8,5,5,8,9,10,10,9,8,6,10,10,10,10,11,8,8,13,13,9,9,13,13,10,10,13,..
 13,14,15,15,12,14,11,11,15,15,15,15,16,17,14,14,17,17,15,15,13,13,17,17,18,18,19,19,16,..
 16,19,19,19,18,19]-5)';
[b,ind]=gsort(a,'lr','i');
t=b(2:$,:)<b(1:$-1,:);
if or(t(:,1)) then pause,end
if or(t(b(2:$,1)==b(1:$-1,1),2)) then pause,end
if or(a(ind,:)<>b) then pause,end

for k=1:30
  p=grand(1,'prm',(1:size(a,1))');
  [b,ind]=gsort(a(p,:),'lr','i');
  t=b(2:$,:)<b(1:$-1,:);
  if or(t(:,1)) then pause,end
  if or(t(b(2:$,1)==b(1:$-1,1),2)) then pause,end
  if or(a(p(ind),:)<>b) then pause,end
end

[b,ind]=gsort(a,'lr','d');
t=b(2:$,:)>b(1:$-1,:);
if or(t(:,1)) then pause,end
if or(t(b(2:$,1)==b(1:$-1,1),2)) then pause,end
if or(a(ind,:)<>b) then pause,end


for k=1:30
  p=grand(1,'prm',(1:size(a,1))');
  [b,ind]=gsort(a(p,:),'lr','d');
  t=b(2:$,:)>b(1:$-1,:);
  if or(t(:,1)) then pause,end
  if or(t(b(2:$,1)==b(1:$-1,1),2)) then pause,end
  if or(a(p(ind),:)<>b) then pause,end
end


a=b;
a([10 60],:)=a([60 10],:);
[b,ind]=gsort(a,'lr','i');
t=b(2:$,:)<b(1:$-1,:);
if or(t(:,1)) then pause,end
if or(t(b(2:$,1)==b(1:$-1,1),2)) then pause,end
if or(a(ind,:)<>b) then pause,end



[b,ind]=gsort(a,'lr','d');
t=b(2:$,:)>b(1:$-1,:);
if or(t(:,1)) then pause,end
if or(t(b(2:$,1)==b(1:$-1,1),2)) then pause,end
if or(a(ind,:)<>b) then pause,end
