function scs_m_new=do_version26(scs_m)
  if typeof(scs_m)=='diagram' then 
    
    scs_m_new=scs_m,

    for k=1:size(scs_m_new.objs)
      if typeof(scs_m_new.objs(k))=='Link' then
	o=scs_m_new.objs(k)
	if size(o.from,'*')==2 then o.from(3)=0,end
	if size(o.to,'*')==2 then o.to(3)=1,end
	scs_m_new.objs(k)=o
      end
    end

    return,
  end
  scs_m_new=scicos_diagram()

  tol=scs_m(1)(3)
  if size(tol,'*')<6 then tol(6)=0,end
  tf=scs_m(1)(4)
  if tf==[] then tf=100000;end
  scs_m_new.props=scicos_params(wpar=scs_m(1)(1),title=scs_m(1)(2),..
				tol=tol,tf=tf,..
				context=scs_m(1)(5),options=scs_m(1)(7),..
				doc=scs_m(1)(10))

  if scs_m(1)(7).Background==[] then 
    scs_m_new.props.options.Background=[8 1]
  end
  
  scs_m_new.objs(1)=mlist('Deleted') // not to change the internal numbering
  n=size(scs_m)
  back_col=8   //white background
  
  for i=2:n //loop on objects
    o=scs_m(i);
    if o(1)=='Block' then
      if size(o(2)) > 8 then
	if type(o(2)(9))==15 then 
	  gr_io=o(2)(9)(1);
	  if o(2)(9)(2)<>[] then
	    back_col=o(2)(9)(2);,
	  end
	else
	  gr_io=o(2)(9);
	end
	gr_i=convert_gri(o(5),gr_io);
	if gr_i==[] then gr_i=gr_io;, end
      elseif size(o(2)) < 9 then
	gr_i=[];
      end
      gr_i=list(gr_i,back_col)
      
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
	  mdl(8)=do_version26(mdl(8))
	end
      end
      
      graphics=scicos_graphics(orig=o(2)(1),sz=o(2)(2),flip=o(2)(3),..
			       exprs=o(2)(4),pin=o(2)(5),pout=o(2)(6),..
			       pein=o(2)(7),peout=o(2)(8),gr_i=gr_i,..
			       id=mdl(15)) 	       
      
      
      model=scicos_model(sim=mdl(1),in=mdl(2),out=mdl(3),evtin=mdl(4),..
			 evtout=mdl(5),state=mdl(6),dstate=mdl(7),..
			 rpar=mdl(8),ipar=mdl(9),blocktype=mdl(10),..
			 firing=mdl(11),dep_ut=mdl(12),label=mdl(13))

      
      objsi=scicos_block(graphics=graphics,model=model,gui=o(5),..
			 doc=mdl(14))
      scs_m_new.objs(i)=objsi
      
    elseif o(1)=='Link' then
      from=o(8);from(3)=0;
      to=o(9);to(3)=1;
      
      objsi=scicos_link(xx=o(2),yy=o(3),id=o(5),thick=o(6),..
			ct=o(7),from=from,to=to)
      scs_m_new.objs(i)=objsi
    elseif o(1)=='Text' then
      
      objsi=TEXT_f('define')
      objsi.model.rpar=o(3)(8)
      objsi.model.ipar=o(3)(9)
      objsi.graphics.orig=o(2)(1)
      objsi.graphics.sz=o(2)(2)
      objsi.graphics.exprs=o(2)(4)
      scs_m_new.objs(i)=objsi
    elseif o(1)=='Deleted' then
      scs_m_new.objs(i)=tlist('Deleted')
    end
  end

  [%cpr,edited]=resume(list(),%t) // doit etre enleve
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
    gr_i=['dx=sz(1)/5;dy=sz(2)/10;';
    'w=sz(1)-2*dx;h=sz(2)-2*dy;';
    'txt=C;'
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

  gr_i=['gain=C,'
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
