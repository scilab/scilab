function []=ssprint(sl,out)
//ssprint(sl [,out])  pretty print of a linear system in state-space form
// sl=(A,B,C,D) syslin list.
// out=output (default out=%io(2))
// See also texprint.
//Examples:
// a=[1 1;0 1];b=[0 1;1 0];c=[1,1];d=[3,2];
// ssprint(syslin('c',a,b,c,d))
// ssprint(syslin('d',a,b,c,d))
//!

// Copyright INRIA
[lhs,rhs]=argn(0)
mess='system cannot be displayed in this page'
fil=%f
if rhs==1 then 
  out=%io(2),
else
  if type(out)==10 then
  out=file('open',out,'unknown')
  fil=%t
  end
end
deff('[ta]=%cv(x)',['[m,n]=size(x);';
                    'if m*n==0 then ta='' '',return,end';
                    'frmt=format();frmt=10**frmt(2)/maxi([1,norm(coeff(x))]);';
                    'x=round(frmt*x)/frmt;';
                    't=[];for k=1:m,t=[t;''|''],end;';
                    'ta=t;for k=1:n,';
                    '        aa=string(x(:,k)),';
                    '        for l=1:m,';
                    '           if part(aa(l),1)<>''-'' then ';
                    '               aa(l)='' ''+aa(l),';
                    '           end,';
                    '        end,';
                    '        n=maxi(length(aa)),';
                    '        aa=part(aa+blank,1:n),';
                    '        ta=ta+aa+part(blank,1),';
                    'end;ta=ta+t;'])
//
// d(x)=ax + bu
//-------------
write(out,' ')
sgn='.';if sl(7)<>'c' then sgn='+',end
ll=lines();ll=ll(1)
[a,b,c,d]=sl(2:5)
na=size(a,'r')
[nc,nb]=size(d)
if na>0 then
  blank=[];for k=1:na,blank=[blank;'           '],end

  ta=%cv(a);
  tb=%cv(b);
  //
  blank=part(blank,1:4)
  if na==1 then
    t=[sgn+'   ';'x = ']+[' ';ta]+[' ';'x']
  else
    blank([na/2,na/2+1])=[sgn+'   ';'x = ']
    t=blank+ta;
  end
  if nb>0 then
    if na==1 then
      t=t+['    ';' + ']+[' ';tb]+[' ';'u   ']
    else
      blank([na/2,na/2+1])=['    ';'x + ']
      t=t+blank+tb
      t(na/2+1)=t(na/2+1)+'u   '
    end
  end
  n1=maxi(length(t))+1
  //
  n2=maxi(length(t))
  if n2<ll then
    write(out,t)
  else
    if n1<ll,
      if n2-n1<ll then
	write(out,part(t,1:n1)),
	write(out,' ')
	write(out,part(t,n1+1:n2),'(3x,a)')
      else          
	error(mess)
      end;
    else 
      error(mess)
    end;
  end;
end
//
//y = cx + du
//-----------
if nc==0 then if fil then file('close',out);end;return,end
write(out,' ')
blank=[];for k=1:nc,blank=[blank;'           '],end
if na==0 then
  td=%cv(d)
  if nc==1 then 
    t='y = '+td+'u'
  else
    blank(nc/2+1)='y = '
    t=blank+td
    t(nc/2+1)=t(nc/2+1)+'u   '
  end
  n1=maxi(length(t))+1
else
  tc=%cv(c);td=%cv(d)
  blank=part(blank,1:4);
  if nc==1 then 
    t='y = '+tc
  else
    blank(nc/2+1)='y = '
    t=blank+tc;
  end
  withd=d<>[]
  if withd then withd=norm(d,1)>0,end
  if withd then
    if nc==1 then 
      t=t+'x + '+td
    else
      blank(nc/2+1)='x + '
      t=t+blank+td
      t(nc/2+1)=t(nc/2+1)+'u   '
    end
    n1=maxi(length(t))+1
  else
    if nc==1 then 
      t=t+'x   '
    else
      t(nc/2+1)=t(nc/2+1)+'x   '
    end  
  end;
end
n2=maxi(length(t))
if n2<ll then
  write(out,t)
else
  if n1<ll,
    if n2-n1<ll then
      write(out,part(t,1:n1)),
      write(out,' ')
      write(out,part(t,n1+1:n2),'(3x,a)')
    else 
      error(mess)
    end
  else 
    error(mess)
  end
end
if fil then file('close',out),end
endfunction
