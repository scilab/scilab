function [txt,rpar,ipar]=create_modelica1( blklst,cmat,name)
  txt=[];tab=ascii(9)
  rpar=[];//will contain all parameters associated with the all modelica blocs
  ipar=[];//will contain the "adress" of each block in rpar
  models=[]//will contain the model declaration part
  eqns=[]//will contain the modelica equations part
  Pin=[]
  Bnumbers=[]
  Bnames=[]
  nb=size(blklst)
  Pars=[]
  for k=1:nb
    ipar(k)=0
    o=blklst(k);
    mo=o.equations;
    np=size(mo.parameters(1),'*');
    P=[];
    
    if size(mo.parameters)==2 then
      for l=1:np
	Pars=[Pars;'P'+string(size(Pars,1)+1)]
	rpar=[rpar;mo.parameters(2)(l)(:)]
	ipar(k)=ipar(k)+size(mo.parameters(2)(l),'*')
	P=[P;mo.parameters(1)(l)+'='+Pars($)];
      end
    else
      for l=1:np
	Pars=[Pars;'P'+string(size(Pars,1)+1)]
	rpar=[rpar;mo.parameters(2)(l)(:)]
	ipar(k)=ipar(k)+size(mo.parameters(2)(l),'*')
	if mo.parameters(3)(l)==0 then
	  P=[P;mo.parameters(1)(l)+'='+Pars($)];
	elseif mo.parameters(3)(l)==1 then
	  P=[P;mo.parameters(1)(l)+'(start='+Pars($)+')'];
	elseif mo.parameters(3)(l)==2 then
	  P=[P;mo.parameters(1)(l)+'(start='+Pars($)+',fixed=true)'];  
	end
      end
    end
    Bnumbers=[Bnumbers k];
    Bnames=[Bnames,'B'+string(k)];
    if P==[] then
      models=[models;'  '+mo.model+' '+tab+'B'+string(k)+';'];
    else
      models=[models;'  '+mo.model+' '+tab+'B'+string(k)+'('+strcat(P,', ')+');'];
    end
    //rajouter les ports
  end
  ipar=cumsum([1;ipar(:)])
  
  //links
  for k=1:size(cmat,1)
    from=cmat(k,1:3)
    to=cmat(k,4:6)
    if from(1)==0 then //input port
      nb=nb+1
      Bnumbers=[Bnumbers nb];
      Bnames=[Bnames,'B'+string(nb)];
      models=[models;'  InPutPort'+' '+tab+'B'+string(nb)+';'];
      n1='B'+string(nb)
    elseif from(3)==1 then
      p1=blklst(from(1)).equations.inputs(from(2))
      n1=Bnames(find(Bnumbers==from(1)))
    else
      p1=blklst(from(1)).equations.outputs(from(2))
      n1=Bnames(find(Bnumbers==from(1)))
    end
    
    if to(1)==0 then //output port
      nb=nb+1
      Bnumbers=[Bnumbers nb];
      Bnames=[Bnames,'B'+string(nb)];
      models=[models;'  OutPutPort'+' '+tab+'B'+string(nb)+';'];
      n1='B'+string(nb)
    elseif to(3)==1 then
      p2=blklst(to(1)).equations.inputs(to(2))
      n2=Bnames(find(Bnumbers==to(1)))
    else
      if size(blklst(to(1)).equations.outputs,'*')<to(2) then pause,end
      p2=blklst(to(1)).equations.outputs(to(2))
      n2=Bnames(find(Bnumbers==to(1)))
    end

    if or(blklst(from(1)).equations.model==['InPutPort','OutPutPort']) ...
	  | or(blklst(to(1)).equations.model==['InPutPort','OutPutPort']) ...
    then 
      eqns=[eqns
	    '  '+n1+'.'+p1+' = '+n2+'.'+p2+';']
    else
      eqns=[eqns
	    '  connect ('+n1+'.'+p1+','+n2+'.'+p2+');']
    end
  end
  
  
  txt=[txt;
       'class '+name
       '  parameter Real '+Pars+';'
       models
       'equation'
       eqns
       'end '+name+';']
endfunction

  
