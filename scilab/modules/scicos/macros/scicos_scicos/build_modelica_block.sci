//  Scicos
//
//  Copyright (C) INRIA - Serge Steer <serge.steer@inria.fr> - 2003
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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function [model,ok]=build_modelica_block(blklstm,corinvm,cmmat,NiM,NoM,NvM,scs_m,path)
    // given the blocks definitions in blklstm and connections in cmmat this
    // function first create  the associated modelicablock  and writes its code
    // in the file named 'imppart_'+name+'.mo' in the directory given by path
    // Then modelica compiler is called to produce the C code of scicos block
    // associated to this modelica block. filbally the C code is compiled and
    // dynamically linked with Scilab.
    // The correspondind model data structure is returned.


    //## get the name of the generated main modelica file
    name=stripblanks(scs_m.props.title(1))+"_im";

    //## generation of the txt for the main modelica file
    //## plus return ipar/rpar for the model of THE modelica block
    [txt,rpar,ipar]=create_modelica(blklstm,corinvm,cmmat,NvM,name,scs_m);

    //## write txt in the file path+name+'.mo'
    path=pathconvert(stripblanks(path),%t,%t)
    mputl(txt,path+name+".mo");
    mprintf("%s\n",["--------------------------------------------\";
    msprintf(_(" Main Modelica : %s"),path+name+".mo")
    ""])

    //## search for

    Mblocks = [];
    for i=1:lstsize(blklstm)
        if type(blklstm(i).sim)==15 then
            if blklstm(i).sim(2)==30004 then
                o = scs_m(scs_full_path(corinvm(i)))
                Mblocks=[Mblocks;
                o.graphics.exprs.nameF]
            end
        end
    end

    //generating XML and Flat_Model
    //## compile modelica files
    [ok,name,nipar,nrpar,nopar,nz,nx,nx_der,nx_ns,nin,nout,nm,ng,dep_u]=compile_modelica(path+name+".mo",Mblocks);

    if ~ok then return,end

    //nx is the state dimension
    //ng is the number of surfaces
    //name1 of the model+flat

    //build model data structure of the block equivalent to the implicit part
    model=scicos_model(sim=list(name,10004),..
    in=ones(nin,1),out=ones(nout,1),..
    state=zeros(nx*2,1),..
    dstate=zeros(nz,1),..
    rpar=rpar,..
    ipar=ipar,..
    dep_ut=[dep_u %t],nzcross=ng,nmode=nm)
endfunction
