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
t=lname+'=scicos_diagram()'
t1=sci2exp(scs_m.props,lmax);
t=[t;lname+'.props='+t1(1);t1(2:$)]
write(u,t,'(a)');t=[]

for k=1:lstsize(scs_m.objs)

  o=scs_m.objs(k)
   if typeof(o)=='Block' then
    lhs=lname+'.objs('+string(k)+')='
    bl1=' ';bl1=part(bl1,1:length(lhs))
    if o.model.sim=='super'| o.model.sim=='csuper' then  //Super blocks
      
      //generate code for model.rpar
      cos2cosf(u,o.model.rpar,count);//model.rpar

      //open a block
      tt= lname+'.objs('+string(k)+')=mlist(..'
      //add the type field
      
      tt=[tt;
	  bl1+sci2exp(getfield(1,o),lmax-count*2)]

      //add graphics code
      tt=catinstr(tt,sci2exp(o.graphics,lmax-count*2),length(lhs))
								   
      //open the model data structure and write code for type 
      tt=catinstr(tt,'mlist(..',length(lhs))
      //add the type field
      fn=getfield(1,o.model)
      tt=[tt;bl1+sci2exp(fn,lmax-count*2)]

      for k=2:size(fn,'*')
	if fn(k)<>'rpar' then
	  tt=catinstr(tt,sci2exp(o.model(fn(k)),lmax-count*2),length(lhs))
	else
	  //introduce model.rpar generated above
	  tt=catinstr(tt,'scs_m_'+string(count+1),0) 
	end
      end
      tt($)=tt($)+')' // close model list
      
      //generate code for last  entries of block
      fn=getfield(1,o)
      for k=4:size(fn,'*')
	tt=catinstr(tt,sci2exp(o(fn(k)),lmax-count*2),length(lhs))
      end
      tt($)=tt($)+')' // close block list
      write(u,tt,'(a)');tt=[];
    else
      t1=sci2exp(o,lmax-length(lhs))
      t=[t;lhs+t1(1);bl1(ones(size(t1,1)-1,1))+t1(2:$)]
      write(u,t,'(a)');t=[]
    end
  else //regular blocks
    lhs=lname+'.objs('+string(k)+')='
    t1=sci2exp(o,lmax-length(lhs))
    n1=size(t1,1)
    bl1=' ';bl1=part(bl1,1:length(lhs))
    t=[t;lhs+t1(1);bl1(ones(n1-1,1))+t1(2:$)]
    write(u,t,'(a)');t=[]
  end
end

endfunction

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
endfunction
