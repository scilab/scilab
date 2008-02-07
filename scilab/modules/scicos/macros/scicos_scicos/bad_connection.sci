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
  gh_wins = gcf();

  if path_in==-1 then
    //** hilite_obj(scs_m.objs(path_out)); //**
    hilite_obj(path_out); //** new
    if typ==0 then
      message(['Hilited block has connected ports ';
               'with  incompatible sizes'])
    else
      message(['Hilited block has connected ports ';
               'with  incompatible types'])
    end
    unhilite_obj(path_out); //** new
    return;
  end

  //[lhs,rhs]=argn(0)
  if prt_in <> -1 then  //two connected blocks
    lp=mini(size(path_out,'*'),size(path_in,'*'))
    k=find(path_out(1:lp)<>path_in(1:lp))
    path=path_out(1:k(1)-1) // common superbloc path
    path_out=path_out(k(1)) // "from" block number
    path_in=path_in(k(1))   // "to" block number

    if path==[] then
      //** hilite_obj(scs_m.objs(path_out)) //** set
      hilite_obj(path_out); //**
      if or(path_in<>path_out) then
          //** hilite_obj(scs_m.objs(path_in))
          hilite_obj(path_in)
      end

      if typ==0 then
        message(['Hilited block(s) have connected ports ';
                 'with  incompatible sizes';
                 ' output port '+string(prt_out)+' size is :'+sci2exp(nout);
                 ' input port '+string(prt_in)+' size is  :'+sci2exp(nin)]);
      else
        message(['Hilited block(s) have connected ports ';
                 'with  incompatible type';
                 ' output port '+string(prt_out)+' type is :'+sci2exp(outtyp);
                 ' input port '+string(prt_in)+' type is  :'+sci2exp(intyp)]);

      end
      unhilite_obj(path_out);
      if or(path_in<>path_out) then unhilite_obj(path_in),end
      //** hilite_obj(scs_m.objs(path_out))
      //** if or(path_in<>path_out) then hilite_obj(scs_m.objs(path_in)),end
    else
      mxwin=maxi(winsid())
//*****************************************
      for k=1:size(path,'*')
	//** hilite_obj(scs_m.objs(path(k))) //**
	hilite_obj(path(k)) ; //**
	scs_m=scs_m.objs(path(k)).model.rpar;
	scs_show(scs_m,mxwin+k)  //** WARNING !
      end
      //** hilite_obj(scs_m.objs(path_out)) //**
      hilite_obj(path_out) ; //**
      if or(path_in<>path_out) then
        //** hilite_obj(scs_m.objs(path_in))
        hilite_obj(path_in)
      end
//*****************************************
      if typ==0 then
        message(['Hilited block(s) have connected ports ';
                 'with  incompatible sizes';
                 string(prt_out)+' output port size is :'+sci2exp(nout);
                 string(prt_in)+' input port size is  :'+sci2exp(nin)]);
      else
        message(['Hilited block(s) have connected ports ';
                 'with  incompatible type';
                 ' output port '+string(prt_out)+' type is :'+sci2exp(outtyp);
                 ' input port '+string(prt_in)+' type is  :'+sci2exp(intyp)]);
      end
      for k=size(path,'*'):-1:1
        //** select the mxwin+k window and get the handle
        gh_del = scf(mxwin+k);
        //** delete the window
        delete(gh_del)
        //xdel(mxwin+k) //** WARNING !
      end

      //** restore the active window
      scf(gh_wins);

      //scs_m=null()
      //** unhilite_obj(scs_m.objs(path(1))) //** WARNING
      unhilite_obj(path(1))
    end
  else // connected links do not verify block contraints
    mess=prt_out;
    if type(path_out)==15 then //problem with implicit block
      message('Problem with the block generated from modelica blocks')
    else
      path=path_out(1:$-1) // superbloc path
      path_out=path_out($) //  block number
      if path==[] then
	//** hilite_obj(scs_m.objs(path_out)) ;//** set
	hilite_obj(path_out) ;//** set
	message(mess)
	//** hilite_obj(scs_m.objs(path_out)) //** clear
        unhilite_obj(path_out) ;
      else
	mxwin=maxi(winsid())
	for k=1:size(path,'*')
	  //** hilite_obj(scs_m.objs(path(k))) //**
	  hilite_obj(path(k)) ; //**
	  scs_m=scs_m.objs(path(k)).model.rpar;
	  scs_show(scs_m,mxwin+k) //**
	end
	//** hilite_obj(scs_m.objs(path_out)) //**
	hilite_obj(path_out) ; //**
	message(mess)
	for k=size(path,'*'):-1:1
          //**WARNING: xdel(mxwin+k) //** delete (mxwin+k) graphic window
          //** select the mxwin+k window and get the handle
          gh_del = scf(mxwin+k);
          //** delete the window
          delete(gh_del)
        end

        //** restore the active window
        scf(gh_wins);

        //scs_m=null()
        //** unhilite_obj(scs_m.objs(path(1))) //**
        unhilite_obj(path(1))
      end
    end
  end

  
endfunction
