// Copyright ENPC
// test of fscanfMat and fprintfMat
// --------------------------------
n=50;
a=rand(n,n,'u');
// now the data 
fd=mopen(TMPDIR+'/Mat','w');
mfprintf(fd,'Some text.....\n');
mfprintf(fd,'Some text again\n');
for i=1:n ,
	for j=1:n, mfprintf(fd,'%5.2f ',a(i,j));end;
	mfprintf(fd,'\n');	
end
mclose(fd);
a1=fscanfMat(TMPDIR+'/Mat');
if maxi(a1-a) > 1.e-1 then pause,end 

// ---- test with fprintfMat 
n=50;
a=rand(n,n,'u');
fprintfMat(TMPDIR+'/Mat',a,'%5.2f');
a1=fscanfMat(TMPDIR+'/Mat');
if maxi(a1-a) > 1.e-1 then pause,end 

