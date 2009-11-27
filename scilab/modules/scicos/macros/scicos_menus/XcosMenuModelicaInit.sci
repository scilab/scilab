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
function XcosMenuModelicaInit()
// Copyright INRIA
  Cmenu=[]

  name=scs_m.props.title(1);
  if ~validvar(name) then 
    messagebox(msprintf(_("%s is not a valid name, please change the title of the diagram."),name),'error','modal');
    return
  end
  
  name=stripblanks(name)+'_im';
  path=TMPDIR+'/';
  path=pathconvert(stripblanks(path),%t,%t)
  

  mofile=path+name+'.mo';
  xmlfile=path+name+'f_init.xml';
//========================================
  compile=%f;
  
  [info,err1]=fileinfo(xmlfile);
  [info,err2]=fileinfo(mofile);
  
  if (err1==0 & err2==0) then,   
    if (newest(xmlfile,mofile)==2) then compile=%t;end;
  else 
    compile=%t;
  end

  compile=%t; // Very conservative

  if (needcompile>=2) then
    compile=%t;// needcompile=2: when context changes  
	       // needcompile=4: when model chanegs & it's not compiled
  end
  ok=%t
  //if (fileinfo(xmlfile)==[]) then 
    needcompile=4;
  //end
  
  if compile then 
    %Modelica_Init=%t
    // in order to generate *_im.mo -> *_im_f.mo -> *_im.xml 
    [bllst,connectmat,clkconnect,cor,corinv,ok]=c_pass1(scs_m);    
    %Modelica_Init=%f
  end
  [info,err1]=fileinfo(xmlfile);
  
  if err1==0 then 
    scimihm xmlfile
  end  
  
endfunction

function  Doubleclick(name,last_name)
 
  %cpr=tlist(['cpr','corinv'],corinv)

  if last_name<>"" then unhilite_modelica_block(modelica_cind_from_name(last_name));end
  hilite_modelica_block(modelica_cind_from_name(name))

endfunction
