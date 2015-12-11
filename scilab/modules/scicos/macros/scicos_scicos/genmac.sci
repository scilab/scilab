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

function mac=genmac(tt,nin,nout)
    [txt1,txt0,txt2,txt3,txt4,txt5,txt6]=tt(1:7)
    mac=null()
    //    [y,  x,  z,  tvec,xd]=func(flag,nevprt,t,x,z,rpar,ipar,u)
    blank="  "
    // if txt2_1<>' ' then
    //  txt2=[txt2;
    //      'if n_evi<>0 then'
    //         blank(ones(txt2_1))+txt2_1
    //      'end']
    //end
    semi=";"


    txt1=blank(ones(txt1))+txt1+semi(ones(txt1))
    txt2=blank(ones(txt2))+txt2+semi(ones(txt2))
    txt3=blank(ones(txt3))+txt3+semi(ones(txt3))
    txt4=blank(ones(txt4))+txt4+semi(ones(txt4))
    txt5=blank(ones(txt5))+txt5+semi(ones(txt5))
    txt6=blank(ones(txt6))+txt6+semi(ones(txt6))

    u="u"
    if nin>0 then
        get_u=blank+"["+strcat(u(ones(1,nin))+string(1:nin),",")+"]=u(:)"
    else
        get_u=[]
    end
    y="y"
    set_y=blank+"y=list("+strcat(y(ones(1,nout))+string(1:nout),",")+")"
    mac_txt=[
    get_u
    "select %_flag";
    "case 0 then";
    txt0
    "  t_evo=[]";
    "  y=list()";
    "case 1 then";
    txt1
    "  t_evo=[]";
    "  xd=[]";
    set_y;
    "case 2 then";
    txt2
    "  y=list()";
    "  t_evo=[]";
    "  xd=[]";
    "case 3 then";
    txt3
    "  xd=[]";
    "  y=list()";
    "case 4 then";
    txt4
    "  y=list()";
    "  t_evo=[]";
    "  xd=[]";
    "case 5 then";
    txt5
    "  y=list()";
    "  t_evo=[]";
    "  xd=[]";
    "case 6 then";
    txt6
    "  t_evo=[]";
    "  xd=[]";
    set_y
    "end";]

    deff("[xd,t_evo,z,x,y]=mac(%_flag,n_evi,t,x,z,rpar,ipar,u)",mac_txt)
endfunction
