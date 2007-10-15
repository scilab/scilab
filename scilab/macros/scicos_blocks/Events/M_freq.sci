function [x,y,typ]=M_freq(job,arg1,arg2)
  x=[];y=[];typ=[]
  select job
   case 'plot' then
    standard_draw(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics;
  model=arg1.model;
  exprs=graphics.exprs
  while %t do
    [ok,frequ,offset,exprs]=getvalue('Set block parameters',..
	['Sample time';'Offset'],..
	list('vec',-1,'vec',-1),exprs)
    if ~ok then break,end
    offset=offset(:);frequ=frequ(:);
    if (size(frequ,'*'))<>(size(offset,'*')) then message("offset and frequency must have the same size");ok=%f;
    elseif or(frequ<0) then message("Frequency must be a positif number");ok=%f;
    elseif or(abs(offset) > frequ) then
      message("The |Offset| must be less than the Frequency");ok=%f
    end
    if ok then
      v=[frequ;offset];
      v=v(find(v<>0));
      min_v=min(v);max_v=max(v);
      if (max_v/min_v)>1e5 then message(['The difference between the frequencies is very large';..
	      'the clocks could not be synchronized']);
        ok=%f;
	return; 
      end
      [pgcd,den]=fixedpointgcd(v);
      off=0;
      [m1,k]=uni(frequ,offset);
      frd=m1.*(den)
      frequ=frequ(k);
      frd1=round((frequ.*(den)));
      frd1=uint32(frd1);
      ppcm=lcm(frd1);
      frd1=double(frd1);
      frd=double(frd);
      offset=offset(k);
      mat=[];
      for i=1:size(frd,'*')
	mat1=[offset(i)*double(den):frd1(i):double(ppcm)]';
	mat=[mat;[mat1 2^(i-1)*ones(size(mat1,'*'),1)]];
      end
      [n,k]=gsort(mat(:,1),'g','i');
      mat=mat(k,:);
      if size(mat,1)>1 then
	while (find(mat(1:$-1,1)==mat(2:$,1))<>[]) then
	  ind=find(mat(1:$-1,1)==mat(2:$,1));
	  ind=ind(1);
	  mat(ind,2)=mat(ind,2)+mat(ind+1,2);
	  mat(ind+1,:)=[];
	end
	m=[mat(2,1);mat(2:$,1)-mat(1:$-1,1)];
	m=[m,mat(:,2),mat(:,1)];
	count=1;
	if find(m(:,3)==0)<>[] then
	  m(find(m(:,3)==0),:)=[];
	  count=0;
	end
	mn=(2**size(m1,'*'))-1;
	fir=-ones(1,mn);
	fir(mat(1,2))=mat(1,1)/double(den);
      else
	m=[frd1 1 frd1];
	mat=m;
	count=0;
	off=offset;
	fir=off;
      end
      if or(model.rpar(:)<>[frequ;offset]) then needcompile=4;y=needcompile,end
      model.opar=list(m,double(den),off,count)
      mn=(2**size(m1,'*'))-1;
      if mn>3 then graphics.sz=[40+(mn-3)*10 40]
      else graphics.sz=[50 40]
      end
      model.evtin=1
      model.evtout=ones(mn,1);
      model.firing=fir;
      graphics.exprs=exprs
      x.graphics=graphics
      x.model=model
      break
    end
  end
  needcompile=resume(needcompile)
case 'define' then
  model=scicos_model()
   model.sim=list('m_frequ',4)
  model.evtout=[1;1;1]
  model.evtin=1
  model.rpar=[]
  model.opar=list([1 1 0;1 1 1;1 3 2],1,0,0);
  model.blocktype='d'
  model.firing=[0 -1 -1]
  model.dep_ut=[%f %f]

  exprs=[sci2exp([1;2]);sci2exp([0;0])]
     gr_i=[
//            'a=evstr(arg1.graphics.exprs(1))';
//            '[x,y,typ]=standard_outputs(arg1) ';
//            'xx=[x,orig(1)+sz(1)]';
//            'for i=1:size(a,''*'')';
// 	   'xstringb(x(2^(i-1))-sz(1)/16,orig(2)+sz(2)/32,string(a(i)),xx(2**(i-1)+1)-xx(2**(i-1)),sz(2)/16)';
//            'end';
           'xstringb(orig(1),orig(2),[''      Multiple    '';''    Frequency    ''],sz(1),sz(2));']
  x=standard_define([2.5 2],model,exprs,gr_i)
end
endfunction

function [value,denom_com]=fixedpointgcd(v)
x=log10(v);
f=round((min(x)+max(x))/2);
v=v./10^(f);
[N,D]=rat(v,1d-5);
denom_com=lcm(uint32(D));
N=uint32(N)*denom_com./uint32(D);
value=gcd(N);
if f>0 then value=value*10^f;
else denom_com=double(denom_com)*10^(-f);
end
endfunction

function [m,k]=uni(fr,of)
k=[];
m=[];
for i=1:size(fr,'*')
     ind=find(m==fr(i));
     if ind==[] then
         m=[m;fr(i)];
         k=[k;i];
     else
         for j=ind
              if of(i)<> of(j) then
                 m=[m;fr(i)];
                 k=[k;i];
              end
         end
      end
end
endfunction 
