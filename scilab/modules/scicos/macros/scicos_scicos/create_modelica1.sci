//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

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
    //** mo.parameters have size=2
    //** it only contains parameters
    if size(mo.parameters)==2 then
      for l=1:np
        for i=1:size(mo.parameters(2)(l),'*')
          Pars=[Pars;
                'P'+string(size(Pars,1)+1)]
        end
	rpar=[rpar;matrix(mo.parameters(2)(l),-1,1)]
	ipar(k)=ipar(k)+size(mo.parameters(2)(l),'*')
        if size(mo.parameters(2)(l),'*')==1 then
	  P=[P;mo.parameters(1)(l)+'='+Pars($)];
        else
          P=[P;mo.parameters(1)(l)+'={']
          for i=size(mo.parameters(2)(l),'*'):-1:1
            P($)=P($)+Pars($-i+1)
            if i<>1 then
              P($)=P($)+','
            end
          end
          P($)=P($)+'}'
        end
      end
    //** mo.parameters have size>2
    //** it can contains parameters, initial values of states
    //** and initial values of initial fixed states
    else
      for l=1:np
        for i=1:size(mo.parameters(2)(l),'*')
          Pars=[Pars;
                'P'+string(size(Pars,1)+1)]
        end
	rpar=[rpar;matrix(mo.parameters(2)(l),-1,1)]
	ipar(k)=ipar(k)+size(mo.parameters(2)(l),'*')
        //** mo.parameters(3)(l) = 0 :that's a parameters
	if mo.parameters(3)(l)==0 then
	  //P=[P;mo.parameters(1)(l)+'='+Pars($)];
          if size(mo.parameters(2)(l),'*')==1 then
	    P=[P;mo.parameters(1)(l)+'='+Pars($)];
          else
            P=[P;mo.parameters(1)(l)+'={']
            for i=size(mo.parameters(2)(l),'*'):-1:1
              P($)=P($)+Pars($-i+1)
              if i<>1 then
                P($)=P($)+','
              end
            end
            P($)=P($)+'}'
          end
        //** mo.parameters(3)(l) = 1 :that's an initial condition
        //** of a state
	elseif mo.parameters(3)(l)==1 then
          if size(mo.parameters(2)(l),'*')==1 then
            P=[P;mo.parameters(1)(l)+'(start='+Pars($)+')'];
          else
            P=[P;mo.parameters(1)(l)+'(start={'];
            for i=size(mo.parameters(2)(l),'*'):-1:1
              P($)=P($)+Pars($-i+1)
              if i<>1 then
                P($)=P($)+','
              end
            end
            P($)=P($)+'})'
          end
        //** mo.parameters(3)(l) = 2 :that's an initial condition
        //** of an initially fixed state
	elseif mo.parameters(3)(l)==2 then
          if size(mo.parameters(2)(l),'*')==1 then
            P=[P;mo.parameters(1)(l)+'(start='+Pars($)+',fixed=true)'];
          else
            P=[P;mo.parameters(1)(l)+'(start={'];
            P_fix='fixed={'
            for i=size(mo.parameters(2)(l),'*'):-1:1
              P($)=P($)+Pars($-i+1)
              P_fix=P_fix+'true'
              if i<>1 then
                P($)=P($)+','
                P_fix=P_fix+','
              end
            end
            P($)=P($)+'},'+P_fix+'})'
          end
	end
      end
    end
    Bnumbers=[Bnumbers k];
    Bnames=[Bnames,'B'+string(k)];

    

    if P==[] then
      models=[models;'  ' + mo.model + ' ' + tab + 'B' + string(k) + ';']; //** 
    else
      //** This was the line source of all the problems (Allan) 
      models=[models;'  ' + mo.model + ' ' + tab + 'B' + string(k) + '(' + strcat(P,', ') + ');' ]; //**
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
      models=[models;'  InPutPort'+' '+tab+'B'+string(nb)+';']; //**
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
      models=[models;'  OutPutPort'+' '+tab+'B'+string(nb)+';']; //**
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
  
  if Pars<>[] then
    Pars='  parameter Real '+Pars+'(fixed=false);'
  end
  txt=[txt;
       'class '+name
       Pars
       models
       'equation'
       eqns
       'end '+name+';']

  

endfunction

  
