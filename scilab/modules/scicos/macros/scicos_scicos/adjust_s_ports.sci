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

function [ok,sbloc]=adjust_s_ports(sbloc)

graphics=sbloc.graphics;
model=sbloc.model;

nin=0;nout=0;nclkin=0;nclkout=0;
in=[],out=[],in2=[],out2=[],cin=[],cout=[];
intyp=[];outtyp=[];
inp=[],outp=[],cinp=[],coutp=[];
out_implicit=[];in_implicit=[];
//INIMPL_f';'IMP2';'OUTIMPL_f

scs_m=model.rpar
for k=1:lstsize(scs_m.objs)
  o=scs_m.objs(k)
  if typeof(o)=='Block' then
    modelb=o.model
    select o.gui
    case 'IN_f' then
      nin=nin+1
      inp=[inp o.model.ipar]
      in=[in;o.model.out]
      in2=[in2;o.model.out2]
      intyp=[intyp;o.model.outtyp]
    case 'OUTIMPL_f' then
      nout=nout+1
      outp=[outp o.model.ipar]
      out=[out;o.model.in]
      out2=[out2;o.model.in2]
      outtyp=[outtyp;o.model.intyp]
      // graphics.out_implicit=[graphics.out_implicit;'I']
      out_implicit=[out_implicit; o.model.ipar]//RN corrected, need checking
    case 'OUT_f' then
      nout=nout+1
      outp=[outp o.model.ipar]
      out=[out;o.model.in]
      out2=[out2;o.model.in2]
      outtyp=[outtyp;o.model.intyp]
    case 'INIMPL_f' then
      nin=nin+1
      inp=[inp o.model.ipar]
      in=[in;o.model.out]
      in2=[in2;o.model.out2]
      intyp=[intyp;o.model.outtyp]
      //graphics.in_implicit=[graphics.in_implicit;'I']
      in_implicit=[in_implicit;o.model.ipar]  //RN corrected, need checking
    case 'CLKIN_f' then
      nclkin=nclkin+1
      cinp=[cinp o.model.ipar]
      cin=[cin;o.model.evtout]
    case 'CLKINV_f' then
      nclkin=nclkin+1
      cinp=[cinp o.model.ipar]
      cin=[cin;o.model.evtout]
    case 'CLKOUT_f' then
      nclkout=nclkout+1
      coutp=[coutp o.model.ipar]
      cout=[cout;o.model.evtin]
    case 'CLKOUTV_f' then
      nclkout=nclkout+1
      coutp=[coutp o.model.ipar]
      cout=[cout;o.model.evtin]
    end
  end
end


ok=%t
mess=[]
if nin>0 then
  [inp,k]=gsort(-inp)
  if ~and(inp==-(1:nin)) then
    mess=[mess;
          'Super_block input ports must be numbered';
          'from 1 to '+string(nin);' ']
    ok=%f
  end
  in=in(k,:)
  in2=in2(k,:)
end
if nout>0 then
  [outp,k]=gsort(-outp)
  if ~and(outp==-(1:nout)) then
    mess=[mess;
          'Super_block output ports must be numbered';
          'from 1 to '+string(nout);' ']
    ok=%f
  end
  out=out(k,:)
  out2=out2(k,:)
end


if nclkin>0 then
  [cinp,k]=gsort(-cinp)
  if ~and(cinp==-(1:nclkin)) then
    mess=[mess;
          'Super_block event input ports must be numbered';
          'from 1 to '+string(nclkin);' ']
    ok=%f
  end
  cin=cin(k)
end
if nclkout>0 then
  [coutp,k]=gsort(-coutp)
  if ~and(coutp==-(1:nclkout)) then
    mess=[mess;
          'Super_block event output ports must be numbered';
          'from 1 to '+string(nclkout);' ']
    ok=%f
  end
  cout=cout(k)
end
if ok then
  //21/06/07 : Alan : add set_io instead of check_io
  [model,graphics,ok]=set_io(model,graphics,...
                             list([in in2],intyp),...
                             list([out out2],outtyp),...
                             cin,cout,...
                             in_implicit,out_implicit)
else
  message(mess)
end

if ok then
  model.rpar=scs_m
  sbloc.model=model;sbloc.graphics=graphics;
end

endfunction
