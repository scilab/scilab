function Modelica_()
// Copyright INRIA
  Cmenu=[]
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
	all_scs_m=scs_m;
	XX=scs_m.objs(k).model.rpar;
	name=XX.props.title(1)
	txt=gen_modelica(XX,name)
	mputl(txt,scs_m.props.title(2)+name+'.mo')
	compile_modelica(scs_m.props.title(2)+name+'.mo')
	Cmenu=[] 
      else
	message('Generation Code only work for a Superblock ! ')
	break
      end
    end
  end    
  
endfunction 
 
