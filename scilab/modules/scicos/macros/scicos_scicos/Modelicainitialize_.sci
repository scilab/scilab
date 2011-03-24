
function Modelicainitialize_()
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

// Copyright INRIA
  Cmenu=[]

  name=scs_m.props.title(1);
  if ~validvar(name) then 
    x_message([name;'is not a valid name, please change the title of the diagram.']);
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
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")

 
  %cpr=tlist(['cpr','corinv'],corinv)

  if last_name<>"" then unhilite_modelica_block(modelica_cind_from_name(last_name));end
  hilite_modelica_block(modelica_cind_from_name(name))

endfunction
