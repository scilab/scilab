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

function s_port_names(sbloc)

  scs_m=sbloc.model.rpar;
  
  etiquettes_in = []
  etiquettes_out = []
  etiquettes_clkin = []
  etiquettes_clkout = []
  font=xget('font')

  xset('font',options.ID(2)(1),options.ID(2)(2))
  inp=[],outp=[],cinp=[],coutp=[]
  for k=1:lstsize(scs_m.objs)
    o=scs_m.objs(k);

    if typeof(o)=='Block' then
      modelb=o.model;
      ident = o.graphics.id
      //if ident<>emptystr()&ident<>[] then
	select o.gui
	  case 'IN_f' then
	  inp=[inp modelb.ipar];
	  etiquettes_in = [etiquettes_in; ident];
          case  'INIMPL_f'  then
          inp=[inp modelb.ipar];
	  etiquettes_in = [etiquettes_in; ident];
	  case 'OUT_f' then
	  outp=[outp modelb.ipar];
      	  etiquettes_out = [etiquettes_out; ident];
          case 'OUTIMPL_f' then
          outp=[outp modelb.ipar];
      	  etiquettes_out = [etiquettes_out; ident];
	  case 'CLKIN_f' then
	  cinp=[cinp modelb.ipar];
	  etiquettes_clkin = [etiquettes_clkin; ident];
	  case 'CLKINV_f' then
	  cinp=[cinp modelb.ipar];
	  etiquettes_clkin = [etiquettes_clkin; ident];
	  case 'CLKOUT_f' then
	  coutp=[coutp modelb.ipar];
	  etiquettes_clkout = [etiquettes_clkout; ident];
	  case 'CLKOUTV_f' then
	  coutp=[coutp modelb.ipar];
	  etiquettes_clkout = [etiquettes_clkout; ident];
	end
      //end
    end
  end
  if inp<>[] then
    [tmp,n_in]=gsort(-inp)
    standard_etiquette(sbloc, etiquettes_in(n_in), 'in')
  end

  if outp<>[] then
    [tmp,n_out]=gsort(-outp)
    standard_etiquette(sbloc, etiquettes_out(n_out), 'out')
  end
  if cinp<>[] then
    [tmp,n_cin]=gsort(-cinp)
    standard_etiquette(sbloc, etiquettes_clkin(n_cin), 'clkin')
  end
  if coutp<>[] then
    [tmp,n_cout]=gsort(-coutp)
    standard_etiquette(sbloc, etiquettes_clkout(n_cout), 'clkout')
  end
  xset('font',font(1),font(2))
endfunction
