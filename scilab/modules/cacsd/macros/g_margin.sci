function [gm,fr]=g_margin(h)
// Copyright INRIA
if type(h)<>16 then error(97,1),end
flag=h(1);
select flag(1)
 case 'r' then ,
 case 'lss' then h=ss2tf(h)
 else error(97,1),
end;
//
//if h(4)<>'c' then error(93,1),end
[n,d]=h(['num','den']);
if type(n)==1 then n=poly(n,varn(d),'c'),end
// get w for which imaginary part is zero
w=roots( imag(horner(n,%i*poly(0,'w')) *...
         conj(horner(d,%i*poly(0,'w')))) )
eps=1.e-7
ws=[];
for i=w',
  if abs(imag(i))<eps then
    if real(i)<0 then  ws=[ws;real(i)],end
  end,
end;

if ws==[] then gm=%inf,fr=[],return,end

//
mingain=real(freq(n,d,%i*ws))
k=find(mingain<0)
if k==[] then gm=%inf,fr=[],return,end
mingain=mingain(k);ws=ws(k)
gm=-20*log(abs(mingain))/log(10)
fr=abs(ws/(2*%pi)) // choix de la frequence positive
endfunction
