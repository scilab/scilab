function cos2cosf(u,scs_m,count)
//write scilab instructions whose evaluation 
//returns the  value of scicos data structure scs_m.
// in the opened file associated with logical unit u
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<3 then 
  count=0,
  lname='scs_m'
else
  count=count+1
  lname='scs_m_'+string(count)
end
bl=''
lmax=80;
t=lname+'=list()'
t1=sci2exp(scs_m(1),lmax);
t=[t;lname+'(1)='+t1(1);t1(2:$)]
write(u,t,'(a)');t=[]

for k=2:size(scs_m)

  o=scs_m(k)
  if o(1)=='Block' then
    model=o(3)
    lhs=lname+'('+string(k)+')='
    bl1=' ';bl1=part(bl1,1:length(lhs))
    
    if model(1)=='super'| model(1)=='csuper' then  //Super blocks
      cos2cosf(u,o(3)(8),count);//t=[t;t1]
	    
      tt=lname+'('+string(k)+')=list('+sci2exp(o(1),lmax-count*2) //"Block"
      t1=sci2exp(o(2),lmax-count*2) //"Graphic"
      tt=catinstr(tt,t1,length(lhs)) 
      //generate code for 7 first entries of model
      tt=catinstr(tt,'list('+sci2exp(o(3)(1),lmax-count*2),length(lhs))
      for k=2:7
	tt=catinstr(tt,sci2exp(o(3)(k),lmax-count*2),length(lhs))
      end
      //add referece for eigth entries of model
      tt=catinstr(tt,'scs_m_'+string(count+1),0)
      //generate code for last  entries of model
      for k=9:size(o(3))
	tt=catinstr(tt,sci2exp(o(3)(k),lmax-count*2),length(lhs))
      end
      tt($)=tt($)+')' // close model list
      //generate code for last  entries of block
      for k=4:size(o)
	tt=catinstr(tt,sci2exp(o(k),lmax-count*2),length(lhs))
      end
      tt($)=tt($)+')' // close block list
      write(u,tt,'(a)');tt=[];
    else
      t1=sci2exp(o,lmax-length(lhs))
      t=[t;lhs+t1(1);bl1(ones(size(t1,1)-1,1))+t1(2:$)]
      write(u,t,'(a)');t=[]
    end
  else //regular blocks
    lhs=lname+'('+string(k)+')='
    t1=sci2exp(o,lmax-length(lhs))
    n1=size(t1,1)
    bl1=' ';bl1=part(bl1,1:length(lhs))
    t=[t;lhs+t1(1);bl1(ones(n1-1,1))+t1(2:$)]
    write(u,t,'(a)');t=[]
  end
//  write(u,t,'(a)')
end

function t=catinstr(t,t1,n)
sep=','
dots='.'+'.';
if size(t1,'*')==1&(lmax==0|maxi(length(t1))+length(t($))<lmax) then
  t($)=t($)+sep+t1
else
  t($)=t($)+sep+dots
  bl1=' ';bl1=part(bl1,1:n)
  t=[t;bl1(ones(size(t1,1),1))+t1]
end
