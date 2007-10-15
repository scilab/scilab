function txt=value2modelica(v)
// Copyright INRIA
  [m,n]=size(v)
  if m==1&n==1 then
    txt=stripblanks(sci2exp(v))
    kdot=strindex(txt,'.')
    if kdot==[] then return,end
    if kdot==1 then 
      txt='0'+txt
    else
      if or(part(txt,kdot-1)==['+','-']) then
	txt=part(txt,1:kdot-1)+'0'+part(txt,kdot:length(txt))
      end
    end
  else //matrix case
    txt=stripblanks(sci2exp(v,0))
    kdot=strindex(txt,'.')
    if kdot==[] then return,end
    k=strindex(part(txt,kdot-1),['+','-',',',';','['])
    if k==[] then return,end
    t=txt;txt='';k0=1;
    for i=k
      ki=kdot(i);
      txt=txt+part(t,k0:ki-1)+'0';
      k0=ki;
    end
    txt=txt+part(t,k0:length(t))
  end
endfunction
