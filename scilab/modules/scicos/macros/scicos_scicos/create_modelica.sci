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

function txt=create_modelica( blklst,cmat,name)
  txt=[];tab=ascii(9)
  models=[]
  eqns=[]
  Pin=[]
  Bnumbers=[]
  Bnames=[]
  nb=size(blklst)
  for k=1:nb
    o=blklst(k);
    mo=o.equations;
    np=size(mo.parameters(1),'*');
    P=[];
    if size(mo.parameters)==2 then
      for l=1:np
	P=[P;mo.parameters(1)(l)+'='+value2modelica(mo.parameters(2)(l))];
      end
    else
      for l=1:np
	if mo.parameters(3)(l)==0 then
	  P=[P;mo.parameters(1)(l)+'='+value2modelica(mo.parameters(2)(l))];
	elseif mo.parameters(3)(l)==1 then
	  P=[P;mo.parameters(1)(l)+'(start='+value2modelica(mo.parameters(2)(l))+')'];
	elseif mo.parameters(3)(l)==2 then
	  P=[P;mo.parameters(1)(l)+'(start='+value2modelica(mo.parameters(2)(l))+',fixed=true)'];  
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
       models
       'equation'
       eqns
       'end '+name+';']
endfunction

  
