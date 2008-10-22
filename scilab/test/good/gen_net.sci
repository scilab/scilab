function [g]=gen_net(name,directed,v)
// Copyright INRIA
[lhs,rhs]=argn(0)
g=[]
if rhs==0 then
  directed=x_choices('Is the graph directed?',..
      list(list(' ',1,['Yes','No'])));
  if directed==[] then return, end
  if directed==2 then directed=0, end
  name=x_dialog('Name of the graph','foo');
  if name==[] then return, end
  txt=['Seed for random';'Number of nodes';'Number of sources';..
      'Number of sinks';'Minimum cost';'Maximum cost';'Input supply';..
      'Output supply';'Minimum capacity';'Maximum capacity';..
      'Percentage of edges with costs';'Percentage of edges with capacities'];
  v=x_mdialog('Values for generating the network',txt,..
      ['1';'4';'1';'1';'0';'10';'100';'100';'0';'100';'50';'50']);
  if v==[] then return, end
  v=evstr(v);
else
  // name
  if type(name)<>10|prod(size(name))<>1 then
    error('""name"" must be a string')
  end  
  // directed
  if directed<>1&directed<>0 then
    error('""directed"" must be 0 or 1')
  end
  // v
  s=size(v)
  if s(1)<>1 then
    error('""v"" must be a row vector')
  end
  if s(2)<>12 then
    error('""v"" must have 12 elements')
  end 
end

inseed=v(1)
n=v(2)
nsorc=v(3)
nsink=v(4)
mincst=v(5)
maxcst=v(6)
itsup=v(7)
otsup=v(8)
mincap=v(9)
maxcap=v(10)
bhicst=v(11)
bcap=v(12)

ns=n+nsorc+nsink;ns2=ns*2;ns4=ns*4;ns10=ns*10;ns12=ns*12;
ns20=ns*20;ns30=ns*30;

[ntype,ma,tail,head,x,y,cap,cost]=m6meshmesh(inseed,..
    n,nsorc,nsink,mincst,maxcst,itsup,otsup,..
    bhicst,bcap,mincap,maxcap,..
    ns,ns2,ns4,ns10,ns12,ns20,ns30)

g=glist(name,directed,n,tail(1:ma),head(1:ma),string(1:n),..
    ntype,x,y,[],[],[],[],[],[],[],..
    [],[],[],[],cost(1:ma),[],cap(1:ma),[],[],[],..
    [],10,1,1,1,1,[],[])
endfunction
