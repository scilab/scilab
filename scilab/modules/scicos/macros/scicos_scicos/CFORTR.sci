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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function [ok,tt]=CFORTR(funam,tt,inp,out)
    //
    ni=size(inp,"*")
    no=size(out,"*")
    if tt==[] then

        tete1=["#include <machine.h>";"#include <math.h>";...
        "void "+funam+"(flag,nevprt,t,xd,x,nx,z,nz,tvec,";..
        "             ntvec,rpar,nrpar,ipar,nipar"]

        tete2= "      "
        decl=""
        for i=1:ni
            tete2=tete2+",u"+string(i)+",nu"+string(i)
            decl=decl+",*nu"+string(i)
        end
        for i=1:no
            tete2=tete2+",y"+string(i)+",ny"+string(i)
            decl=decl+",*ny"+string(i)
        end
        tete2=tete2+")"

        tete3=["      double *t,xd[],x[],z[],tvec[];";..
        "      int *flag,*nevprt,*nx,*nz,*ntvec,*nrpar,ipar[],*nipar"+decl+";"]


        tete4= "      double rpar[]"
        for i=1:ni
            tete4=tete4+",u"+string(i)+"[]"
        end
        for i=1:no
            tete4=tete4+",y"+string(i)+"[]"
        end
        tetev=[" ";" "]

        textmp=[tete1;tete2;tetev;tete3;tete4+";";
        "/* modify below this line */";
        "{"
        "}"
        tetev];
    else
        textmp=tt;
    end

    while 1==1
        [txt]=x_dialog(["Function definition in C";
        "Here is a skeleton of the functions which you should edit"],..
        textmp);

        if txt<>[] then
            tt=txt
            [ok]=scicos_block_link(funam,tt,"c")
            if ok then
                textmp=txt;
            end
            break;
        else
            ok=%f;break;
        end
    end


endfunction
