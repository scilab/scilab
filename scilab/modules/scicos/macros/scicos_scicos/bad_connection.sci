//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>
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

function bad_connection(path_out,prt_out,nout,outtyp,path_in,prt_in,nin,intyp,typ)
// alert for badly connected blocks
// path_out : Path of the "from block" in scs_m
// path_in  : Path of the "to block" in scs_m
// Alan, 28/12/06 : added rhs parameter : outtyp/intyp and typ flag.
// typ : a flag. If not present or equal to zero then
//               display a message concerning size.
//               Else if equal to 1 then display a message
//               concerning type.
//!

  if %scicos_debug_gr then
    disp("bad_connection...")
  end

  rhs = argn(2) ;
  if (rhs == 8) then typ=0, end

  if type(path_out)==15 then //set of modelica blocks
    // look for modelica bloc associated with prt_out
    outports=list()
    for b=path_out,
      path=list();
      for l=b(1:$-1),
	path($+1)=l;path($+1)='model';path($+1)='rpar';path($+1)='objs';
      end
      path($+1)=b($);
      if size(path)==1 then path=path(1),end
      mb=scs_m.objs(path)
      k=find(mb.graphics.out_implicit=='E')
      for kk=k,outports($+1)=path,end
    end
    path_out=outports(prt_out)
  end
  if type(path_in)==15 then //set of modelica blocks
    // look for modelica bloc associated with prt_in
    inports=list()
    for b=path_in,
      path=list();
      for l=b(1:$-1),
	path($+1)=l;path($+1)='model';path($+1)='rpar';path($+1)='objs';
      end
      path($+1)=b($);
      if size(path)==1 then path=path(1),end
      mb=scs_m.objs(path)
      k=find(mb.graphics.in_implicit=='E')
      for kk=k,inports($+1)=path,end
    end
    path_in=inports(prt_in)
  end

  //** save the current figure handle
  //gh_wins = gcf();

  if path_in==-1 then
    xcosShowBlockWarning(path_out);
    if typ==0 then
      message(['Hilited block has connected ports ';
               'with  incompatible sizes'])
    else
      message(['Hilited block has connected ports ';
               'with  incompatible types'])
    end
    xcosClearBlockWarning(path_out); //** new
    return;
  end

    //[lhs,rhs]=argn(0)
    if prt_in <> -1 then  //two connected blocks
        lp=mini(size(path_out,'*'),size(path_in,'*'))
        k=find(path_out(1:lp)<>path_in(1:lp))
        path=path_out(1:k(1)-1) // common superbloc path
        path_out=path_out(k(1)) // "from" block number
        path_in=path_in(k(1))   // "to" block number

        msg = [];
        if typ==0 then
            msg = ['Hilited block(s) have connected ports ';
            'with  incompatible sizes';
            ' output port '+string(prt_out)+' size is :'+sci2exp(nout);
            ' input port '+string(prt_in)+' size is  :'+sci2exp(nin)];
        else
            msg = ['Hilited block(s) have connected ports ';
            'with  incompatible type';
            ' output port '+string(prt_out)+' type is :'+sci2exp(outtyp);
            ' input port '+string(prt_in)+' type is  :'+sci2exp(intyp)];
        end

        if path==[] then
            errorDiagramPath(path, [path_out, path_in], msg, "", %t);
        else
            errorDiagramPath(path, [path_out, path_in], msg, "", %t);
        end
    else // connected links do not verify block contraints
        mess=prt_out;
        if type(path_out)==15 then //problem with implicit block
            message('Problem with the block generated from modelica blocks')
        else
            path=path_out(1:$-1) // superbloc path
            path_out=path_out($) //  block number
            errorDiagramPath(path, path_out, mess, "", %t);
        end
    end
endfunction
