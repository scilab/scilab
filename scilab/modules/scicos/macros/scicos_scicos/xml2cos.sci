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

function [scs_m,atomicflg]=xml2cos(xmlfilename,typ)
 atomicflg=%f;
 [lhs,rhs]=argn(0)
 if exists('scicos_scicoslib')==0 then load("SCI/modules/scicos/macros/scicos_scicos/lib") ;,end
 exec(loadpallibs,-1) //to load the palettes libraries
 if rhs<2 then
   typ=[];
   global txtline
   txtline=1
   flag=%f;
   txt=mgetl(xmlfilename)
 end
 while and(typ<>'</Diagram>') do
    if typ(1)=='<Diagram' then
     scs_m=scicos_diagram();
   elseif typ(1)=='<ScicosVersion' then
     ttyp=tokens(typ(2),'""')
     if size(ttyp,'*')<2 then
       scs_m.version="scicos4.3"
     else
       scs_m.version=ttyp(2);
     end
   elseif typ(1)=='<AtomicDiagram' then
     ttyp=tokens(typ(2),'""');
     if ttyp(2)=='yes' then atomicflg=%t; else atomicflg=%f;end
   elseif typ(1)=='<Parameters>' then
     scs_m.props=scicos_params();
     t=read_new_line(txt);
     typ=tokens(t);
     while typ(1)<>'</Parameters>' do
       ttyp=tokens(typ(2),'''')
       //disp(ttyp(2));
       if ttyp(2)=='context' then
	 t=read_new_line(txt);
	 cntxt=cos_subst(t);
	 scs_m.props.context=evstr(cntxt);
       elseif ttyp(2)=='options' then
         scs_m=load_par('</params>','options','props')
       else
	 indx1=strindex(t,'value=''');indx1=indx1+6;
	 indx2=length(t)-4;
	 tttyp=strsplit(t,[indx1,indx2]);
	 execstr('scs_m.props.'+ttyp(2)+'='+tttyp(2))
       end
       t=read_new_line(txt);
       typ=tokens(t);
       if typ(1)=='</params>' then
	 t=read_new_line(txt);
         typ=tokens(t);         
       end
     end
   elseif typ(1)=='<Objects>' then
     t=read_new_line(txt);
     typ=tokens(t);
     while typ(1)<>'</Objects>' do
     if typ(1)=='<Block' then
       scs_m=treat_blocks('</Block>',typ)
     elseif typ(1)=='<Text' then
       scs_m=treat_blocks('</Text>',typ)
     elseif typ(1)=='<Link' then
       numb=get_numb(typ);
       execstr('scs_m.objs('+numb+')=scicos_link()');
       t=read_new_line(txt);
       typ=tokens(t);
       while typ(1)<>'</Link>' do
         ttyp=tokens(typ(2),'''')
	 indx1=strindex(t,'value=''');indx1=indx1+6;
	 indx2=length(t)-4;
	 tttyp=strsplit(t,[indx1,indx2]);
         execstr('scs_m.objs('+numb+').'+ttyp(2)+'='+tttyp(2));
	 t=read_new_line(txt);
         typ=tokens(t);
       end    
     elseif typ(1)=='<Deleted' then
       numb=get_numb(typ);
       execstr('scs_m.objs('+numb+')=mlist(''Deleted'')');
     end
     t=read_new_line(txt);
     typ=tokens(t);
     end
   end
   t=read_new_line(txt);
   typ=tokens(t);   
 end
 if rhs<2 then
   clearglobal txtline;
 end
endfunction

function field=cos_subst(field)
  field=strsubst(field,'&amp;','&');
  field=strsubst(field,'&lt;','<');
  field=strsubst(field,'&gt;','>');
  field=strsubst(field,'&quot;','""');
  field=strsubst(field,'&apos;','''');
  field=strsubst(field,'<p>','');
  field=strsubst(field,'</p>','');
endfunction

function scs_m=load_par(symbol,id1,id2)
  scs_m=scs_m;global txtline
  t=read_new_line(txt);
  typ=tokens(t);
  while typ(1)<>symbol do
    //disp(typ(1))
    if typ(1)=='<Diagram' then
 //disp('here')
      [scs_m1,atomicflg]=xml2cos(xmlfilename,typ)
      execstr('scs_m.'+id2+'.model.rpar=scs_m1');
      if atomicflg then execstr('scs_m.'+id2+'.model.sim=list(""asuper"",2004)');end
      t=read_new_line(txt);
      typ=tokens(t);
    else
      ttyp=tokens(typ(2),'''')
      //disp(ttyp(2));
      if ttyp(2)=='rpar' & size(typ,'*')==2 then  // is kept for compatibility
	if symbol=='</Block>' then
	  scs_m1=xml2cos(xmlfilename,typ)
	  execstr('scs_m.'+id2+'.'+id1+'('''+ttyp(2)+''')='+sci2exp(scs_m1,0));
	else
	  t=read_new_line(txt);
	  execstr('scs_m.'+id2+'.'+id1+'('''+ttyp(2)+''')='+cos_subst(t));
	end
      elseif ttyp(2)=='gr_i' then
	t=read_new_line(txt);
	execstr('scs_m.'+id2+'.'+id1+'('''+ttyp(2)+''')='+cos_subst(t));
      elseif ttyp(2)=='exprs' then
	exprstxt=[];
	t=read_new_line(txt);
	typ=tokens(t);
        indx_end=grep(txt,'</graphics>');
        ind=find(indx_end>txtline);ind=ind(1);
        indx_end=indx_end(ind);
        exprstxt=cos_subst(txt(txtline:indx_end-1))
        txtline=indx_end;
	if size(exprstxt,'*')>1 then
	  unit=file('open',TMPDIR+'/exprstxt.sce','unknown');
	  exprstxt=['scs_m.'+id2+'.'+id1+'('''+ttyp(2)+''')=..';exprstxt]
	  write(unit,exprstxt);
	  file('close',unit);
	  execstr('exec(""'+TMPDIR+'/exprstxt.sce"",-1);');
	else
	  execstr('scs_m.'+id2+'.'+id1+'('''+ttyp(2)+''')='+exprstxt);
	end 
      else
	indx1=strindex(t,'value=''');indx1=indx1+6;
	indx2=length(t)-4;
	tttyp=strsplit(t,[indx1,indx2]);
	//tttyp=tokens(typ(3),'''')
	//disp(tttyp(2));
	execstr('scs_m.'+id2+'.'+id1+'('''+ttyp(2)+''')='+cos_subst(tttyp(2)))
      end
      t=read_new_line(txt);
      typ=tokens(t);
      if or(typ==['</graphics>','</model>']) then
	t=read_new_line(txt);
	typ=tokens(t);
      end
    end
  end
endfunction

function numb=get_numb(typ)
 ttyp=tokens(typ(2),'''')
 ttyp=tokens(ttyp(1),'""')
 //disp(ttyp(2))
 //numb=strsplit(ttyp(2),[1:length(ttyp(2))-1])
 ind=strindex(ttyp(2),'_');
 //ind=grep(numb,'_')
 if ind==[] then
   numb=strsplit(ttyp(2),3);
 else
   numb=strsplit(ttyp(2),ind($));  
 end
 numb=numb(2);
endfunction

function scs_m=treat_blocks(symbol,typ)
 scs_m=scs_m;global txtline
 numb=get_numb(typ);
 if symbol=='</Block>' then
   execstr('scs_m.objs('+numb+')=scicos_block();');
 else
   execstr('scs_m.objs('+numb+')=scicos_text();');
 end
 t=read_new_line(txt);
 typ=tokens(t);
 while typ(1)<>symbol do
   ttyp=tokens(typ(2),'''')
   //disp(ttyp(2))
   //disp(symbol)
   if or(ttyp(2)==['graphics','model','diagram']) then
     //if ttyp(2)=='diagram' then pause;end
     scs_m=load_par(symbol,ttyp(2),'objs('+numb+')');
   //elseif ttyp(2)=='void' then
 else
   //aboif ttyp(2)=='gui' then pause;end
     indx1=strindex(t,'value=''');indx1=indx1+6;
     indx2=length(t)-4;
     tttyp=strsplit(t,[indx1,indx2]);
     //tttyp=tokens(typ(3),'''')
     execstr('scs_m.objs('+numb+').'+ttyp(2)+'='+cos_subst(tttyp(2)))
   end
   t=read_new_line(txt);
   typ=tokens(t);
 end
endfunction

function blk=scicos_text(v1,v2,v3,v4,v5)
// Copyright INRIA
//Block data structure initialization
  if exists('graphics','local')==0 then graphics=scicos_graphics(),end
  if exists('model','local')==0 then model=scicos_model(),end
  if exists('void','local')==0 then void='',end
  if exists('gui','local')==0 then gui='',end
  
  blk=mlist(['Text','graphics','model','void','gui'],graphics,model,void,gui)
endfunction

function t=read_new_line(txt)
global txtline
txtline=txtline+1;
t=txt(txtline)
endfunction

