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
function [ok,modelicac,translator,xml2modelica]=Modelica_execs()

  ok=%f
  
  if MSDOS then
    modelicac=pathconvert(SCI+'/bin/modelicac.exe',%f,%t)         
    translator=pathconvert(SCI+'/bin/translator.exe',%f,%t) 
    xml2modelica=pathconvert(SCI+'/bin/XML2Modelica.exe',%f,%t)
  else
    modelicac=pathconvert(SCI+'/modules/scicos/modelicac',%f,%t)
    translator=pathconvert(SCI+'/modules/scicos/translator',%f,%t)
    xml2modelica=pathconvert(SCI+'/modules/scicos/xml2modelica',%f,%t)
  end
  
  ok =%t
  
//  
//  OS=getos() 
//  select OS 
//    case 'Windows' then 
//     modelicac=pathconvert(SCI+'/bin/modelicac.exe',%f,%t)         
//     translator=pathconvert(SCI+'/bin/translator.exe',%f,%t) 
//     xml2modelica=pathconvert(SCI+'/bin/XML2Modelica.exe',%f,%t)
//     ok =%t
//   case 'Linux' then 
//    modelicac=pathconvert(SCI+'/bin/modelicac',%f,%t)    
//    translator=pathconvert(SCI+'/bin/translator',%f,%t)
//    xml2modelica=pathconvert(SCI+'/bin/XML2Modelica',%f,%t)
//    ok =%t
//   case 'Darwin' then  // Mac
//    modelicac=pathconvert(SCI+'/bin/modelicac',%f,%t)    
//    translator=pathconvert(SCI+'/bin/translator_mac',%f,%t)
//    xml2modelica=pathconvert(SCI+'/bin/XML2Modelica_mac',%f,%t)
//    ok =%t
//  end
//
//  if ~ok then 
//    x_message(['The Modelica compiler is not available for the '+OS+' operating system'])
//    return
//  end
  
  if strindex(modelicac,' ')<>[] then modelicac='""'+modelicac+'""',end
  if strindex(translator,' ')<>[] then translator='""'+translator+'""',end
  if strindex(xml2modelica,' ')<>[] then xml2modelica='""'+xml2modelica+'""',end

  if (fileinfo(modelicac)==[])    then 
    messagebox([_('Scilab cannot find the Modelica compiler:');modelicac],'error','modal');
    ok=%f;return;
  end
  if (fileinfo(translator)==[])   then 
    messagebox([_('Scilab cannot find the Modelica translator:');translator],'error','modal');
    ok=%f;return;
  end
  if (fileinfo(xml2modelica)==[]) then
    messagebox([_('Scilab cannot find the XML to modelica convertor:'); 
	       xml2modelica],'error','modal');
    ok=%f;return;
  end

  endfunction
 
 
