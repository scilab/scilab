function scs_m=do_version26(scs_m)

  if type(scs_m(1)) <> 17 then
    tol=scs_m(1)(3)
    if size(tol,'*')<6 then tol(6)=0,end
    tf=scs_m(1)(4)
    if tf==[] then tf=100000;end
    scs_m(1)=scicos_params(wpar=scs_m(1)(1),title=scs_m(1)(2),..
			   tol=tol,tf=tf,..
			   context=scs_m(1)(5),options=scs_m(1)(7),..
			   doc=scs_m(1)(10))
  else
    return
  end
  
  if scs_m(1).options.Background==[] then scs_m(1).options.Background=8,end
  
  n=size(scs_m)
  for i=2:n //loop on objects
    o=scs_m(i)
    if o(1)=='Block' then
      if size(o(2)) > 8 then
	gr_i=convert_gri(o(5),o(2)(9))
	if gr_i==[] then gr_i=o(2)(9), end
      elseif size(o(2)) < 9 then
	gr_i=[];
      end
      graphics=scicos_graphics()
      graphics.orig  = o(2)(1)
      graphics.sz    = o(2)(2)
      graphics.flip  = o(2)(3)
      graphics.exprs = o(2)(4)
      graphics.pin   = o(2)(5)
      graphics.pout  = o(2)(6)
      graphics.pein  = o(2)(7)
      graphics.peout = o(2)(8)
      graphics.gr_i  = gr_i
      
      mdl=o(3);
      if size(o(3))<=12 then 
	mdl(13)=''; mdl(14)=[] ; mdl(15)='';
      elseif size(o(3))<=13 then 
	mdl(14)=[] ; mdl(15)='';
      elseif size(o(3))<=14 then 
	mdl(15)='';
      end
      
      if mdl(1)(1)=='super'|mdl(1)(1)=='csuper' then
	if type(mdl(8))==15 then
	  mdl(8)=traductor(mdl(8))
	end
      end
      
      model=scicos_model()
      model.sim       = mdl(1)
      model.in        = mdl(2)
      model.out       = mdl(3)
      model.evtin     = mdl(4)
      model.evtout    = mdl(5)
      model.state     = mdl(6)
      model.dstate    = mdl(7)
      model.rpar      = mdl(8)
      model.ipar      = mdl(9)
      model.blocktype = mdl(10)
      model.firing    = mdl(11)
      model.dep_ut    = mdl(12)
      model.label     = mdl(13)
      graphics.id        = mdl(15)


      scs_m(i)=scicos_block()
      scs_m(i).graphics = graphics, 
      scs_m(i).model    = model
      scs_m(i).gui      = o(5)
      scs_m(i).doc      = mdl(14)


    elseif o(1)=='Link' then
      scs_m(i)=scicos_link()
      scs_m(i).xx=o(2),
      scs_m(i).yy=o(3), 
      scs_m(i).id=o(5),
      scs_m(i).thick=o(6),
      scs_m(i).ct=o(7),
      scs_m(i).from=o(8),
      scs_m(i).to=o(9)
    elseif o(1)=='Text' then
      scs_m(i)=TEXT_f('define')
      scs_m(i).model.rpar=o(3)(8)
      scs_m(i).model.ipar=o(3)(9)
      scs_m(i).graphics.orig=o(2)(1)
      scs_m(i).graphics.sz=o(2)(2)
      scs_m(i).graphics.exprs=o(2)(4)
    elseif o(1)=='Deleted' then
      scs_m(i)=tlist('Deleted')
    end
  end
  %cpr=resume(list()) // doit etre enleve
endfunction

function  gr_i=convert_gri(name,gri)
// Copyright INRIA
gr_i=[];
select name
case 'BIGSOM_f' then
  gr_i=['[x,y,typ]=standard_inputs(o) ';
        'sgn=o.model.rpar';
        'for k=1:size(x,''*'')';
        '  if sgn(k)>0 then';
        '    xstring(orig(1)+sz(1)/8 ,y(k)-4,''+'')';
        '  else';
        '    xstring(orig(1)+sz(1)/8 ,y(k)-4,''-'')';
        '  end';
        'end';
        'xx=sz(1)*[.8 .4 0.75 .4 .8]+orig(1)';
        'yy=sz(2)*[.8 .8 .5 .2 .2]+orig(2)';
        'xpoly(xx,yy,''lines'')']

case 'CONST_f' then
  gr_i=['model=arg1.model;C=string(model.rpar);';
    'dx=sz(1)/5;dy=sz(2)/10;';
    'w=sz(1)-2*dx;h=sz(2)-2*dy;';
    'if size(C,''*'')==1 then ';
    '  txt=C;'
    '  if length(txt)>4 then txt=''C'',end;'
    'else ';
    '  txt=''C'';'
    'end';
    'xstringb(orig(1)+dx,orig(2)+dy,txt,w,h,''fill'');']

case 'CURV_f' then
  gr_i=['model=arg1.model;rpar=model.rpar;ipar=model.ipar;n=ipar(1);';
    'thick=xget(''thickness'');xset(''thickness'',2);';
    'xx=rpar(1:n);yy=rpar(n+1:2*n);';
    'rect=rpar(2*n+1:2*n+4);';
    'mxx=rect(3)-rect(1);';
    'mxy=rect(4)-rect(2);';
    'xx=orig(1)+sz(1)*(1/10+(4/5)*((xx-rect(1))/mxx));';
    'yy=orig(2)+sz(2)*(1/10+(4/5)*((yy-rect(2))/mxy));';
    'xpoly(xx,yy,''lines'');';
    'xset(''thickness'',thick);']

case 'EVTDLY_f' then
  gr_i=['dt=model.rpar;';
    'txt=[''Delay'';string(dt)];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']

case 'EVTGEN_f' then
  gr_i=['tt=model.firing;';
    'txt=[''Event at'';''time ''+string(tt)];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']

case 'GAIN_f' then

  gr_i=['[nin,nout]=(model.in,model.out);';
      'if nin*nout==1 then gain=string(model.rpar),else gain=''Gain'',end';
      'dx=sz(1)/5;';
      'dy=sz(2)/10;';
      'xx=orig(1)+      [1 4 1 1]*dx;';
      'yy=orig(2)+sz(2)-[1 5 9 1]*dy;';
      'xpoly(xx,yy,''lines'');';
      'w=sz(1)-2*dx;h=sz(2)-2*dy;';
      'xstringb(orig(1)+dx,orig(2)+dy,gain,w,h,''fill'');']

case 'LOOKUP_f' then
  gr_i=['rpar=model.rpar;n=size(rpar,''*'')/2;';
    'thick=xget(''thickness'');xset(''thickness'',2);';
    'xx=rpar(1:n);yy=rpar(n+1:2*n);';
    'mnx=mini(xx);xx=xx-mnx*ones(xx);mxx=maxi(xx);';
    'xx=orig(1)+sz(1)*(1/10+(4/5)*xx/mxx);';
    'mnx=mini(yy);yy=yy-mnx*ones(yy);mxx=maxi(yy);';
    'yy=orig(2)+sz(2)*(1/10+(4/5)*yy/mxx);';
    'xpoly(xx,yy,''lines'');';
    'xset(''thickness'',thick);']

case 'REGISTER_f' then

  gr_i=['dly=model.rpar;';
    'txt=[''Shift'';''Register'';string(dly)];';
    'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
case 'SWITCH_f' then

  gr_i='xstringb(orig(1),orig(2),[''switch'';string(model.ipar+1)],sz(1),sz(2),''fill'');'
case 'EXPBLK_f' then
  gr_i='xstringb(orig(1),orig(2),''a^u'',sz(1),sz(2),''fill'');'
case 'POWBLK_f' then
  gr_i='xstringb(orig(1),orig(2),''u^a'',sz(1),sz(2),''fill'');'
  
end
endfunction
