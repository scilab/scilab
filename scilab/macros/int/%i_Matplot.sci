function %i_Matplot(z,varargin)
xsetech([0 0 1 1])
strf='011'
[N,M]=size(z)
rect=[1 1 N M] // je fixe le cadre de l'image complete
nax=[2 10 2 10]
nv=size(varargin)
if nv>=1 then strf=varargin(1),end
if nv>=2 then rect=varargin(1),end
if nv>=3 then nax=varargin(1),end
if part(strf,2)<>'0' then //je dessine le cadre et les echelles
  plot2d(0,0,1,strf,' ',rect,nax)
end

l=1
Max=stacksize();Max=int((Max(1)-Max(2))/3)
step=max(1,int(Max/N))
while l<M
  l1=min(l+step-1,M);
  Matplot(double(z(:,l:l1)),"010",[-(l-1),0,M-l+1,N]+0.5)
  l=l1
end

