function who_user()
//get user variables
[nams,mem]=who('get'); //get all variables
p=predef(); //number of system variable
st=stacksize()
nams=nams(1:$-p+1);mem=mem(1:$-p+1);
//modifiable system variables
excluded=['demolist','scicos_pal','%scicos_menu',..
	'%scicos_short','%helps','MSDOS','who_user','%scicos_display_mode', ...
	  '%scicos_help'];
ke=grep(nams,excluded)
nams(ke)=[];mem(ke)=[];

n=size(nams,1);
if n==0 then return,end

//format names on n*10 characters
ll=length(nams)+2;m=int((ll-1)/10)+1;
for k=1:max(m)
  ks=find(m==k);
  if ks<>[] then nams(ks)=part(nams(ks),1:(k*10));end
end



nlc=lines(); nc=nlc(1)//window sizes

txt=[]
k=1
while k<=n
  m=find(cumsum(length(nams(k:$)))<nc);
  m=m($)
  txt=[txt;strcat(nams(k:k-1+m))];
  k=k+m;
end
txt=['User variables are:';
    '';
    txt;
    '';
    'using '+string(sum(mem))+' elements out of '+string(st(1)-(st(2)-sum(mem)))]
write(%io(2),txt,'(1x,a)')
endfunction
