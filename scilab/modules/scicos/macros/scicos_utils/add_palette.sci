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
// See the file ./license.txt
//

function add_palette(Title,path)
//## Add a palette to the end of the list of the scicos palette.
//## If the palette exists with same Title and same path
//## noting is done. If a palette with same name but
//## different path exist then the palette is added
//## with a suffixe (x)
//## Note that scicos_pal variable is locally recreated
//## and passed to the upper environnement via
//## scicos_pal=resume(scicos_pal)
//##
//## Inputs :
//## Title : the title of the palette
//## path  : the path + name of the cos or cosf file
//##         that contains the palette

  if exists('scicos_pal') then
    scicos_pal=scicos_pal
  else
    scicos_pal=[]
  end
  path=pathconvert(path,%f,%t)
  if fileinfo(path)==[] then
    error('File: '+path+' do not exist')
  end
  if or(fileparts(path,'extension')==['.cos','.cosf']) then
    //############################
    k=find(scicos_pal(:,1)==Title)
    if k==[] then
      scicos_pal=[scicos_pal;Title,path]
    else //## if a palette with same title exist
      k1=find(scicos_pal(k,2)==path)
      if k1==[] then //## not the same path
        indt=1;
        while (find(scicos_pal(:,1)==Title+' ('+string(indt)+')')<>[]) then
          indt=indt+1
        end
        kk=find(scicos_pal(:,1)==Title+' ('+string(indt-1)+')')
        kk1=find(scicos_pal(kk,2)==path)
        if kk1==[] then
          scicos_pal=[scicos_pal;Title+' ('+string(indt)+')',path]
        end
      end
    end
    //############################
    scicos_pal=resume(scicos_pal)
  else
    error('Second argument should be a path to a .cos or .cosf file.')
  end
endfunction

