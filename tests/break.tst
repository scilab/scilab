// Copyright INRIA
// for in macro
//--------------------------------------
deff('[k]=tt1()','k=0,for i=1:5 ,k=k+1;if k==3 then break,end,end,k=k+1','n')
if tt1()<>4 then pause,end
if tt1()<>4 then pause,end
clear tt1

deff('[k]=tt1()',[
'k=0,';
'for i=1:5 ,';
'  k=k+1;';
'  if k==3 then break,end,';
'  if k==-1 then 1,end,';
'end,k=k+1'],'n')
if tt1()<>4 then pause,end
if tt1()<>4 then pause,end

clear tt1

deff('[k]=tt1()',[
'k=0,';
'for i=1:5 ,';
'  k=k+1;';
'  if k==3 then break,end,';
'  for j=1:5,j,end,';
'end,';
'k=k+1'],'n')
if tt1()<>4 then pause,end
if tt1()<>4 then pause,end

// while
//----------------------------------------
deff('[k]=tt1()','k=0,while k<10 ,k=k+1;if k==3 then break,end,end,k=k+1','n')
if tt1()<>4 then pause,end
if tt1()<>4 then pause,end
clear tt1

deff('[k]=tt1()',[
'k=0,';
'while k<10 ,';
'  k=k+1;';
'  if k==3 then break,end,';
'  if k==-1 then 1,end,';
'end,k=k+1'],'n')
if tt1()<>4 then pause,end
if tt1()<>4 then pause,end

clear tt1

deff('[k]=tt1()',[
'k=0,';
'while k<10 ,';
'  k=k+1;';
'  if k==3 then break,end,';
'  for j=1:5,j,end,';
'end,';
'k=k+1'],'n')
if tt1()<>4 then pause,end
if tt1()<>4 then pause,end
//
// keyboard mode 
//------------------------
k=0;while k<10 ,k=k+1;if k==3 then break,end,end,k=k+1;
if k<>4 then pause,end

k=0;while k<10 ,k=k+1;if k==3 then break,end,end,
k=k+1;
if k<>4 then pause,end

k=0;for  i=1:5 ,k=k+1;if k==3 then break,end,end,k=k+1;
if k<>4 then pause,end

k=0;for  i=1:5 ,k=k+1;if k==3 then break,end,end,
k=k+1;
if k<>4 then pause,end

//
k=0;while k<10,if k==0 then break,end,k=k+1;end,k=k+1;
if k<>1 then pause,end

k=0;while k<10,if k==0 then break,end,k=k+1;end,
k=k+1;
if k<>1 then pause,end

k=0;for i=1:5,if k==0 then break,end,k=k+1;end,k=k+1;
if k<>1 then pause,end

k=0;for i=1:5,if k==0 then break,end,k=k+1;end,
k=k+1;
if k<>1 then pause,end

