//  Scicos
//
//  Copyright (C) INRIA - E. Demay - E.D.F - 1997
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

function standard_etiquette(bloc, legende, job)
//
// standard_etiquette - Draw legends on scicos blocks
//
//%Syntaxe
// standart_etiquette(bloc, legende, 'in')
// standart_etiquette(bloc, legende, 'out')
// standart_etiquette(bloc, legende, 'clkin')
// standart_etiquette(bloc, legende, 'clkout')
// standart_etiquette(bloc, legende, 'centre')
// standart_etiquette(bloc, couleur, 'croix')
//
//%Inputs
// bloc    : Scicos bloc data structure
// legende : vector of character strings to draw or color code for job='croix'
// job     : Character string specifies where legend has to be drawn
//           'in'     : input ports
//           'out'    : output ports
//           'clkin'  : event input ports
//           'clkout' : event output ports
//           'centre' : in the block shape
//           'croix'  : draw a cross in the block shape
//%Origine 
// E. Demay E.D.F 97
//


//= Initialisations ==
//GRAPHIQUE = 2;ORIGINE = 1;TAILLE = 2
macro = bloc.gui
orient=bloc.graphics.flip
select job
case 'in' then //= Ports d'entree ==
  
  execstr('[x, y, typ] = '+macro+'(''getinputs'', bloc)')
  x = x(find(typ > 0))
  y = y(find(typ > 0))

  for i = 1:size(legende,'*')
    rect = xstringl(0, 0, legende(i))
    if orient then //standard orientation
      xstring(x(i)-rect(3),y(i),legende(i))
    else //tilded orientation
      xstring(x(i)+rect(3)/2,y(i),legende(i))
    end
  end

case 'out' then //= Ports de sortie ==
  execstr('[x, y, typ] = '+macro+'(''getoutputs'', bloc)')
  x = x(find(typ > 0))
  y = y(find(typ > 0))
  for i = 1:size(legende,'*')
    rect = xstringl(0, 0, legende(i))
    if orient then //standard orientation
      xstring(x(i),y(i),legende(i))
    else //tilded orientation
      xstring(x(i)-rect(3),y(i),legende(i))
    end
  end
case 'clkin' then //= Port d'entree evenement ==
  execstr('[x, y, typ] = '+macro+'(''getinputs'', bloc)')
  x = x(find(typ == -1))
  y = y(find(typ == -1))
  for i = 1:size(legende,'*')
    rect = xstringl(0, 0, legende(i))
    xstring(x(i)-rect(3),y(i)+(i-1)*rect(4),legende(i))
  end
case 'clkout' then //= Ports de sortie evenement ==
  execstr('[x, y, typ] = '+macro+'(''getoutputs'', bloc)')
  x = x(find(typ == -1))
  y = y(find(typ == -1))
  for i = 1:size(legende,'*')
    rect = xstringl(0, 0, legende(i))
    xstring(x(i)-rect(3), y(i)-i*rect(4)*1.2,legende(i))
  end
case 'centre' then //= Centre du bloc ==
 // origine = bloc(GRAPHIQUE)(ORIGINE)
 // taille = bloc(GRAPHIQUE)(TAILLE)
  origine = bloc.graphics.orig
  taille = bloc.graphics.sz
  xstringb(origine(1), origine(2), legende, taille(1), taille(2), 'fill')
case 'croix' then //= Identification des bases de donnees ==
//  origine = bloc(GRAPHIQUE)(ORIGINE)
//  taille = bloc(GRAPHIQUE)(TAILLE)
  origine = bloc.graphics.orig
  taille = bloc.graphics.sz
  nx = [origine(1), origine(1)+taille(1); origine(1), origine(1)+taille(1)] 
  ny = [origine(2), origine(2)+taille(2); origine(2)+taille(2), origine(2)]
  dashes = xget('dashes')
  xsegs(nx', ny', legende)
  xset('dashes', dashes)
end
endfunction





