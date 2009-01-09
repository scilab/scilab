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

function scs_m = do_doc(scs_m,%pt)
  xc=%pt(1);yc=%pt(2);
  k=getobj(scs_m,[xc;yc])
  if k==[] then return,end

  numero_objet=k
  scs_m_save=scs_m

  objet = scs_m.objs(numero_objet)
  type_objet = typeof(objet)

  //
  if type_objet == 'Block' then
    documentation = objet.doc
    if documentation == []|documentation == list() then
      rep=messagebox(['No documentation function specified'
		     'would you like to use standard_doc ?'],"modal","question",['yes','no'])
      funname='standard_doc'
      if rep==2 then
	[ok, funname] = getvalue('Enter the name of the documentation function',..
				 'fun name',list('str', 1),'standard_doc')
	if ~ok then return,end
      end
      doc=[]
      ierr=execstr('docfun='+funname,'errcatch')
      if ierr<>0 then
	messagebox('function '+funname+' not found',"modal","error");
	return
      end
      documentation=list(docfun,doc)
    end
    funname=documentation(1);doc=documentation(2)
    if type(funname)==10 then 
      ierr=execstr('docfun='+funname,'errcatch')
      if ierr<>0 then
	messagebox('function '+funname+' not found',"modal","error");
	return
      end
    else
      docfun=funname
    end
    
    ierr=execstr('doc=docfun(''set'',doc)','errcatch')
    ok=ierr==0
    if ok then
      documentation(2)=doc
      objet.doc = documentation
      scs_m.objs(numero_objet) = objet
    else
      messagebox(documentation(1)+'(''set'',...) failed',"modal","error");
    end
  else
    messagebox('It is impossible to set Documentation for this type of object',"modal","error");
  end
  //
  if ok then [scs_m_save,enable_undo,edited]=resume(scs_m_save,%t,%t),end

endfunction
function doc=standard_doc(job,doc)
  if job=='set' then
    if type(doc)<>10 then doc=' ',end
    text=dialog('You may enter here the document for this block ',doc)
    if text<>[] then doc=text,end
  elseif job=='get' then
    return
  end

endfunction
function doc=complex_doc(job,doc)

  if job=='set' then
    if type(doc)<>16 then 
      doc=list(' ',' ',' '),
    end
    d3=[]
    t3=doc(3)
    for k=1:size(t3,1)
      d3=[d3 ascii(t3(k)) 48]
    end
    d3=ascii(d3)
    
    text=x_mdialog('You may enter here the document for this block ',..
		   ['Author','Date','Comments'],[doc(1);doc(2);d3])
    

    if text<>[] then 
      doc(1)=text(1)
      doc(2)=text(2)
      t3=[ascii(text(3)) 48]
      k=find(t3==48)
      tt=[]
      k1=1
      for i=1:size(k,'*')-1
	tt=[tt;ascii(t3(k1:k(i)-1))]
	k1=k(i+1)
      end  
      doc(3)=tt
    end
  elseif job=='get' then
    doc=[
	'Author  :'+doc(1)
	'Date    :'+doc(2)
	'Comments:'
	'         '+doc(3)
	]
    return
  end
endfunction
