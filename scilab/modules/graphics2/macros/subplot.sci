function subplot(m,n,p)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then
  p=modulo(m,10)
  n=modulo((m-p)/10,10)
  m=(m-p-10*n)/100
end
j=int((p-1)/n)
i=p-1-n*j
axes_bounds=[i/n,j/m,1/n,1/m];

if get('figure_style')=='new' then
  f=gcf();
  na=size(f.children,'*')
  if na==1 then 
    // an axes is automatically created when a figure is created
    // do not create a new axes if we have just this one
      a=f.children;da=gda()
      if a.children==[]& and(a.axes_bounds==da.axes_bounds) then
       //a single axes with no children, just resize it
       a.axes_bounds=axes_bounds;
       return	
     end
  end
  // look for an axes with the same axes_bounds
  for k=1:na
    if and(f.children(k).axes_bounds==axes_bounds) then
      //make it current
      sca(f.children(k))
      return
    end
  end	
  //create a new axes
  a=newaxes();
  a.axes_bounds=axes_bounds;
  sca(a)
else //old graphic
  xsetech(axes_bounds)
end
endfunction
