function Modelica_()
  Cmenu='Open/Set'
  xinfo('Click on a Implcit Superblock '+..
	' to obtain a modelica block ! ')
  k=[]
  
  while %t 
    [btn,xc,yc,win,Cmenu]=cosclick()
    
    if Cmenu<>[] then
      %pt=[];break
    elseif btn>31 then
      Cmenu=%tableau(min(100,btn-31));%pt=[xc;yc];%win=win
      if Cmenu==emptystr() then 
	Cmenu=[];%pt=[];
      end 
      break
    end
    k=getobj(scs_m,[xc;yc])
    if k<>[] then
      if scs_m.objs(k).model.sim(1)=='super' then
	disablemenus()
	all_scs_m=scs_m;
	XX=scs_m.objs(k).model.rpar;
	name=XX.props.title(1)
	txt=gen_modelica(XX,name)
	mputl(txt,scs_m.props.title(2)+name+'.mo')
	compile_modelica(scs_m.props.title(2)+name+'.mo')
	enablemenus()
	//if ok then 
	//	scs_m.objs(k)=XX
	//	edited=%t;
	//	needcompile=4
	//	Cmenu='Replot';
	//	break
	//      else
	Cmenu='Open/Set' 
	//	break
	//      end     
      else
	message('Generation Code only work for a Superblock ! ')
	break
      end
    end
  end    
  
endfunction 
 
