// test de gsort : a Finir pour ind ds le cas 'l' 
// a faire ensuite marcher pour vecteur complexes 

N=4;P=5;
// a random permutation 
[ax,perm]=sort(rand(1,N*P,'u'));
a=matrix(perm,N,P);

//-----Global sort 
[a1,ind]=gsort(a,'g');

if norm(a1- matrix(N*P:-1:1,N,P)) > %eps then pause;end;
if norm(a1- matrix(a(ind),N,P)) > %eps then pause;end;

//----sort each column of a ('r' means that the row indice is used for sorting)
[a1,ind]=gsort(a,'r');
nc=size(a,'c');
test=[];
for i=1:nc ; 
   test= [test, matrix(a(ind(:,i),i),N,1)];
end 
if norm(a1- test) > %eps then pause;end;

//----sort each row of a ('c' means that the column indice is used for sorting)

[a1,ind]=gsort(a,'c')   ;
nr=size(a,'r');
test=[];
for i=1:nr ; 
   test= [test; matrix(a(i,ind(i,:)),1,P)];
end 
if norm(a1- test) > %eps then pause;end;

//----sort the rows of a in lexicographic order 
//    i.e a(k,:) < a(l,:) if there's a number j 
//    such that a(k,j) < a(l,j) and a(k,p)=a(l,p) for p in [1,j-1];
//
N=4;P=3;
alr=[1,2,2;
     1,2,1;
     1,1,2;
     1,1,1];
// a random permutation 
[ax,perm]=sort(rand(1,N,'u'));
a=alr(perm,:);

[a1,ind]=gsort(a,'lr');                   

if norm(a1- alr) > %eps then pause;end;
if norm(a1- matrix(a(ind,:),N,P)) > %eps then pause;end;

//----sort the columns of a in lexicographic order 
N=3;P=4;
alr=alr';
// a random permutation 
[ax,perm]=sort(rand(1,P,'u'));
a=alr(:,perm);

[a1,ind]=gsort(a,'lc');                   

if norm(a1- alr) > %eps then pause;end;
if norm(a1- matrix(a(:,ind),N,P)) > %eps then pause;end;

//========================String sorts ================
N=10;P=20;
a=int(10*rand(N,P,'u'));

//-----Global sort 
[a1,ind]=gsort(string(a),'g');
[a2,ind2]=gsort(a,'g');
if norm(evstr(a1)- matrix(a(ind),N,P)) > %eps then pause;end;
// a and string(a) are in the same order in the previous example 
if norm(evstr(a1)- a2 ) > %eps then pause;end;

//-- rows 
[a1,ind1]=gsort(string(a),'r');   
[a2,ind2]=gsort(a,'r');
if norm(ind1-ind2) > %eps then pause;end;
// a and string(a) are in the same order in the previous example 
if norm(evstr(a1)- a2 ) > %eps then pause;end;

//--columns 
[a1,ind1]=gsort(string(a),'c')   ;                      
[a2,ind2]=gsort(a,'c');
if norm(ind1-ind2) > %eps then pause;end;
// a and string(a) are in the same order in the previous example 
if norm(evstr(a1)- a2 ) > %eps then pause;end;

//----sort the rows of a in lexicographic order 

N=4;P=3;
alr=[1,2,2;
     1,2,1;
     1,1,2;
     1,1,1];
// a random permutation 
[ax,perm]=sort(rand(1,N,'u'));
a=alr(perm,:);
[a1,ind]=gsort(string(a),'lr') ;        
[a2,ind]=gsort(a,'lr') ;
//
if norm(evstr(a1)- matrix(a(ind,:),N,P)) > %eps then pause;end;
// a and string(a) are in the same order in the previous example 
if norm(evstr(a1)- a2 ) > %eps then pause;end;


//----sort the columns of a in lexicographic order 
N=3;P=4;
alr=alr';
// a random permutation 
[ax,perm]=sort(rand(1,P,'u'));
a=alr(:,perm);

[a1,ind]=gsort(string(a),'lc');                   
[a2,ind]=gsort(a,'lc') ;
//
if norm(evstr(a1)- matrix(a(:,ind),N,P)) > %eps then pause;end;
// a and string(a) are in the same order in the previous example 
if norm(evstr(a1)- a2 ) > %eps then pause;end;


