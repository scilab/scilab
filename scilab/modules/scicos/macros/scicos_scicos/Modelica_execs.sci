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
function [ok,modelicac,translator,xml2modelica] = Modelica_execs()
  // any old scicos macros, will be removed on 5.4.0
  warnobsolete(scilabRemovedVersion="5.4.0")


  ok = %f;

  MODELICAC_FILENAME = 'modelicac';
  TRANSLATOR_FILENAME = 'modelicat';
  XML2MODELICA_FILENAME = 'XML2Modelica';

  if getos() == 'Windows' then
    MODELICAC_FILENAME = MODELICAC_FILENAME + '.exe';
    TRANSLATOR_FILENAME = TRANSLATOR_FILENAME + '.exe';
    XML2MODELICA_FILENAME = XML2MODELICA_FILENAME + '.exe';
  end
  
  modelicac = pathconvert(getmodelicacpath() + MODELICAC_FILENAME, %f, %t);
  translator = pathconvert(getmodelicacpath() + TRANSLATOR_FILENAME, %f, %t);
  xml2modelica = pathconvert(getmodelicacpath() + XML2MODELICA_FILENAME, %f, %t);
  
  ok = %t;
  
  if strindex(modelicac,' ') <> [] then 
    modelicac = '""' + modelicac + '""';
  end

  if strindex(translator,' ') <> [] then 
    translator = '""' + translator + '""';
  end

  if strindex(xml2modelica,' ') <> [] then 
    xml2modelica = '""' + xml2modelica + '""'
  end

  if (fileinfo(modelicac) == []) then 
    messagebox([_('Scilab cannot find the Modelica compiler:'); modelicac], 'error', 'modal');
    ok = %f;
    return;
  end

  if (fileinfo(translator) == [])   then 
    messagebox([_('Scilab cannot find the Modelica translator:'); translator], 'error', 'modal');
    ok = %f;
    return;
  end

  if (fileinfo(xml2modelica) == []) then
    messagebox([_('Scilab cannot find the XML to modelica convertor:'); 
	       xml2modelica],'error','modal');
    ok = %f;
    return;
  end

endfunction
 

