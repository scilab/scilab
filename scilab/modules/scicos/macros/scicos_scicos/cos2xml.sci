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
function [ok, t] = cos2xml(scs_m, name, atomicflag)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

 ok = %t;
 t=[];
 ft = format();
 format(20);
 if argn(2)<3 then atomicflag=%f,end
 if exists('scicos_scicoslib') ==0 then load("SCI/modules/scicos/macros/scicos_scicos/lib"),end
 exec(loadpallibs,-1) //to load the palettes libraries
 Bn='OBJ';
 tit=scs_m.props.title(1);
 version=scs_m.version
 if exists('supername') then
   Bn=supername+'_',
   if atomicflag then flg='yes'; else flg='no';end
   head=['   <Diagram Name=""'+tit+'"">'
         '   <ScicosVersion Name=""'+version+'"" />'
         '   <AtomicDiagram Atomic=""'+flg+'"" />']
   tail=['   </Diagram>']
   [%scicos_context,ierr]=script2var(scs_m.props.context,%scicos_context)
 else
   head=['<?xml version=""1.0"" encoding=""UTF-8""?>'
         ' <ScicosModel Name=""'+tit+'"" >'
         '   <Diagram Name=""'+tit+'"">'
         '   <ScicosVersion Name=""'+version+'"" />']
   tail=[
       '    </Diagram>'
       ' </ScicosModel>']
   [%scicos_context,ierr]=script2var(scs_m.props.context,struct())
 end
 p=scs_m.props;
 xml_txt=block2xml(p);
 t=[t;
    '<Parameters>';
    '  '+xml_txt;
    '</Parameters>']
 objs=scs_m.objs
 t=[t;
    '<Objects>'];
 for k=1:size(objs)
   name=Bn+string(k)
   o=objs(k)
   x=getfield(1,o);
   if x(1)=='Block' then
    //disp('it is block');//pause
    if o.model.sim=='super'|o.model.sim=='csuper'|o.model.sim(1)=='asuper' then
    supername=name
    t=[t;
        '<Block Name=""'+name+'"" >']
    if o.model.sim(1)=='asuper' then
      atomicflag=%t;
      [ok,t1]=cos2xml(o.model.rpar,name,%t);
    else
      atomicflag=%f;
      [ok,t1]=cos2xml(o.model.rpar,name,%f);
    end
    if ~ok then t=[];ok=%f;return; end
    xml_txt=block2xml(o,2,t1,atomicflag)
    t=[t;
       xml_txt;
        '</Block>' ]
    else //standard block
      xml_txt=block2xml(o,1);
        t=[t;
        '<Block Name=""'+name+'"" >';
        xml_txt;
        '</Block>'];
    end
   elseif x(1)=='Deleted' then
     t=[t;'   <Deleted Name=""'+name+'""></Deleted>']
   elseif x(1)=='Text' then
     xml_txt=block2xml(o,3);
     t=[t;
        '<Text Name=""'+name+'"" >';
        xml_txt;
        '</Text>'];
   else //links
    xml_txt=block2xml(o);
     t=[t;
        '<Link Name=""'+name+'"" >';
        xml_txt;
        '</Link>'];
   end
 end //end of loop on objects
  t=[t;
    '</Objects>'];
  t=[head;'   '+t;tail]
  
  format(ft(2),ft(1));
  
endfunction

function xml_txt=block2xml(o,flag,t1,atomicflag)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

 xml_txt=[];format(20);
 x=getfield(1,o);
 for i=2:size(x,'*')
   if x(i)=='context' then
     xml_txt=[xml_txt;
     '   <'+x(1)+' id='''+x(i)+''' >';
     '      '+xml_subst(sci2exp(o(x(i)),0));
     '    </'+x(1)+'>'];
   elseif x(1)=='Link'|(x(1)=='params' & x(i)<>'options') then
     xml_txt=[xml_txt;
     '   <'+x(1)+' id='''+x(i)+''' value='''+sci2exp(o(x(i)),0)+''' />'];
   elseif x(i)=='doc'|x(i)=='void'| x(i)=='gui' then
     //S. Steer strcat added because of a bug in sci2exp for functions
     xml_txt=[xml_txt;
     ' <'+x(i)+' id='''+x(i)+''' value='''+strcat(sci2exp(o(x(i)),0))+''' />'];
   else
     if x(i)<>'model' then
       xml_txt=[xml_txt;
       ' <'+x(1)+' id='''+x(i)+''' >'];
     end
     xx=getfield(1,evstr('o.'+x(i)));
     for j=2:size(xx,'*')
       if x(i)=='model' & xx(j)<>'rpar'
       elseif xx(j)=='gr_i' then
     xml_txt=[xml_txt;
         '   <'+x(i)+' id='''+xx(j)+'''>';
         '      '+xml_subst(sci2exp(o(x(i))(xx(j)),0));
         '   </'+x(i)+'>'];
       elseif xx(j)=='exprs' then
     if flag==2 & atomicflag then
       xml_txt=[xml_txt;
           '   <'+x(i)+' id='''+xx(j)+'''>';
           '       <p>'+xml_subst(sci2exp(o(x(i))(xx(j))(1),90))+'</p>';
           '   </'+x(i)+'>'];
     else
       xml_txt=[xml_txt;
           '   <'+x(i)+' id='''+xx(j)+'''>';
           '       <p>'+xml_subst(sci2exp(o(x(i))(xx(j)),90))+'</p>';
           '   </'+x(i)+'>'];
     end
       elseif xx(j)=='rpar' & flag==2 then
     xml_txt=[xml_txt;
         ' <Block  id=''diagram'' >';
         '      '+t1;
         ' </Block>'];
       elseif xx(j)=='rpar' then
     xml_txt=[xml_txt;
         ' <'+x(1)+' id=''diagram'' >';
         ' </'+x(1)+'>'];
       elseif xx(j)=='3D' then
     xml_txt=[xml_txt;
         '   <'+x(i)+' id='''+xx(j)+''' value='''+sci2exp(o(x(i))(j),0)+''' />'];
       else
     xml_txt=[xml_txt;
         '   <'+x(i)+' id='''+xx(j)+''' value='''+xml_subst(strsubst(sci2exp(o(x(i))(xx(j)),0),' ',''))+''' />'];
       end
     end
     if x(i)<>'model' then
       xml_txt=[xml_txt;
       ' </'+x(1)+'>'];
     end
   end
 end
endfunction

function field=xml_subst(field)
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

  field=strsubst(field,'&','&amp;');
  field=strsubst(field,'<','&lt;');
  field=strsubst(field,'>','&gt;');
  field=strsubst(field,'""','&quot;');
  field=strsubst(field,'''','&apos;');
endfunction

