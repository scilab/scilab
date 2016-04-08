//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//  Copyright (C) DIGITEO - 2010 - Jérôme PICARD
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

function scs_m=do_version(scs_m,version)

    // available versions of Scicos
    availableVersions = [ ..
    "scicos2.2"; ..
    "scicos2.3"; ..
    "scicos2.3.1"; ..
    "scicos2.4"; ..
    "scicos2.5.1"; ..
    "scicos2.7"; ..
    "scicos2.7.1"; ..
    "scicos2.7.2"; ..
    "scicos2.7.3"; ..
    "scicos4"; ..
    "scicos4.0.1"; ..
    "scicos4.0.2"; ..
    "scicos4.2"; ..
    "scicos4.0.1"; ..
    "scicos4.0.2"; ..
    "scicos4.2"; ..
    "scicos4.3"; ..
    "scicos4.4"];

    if ~or(version==availableVersions) then
        error("No version update defined to "+version+" version")
    end

    if version=="scicos2.2" then scs_m=do_version22(scs_m);version="scicos2.3";end
    if version=="scicos2.3" then scs_m=do_version23(scs_m);version="scicos2.3.1";end
    if version=="scicos2.3.1" then scs_m=do_version231(scs_m); version="scicos2.4";end
    if version=="scicos2.4" then scs_m=do_version251(scs_m),version="scicos2.5.1";end
    if version=="scicos2.5.1" then
        ncl=lines()
        lines(0)
        disp("This is an old scicos diagram. I have to translate.")
        scs_m=do_versionxx(scs_m);scs_m=do_version27(scs_m);version="scicos2.7";
        disp("I am now going to clean your diagram. Block numbers may change.")
        scs_m=do_purge(scs_m);
        disp("Save the diagram (under a different name just in case)")
        lines(ncl(2))
    end
    if version=="scicos2.7" then
        disp("This is an old scicos diagram. I have to translate. Be patient.")
        scs_m=do_version271(scs_m),
        version="scicos2.7.1";
    end
    if version=="scicos2.7.1" then scs_m=do_version272(scs_m),version="scicos2.7.2";end
    if version=="scicos2.7.2" then scs_m=do_version273(scs_m),version="scicos2.7.3";end
    if version=="scicos2.7.3" | version=="scicos4" |...
        version=="scicos4.0.1" | version=="scicos4.0.2" then
        version="scicos4.2";
        //*** do certification ***//
        scs_m=update_scs_m(scs_m,version);
        //*** update scope ***//
        scs_m=do_version42(scs_m);
        //*********************************//
    end
    if version=="scicos4.2" then
        ncl=lines(); lines(0);
        version="scicos4.3";
        //*** do certification ***//
        scs_m=update_scs_m(scs_m,version);
        scs_m=do_version43(scs_m);
        lines(ncl(2))
    end

    if or(version==["scicos4.3";"scicos4.4"]) then
        ncl=lines(); lines(0);
        version="scicos4.3";
        scs_m=update_scs_m(scs_m,version);
        scs_m=do_version43(scs_m);
        scs_m.version = version;
        lines(ncl(2))
    end
endfunction

function scs_m_new=do_version43(scs_m)
    //disp('do_version43');
    scs_m_new=scs_m;

    //@@ adjust ID.fonts
    scs_m_new.props.options.ID(1)=[scs_m.props.options.ID(1)(1),scs_m.props.options.ID(1)(2),2,1];
    scs_m_new.props.options.ID(2)=[scs_m.props.options.ID(2)(1),scs_m.props.options.ID(2)(2),10,1];

    n=size(scs_m.objs);
    for j=1:n //loop on objects
        o=scs_m.objs(j);
        if typeof(o)=="Block" then
            omod=o.model;

            // Clear the doc fields if it contains a function pointer
            if type(o.doc) == 15 & size(o.doc) > 1 & (type(o.doc(1)) == 11 | type(o.doc(1)) == 13) then
                scs_m_new.objs(j).doc = list();
            end

            //@@ sbloc
            if omod.sim=="super"|omod.sim=="csuper"|omod.sim(1)=="asuper" then
                rpar=do_version43(omod.rpar)
                scs_m_new.objs(j).model.rpar=rpar
            end

            //@@ Change gr_i of MUX if needed
            if o.gui=="MUX" then
                gr_i=["txt=''Mux'';"
                "style=5;"
                "rectstr=stringbox(txt,orig(1),orig(2),0,style,1);"
                "if ~exists(''%zoom'') then %zoom=1, end;"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+sz(1)/2-w/2,orig(2)-h-4,txt,w,h,''fill'');"
                "e=gce();"
                "e.font_style=style;"]
                id=o.graphics.id
                if id=="Mux" then id="",end
                scs_m_new.objs(j).graphics.id=id
                if (type(scs_m_new.objs(j).graphics.gr_i)==15) then
                    if scs_m_new.objs(j).graphics.gr_i(1) == " " then
                        scs_m_new.objs(j).graphics.gr_i=list(gr_i,scs_m_new.objs(j).graphics.gr_i(2))
                    end
                else
                    if scs_m_new.objs(j).graphics.gr_i == " " then
                        scs_m_new.objs(j).graphics.gr_i=gr_i
                    end
                end

                //@@ Change gr_i of DEMUX if needed
            elseif o.gui=="DEMUX" then
                gr_i=["txt=''Demux'';"
                "style=5;"
                "rectstr=stringbox(txt,orig(1),orig(2),0,style,1);"
                "if ~exists(''%zoom'') then %zoom=1, end;"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+sz(1)/2-w/2,orig(2)-h-4,txt,w,h,''fill'');"
                "e=gce();"
                "e.font_style=style;"]
                id=o.graphics.id
                if id=="Demux" then id="",end
                scs_m_new.objs(j).graphics.id=id
                if (type(scs_m_new.objs(j).graphics.gr_i)==15) then
                    if scs_m_new.objs(j).graphics.gr_i(1) == "" then
                        scs_m_new.objs(j).graphics.gr_i=list(gr_i,scs_m_new.objs(j).graphics.gr_i(2))
                    end
                else
                    if scs_m_new.objs(j).graphics.gr_i == "" then
                        scs_m_new.objs(j).graphics.gr_i=gr_i
                    end
                end

                //@@ Change gr_i of FROMWSB
            elseif o.gui=="FROMWSB" then
                gr_i=["xstringb(orig(1),orig(2),''From workspace'',sz(1),sz(2),''fill'')"
                "txt=varnam;"
                "style=5;"
                "rectstr=stringbox(txt,orig(1),orig(2),0,style,1);"
                "if ~exists(''%zoom'') then %zoom=1, end;"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+sz(1)/2-w/2,orig(2)-h-4,txt,w,h,''fill'');"
                "e=gce();"
                "e.font_style=style;"]
                id=""
                scs_m_new.objs(j).graphics.id=id
                if (type(scs_m_new.objs(j).graphics.gr_i)==15) then
                    scs_m_new.objs(j).graphics.gr_i=list(gr_i,scs_m_new.objs(j).graphics.gr_i(2))
                else
                    scs_m_new.objs(j).graphics.gr_i=gr_i
                end

                //@@ Change gr_i of TOWS_c
            elseif o.gui=="TOWS_c" then
                gr_i=["xstringb(orig(1),orig(2),''To workspace'',sz(1),sz(2),''fill'')"
                "txt=varnam;"
                "style=5;"
                "rectstr=stringbox(txt,orig(1),orig(2),0,style,1);"
                "if ~exists(''%zoom'') then %zoom=1, end;"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+sz(1)/2-w/2,orig(2)-h-4,txt,w,h,''fill'');"
                "e=gce();"
                "e.font_style=style;"]
                id=""
                scs_m_new.objs(j).graphics.id=id
                if (type(scs_m_new.objs(j).graphics.gr_i)==15) then
                    scs_m_new.objs(j).graphics.gr_i=list(gr_i,scs_m_new.objs(j).graphics.gr_i(2))
                else
                    scs_m_new.objs(j).graphics.gr_i=gr_i
                end

                //@@ Change gr_i of INTEGRAL_m if needed
            elseif o.gui=="INTEGRAL_m" | o.gui=="INTEGRAL" then
                gr_i=["thick=xget(''thickness'')"
                "pat=xget(''pattern'')"
                "fnt=xget(''font'')"
                "xpoly(orig(1)+[0.7;0.62;0.549;0.44;0.364;0.291]*sz(1),orig(2)+[0.947;0.947;0.884;0.321;0.255;0.255]*sz(2),"'lines"')"
                "txt=''1/s'';"
                "style=5;"
                "rectstr=stringbox(txt,orig(1),orig(2),0,style,1);"
                "if ~exists(''%zoom'') then %zoom=1, end;"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+sz(1)/2-w/2,orig(2)-h-4,txt,w,h,''fill'');"
                "e=gce();"
                "e.font_style=style;"
                "xset(''thickness'',thick)"
                "xset(''pattern'',pat)"
                "xset(''font'',fnt(1),fnt(2))"]
                old_gri=["thick=xget(''thickness'')"
                "pat=xget(''pattern'')"
                "fnt=xget(''font'')"
                "xpoly(orig(1)+[0.7;0.62;0.549;0.44;0.364;0.291]*sz(1),orig(2)+[0.947;0.947;0.884;0.321;0.255;0.255]*sz(2),"'lines"')"
                "xset(''thickness'',thick)"
                "xset(''pattern'',pat)"
                "xset(''font'',fnt(1),fnt(2))"
                ]
                id=o.graphics.id
                if id=="1/s" then id="",end
                scs_m_new.objs(j).graphics.id=id
                if (type(scs_m_new.objs(j).graphics.gr_i)==15) then
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=list(gr_i,scs_m_new.objs(j).graphics.gr_i(2))
                    end
                else
                    if scs_m_new.objs(j).graphics.gr_i == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=gr_i
                    end
                end

                //@@ Change gr_i of PRODUCT if needed
            elseif o.gui=="PRODUCT" then
                gr_i=["[x,y,typ]=standard_inputs(o) ";
                "dd=sz(1)/8,de=0,"
                "if ~arg1.graphics.flip then dd=6*sz(1)/8,de=-sz(1)/8,end"
                "for k=1:size(x,''*'')";
                "if size(sgn,1)>1 then"
                "  if sgn(k)>0 then";
                "    xstring(orig(1)+dd,y(k)-4,''*'')";
                "  else";
                "    xstring(orig(1)+dd,y(k)-4,''/'')";
                "  end";
                "end";
                "end";
                "xx=sz(1)*[.8 .8 .4  .4]+orig(1)+de";
                "yy=sz(2)*[.2 .8 .8  .2]+orig(2)";
                "xpoly(xx,yy,''lines'')"
                "txt=''Product'';"
                "style=5;"
                "rectstr=stringbox(txt,orig(1),orig(2),0,style,1);"
                "if ~exists(''%zoom'') then %zoom=1, end;"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+sz(1)/2-w/2,orig(2)-h-4,txt,w,h,''fill'');"
                "e=gce();"
                "e.font_style=style;"]
                old_gri=["[x,y,typ]=standard_inputs(o) ";
                "dd=sz(1)/8,de=0,"
                "if ~arg1.graphics.flip then dd=6*sz(1)/8,de=-sz(1)/8,end"
                "for k=1:size(x,''*'')";
                "if size(sgn,1)>1 then"
                "  if sgn(k)>0 then";
                "    xstring(orig(1)+dd,y(k)-4,''*'')";
                "  else";
                "    xstring(orig(1)+dd,y(k)-4,''/'')";
                "  end";
                "end";
                "end";
                "xx=sz(1)*[.8 .8 .4  .4]+orig(1)+de";
                "yy=sz(2)*[.2 .8 .8  .2]+orig(2)";
                "xpoly(xx,yy,''lines'')"]
                id=o.graphics.id
                if id=="1/s" then id="",end
                scs_m_new.objs(j).graphics.id=id
                if (type(scs_m_new.objs(j).graphics.gr_i)==15) then
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=list(gr_i,scs_m_new.objs(j).graphics.gr_i(2))
                    end
                else
                    if scs_m_new.objs(j).graphics.gr_i == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=gr_i
                    end
                end

                //@@ Change gr_i of DERIV if needed
            elseif o.gui=="DERIV" then
                gr_i=["xstringb(orig(1),orig(2),'' du/dt   '',sz(1),sz(2),''fill'');"
                "txt=''s'';"
                "style=5;"
                "rectstr=stringbox(txt,orig(1),orig(2),0,style,1);"
                "if ~exists(''%zoom'') then %zoom=1, end;"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+sz(1)/2-w/2,orig(2)-h-4,txt,w,h,''fill'');"
                "e=gce();"
                "e.font_style=style;"]
                old_gri=["xstringb(orig(1),orig(2),''  du/dt  '',sz(1),sz(2),''fill'');"]
                id=o.graphics.id
                if id=="s" then id="",end
                scs_m_new.objs(j).graphics.id=id
                if (type(scs_m_new.objs(j).graphics.gr_i)==15) then
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=list(gr_i,scs_m_new.objs(j).graphics.gr_i(2))
                    end
                else
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=gr_i
                    end
                end

                //@@ Change gr_i of JKFLIPFLOP if needed
            elseif o.gui=="JKFLIPFLOP" then
                gr_i=["[x,y,typ]=standard_inputs(o) ";
                "dd=sz(1)/8,de=5.5*sz(1)/8";
                "txt=''J'';"
                "if ~exists(''%zoom'') then %zoom=1, end;"
                "rectstr=stringbox(txt,orig(1)+dd,y(1)-4,0,1,1);"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+dd,y(1)-4,txt,w,h,''fill'')";
                "txt=''clk'';"
                "rectstr=stringbox(txt,orig(1)+dd,y(2)-4,0,1,1);"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+dd,y(2)-4,txt,w,h,''fill'')";
                "txt=''K'';"
                "rectstr=stringbox(txt,orig(1)+dd,y(3)-4,0,1,1);"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+dd,y(3)-4,txt,w,h,''fill'')";
                "[x,y,typ]=standard_outputs(o) ";
                "txt=''Q'';"
                "rectstr=stringbox(txt,orig(1)+de,y(1)-4,0,1,1);"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+de,y(1)-4,txt,w,h,''fill'')";
                "txt=''!Q'';"
                "rectstr=stringbox(txt,orig(1)+4.5*dd,y(2)-4,0,1,1);"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+4.5*dd,y(2)-4,txt,w,h,''fill'')";
                "txt=''JK FLIP-FLOP'';"
                "style=5;"
                "rectstr=stringbox(txt,orig(1),orig(2),0,style,1);"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+sz(1)/2-w/2,orig(2)-h-4,txt,w,h,''fill'');"
                "e=gce();"
                "e.font_style=style;"]
                old_gri=["[x,y,typ]=standard_inputs(o) ";
                "dd=sz(1)/8,de=6*sz(1)/8";
                "xstring(orig(1)+dd,y(1)-4,''J'')";
                "xstring(orig(1)+dd,y(2)-4,''clk'')";
                "xstring(orig(1)+dd,y(3)-4,''K'')";
                "[x,y,typ]=standard_outputs(o) ";
                "xstring(orig(1)+de,y(1)-4,''Q'')";
                "xstring(orig(1)+5*dd,y(2)-4,''!Q'')"]
                id=o.graphics.id
                if id=="JK FLIP-FLOP" then id="",end
                scs_m_new.objs(j).graphics.id=id
                if (type(scs_m_new.objs(j).graphics.gr_i)==15) then
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=list(gr_i,scs_m_new.objs(j).graphics.gr_i(2))
                    end
                else
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=gr_i
                    end
                end

                //@@ Change gr_i of SRFLIPFLOP if needed
            elseif o.gui=="SRFLIPFLOP" then
                gr_i=["[x,y,typ]=standard_inputs(o) ";
                "dd=sz(1)/8,de=5.5*sz(1)/8";
                "txt=''S'';"
                "if ~exists(''%zoom'') then %zoom=1, end;"
                "rectstr=stringbox(txt,orig(1)+dd,y(1)-4,0,1,1);"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+dd,y(1)-4,txt,w,h,''fill'')";
                "txt=''R'';"
                "rectstr=stringbox(txt,orig(1)+dd,y(2)-4,0,1,1);"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+dd,y(2)-4,txt,w,h,''fill'')";
                "[x,y,typ]=standard_outputs(o) ";
                "txt=''Q'';"
                "rectstr=stringbox(txt,orig(1)+de,y(1)-4,0,1,1);"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+de,y(1)-4,txt,w,h,''fill'')";
                "txt=''!Q'';"
                "rectstr=stringbox(txt,orig(1)+4.5*dd,y(2)-4,0,1,1);"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+4.5*dd,y(2)-4,txt,w,h,''fill'')";
                "txt=''SR FLIP-FLOP'';"
                "style=5;"
                "rectstr=stringbox(txt,orig(1),orig(2),0,style,1);"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+sz(1)/2-w/2,orig(2)-h-4,txt,w,h,''fill'');"
                "e=gce();"
                "e.font_style=style;"]
                old_gri=["[x,y,typ]=standard_inputs(o) ";
                "dd=sz(1)/8,de=6*sz(1)/8";
                "xstring(orig(1)+dd,y(1)-4,''S'')";
                "xstring(orig(1)+dd,y(2)-4,''R'')";
                "[x,y,typ]=standard_outputs(o) ";
                "xstring(orig(1)+de,y(1)-4,''Q'')";
                "xstring(orig(1)+5*dd,y(2)-4,''!Q'')"]
                id=o.graphics.id
                if id=="SR FLIP-FLOP" then id="",end
                scs_m_new.objs(j).graphics.id=id
                if (type(scs_m_new.objs(j).graphics.gr_i)==15) then
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=list(gr_i,scs_m_new.objs(j).graphics.gr_i(2))
                    end
                else
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=gr_i
                    end
                end

                //@@ Change gr_i of DLATCH if needed
            elseif o.gui=="DLATCH" then
                gr_i=["[x,y,typ]=standard_inputs(o) ";
                "dd=sz(1)/8,de=5.5*sz(1)/8";
                "txt=''D'';"
                "if ~exists(''%zoom'') then %zoom=1, end;"
                "rectstr=stringbox(txt,orig(1)+dd,y(1)-4,0,1,1);"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+dd,y(1)-4,txt,w,h,''fill'')";
                "txt=''C'';"
                "rectstr=stringbox(txt,orig(1)+dd,y(2)-4,0,1,1);"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+dd,y(2)-4,txt,w,h,''fill'')";
                "txt=''Q'';"
                "rectstr=stringbox(txt,orig(1)+de,y(1)-4,0,1,1);"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+de,y(1)-4,txt,w,h,''fill'')";
                "txt=''!Q'';"
                "rectstr=stringbox(txt,orig(1)+4.5*dd,y(2)-4,0,1,1);"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+4.5*dd,y(2)-4,txt,w,h,''fill'')";
                "txt=''DLATCH'';"
                "style=5;"
                "rectstr=stringbox(txt,orig(1),orig(2),0,style,1);"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+sz(1)/2-w/2,orig(2)-h-4,txt,w,h,''fill'');"
                "e=gce();"
                "e.font_style=style;"]
                old_gri=["[x,y,typ]=standard_inputs(o) ";
                "dd=sz(1)/8,de=6*sz(1)/8";
                "xstring(orig(1)+dd,y(1)-4,''D'')";
                "xstring(orig(1)+dd,y(2)-4,''C'')";
                "xstring(orig(1)+de,y(1)-4,''Q'')";
                "xstring(orig(1)+5*dd,y(2)-4,''!Q'')"]
                id=o.graphics.id
                if id=="DLATCH" then id="",end
                scs_m_new.objs(j).graphics.id=id
                if (type(scs_m_new.objs(j).graphics.gr_i)==15) then
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=list(gr_i,scs_m_new.objs(j).graphics.gr_i(2))
                    end
                else
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=gr_i
                    end
                end

                //@@ Change gr_i of DFLIPFLOP if needed
            elseif o.gui=="DFLIPFLOP" then
                gr_i=["[x,y,typ]=standard_inputs(o) ";
                "dd=sz(1)/8,de=5.5*sz(1)/8";
                "txt=''D'';"
                "if ~exists(''%zoom'') then %zoom=1, end;"
                "rectstr=stringbox(txt,orig(1)+dd,y(1)-4,0,1,1);"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+dd,y(1)-4,txt,w,h,''fill'')";
                "txt=''clk'';"
                "rectstr=stringbox(txt,orig(1)+dd,y(2)-4,0,1,1);"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+dd,y(2)-4,txt,w,h,''fill'')";
                "txt=''en'';"
                "rectstr=stringbox(txt,orig(1)+dd,y(3)-4,0,1,1);"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+dd,y(3)-4,txt,w,h,''fill'')";
                "[x,y,typ]=standard_outputs(o) ";
                "txt=''Q'';"
                "rectstr=stringbox(txt,orig(1)+de,y(1)-4,0,1,1);"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+de,y(1)-4,txt,w,h,''fill'')";
                "txt=''!Q'';"
                "rectstr=stringbox(txt,orig(1)+4.5*dd,y(2)-4,0,1,1);"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+4.5*dd,y(2)-4,txt,w,h,''fill'')";
                "txt=''D FLIP-FLOP'';"
                "style=5;"
                "rectstr=stringbox(txt,orig(1),orig(2),0,style,1);"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+sz(1)/2-w/2,orig(2)-h-4,txt,w,h,''fill'');"
                "e=gce();"
                "e.font_style=style;"]
                old_gri=["[x,y,typ]=standard_inputs(o) ";
                "dd=sz(1)/8,de=6*sz(1)/8";
                "xstring(orig(1)+dd,y(1)-4,''D'')";
                "xstring(orig(1)+dd,y(2)-4,''clk'')";
                "xstring(orig(1)+dd,y(3)-4,''en'')";
                "[x,y,typ]=standard_outputs(o) ";
                "xstring(orig(1)+de,y(1)-4,''Q'')";
                "xstring(orig(1)+5*dd,y(2)-4,''!Q'')"]
                id=o.graphics.id
                if id=="D FLIP-FLOP" then id="",end
                scs_m_new.objs(j).graphics.id=id
                if (type(scs_m_new.objs(j).graphics.gr_i)==15) then
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=list(gr_i,scs_m_new.objs(j).graphics.gr_i(2))
                    end
                else
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=gr_i
                    end
                end

                //@@ Change gr_i of CLKGotoTagVisibility if needed
            elseif o.gui=="CLKGotoTagVisibility" then
                gr_i=["xstringb(orig(1),orig(2),[''{''+arg1.graphics.exprs(1)+''}''],sz(1),sz(2),''fill'');";
                "wd=xget(''wdim'').*[1.016,1.12];";
                "thick=xget(''thickness'');xset(''thickness'',2);";
                "p=wd(2)/wd(1);p=1;";
                "xarcs([orig(1)+0.05*sz(1);";
                "orig(2)+0.95*sz(2);";
                "0.9*sz(1)*p;";
                "0.9*sz(2);";
                "0;";
                "360*64],scs_color(5));";
                "txt=[''Goto Tag'';''Visibility'' ];"
                "style=5;"
                "gh_axes = gca();"
                "axes_font_style = gh_axes.font_style ;"
                "axes_font_size  = gh_axes.font_size  ;"
                "gh_axes.font_style = 5;"
                "gh_axes.font_size  = 1;"
                "rectstr=stringbox(txt,orig(1),orig(2),0,style,1);"
                "if ~exists(''%zoom'') then %zoom=1, end;"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+sz(1)/2-w/2,orig(2)-h-5,txt,w,h,''fill'');"
                "gh_axes.font_style = axes_font_style ;"
                "gh_axes.font_size  = axes_font_size  ;"
                "xset(''thickness'',thick)"]
                old_gri=["xstringb(orig(1),orig(2),[''{''+arg1.graphics.exprs(1)+''}''],sz(1),sz(2),''fill'');";
                "wd=xget(''wdim'').*[1.016,1.12];";
                "thick=xget(''thickness'');xset(''thickness'',2);";
                "p=wd(2)/wd(1);p=1;";
                "xarcs([orig(1)+0.05*sz(1);";
                "orig(2)+0.95*sz(2);";
                "0.9*sz(1)*p;";
                "0.9*sz(2);";
                "0;";
                "360*64],scs_color(5));";
                "xset(''thickness'',thick)"]
                id=o.graphics.id
                if id==["Goto Tag";"Visibility"] then id="",end
                scs_m_new.objs(j).graphics.id=id
                if (type(scs_m_new.objs(j).graphics.gr_i)==15) then
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=list(gr_i,scs_m_new.objs(j).graphics.gr_i(2))
                    end
                else
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=gr_i
                    end
                end

                //@@ Change gr_i of GotoTagVisibilityMO if needed
            elseif o.gui=="GotoTagVisibilityMO" then
                gr_i=["xstringb(orig(1),orig(2),[''{''+arg1.graphics.exprs(1)+''}''],sz(1),sz(2),''fill'');";
                "wd=xget(''wdim'').*[1.016,1.12];";
                "thick=xget(''thickness'');xset(''thickness'',2);";
                "p=wd(2)/wd(1);p=1;";
                "xarcs([orig(1)+0.05*sz(1);";
                "orig(2)+0.95*sz(2);";
                "0.9*sz(1)*p;";
                "0.9*sz(2);";
                "0;";
                "360*64],scs_color(3));";
                "txt=[''Goto Tag'';''Visibility'' ];"
                "style=5;"
                "gh_axes = gca();"
                "axes_font_style = gh_axes.font_style ;"
                "axes_font_size  = gh_axes.font_size  ;"
                "gh_axes.font_style = 5;"
                "gh_axes.font_size  = 1;"
                "rectstr=stringbox(txt,orig(1),orig(2),0,style,1);"
                "if ~exists(''%zoom'') then %zoom=1, end;"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+sz(1)/2-w/2,orig(2)-h-5,txt,w,h,''fill'');"
                "gh_axes.font_style = axes_font_style ;"
                "gh_axes.font_size  = axes_font_size  ;"
                "xset(''thickness'',thick)"]
                old_gri=["xstringb(orig(1),orig(2),[''{''+arg1.graphics.exprs(1)+''}''],sz(1),sz(2),''fill'');";
                "wd=xget(''wdim'').*[1.016,1.12];";
                "thick=xget(''thickness'');xset(''thickness'',2);";
                "p=wd(2)/wd(1);p=1;";
                "xarcs([orig(1)+0.05*sz(1);";
                "orig(2)+0.95*sz(2);";
                "0.9*sz(1)*p;";
                "0.9*sz(2);";
                "0;";
                "360*64],scs_color(3));";
                "xset(''thickness'',thick)"]
                id=o.graphics.id
                if id==["Goto Tag";"Visibility"] then id="",end
                scs_m_new.objs(j).graphics.id=id
                if (type(scs_m_new.objs(j).graphics.gr_i)==15) then
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=list(gr_i,scs_m_new.objs(j).graphics.gr_i(2))
                    end
                else
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=gr_i
                    end
                end

                //@@ Change gr_i of GotoTagVisibility if needed
            elseif o.gui=="GotoTagVisibility" then
                gr_i=["xstringb(orig(1),orig(2),[''{''+arg1.graphics.exprs(1)+''}''],sz(1),sz(2),''fill'');"
                "txt=[''Goto Tag'';''Visibility'' ];"
                "style=5;"
                "gh_axes = gca();"
                "axes_font_style = gh_axes.font_style ;"
                "axes_font_size  = gh_axes.font_size  ;"
                "gh_axes.font_style = 5;"
                "gh_axes.font_size  = 1;"
                "rectstr=stringbox(txt,orig(1),orig(2),0,style,1);"
                "if ~exists(''%zoom'') then %zoom=1, end;"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+sz(1)/2-w/2,orig(2)-h-5,txt,w,h,''fill'');"
                "gh_axes.font_style = axes_font_style ;"
                "gh_axes.font_size  = axes_font_size  ;"]
                old_gri=["xstringb(orig(1),orig(2),[''{''+arg1.graphics.exprs(1)+''}''],sz(1),sz(2),''fill'');"]
                id=o.graphics.id
                if id==["Goto Tag";"Visibility"] then id="",end
                scs_m_new.objs(j).graphics.id=id
                if (type(scs_m_new.objs(j).graphics.gr_i)==15) then
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=list(gr_i,scs_m_new.objs(j).graphics.gr_i(2))
                    end
                else
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=gr_i
                    end
                end

                //@@ Change gr_i of Inductor if needed
            elseif o.gui=="Inductor" then
                gr_i=["tt=linspace(0.04,0.96,100)'';"
                "xpoly(tt*sz(1)+orig(1),+orig(2)+abs(sin(18*(tt-0.04)))*sz(2),""lines"");";
                "xx=orig(1)+[0 0.04 0.04 0.04 0]*sz(1);";
                "yy=orig(2)+[1/2 1/2 0  1/2 1/2]*sz(2);";
                "xpoly(xx,yy) ";
                "xx=orig(1)+[0.96 0.96 1   0.96 0.96 ]*sz(1);";
                "yy=orig(2)+[abs(sin(18*0.92))   1/2   1/2 1/2 abs(sin(18*0.92))]*sz(2);";
                "xpoly(xx,yy) ";
                "txt=''L= ''+L;"
                "style=2;"
                "rectstr=stringbox(txt,orig(1),orig(2),0,style,1);"
                "if ~exists(''%zoom'') then %zoom=1, end;"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+sz(1)/2-w/2,orig(2)-h-4,txt,w,h,''fill'');"
                "e=gce();"]
                old_gri=["tt=linspace(0.04,0.96,100)'';"
                "xpoly(tt*sz(1)+orig(1),+orig(2)+abs(sin(18*(tt-0.04)))*sz(2),""lines"");";
                "xx=orig(1)+[0 0.04 0.04 0.04 0]*sz(1);";
                "yy=orig(2)+[1/2 1/2 0  1/2 1/2]*sz(2);";
                "xpoly(xx,yy) ";
                "xx=orig(1)+[0.96 0.96 1   0.96 0.96 ]*sz(1);";
                "yy=orig(2)+[abs(sin(18*0.92))   1/2   1/2 1/2 abs(sin(18*0.92))]*sz(2);";
                "xpoly(xx,yy) ";
                "rect=xstringl(0,0,''L=''+L)"
                "xstring(orig(1)+(sz(1)-rect(3))/2,orig(2)-rect(4)*1.2,''L=''+L)"]
                if (type(scs_m_new.objs(j).graphics.gr_i)==15) then
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=list(gr_i,scs_m_new.objs(j).graphics.gr_i(2))
                    end
                else
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=gr_i
                    end
                end

                //@@ Change gr_i of Capacitor if needed
            elseif o.gui=="Capacitor" then
                gr_i=["xset(''thickness'',2)"
                "xx=orig(1)+[0 1/3 1/3 1/3 ]*sz(1);";
                "yy=orig(2)+[1/2 1/2 1 0]*sz(2);";
                "xpoly(xx,yy);"
                "xsegs(orig(1)+ sz(1)*2/3*[1 1 1 3/2],orig(2)+(sz(2)*1/2)*[2 0 1 1],0);";
                "if orient then"
                "  xrects([orig(1)+sz(1)*1/2;orig(2)+sz(2);sz(1)*1/6;sz(2)],scs_color(33));"
                "  xstring(orig(1)+sz(1)*1/12,orig(2)+sz(2)*3/4,''+'');";
                "  xstring(orig(1)+sz(1)*7/8,orig(2)+sz(2)*3/4,''-'');";
                "else"
                "  xrects([orig(1)+sz(1)*1/3;orig(2)+sz(2);sz(1)*1/6;sz(2)],scs_color(33));"
                "  xstring(orig(1)+sz(1)*1/12,orig(2)+sz(2)*3/4,''-'');";
                "  xstring(orig(1)+sz(1)*7/8,orig(2)+sz(2)*3/4,''+'');";
                "end"
                "txt=''C= ''+C;"
                "style=2;"
                "rectstr=stringbox(txt,orig(1),orig(2),0,style,1);"
                "if ~exists(''%zoom'') then %zoom=1, end;"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+sz(1)/2-w/2,orig(2)-h-4,txt,w,h,''fill'');"
                "e=gce();"
                "e.font_style=style;"]
                old_gri=["xset(''thickness'',2)"
                "xx=orig(1)+[0 1/3 1/3 1/3 ]*sz(1);";
                "yy=orig(2)+[1/2 1/2 1 0]*sz(2);";
                "xpoly(xx,yy);"
                "xsegs(orig(1)+ sz(1)*2/3*[1 1 1 3/2],orig(2)+(sz(2)*1/2)*[2 0 1 1],0);";
                "if orient then"
                "  xrects([orig(1)+sz(1)*1/2;orig(2)+sz(2);sz(1)*1/6;sz(2)],scs_color(33));"
                "  xstring(orig(1)+sz(1)*1/12,orig(2)+sz(2)*3/4,''+'');";
                "  xstring(orig(1)+sz(1)*7/8,orig(2)+sz(2)*3/4,''-'');";
                "else"
                "  xrects([orig(1)+sz(1)*1/3;orig(2)+sz(2);sz(1)*1/6;sz(2)],scs_color(33));"
                "  xstring(orig(1)+sz(1)*1/12,orig(2)+sz(2)*3/4,''-'');";
                "  xstring(orig(1)+sz(1)*7/8,orig(2)+sz(2)*3/4,''+'');";
                "end"
                "rect=xstringl(0,0,''C=''+C)"
                "xstring(orig(1)+(sz(1)-rect(3))/2,orig(2)-rect(4)*1.2,''C= ''+C);"]
                if (type(scs_m_new.objs(j).graphics.gr_i)==15) then
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=list(gr_i,scs_m_new.objs(j).graphics.gr_i(2))
                    end
                else
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=gr_i
                    end
                end

                //@@ Change gr_i of Resistor if needed
            elseif o.gui=="Resistor" then
                gr_i=["xx=[0,1,1,7,7,8,7,7,1,1]/8;";
                "yy=[1,1,0,0,1,1,1,2,2,1]/2;";
                "xpoly(orig(1)+xx*sz(1),orig(2)+yy*sz(2)); "
                "txt=''R= ''+R;"
                "style=2;"
                "rectstr=stringbox(txt,orig(1),orig(2),0,style,1);"
                "if ~exists(''%zoom'') then %zoom=1, end;"
                "w=(rectstr(1,3)-rectstr(1,2))*%zoom;"
                "h=(rectstr(2,2)-rectstr(2,4))*%zoom;"
                "xstringb(orig(1)+sz(1)/2-w/2,orig(2)-h-4,txt,w,h,''fill'');"
                "e=gce();"]
                old_gri=["xx=[0,1,1,7,7,8,7,7,1,1]/8;";
                "yy=[1,1,0,0,1,1,1,2,2,1]/2;";
                "xpoly(orig(1)+xx*sz(1),orig(2)+yy*sz(2)); "
                "rect=xstringl(0,0,''R=''+R)"
                "xstring(orig(1)+(sz(1)-rect(3))/2,orig(2)-rect(4)*1.2,''R=''+R);"]
                if (type(scs_m_new.objs(j).graphics.gr_i)==15) then
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=list(gr_i,scs_m_new.objs(j).graphics.gr_i(2))
                    end
                else
                    if scs_m_new.objs(j).graphics.gr_i(1) == old_gri then
                        scs_m_new.objs(j).graphics.gr_i=gr_i
                    end
                end

            end
        end
    end
endfunction


//*** update scope ***//
function scs_m_new=do_version42(scs_m)
    scs_m_new=scs_m
    n=size(scs_m.objs);
    for j=1:n //loop on objects
        o=scs_m.objs(j);
        if typeof(o)=="Block" then
            omod=o.model;
            //SUPER BLOCK
            if omod.sim=="super"|omod.sim=="csuper" then
                rpar=do_version42(omod.rpar)
                scs_m_new.objs(j).model.rpar=rpar
                //name of gui and sim list change
            elseif o.gui=="SCOPE_f" then
                scs_m_new.objs(j).gui="CSCOPE"
                scs_m_new.objs(j).model.dstate=[]
                //Alan's patch 01/10/07 : remove the last parameter
                // (inheritance not used in cscope)
                ipar = scs_m_new.objs(j).model.ipar(:);
                scs_m_new.objs(j).model.ipar = ipar(1:$-1);
                scs_m_new.objs(j).model.sim=list("cscope", 4)
                in = scs_m_new.objs(j).model.in(:);
                a = size(in,1);
                in2 = ones(a,1);
                scs_m_new.objs(j).model.in2 = in2;
                scs_m_new.objs(j).model.intyp = in2;
                //Alan's patch 04/10/07 : update exprs
                exprs = scs_m.objs(j).graphics.exprs;
                if size(exprs)<9 then exprs(9)="0",end // compatibility
                if size(exprs)<10 then exprs(10)=emptystr(),end // compatibility
                scs_m_new.objs(j).graphics.exprs=exprs;
            elseif o.gui=="CSCOPE" then
                //Alan's patch 04/10/07 : update exprs
                exprs = scs_m.objs(j).graphics.exprs;
                if size(exprs)<10 then exprs(10)=emptystr(),end // compatibility
                scs_m_new.objs(j).graphics.exprs=exprs;
            elseif o.gui=="BOUNCEXY" then
                in = scs_m_new.objs(j).model.in(:);
                a = size(in,1);
                in2 = ones(a,1);
                scs_m_new.objs(j).model.in2 = in2;
                scs_m_new.objs(j).model.intyp = in2;
            elseif o.gui=="MSCOPE_f" then
                //Alan's patch 04/10/07 : update exprs
                exprs = scs_m.objs(j).graphics.exprs;
                if size(exprs)<10 then exprs(10)="0",end // compatibility
                if size(exprs)<11 then exprs(11)=emptystr(),end // compatibility
                scs_m_new.objs(j).graphics.exprs=exprs;
                scs_m_new.objs(j).gui="CMSCOPE"
                scs_m_new.objs(j).model.dstate=[]
                scs_m_new.objs(j).model.sim=list("cmscope", 4)
                in = scs_m_new.objs(j).model.in(:);
                a = size(in,1);
                B=stripblanks(scs_m.objs(j).graphics.exprs(8));
                B(1:a)=B;
                B = strcat(B', " ");
                scs_m_new.objs(j).graphics.exprs(8) = B;
                rpar=scs_m_new.objs(j).model.rpar(:);
                N=scs_m_new.objs(j).model.ipar(2);
                period = [];
                for i=1:N
                    period(i)=rpar(2);
                end
                scs_m_new.objs(j).model.rpar = [rpar(1);period(:);rpar(3:size(rpar,1))]
                in2 = ones(a,1);
                scs_m_new.objs(j).model.in2 = in2;
                scs_m_new.objs(j).model.intyp = in2;
            elseif o.gui=="ANIMXY_f" then
                scs_m_new.objs(j).gui="CANIMXY"
                scs_m_new.objs(j).model.dstate=[]
                scs_m_new.objs(j).model.sim=list("canimxy", 4)
                in = scs_m_new.objs(j).model.in(:);
                a = size(in,1);
                in2 = ones(a,1);
                scs_m_new.objs(j).model.in2 = in2;
                scs_m_new.objs(j).model.intyp = in2;
                scs_m_new.objs(j).graphics.exprs = [string(1);scs_m_new.objs(j).graphics.exprs(:)]
                scs_m_new.objs(j).model.ipar = [scs_m_new.objs(j).model.ipar(:);1]
                //Alan's patch 04/10/07 : update exprs
                exprs = scs_m_new.objs(j).graphics.exprs(:)
                if size(exprs,"*")==8 then exprs=[1;exprs(1:3);"[]";"[]";exprs(4:8)],end
                scs_m_new.objs(j).graphics.exprs=exprs;
            elseif o.gui=="EVENTSCOPE_f" then
                scs_m_new.objs(j).gui="CEVENTSCOPE"
                scs_m_new.objs(j).model.dstate=[]
                scs_m_new.objs(j).model.sim=list("cevscpe", 4)
                in = scs_m_new.objs(j).model.in(:);
                a = size(in,1);
                in2 = ones(a,1);
                scs_m_new.objs(j).model.in2 = in2;
                scs_m_new.objs(j).model.intyp = in2;
            elseif o.gui=="FSCOPE_f" then
                scs_m_new.objs(j).gui="CFSCOPE"
                scs_m_new.objs(j).model.dstate=[]
                scs_m_new.objs(j).model.sim=list("cfscope", 4)
                in = scs_m_new.objs(j).model.in(:);
                a = size(in,1);
                in2 = ones(a,1);
                scs_m_new.objs(j).model.in2 = in2;
                scs_m_new.objs(j).model.intyp = in2;
                //Alan's patch 04/10/07 : update exprs
                exprs = scs_m.objs(j).graphics.exprs;
                if size(exprs)<9 then exprs(9)="0",end // compatibility
                scs_m_new.objs(j).graphics.exprs=exprs;
            elseif o.gui=="SCOPXY_f" then
                scs_m_new.objs(j).gui="CSCOPXY"
                scs_m_new.objs(j).model.dstate=[]
                scs_m_new.objs(j).model.sim=list("cscopxy", 4)
                in = scs_m_new.objs(j).model.in(:);
                a = size(in,1);
                in2 = ones(a,1);
                scs_m_new.objs(j).model.in2 = in2;
                scs_m_new.objs(j).model.intyp = in2;
                scs_m_new.objs(j).graphics.exprs = [string(1);scs_m_new.objs(j).graphics.exprs(:)]
                scs_m_new.objs(j).model.ipar = [scs_m_new.objs(j).model.ipar(:);1]
            elseif o.gui=="CMSCOPE" then
                //Alan's patch 04/10/07 : update exprs
                exprs = scs_m.objs(j).graphics.exprs;
                if size(exprs)<11 then exprs(11)=emptystr(),end // compatibility
                scs_m_new.objs(j).graphics.exprs=exprs;
                scs_m_new.objs(j).model.dstate=[]
                in = scs_m_new.objs(j).model.in(:);
                a = size(in,1);
                B=stripblanks(scs_m.objs(j).graphics.exprs(8));
                B(1:a)=B;
                B = strcat(B', " ");
                scs_m_new.objs(j).graphics.exprs(8)=B;
                rpar=scs_m_new.objs(j).model.rpar(:);
                N=scs_m_new.objs(j).model.ipar(2);
                period = [];
                for i=1:N
                    period(i)=rpar(2);
                end
                scs_m_new.objs(j).model.rpar = [rpar(1);period(:);rpar(3:size(rpar,1))]
                in2 = ones(a,1);
                scs_m_new.objs(j).model.in2 = in2;
                scs_m_new.objs(j).model.intyp = in2;
            elseif o.gui=="IN_f" then
                scs_m_new.objs(j).model.out    = -1;
                scs_m_new.objs(j).model.out2   = -2;
                scs_m_new.objs(j).model.outtyp = -1;
            elseif o.gui=="OUT_f" then
                scs_m_new.objs(j).model.in    = -1;
                scs_m_new.objs(j).model.in2   = -2;
                scs_m_new.objs(j).model.intyp = -1;
            elseif o.gui=="INIMPL_f" then
                scs_m_new.objs(j).model.out    = -1;
                scs_m_new.objs(j).model.out2   = 1;
                scs_m_new.objs(j).model.outtyp = -1;
            elseif o.gui=="OUTIMPL_f" then
                scs_m_new.objs(j).model.in    = -1;
                scs_m_new.objs(j).model.in2   = 1;
                scs_m_new.objs(j).model.intyp = -1;
            end
        end
    end
endfunction

function scs_m_new=do_version273(scs_m)
    scs_m_new=scs_m;
    n=size(scs_m.objs);
    for i=1:n //loop on objects
        o=scs_m.objs(i);
        if typeof(o)=="Block" then
            omod=o.model;
            T=getfield(1,omod)
            if T($)<>"equations" then
                T($+1)="equations"
                scicos_setfield(1,T,omod)
                scicos_setfield($+1,list(),omod)
            end
            if omod.sim=="super"|omod.sim=="csuper" then
                rpar=do_version273(omod.rpar)
                scicos_setfield($+1,getfield($,omod),omod)
                scicos_setfield($-1,0,omod)
                omod.rpar=rpar
            elseif omod.sim(1)=="ifthel" then // |omod.sim(1)=='eselect' then
                scicos_setfield($+1,getfield($,omod),omod)
                scicos_setfield($-1,1,omod)
                scicos_setfield($-2,1,omod)
            else
                scicos_setfield($+1,getfield($,omod),omod)
                scicos_setfield($-1,0,omod)
            end
            xx=getfield(1,omod);
            yy=xx($);
            xx($)="nmode"
            xx($+1)=yy;
            scicos_setfield(1,xx,omod)
            o.model= omod
            scs_m_new.objs(i)=o
        elseif typeof(o)=="Link" then
            if size(o.from,"*")==2 then o.from(3)=0;end
            if size(o.to,"*")==2 then o.to(3)=1;end
            scs_m_new.objs(i)=o
        end
    end
endfunction


function scs_m_new=do_version272(scs_m)

    scs_m_new=scs_m
    for i=1:lstsize(scs_m.objs)

        if typeof(scs_m.objs(i))=="Block" then
            grphic=scs_m.objs(i).graphics
            chmps=size(getfield(1,grphic))
            if or(getfield(1,grphic)=="in_implicit")&..
                size(grphic.in_implicit,"*")==size(grphic.pin,"*") then
                in_implicit=grphic.in_implicit
            else
                I="E";
                in_implicit=I(ones(grphic.pin(:)));
            end

            if or(getfield(1,grphic)=="out_implicit")&..
                size(grphic.out_implicit,"*")==size(grphic.pout,"*") then
                out_implicit=grphic.out_implicit
            else
                I="E";
                out_implicit=I(ones(grphic.pout(:)));
            end

            if grphic.flip == %f then
                style = "flip=0;";
            else
                style = "";
            end
            scs_m_new.objs(i).graphics=scicos_new(["graphics","orig","sz","exprs","pin",..
            "pout","pein","peout","gr_i","id","in_implicit","out_implicit","style"],..
            grphic.orig,grphic.sz,grphic.exprs,grphic.pin,..
            grphic.pout,grphic.pein,grphic.peout,grphic.gr_i,grphic.id,..
            in_implicit,out_implicit,style)

            if or(scs_m_new.objs(i).model.sim==["super","csuper"]) then
                rpar=do_version272(scs_m_new.objs(i).model.rpar)
                scs_m_new.objs(i).model.rpar=rpar
            end
        end
    end
endfunction

function scs_m_new=do_version271(scs_m)
    scs_m_new=scs_m;
    n=size(scs_m.objs);
    for i=1:n //loop on objects
        o=scs_m.objs(i);
        if typeof(o)=="Block" then
            omod=o.model;
            if omod.sim=="super"|omod.sim=="csuper" then
                rpar=do_version271(omod.rpar)
                scicos_setfield($+1,0,omod)
                omod.rpar=rpar
            elseif omod.sim=="zcross" then
                scicos_setfield($+1,omod.in,omod)
                omod.sim=list(omod.sim,1)
            elseif omod.sim=="lusat" then
                scicos_setfield($+1,2*omod.in,omod)
                omod.sim=list(omod.sim,1)
            else
                scicos_setfield($+1,0,omod)
            end
            scicos_setfield(1,[getfield(1,omod),"nzcross"],omod)
            o.model= omod
            scs_m_new.objs(i)=o
        end
    end
endfunction

function scs_m=do_version251(scs_m)
    nx=size(scs_m)
    obsolete=%f
    for k=2:nx
        o=scs_m(k)
        if o(1)=="Block" then
            model=o(3)
            if model(1)=="super"|model(1)=="csuper" then
                model(8)=do_version251(model(8))
                o(3)=model
                scs_m(k)=o
            elseif o(5)=="SOM_f" then
                if and(model(8)==1) then
                    model(8)=[]
                    model(1)=list("plusblk",2)
                    scs_m(k)(3)=model
                    scs_m(k)(5)="SUM_f"
                else
                    scs_m(k)(3)(1)=list("sum",2)
                    obsolete=%t
                end
            elseif o(5)=="AFFICH_f" then
                scs_m(k)(3)(7)=[0;-1;0;0;1;1]
                scs_m(k)(2)(9)=["xset(''font'',ipar(1),ipar(2))";
                "str='' ''+part(''0'',ones(1,ipar(5)-ipar(6)-2))+''.''+part(''0'',ones(1,ipar(6)))"
                "rr=xstringl(orig(1),orig(2),str)"
                "xstring(orig(1)+max(0,(sz(1)-rr(3))/2),orig(2)+max(0,(sz(2)-rr(4))/2),str)"]
            elseif o(5)=="c_block" then
                model(1)(2)=model(1)(2)-int(model(1)(2)/1000)*1000+2000
                scs_m(k)(3)=model
                tt=scs_m(k)(2)(4)(2)
                ii=grep(tt,"machine.h")
                if size(ii,"*")==1 then
                    tt(ii)="#include <machine.h>",
                    scs_m(k)(2)(4)(2)=tt
                end
            end
        elseif o(1)=="Link" then
            if size(o(2),"*")==1 then
                o(2)=o(2)*[1;1];o(3)=o(3)*[1;1];
                scs_m(k)=o
            end
        end
    end
    if obsolete then
        messagebox(["Diagram contains obsolete signed blocks sum"
        "They are drawn in brown, they work as before but,"
        "please replace them with the new block sum"],"modal")
    end

endfunction
function scs_m=do_version231(scs_m)
    //2.3.1 to 2.4 version
    if size(scs_m(1))<5 then scs_m(1)(5)=" ",end  //compatibility
    if type(scs_m(1)(5))<>10 then scs_m(1)(5)=" ",end //compatibility
    if size(scs_m(1))<6 then //compatibility
        wpar=scs_m(1)
        wpar(6)=list()
        wpar(7)=list(%t,[0.8 0.8 0.8])
        wpar(8)=[]
        wpar(9)=[]
        wpar(10)=[]
        scs_m(1)=wpar
    end

    if size(scs_m(1)(1),"*") <4 then scs_m(1)(1)=[scs_m(1)(1),0,0],end //compatibility


    scs_m(1)(1)(2)=max(scs_m(1)(1)(2),450)
    if size(scs_m(1))<6 then
        options=default_options()
        doc=list() //documentation structure
        wpar=scs_m(1)
        wpar(6)=list()
        wpar(7)=options
        wpar(8)=[]
        wpar(9)=[]
        wpar(10)=doc
        scs_m(1)=wpar
    end
    wsiz=scs_m(1)(1)
    if size(wsiz,"*")<6 then //compatibility
        if size(wsiz,"*")<4 then wsiz(3)=0;wsiz(4)=0;end
        wsiz(5)=wsiz(1);wsiz(6)=wsiz(2);
        scs_m(1)(1)=wsiz;
    end
    if type(scs_m(1)(7))==15 then //options
        old_opt=scs_m(1)(7)
        options=default_options()
        options("3D")(1)=old_opt(1)
        options("Cmap")=old_opt(2)
        scs_m(1)(7)=options
    end

    nx=size(scs_m)
    for k=2:nx
        o=scs_m(k)
        if o(1)=="Block" then
            model=o(3)
            if model(1)=="super"|model(1)=="csuper" then
                model(8)=do_version231(model(8))
                o(3)=model
                scs_m(k)=o
            elseif model(1)(1)=="ifthel" then
                model(1)=list("ifthel",-1)
                scs_m(k)(3)=model
            elseif model(1)(1)=="eselect" then
                model(1)=list("eselect",-1)
                scs_m(k)(3)=model
            end
        end
    end


endfunction
function x_new=do_version23(scs_m)
    //2.3 to 2.3.1
    x_new=list()
    x_new(1)=scs_m(1)
    nx=size(scs_m)
    for k=2:nx
        o=scs_m(k)
        if o(1)=="Link" then
        elseif o(1)=="Block" then
            model=o(3)
            if model(1)=="super"|model(1)=="csuper" then
                model(8)=do_version23(model(8))
                o(3)=model
                o=block2_version(o)
                scs_m(k)=o
            else
                o=block2_version(o)
            end
        end
        x_new(k)=o;
    end


endfunction
function o=block2_version(o)
    if o(3)(6)<>[] then o(3)(12)(2)=%t;end

endfunction


function o=block_version(o)
    [graphics,model]=o(2:3)
    for k=2:5, model(k)=ones(model(k),1),end
    blocktype=o(5)
    ok=%t
    label=" "
    gr_i=" "
    select blocktype
    case "ABSBLK_f" then
        model(2)=-1
        model(3)=-1
        label=" "
        gr_i="xstringb(orig(1),orig(2),''abs'',sz(1),sz(2),''fill'')"
    case "ANIMXY_f" then
        [rpar,ipar]=model([8:9])
        model(9)=[model(9);0;[-1;-1];[-1;-1]]
        win=ipar(1);N=ipar(3);clrs=ipar(4);siz=ipar(5)
        xmin=rpar(1);xmax=rpar(2);ymin=rpar(3);ymax=rpar(4)
        label=[string(clrs);
        string(siz);
        string(win);
        "[]";
        "[]";
        string(xmin);
        string(xmax);
        string(ymin);
        string(ymax);
        string(N)]
        gr_i=["thick=xget(''thickness'');xset(''thickness'',2);";
        "t=(0:0.3:2*%pi)'';";
        "xx=orig(1)+(1/5+(cos(2.2*t)+1)*3/10)*sz(1);";
        "yy=orig(2)+(1/4.3+(sin(t)+1)*3/10)*sz(2);";
        "xpoly(xx,yy,''lines'');"
        "xset(''thickness'',thick);"]
    case "BOUND_f" then
        rpar=model(8);in=model(2);nin=sum(in)
        if nin<>1 then
            write(%io(2),["Pb with BOUND_f block";
            "previously block has more than one input port";
            "It is better to change it with the new block version";
            "preceded by a multiplexer"])
        end
        thresh=rpar(1:nin),v=rpar(nin+1:2*nin)
        label=[strcat(sci2exp(thresh));
        strcat(sci2exp(v))]
        gr_i=["thick=xget(''thickness'');xset(''thickness'',2);";
        "xx=orig(1)+[1/5;1/2;1/2;1-1/5]*sz(1);";
        "yy=orig(2)+[1/2;1/2;1-1/5;1-1/5]*sz(2);";
        "xpoly(xx,yy,''lines'');";
        "xset(''thickness'',1);";
        "xpoly(orig(1)+[1/9;1/5]*sz(1),orig(2)+[1/2;1/2]*sz(2),''lines'');";
        "xpoly(orig(1)+[1/2;1-1/9]*sz(1),orig(2)+[1/2;1/2]*sz(2),''lines'');";
        "xpoly(orig(1)+[1/2;1/2]*sz(1),orig(2)+[1/9;1/2]*sz(2),''lines'');";
        "xpoly(orig(1)+[1/2;1/2]*sz(1),orig(2)+[1-1/5;1-1/9]*sz(2),''lines'');";
        "xset(''thickness'',thick);"]
    case "CLINDUMMY_f" then
        label=[]
        gr_i=["xstringb(orig(1),orig(2),[''DUMMY'';''CLSS''],sz(1),sz(2),''fill'');"]
    case "CLKIN_f" then
        prt=model(9);
        label=string(prt)
        gr_i=[]
    case "CLKOUT_f" then
        prt=model(9);
        label=string(prt)
        gr_i=[]
    case "CLKSPLIT_f" then
        label=[]
        gr_i=[]
    case "CLKSOM_f" then
        label=[]
        gr_i=[]
    case "CLOCK_f" then
        orig=o(2)(1)
        sz=o(2)(2)
        oo=o(3)(8)
        dt=0.1
        for ko=2:size(oo)
            if oo(ko)(3)(1)=="evtdly" then
                dt=oo(ko)(3)(8)
                break
            end
        end
        o = list("Block",..
        list([0,0],[2,2],%t,[],[],[],[],0,..
        list(..
        ["wd=xget(''wdim'').*[1.016,1.12];";
        "thick=xget(''thickness'');xset(''thickness'',2);";
        "p=wd(2)/wd(1);p=1;";
        "rx=sz(1)*p/2;ry=sz(2)/2;";
        "xarcs([orig(1)+0.05*sz(1);";
        "orig(2)+0.95*sz(2);";
        "   0.9*sz(1)*p;";
        "   0.9*sz(2);";
        "   0;";
        "   360*64],scs_color(5));";
        "xset(''thickness'',1);";
        "xx=[orig(1)+rx    orig(1)+rx;";
        "    orig(1)+rx    orig(1)+rx+0.6*rx*cos(%pi/6)];";
        "yy=[orig(2)+ry    orig(2)+ry ;";
        "  orig(2)+1.8*ry  orig(2)+ry+0.6*ry*sin(%pi/6)];";
        "xsegs(xx,yy,scs_color(10));";
        "xset(''thickness'',thick);"],[])),..
        list("csuper",[],[],[],1,[]," ",..
        list(list([600,400,0,0],"foo",[],[]),..
        list("Block",list([399,162],[20,20],%t,"1",[],[],6,[],[]),..
        list("output",[],[],1,[],[],[],[],1,"d",[],[%f,%f]," ",list())," ","CLKOUT_f"),..
        list("Block",list([320,232],[40,40],%t,["0.1";"0.1"],[],[],7,4,..
        ["dt=model(8);";
        "txt=[''Delay'';string(dt)];";
        "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');"]),..
        list("evtdly",[],[],1,1,[],[],0.1,[],"d",0.1,[%f,%f]," ",list())," ","EVTDLY_f"),..
        list("Link",[340;340;380.71066],[226.28571;172;172],"drawlink"," ",[0,0],[5,-1],[3,1],..
        [5,1]),..
        list("Block",list([380.71066;172],[1,1],%t," ",[],[],4,[6;7]),..
        list("lsplit",[],[],1,[1;1],[],[],[],[],"d",[-1,-1],[%t,%f]," ",list())," ","CLKSPLIT_f"),..
        list("Link",[380.71066;399],[172;172],"drawlink"," ",[0,0],[5,-1],[5,1],[2,1]),..
        list("Link",[380.71066;380.71066;340;340],[172;301.9943;301.9943;277.71429],"drawlink"," ",..
        [0,0],[5,-1],[5,2],[3,1])),[],"h",[],[%f,%f])," ","CLOCK_f")

        o(2)(1)=orig
        o(2)(2)=sz
        xx=o(3)(8)(3)
        xx(2)(4)=string([dt;dt])
        xx(3)(11)=dt
        xx(3)(8)=dt
        o(3)(8)(3)=xx
        model=o(3)
    else
        ok=%f
    end

    if ok then
        graphics(4)=label
        graphics(9)=gr_i
        o(2)=graphics
        model(13)=" ";model(14)=list()
        o(3)=model
        o=replace_firing(o)
        return
    end

    ok=%t
    select blocktype
    case "CLR_f" then
        ipar=model(9);model(9)=[]
        ns=size(model(6),"*");nin=1;nout=1;
        rpar=model(8);
        A=matrix(rpar(1:ns*ns),ns,ns);
        B=matrix(rpar(ns*ns+1:ns*(ns+nin)),ns,nin);
        C=matrix(rpar(ns*(ns+nin)+1:ns*(ns+nin+nout)),nout,ns);
        D=rpar(ns*(ns+nin+nout)+1);
        H=ss2tf(syslin("c",A,B,C,D));
        H=clean(H);
        if type(H)==16 then
            num=H(2);den=H(3)
        else
            num=H,den=1
        end
        label=[sci2exp(num);sci2exp(den)]
        gr_i=["xstringb(orig(1),orig(2),[''Num(s)'';''-----'';''Den(s)''],sz(1),sz(2),''fill'');"]
    case "CLSS_f" then
        in=model(2);out=model(3)
        nin=sum(in)
        nout=sum(out)
        x0=model(6),
        rpar=model(8)
        ns=prod(size(x0))
        A=matrix(rpar(1:ns*ns),ns,ns)
        B=matrix(rpar(ns*ns+1:ns*(ns+nin)),ns,nin)
        C=matrix(rpar(ns*(ns+nin)+1:ns*(ns+nin+nout)),nout,ns)
        D=matrix(rpar(ns*(ns+nin+nout)+1:ns*(ns+nin+nout)+(nin*nout)),nout,nin)
        label=[  strcat(sci2exp(A));
        strcat(sci2exp(B));
        strcat(sci2exp(C));
        strcat(sci2exp(D));
        strcat(sci2exp(x0))]
        gr_i=["txt=[''xd=Ax+Bu'';''y=Cx+Du''];";
        "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');"]
        model(2)=nin;model(3)=nout
    case "CONST_f" then
        C=model(8);model(3)=sum(model(3))
        label=[strcat(sci2exp(C))]
        gr_i=["model=arg1(3);C=model(8);";
        "dx=sz(1)/5;dy=sz(2)/10;";
        "w=sz(1)-2*dx;h=sz(2)-2*dy;";
        "xstringb(orig(1)+dx,orig(2)+dy,string(C),w,h,''fill'');"]
    case "COSBLK_f" then
        in=model(2);nin=sum(in)
        if nin<>1 then
            write(%io(2),["Pb with COSBLK_f block";
            "previously block has more than one input port";
            "It is better to change it with the new block version";
            "preceded by a multiplexer and followed by a demultiplxer"])
        end
        label=" "
        gr_i=["xstringb(orig(1),orig(2),[''cos''],sz(1),sz(2),''fill'');"]
    case "CURV_f" then
        label=[]
        gr_i=["model=arg1(3);rpar=model(8);ipar=model(9);n=ipar(1);";
        "thick=xget(''thickness'');xset(''thickness'',2);";
        "xx=rpar(1:n);yy=rpar(n+1:2*n);";
        "rect=rpar(2*n+1:2*n+4);";
        "mxx=rect(3)-rect(1);";
        "mxy=rect(4)-rect(2);";
        "xx=orig(1)+sz(1)*(1/10+(4/5)*((xx-rect(1))/mxx));";
        "yy=orig(2)+sz(2)*(1/10+(4/5)*((yy-rect(2))/mxy));";
        "xpoly(xx,yy,''lines'');";
        "xset(''thickness'',thick);"]
    case "DELAY_f" then
        orig=o(2)(1)
        sz=o(2)(2)
        oo=o(3)(8)
        dt=[];z0=[]
        for ko=2:size(oo)
            if oo(ko)(3)(1)=="evtdly" then
                dt=oo(ko)(3)(8)
            elseif oo(ko)(3)(1)=="register" then
                z0=oo(ko)(3)(7)
            end
        end
        if dt==[]|z0==[] then
            write(%io(2), "Pb with DELAY_f block dt or z0 not found")
            pause
        end
        o=DELAY_f("define")
        o(2)(1)=orig
        o(2)(2)=sz
        o(3)(8)(4)(2)(4)=sci2exp(z0)
        o(3)(8)(5)(2)(4)=sci2exp(dt)
        o(3)(8)(5)(3)(8)=dt
        o(3)(8)(5)(3)(11)=0
        o(3)(8)(4)(3)(7)=z0(:)
        model=o(3)
        label=[]
        gr_i=["b1=xstringl(0,0,''e'');";
        "b2=xstringl(0,0,''-Ts'');";
        "h=-b1(2)+max(0,sz(2)-0.5*b1(4)+b2(4))/2;";
        "w=max(0,sz(1)-b1(3)-b1(4))/2;";
        "xstring(orig(1)+w,orig(2)+h,''e'');";
        "xstring(orig(1)+w+b1(3)/2,orig(2)+h+b1(4)*0.5,''-Ts'');"]
    case "DLRADAPT_f" then
        [dstate,rpar,ipar]=model(7:9)
        m=ipar(1);
        n=ipar(2)
        npt=ipar(3)
        p=rpar(1:npt)

        rn=matrix(rpar(npt+1:npt+m*npt)+%i*rpar(npt+m*npt+1:npt+2*m*npt),npt,m)
        rd=matrix(rpar(npt+2*m*npt+1:npt+(2*m+n)*npt)+..
        %i*rpar(npt+(2*m+n)*npt+1:npt+2*(m+n)*npt),npt,n)
        g=rpar(npt+2*(m+n)*npt+1:npt+2*(m+n)*npt+npt)
        last_u=dstate(1:m);last_y=dstate(m+1:m+n)
        label=[sci2exp(p);
        sci2exp(rn);
        sci2exp(rd);
        sci2exp(g);
        sci2exp(last_u);
        sci2exp(last_y)]
        gr_i=["txt=[''N(z,p)'';''-----'';''D(z,p)''];";
        "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');"]
    case "DLR_f" then
        model(9)=[];model(10)="d"
        ns=size(model(7),"*");nin=1;nout=1;
        rpar=model(8);
        A=matrix(rpar(1:ns*ns),ns,ns);
        B=matrix(rpar(ns*ns+1:ns*(ns+nin)),ns,nin);
        C=matrix(rpar(ns*(ns+nin)+1:ns*(ns+nin+nout)),nout,ns);
        D=rpar(ns*(ns+nin+nout)+1);
        H=ss2tf(syslin("d",A,B,C,D));
        H=clean(H);
        if type(H)==16 then
            num=H(2);den=H(3)
        else
            num=H,den=1
        end
        label=[sci2exp(num);sci2exp(den)]
        gr_i=["txt=[''Num(z)'';''-----'';''Den(z)''];";
        "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');"]
    case "DLSS_f" then
        in=model(2);out=model(3)
        nin=sum(in)
        nout=sum(out)
        x0=model(6),rpar=model(8)
        ns=prod(size(x0))
        A=matrix(rpar(1:ns*ns),ns,ns)
        B=matrix(rpar(ns*ns+1:ns*(ns+nin)),ns,nin)
        C=matrix(rpar(ns*(ns+nin)+1:ns*(ns+nin+nout)),nout,ns)
        D=matrix(rpar(ns*(ns+nin+nout)+1:ns*(ns+nin+nout)+(nin*nout)),nout,nin)

        label=[  strcat(sci2exp(A));
        strcat(sci2exp(B));
        strcat(sci2exp(C));
        strcat(sci2exp(D));
        strcat(sci2exp(x0))]
        gr_i=["txt=[''x+=Ax+Bu'';''y=Cx+Du''];";
        "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');"]
    case "EVENTSCOPE_f" then
        [nclock,rpar,ipar]=model([4 8 9])
        win=ipar(1);
        per=rpar(1);
        wdim=[-1;-1]
        wpos=[-1;-1]
        clrs=[1;3;5;7;9;11;13;15];
        model(9)=[win;1;clrs;wpos(:);wdim(:)]
        label=[sci2exp(nclock);
        strcat(sci2exp(clrs)," ");
        string(win);
        sci2exp([]);
        sci2exp([]);
        string(per)]
        gr_i=["thick=xget(''thickness'');xset(''thickness'',2);";
        "xrect(orig(1)+sz(1)/10,orig(2)+(1-1/10)*sz(2),sz(1)*8/10,sz(2)*8/10);";
        "xx=[orig(1)+sz(1)/5,orig(1)+sz(1)/5;";
        "orig(1)+(1-1/5)*sz(1),orig(1)+sz(1)/5];";
        "yy=[orig(2)+sz(2)/5,orig(2)+sz(2)/5;";
        "orig(2)+sz(2)/5,orig(2)+(1-1/5)*sz(2)];";
        "xarrows(xx,yy);";
        "t=(0:0.3:2*%pi)'';";
        "xx=orig(1)+(1/5+3*t/(10*%pi))*sz(1);";
        "yy=orig(2)+(1/4.3+(sin(t)+1)*3/10)*sz(2);";
        "xpoly(xx,yy,''lines'');";
        "xset(''thickness'',thick);"]
    case "EVTDLY_f" then
        dt=model(8);
        if model(11) then ff=0; else ff=-1; end
        model(11)=ff
        label=[string(dt);string(ff)]
        gr_i=["dt=model(8);";
        "txt=[''Delay'';string(dt)];";
        "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');"]
    case "EVTGEN_f" then
        tt=model(11);
        label=string(tt)
        gr_i=["xstringb(orig(1),orig(2),''Event'',sz(1),sz(2),''fill'');"]
    case "FOR_f" then
        write(%io(2),"FOR block nor more exist")
    else
        ok=%f
    end
    if ok then
        graphics(4)=label
        graphics(9)=gr_i
        o(2)=graphics
        model(13)=" ";model(14)=list()
        o(3)=model
        o=replace_firing(o)
        return
    end
    ok=%t
    select blocktype
    case "GAIN_f" then
        [in,out]=model(2:3)
        gain=matrix(model(8),out,in)
        label=[strcat(sci2exp(gain))]
        gr_i=["[nin,nout]=model(2:3);";
        "if nin*nout==1 then gain=string(model(8)),else gain=''Gain'',end";
        "dx=sz(1)/5;";
        "dy=sz(2)/10;";
        "xx=orig(1)+      [1 4 1 1]*dx;";
        "yy=orig(2)+sz(2)-[1 5 9 1]*dy;";
        "xpoly(xx,yy,''lines'');";
        "w=sz(1)-2*dx;h=sz(2)-2*dy;";
        "xstringb(orig(1)+dx,orig(2)+dy,gain,w,h,''fill'');"]
    case "GENERAL_f" then
        in=model(2);out=model(5)
        label=[strcat(sci2exp(in));strcat(sci2exp(out))]
        gr_i=["xstringb(orig(1),orig(2),''GENERAL'',sz(1),sz(2),''fill'');"]
    case "GENSIN_f" then
        rpar=model(8)
        label=[string(rpar(1));string(rpar(2));string(rpar(3))]
        gr_i=["txt=[''sinusoid'';''generator''];";
        "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');"]
    case "GENSQR_f" then
        //rpar=model(8); ?
        Amplitude=model(7)
        label=string(Amplitude)
        gr_i=["txt=[''square wave'';''generator''];";
        "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');"]
    case "HALT_f" then
        n=model(9);
        label=string(n)
        gr_i=["xstringb(orig(1),orig(2),''STOP'',sz(1),sz(2),''fill'');"]
    case "IFTHEL_f" then
        label=[]
        model(11)=[-1,-1]
        gr_i=["txt=[''If in>=0'';'' '';'' then    else''];";
        "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');"]
    case "INTEGRAL_f" then
        x0=model(6);
        label=strcat(sci2exp(x0))
        gr_i=["xstringb(orig(1),orig(2),''  1/s  '',sz(1),sz(2),''fill'');"]
    case "INVBLK_f" then
        model(2)=-1;model(3)=-1
        label=" "
        gr_i=["xstringb(orig(1),orig(2),''1/u'',sz(1),sz(2),''fill'');"]
    case "IN_f" then
        prt=model(9);
        label=[string(prt)]
        model(3)=-1
        gr_i=[]
    case "LOGBLK_f" then
        a=model(8)
        in=model(2)
        label=[string(a)]
        model(2)=-1;model(3)=-1
        gr_i=["xstringb(orig(1),orig(2),''log'',sz(1),sz(2),''fill'');"]
    case "LOOKUP_f" then
        model(10)="c"
        gr_i=["rpar=model(8);n=size(rpar,''*'')/2;";
        "thick=xget(''thickness'');xset(''thickness'',2);";
        "xx=rpar(1:n);yy=rpar(n+1:2*n);";
        "mnx=min(xx);xx=xx-mnx*ones(xx);mxx=max(xx);";
        "xx=orig(1)+sz(1)*(1/10+(4/5)*xx/mxx);";
        "mnx=min(yy);yy=yy-mnx*ones(yy);mxx=max(yy);";
        "yy=orig(2)+sz(2)*(1/10+(4/5)*yy/mxx);";
        "xpoly(xx,yy,''lines'');";
        "xset(''thickness'',thick);"]
        label=[]
    case "MAX_f" then
        in=model(2);nin=sum(in)
        if nin<>1 then
            write(%io(2),["Pb with MAX_f block";
            "previously block has more than one input port";
            "It is better to change it with the new block version";
            "preceded by a multiplexer"])
        end
        label=" "
        gr_i=["xstringb(orig(1),orig(2),''Max'',sz(1),sz(2),''fill'');"]
    case "MCLOCK_f" then
        label=[]
        gr_i=["txt=[''2freq clock'';''  f/n     f''];";
        "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')"]
    case "MFCLCK_f" then
        dt=model(8)
        nn=model(9)
        label=[string(dt);string(nn)]
        gr_i=["txt=[''M. freq'';''clock''];";
        "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')"]
    case "MIN_f" then
        in=model(2);nin=sum(in)
        if nin<>1 then
            write(%io(2),["Pb with MIN_f block";
            "previously block has more than one input port";
            "It is better to change it with the new block version";
            "preceded by a multiplexer"])
        end
        label=" "
        gr_i=["xstringb(orig(1),orig(2),''MIN'',sz(1),sz(2),''fill'')"]
    case "NEGTOPOS_f" then
        label=[]
        gr_i=["xstringb(orig(1),orig(2),'' - to + '',sz(1),sz(2),''fill'');"]
    case "OUT_f" then
        prt=model(9);
        model(2)=-1
        label=[string(prt)]
        gr_i=[]
    case "POSTONEG_f" then
        label=[]
        gr_i=["xstringb(orig(1),orig(2),'' + to - '',sz(1),sz(2),''fill'')"]
    case "POWBLK_f" then
        in=model(2);nin=sum(in)
        if nin<>1 then
            write(%io(2),["Pb with MIN_f block";
            "previously block has more than one input port";
            "It is better to change it with the new block version";
            "preceded by a multiplexer and followed by a demux"])
        end
        if model(8)<>[] then
            a=model(8)
        else
            a=model(9)
        end
        in=model(2)
        label=[string(a)]
        gr_i=["xstringb(orig(1),orig(2),''u^a'',sz(1),sz(2),''fill'');"]
    case "PROD_f" then
        label=[]
        gr_i=[]
        model(1)=list("prod",2)
    case "QUANT_f" then
        rpar=model(8);ipar=model(9);
        pas=rpar(1)
        meth=ipar
        label=[string(pas);string(meth)]
        gr_i=["thick=xget(''thickness'');xset(''thickness'',2);";
        "xx=orig(1)+[1;2;2;3;3;4;4]/5*sz(1);";
        "yy= orig(2)+[1;1;2;2;3;3;4]/5*sz(2);";
        "xpoly(xx,yy,''lines'');";
        "xset(''thickness'',thick);"]
    case "RAND_f" then
        rpar=model(8);flag=model(9);
        out=model(3);nout=sum(out)
        if nout<>1 then
            write(%io(2),["Pb with RAND_f block";
            "previously block has more than one output port";
            "It is better to change it with the new block version";
            " followed by a demux"])
        end

        a=rpar(1:nout)
        b=rpar(nout+1:2*nout)
        label=[string(flag);sci2exp(a(:));sci2exp(b(:))]
        gr_i=["txt=[''random'';''generator''];";
        "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')"]
    case "REGISTER_f" then
        z0=model(7)
        label=strcat(string(z0),";")

        gr_i=["dly=model(8);";
        "txt=[''Shift'';''Register'';string(dly)];";
        "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')"]
    else
        ok=%f
    end
    if ok then
        graphics(4)=label
        graphics(9)=gr_i
        o(2)=graphics
        model(13)=" ";model(14)=list()
        o(3)=model
        o=replace_firing(o)
        return
    end
    ok=%t
    select blocktype
    case "RFILE_f" then
        [out,state,ipar]=model([3 7 9])
        nout=sum(out)
        ievt=ipar(3);N=ipar(4);
        imask=5+ipar(1)+ipar(2)
        if ievt<>0 then tmask=ipar(imask),else tmask=[],end
        outmask=ipar(imask+ievt:imask+nout+ievt-1)
        lunit=state(2)
        ievt=ipar(3);N=ipar(4);
        imask=5+ipar(1)+ipar(2)
        if ievt<>0 then tmask=ipar(imask),else tmask=[],end
        outmask=ipar(imask+ievt:imask+nout+ievt-1)
        lfil=ipar(1)
        lfmt=ipar(2)
        if lfil>0 then fname=ascii(ipar(5:4+lfil)),else fname=" ",end
        if lfmt>0 then Fmt=ascii(ipar(5+lfil:4+lfil+lfmt)),else Fmt=" ",end
        label=[ sci2exp(tmask);
        sci2exp(outmask);
        fname;
        Fmt;
        string(N);
        sci2exp(out)]
        gr_i=["txt=[''read from'';''input file''];";
        "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')"]
    case "SAT_f" then
        rpar=model(8)
        minp=rpar(1),maxp=rpar(2),pente=rpar(3)
        label=[string(minp);string(maxp);string(pente)]

        gr_i=["thick=xget(''thickness'');xset(''thickness'',2);";
        "xx=orig(1)+[4/5;1/2+1/5;1/2-1/5;1/5]*sz(1);";
        "yy=orig(2)+[1-1/5;1-1/5;1/5;1/5]*sz(2);";
        "xpoly(xx,yy,''lines'');";
        "xset(''thickness'',thick)"]
    case "SAWTOOTH_f" then
        model(10)="c"
        gr_i=["txt=[''sawtooth'';''generator''];";
        "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');"]
    case "SCOPE_f" then
        [in,rpar,ipar]=model([2 8 9])
        in=model(2);nin=sum(in)
        if nin<>1 then
            write(%io(2),["Pb with SCOPE_f block";
            "previously block has more than one input port";
            "It is better to change it with the new block version";
            "preceded by a multiplexer"])
        end
        win=ipar(1);N=ipar(3);
        clrs=-ipar(4:nin+3)
        if size(clrs,"*")<8 then clrs(8)=1;end

        wdim=[-1;-1]
        wpos=[-1;-1]
        model(9)=[win;1;N;clrs;wpos(:);wdim(:)]
        dt=rpar(1);ymin=rpar(2);ymax=rpar(3);per=rpar(4)
        label=[strcat(string(clrs)," ");
        string(win);
        sci2exp([]);
        sci2exp([]);
        string(ymin);
        string(ymax);
        string(per);
        string(N)];

        gr_i=["thick=xget(''thickness'');xset(''thickness'',2);";
        "xrect(orig(1)+sz(1)/10,orig(2)+(1-1/10)*sz(2),sz(1)*8/10,sz(2)*8/10);";
        "xx=[orig(1)+sz(1)/5,orig(1)+sz(1)/5;";
        "orig(1)+(1-1/5)*sz(1),orig(1)+sz(1)/5];";
        "yy=[orig(2)+sz(2)/5,orig(2)+sz(2)/5;";
        "orig(2)+sz(2)/5,orig(2)+(1-1/5)*sz(2)];";
        "xarrows(xx,yy);";
        "t=(0:0.3:2*%pi)'';";
        "xx=orig(1)+(1/5+3*t/(10*%pi))*sz(1);";
        "yy=orig(2)+(1/4.3+(sin(t)+1)*3/10)*sz(2);";
        "xpoly(xx,yy,''lines'');";
        "xset(''thickness'',thick)"]
    case "SCOPXY_f" then
        [rpar,ipar]=model(8:9)
        win=ipar(1);N=ipar(3);clrs=-ipar(4);siz=ipar(5)
        xmin=rpar(1);xmax=rpar(2);ymin=rpar(3);ymax=rpar(4)
        wdim=[-1;-1]
        wpos=[-1;-1]
        label=[sci2exp(clrs);
        sci2exp(siz);
        string(win);
        sci2exp([]);
        sci2exp([]);
        string(xmin);
        string(xmax);
        string(ymin);
        string(ymax);
        string(N)];
        model(9)=[win;1;N;clrs;siz;1;wpos(:);wdim(:)]
        gr_i=["thick=xget(''thickness'');xset(''thickness'',2);";
        "t=(0:0.2:2*%pi)'';";
        "xx=orig(1)+(1/5+(cos(3*t)+1)*3/10)*sz(1);";
        "yy=orig(2)+(1/4.3+(sin(t+1)+1)*3/10)*sz(2);";
        "xpoly(xx,yy,''lines'');";
        "xset(''thickness'',thick)"]
    case "SELECT_f" then
        z0=model(7);nin=size(model(2),1);
        label=[string(nin);string(z0-1)]
        model(1)=list("selector",2)
        gr_i=["xstringb(orig(1),orig(2),''Selector'',sz(1),sz(2),''fill'');"]
    case "SINBLK_f" then
        in=model(2);nin=sum(in)
        if nin<>1 then
            write(%io(2),["Pb with MIN_f block";
            "previously block has more than one input port";
            "It is better to change it with the new block version";
            "preceded by a multiplexer and followed by a demux"])
        end
        label=" "
        gr_i=["xstringb(orig(1),orig(2),''sin'',sz(1),sz(2),''fill'')"]
    case "SOM_f" then
        sgn=model(8);
        label=sci2exp(sgn)
        model(1)=list("sum",2)
        gr_i=[]
    case "SPLIT_f" then
        label=[]
        gr_i=[]
    case "SUPER_f" then
        label=[]
        gr_i=["thick=xget(''thickness'');xset(''thickness'',2);";
        "xx=orig(1)+      [2 4 4]*(sz(1)/7);";
        "yy=orig(2)+sz(2)-[2 2 6]*(sz(2)/10);";
        "xrects([xx;yy;[sz(1)/7;sz(2)/5]*ones(1,3)]);";
        "xx=orig(1)+      [1 2 3 4 5 6 3.5 3.5 3.5 4 5 5.5 5.5 5.5]*sz(1)/7;";
        "yy=orig(2)+sz(2)-[3 3 3 3 3 3 3   7   7   7 7 7   7   3  ]*sz(2)/10;";
        "xsegs(xx,yy,0);";
        "xset(''thickness'',thick)"]
    case "TANBLK_f" then
        in=model(2);nin=sum(in)
        if nin<>1 then
            write(%io(2),["Pb with TANBLK_f block";
            "previously block has more than one input port";
            "It is better to change it with the new block version";
            "preceded by a multiplexer and followed by a demux"])
        end
        label=" "
        gr_i=["xstringb(orig(1),orig(2),''tan'',sz(1),sz(2),''fill'');"]
    case "TCLSS_f" then
        in=model(2);nin=sum(in)
        out=model(3);nout=sum(out)
        x0=model(6),rpar=model(8)

        ns=prod(size(x0));nin=nin-ns
        A=matrix(rpar(1:ns*ns),ns,ns)
        B=matrix(rpar(ns*ns+1:ns*(ns+nin)),ns,nin)
        C=matrix(rpar(ns*(ns+nin)+1:ns*(ns+nin+nout)),nout,ns)
        D=matrix(rpar(ns*(ns+nin+nout)+1:ns*(ns+nin+nout)+(nin*nout)),nout,nin)
        nin1=nin;nout1=nout

        label=[strcat(sci2exp(A));
        strcat(sci2exp(B));
        strcat(sci2exp(C));
        strcat(sci2exp(D));
        strcat(sci2exp(x0))]
        gr_i=["txt=[''Jump'';''(A,B,C,D)''];";
        "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')"]
    else
        ok=%f
    end
    if ok then
        graphics(4)=label
        graphics(9)=gr_i
        o(2)=graphics
        model(13)=" "
        model(14)=list()
        o(3)=model
        o=replace_firing(o)
        return
    end
    ok=%t
    select blocktype
    case "TEXT_f" then
        ipar=model(9)
        font=ipar(1);siz=ipar(2)
        label=[graphics(4);string(font);string(siz)]
        gr_i=[]
    case "TIME_f" then
        label=[]
        gr_i=["wd=xget(''wdim'').*[1.016,1.12];";
        "thick=xget(''thickness'');xset(''thickness'',2);";
        "p=wd(2)/wd(1);p=1;";
        "rx=sz(1)*p/2;ry=sz(2)/2;";
        "xarc(orig(1)+0.05*sz(1),orig(2)+0.95*sz(2),0.9*sz(1)*p,0.9*sz(2),0,360*64);";
        "xset(''thickness'',1);";
        "xx=[orig(1)+rx    orig(1)+rx;";
        "orig(1)+rx    orig(1)+rx+0.6*rx*cos(%pi/6)];";
        "yy=[orig(2)+ry    orig(2)+ry ;";
        "     orig(2)+1.8*ry  orig(2)+ry+0.6*ry*sin(%pi/6)];";
        "xsegs(xx,yy,0);";
        "xset(''thickness'',thick);"]
    case "TRASH_f" then
        in=model(2);nin=sum(in)
        if nin<>1 then
            write(%io(2),["Pb with MIN_f block";
            "previously block has more than one input port";
            "It is better to change it with the new block version";
            "preceded by a multiplexer"])
        end
        label=" "
        gr_i=["xstringb(orig(1),orig(2),''Trash'',sz(1),sz(2),''fill'')"]
    case "WFILE_f" then
        state=model(7)
        [in,ipar]=model([2  9])
        N=ipar(3);
        lunit=state(2)
        N=ipar(4)
        lfil=ipar(1)
        lfmt=ipar(2)
        if lfil>0 then fname=ascii(ipar(5:4+lfil)),else fname=" ",end
        if lfmt>0 then Fmt=ascii(ipar(5+lfil:4+lfil+lfmt)),else Fmt=" ",end
        label=[sci2exp(in);
        fname;
        Fmt;
        string(N)]

        gr_i=["txt=[''write to'';''output file''];";
        "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')"]
    case "ZCROSS_f" then
        in=model(2)
        label=strcat(sci2exp(in))
        gr_i=["xstringb(orig(1),orig(2),''Zcross'',sz(1),sz(2),''fill'');"]
    case "func_block" then
        write(%io(2),["PB with func_block";
        "version change is not implemented"])
        label=[]
        gr_i=["xstringb(orig(1),orig(2),''Func'',sz(1),sz(2),''fill'');"]
    case "m_sin" then
        rpar=model(8);gain=rpar(1);phase=rpar(2)
        label=[string(gain);string(phase)]
        gr_i=[]
    case "sci_block" then
        write(%io(2),["PB with sci_block";
        "version change is not implemented"])
        label=[]
        gr_i=["xstringb(orig(1),orig(2),''Sci_Block'',sz(1),sz(2),''fill'');"]
    case "scifunc_block" then
        write(%io(2),["PB with scifunc_block";
        "version change is not implemented"])
        i1=model(2);o1=model(3);ci1=model(4);co1=model(5);x1=model(6);
        z1=model(7);auto1=model(11);type_1=model(10);
        tt=model(9),rpar=model(8)
        label=list([sci2exp(i1);sci2exp(o1);sci2exp(ci1);sci2exp(co1);
        strcat(sci2exp(x1));strcat(sci2exp(z1));type_1;
        strcat(sci2exp(rpar));sci2exp(auto1)],tt)

        gr_i=["xstringb(orig(1),orig(2),''Scifunc'',sz(1),sz(2),''fill'');"]
    case "standard_block" then
        write(%io(2),["PB with standard_block";
        "version change is not implemented"])
        label=[]
        gr_i=[]
    else
        disp(blocktype+" unknown, parameters values may be lost, please check")
        label=[]
        gr_i=[]
    end
    graphics(4)=label
    graphics(9)=gr_i
    o(2)=graphics
    model(13)=" "
    model(14)=list()
    o(3)=model
    o=replace_firing(o)

endfunction

function o=replace_firing(o)
    firing=o(3)(11)
    cout=o(3)(5)
    if firing==%f|firing==0 then
        o(3)(11)=-ones(cout)
        //disp('firing changed from %f to '+sci2exp(o(3)(11))+' in '+o(3)(1)(1))
    elseif firing==%t|firing==1  then
        o(3)(11)=0*cout
        //disp('firing changed from %t to '+sci2exp(o(3)(11))+' in '+o(3)(1)(1))
    elseif firing==[] then
        o(3)(11)=[]
    else
        write(%io(2),"Problem with block "+o(3)(1)(1))
        pause
    end

endfunction
function x_new=do_version22(scs_m)
    x_new=list()
    wpar=scs_m(1)
    wsiz=wpar(1)

    if size(wsiz,"*")>=4 then x_new=scs_m,return,end

    wpar(3)(4)=wpar(4)+1
    x_new(1)=wpar
    nx=size(scs_m)
    for k=2:nx
        o=scs_m(k)
        if o(1)=="Link" then
        elseif o(1)=="Block" then
            model=o(3)
            if model(1)=="super"|model(1)=="csuper" then
                model(8)=do_version22(model(8))
                o(3)=model
                o=block_version(o)
                scs_m(k)=o
            else
                o=block_version(o)
            end
        elseif o(1)=="Text" then
            o=block_version(o)
        end
        x_new(k)=o;
    end
endfunction

function scs_m_new=do_version27(scs_m)
    if typeof(scs_m)=="diagram" then
        //following code reported into do_version273
        //     scs_m_new=scs_m,
        //     nlstsize=lstsize(scs_m_new.objs)
        //     for k=1:nlstsize
        //       if typeof(scs_m_new.objs(k))=='Link' then
        //  o=scs_m_new.objs(k)
        //  if size(o.from,'*')==2 then o.from(3)=0,end
        //  if size(o.to,'*')==2 then o.to(3)=1,end
        //  scs_m_new.objs(k)=o
        //       elseif typeof(scs_m_new.objs(k))=='Block' then

        //       end
        //     end
        scs_m_new=scs_m
        return,
    end
    scs_m_new=scicos_diagram()
    tf=scs_m(1)(4)
    if tf==[] then tf=100;end
    tol=scs_m(1)(3)
    if size(tol,"*")<4 then tol(4)=tf+1,end
    if size(tol,"*")<5 then tol(5)=0,end
    if size(tol,"*")<6 then tol(6)=0,end
    for iix=size(scs_m(1))+1:10,scs_m(1)(iix)=[];end
    scs_m_new.props=scicos_params(wpar=scs_m(1)(1),Title=scs_m(1)(2),..
    tol=tol,tf=tf,..
    context=scs_m(1)(5),options=scs_m(1)(7),..
    doc=scs_m(1)(10))

    if scs_m(1)(7).Background==[] then
        scs_m_new.props.options.Background=[8 1]
    end

    scs_m_new.objs(1)=mlist("Deleted") // not to change the internal numbering
    n=lstsize(scs_m)
    back_col=8   //white background

    mprintf(".")

    for i=2:n //loop on objects
        o=scs_m(i);
        if o(1)=="Block" then
            if size(o(2)) > 8 then
                if type(o(2)(9))==15 then
                    gr_io=o(2)(9)(1);
                    if o(2)(9)(2)<>[] then
                        back_col=o(2)(9)(2);,
                    end
                else
                    gr_io=o(2)(9);
                    back_col=8
                end
                gr_i=convert_gri(o(5),gr_io);
                if gr_i==[] then gr_i=gr_io;, end
            elseif size(o(2)) < 9 then
                gr_i=[];
                back_col=8
            end
            gr_i=list(gr_i,back_col)

            mdl=o(3);
            if size(o(3))<=12 then
                mdl(13)=""; mdl(14)=[] ; mdl(15)="";
            elseif size(o(3))<=13 then
                mdl(14)=[] ; mdl(15)="";
            elseif size(o(3))<=14 then
                mdl(15)="";
            end

            if mdl(1)(1)=="super"|mdl(1)(1)=="csuper" then
                if type(mdl(8))==15 then
                    mdl(8)=do_version27(mdl(8))
                end
            end

            if o(2)(3) == %f then
                style = "flip=0;";
            else
                style = "";
            end
            graphics=scicos_graphics27(orig=o(2)(1),sz=o(2)(2),..
            exprs=o(2)(4),pin=o(2)(5),pout=o(2)(6),..
            pein=o(2)(7),peout=o(2)(8),gr_i=gr_i,..
            id=mdl(15),style=style)


            model=scicos_model27(sim=mdl(1),in=mdl(2),out=mdl(3),evtin=mdl(4),..
            evtout=mdl(5),state=mdl(6),dstate=mdl(7),..
            rpar=mdl(8),ipar=mdl(9),blocktype=mdl(10),..
            firing=mdl(11),dep_ut=mdl(12),label=mdl(13))


            objsi=scicos_block(graphics=graphics,model=model,gui=o(5),..
            doc=mdl(14))
            if objsi.gui=="ESELECT_f" then objsi.model.sim(2)=-2,end
            scs_m_new.objs(i)=objsi

        elseif o(1)=="Link" then
            from=o(8);from(3)=0;
            to=o(9);to(3)=1;

            objsi=scicos_link(xx=o(2),yy=o(3),id=o(5),thick=o(6),..
            ct=o(7),from=from,to=to)
            scs_m_new.objs(i)=objsi
        elseif o(1)=="Text" then

            objsi=TEXT_f("define")
            objsi.model.rpar=o(3)(8)
            objsi.model.ipar=o(3)(9)
            objsi.graphics.orig=o(2)(1)
            objsi.graphics.sz=o(2)(2)
            objsi.graphics.exprs=o(2)(4)
            scs_m_new.objs(i)=objsi
        elseif o(1)=="Deleted" then
            scs_m_new.objs(i)=mlist("Deleted")
        end
    end

    [%cpr,edited]=resume(list(),%t) // doit etre enleve
endfunction

function  gr_i=convert_gri(name,gri)
    gr_i=[];
    select name
    case "BIGSOM_f" then
        gr_i=["[x,y,typ]=standard_inputs(o) ";
        "sgn=o.model.rpar";
        "for k=1:size(x,''*'')";
        "  if sgn(k)>0 then";
        "    xstring(orig(1)+sz(1)/8 ,y(k)-4,''+'')";
        "  else";
        "    xstring(orig(1)+sz(1)/8 ,y(k)-4,''-'')";
        "  end";
        "end";
        "xx=sz(1)*[.8 .4 0.75 .4 .8]+orig(1)";
        "yy=sz(2)*[.8 .8 .5 .2 .2]+orig(2)";
        "xpoly(xx,yy,''lines'')"]

    case "CONST_f" then
        gr_i=["dx=sz(1)/5;dy=sz(2)/10;";
        "w=sz(1)-2*dx;h=sz(2)-2*dy;";
        "txt=C;"
        "xstringb(orig(1)+dx,orig(2)+dy,txt,w,h,''fill'');"]

    case "CURV_f" then
        gr_i=["model=arg1.model;rpar=model.rpar;ipar=model.ipar;n=ipar(1);";
        "thick=xget(''thickness'');xset(''thickness'',2);";
        "xx=rpar(1:n);yy=rpar(n+1:2*n);";
        "rect=rpar(2*n+1:2*n+4);";
        "mxx=rect(3)-rect(1);";
        "mxy=rect(4)-rect(2);";
        "xx=orig(1)+sz(1)*(1/10+(4/5)*((xx-rect(1))/mxx));";
        "yy=orig(2)+sz(2)*(1/10+(4/5)*((yy-rect(2))/mxy));";
        "xpoly(xx,yy,''lines'');";
        "xset(''thickness'',thick);"]

    case "EVTDLY_f" then
        gr_i=["dt=model.rpar;";
        "txt=[''Delay'';string(dt)];";
        "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');"]

    case "EVTGEN_f" then
        gr_i=["tt=model.firing;";
        "txt=[''Event at'';''time ''+string(tt)];";
        "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');"]

    case "GAIN_f" then

        gr_i=["gain=C,"
        "dx=sz(1)/5;";
        "dy=sz(2)/10;";
        "xx=orig(1)+      [1 4 1 1]*dx;";
        "yy=orig(2)+sz(2)-[1 5 9 1]*dy;";
        "xpoly(xx,yy,''lines'');";
        "w=sz(1)-2*dx;h=sz(2)-2*dy;";
        "xstringb(orig(1)+dx,orig(2)+dy,gain,w,h,''fill'');"]

    case "LOOKUP_f" then
        gr_i=["rpar=model.rpar;n=size(rpar,''*'')/2;";
        "thick=xget(''thickness'');xset(''thickness'',2);";
        "xx=rpar(1:n);yy=rpar(n+1:2*n);";
        "mnx=min(xx);xx=xx-mnx*ones(xx);mxx=max(xx);";
        "xx=orig(1)+sz(1)*(1/10+(4/5)*xx/mxx);";
        "mnx=min(yy);yy=yy-mnx*ones(yy);mxx=max(yy);";
        "yy=orig(2)+sz(2)*(1/10+(4/5)*yy/mxx);";
        "xpoly(xx,yy,''lines'');";
        "xset(''thickness'',thick);"]

    case "REGISTER_f" then

        gr_i=["dly=model.rpar;";
        "txt=[''Shift'';''Register'';string(dly)];";
        "xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')"]
    case "SWITCH_f" then

        gr_i="xstringb(orig(1),orig(2),[''switch'';string(model.ipar+1)],sz(1),sz(2),''fill'');"
    case "EXPBLK_f" then
        gr_i="xstringb(orig(1),orig(2),''a^u'',sz(1),sz(2),''fill'');"
    case "POWBLK_f" then
        gr_i="xstringb(orig(1),orig(2),''u^a'',sz(1),sz(2),''fill'');"

    end
endfunction


function scs_m_new=do_versionxx(scs_m)
    //A SUPPRIMER CF ci dessus
    if typeof(scs_m)=="diagram" then scs_m_new=scs_m,return,end
    if typeof(scs_m(1))<>"params" then scs_m_new=scs_m,return,end
    scs_m_new=scicos_diagram()
    scs_m_new.props=scs_m(1)
    scs_m_new.objs(1)=mlist("Deleted") // not to change the internal numbering
    n=lstsize(scs_m)
    for i=2:n //loop on objects
        o=scs_m(i)
        scs_m_new.objs(i)=o
        if typeof(o)=="Block" then
            if o.model.sim(1)=="super"| o.model.sim(1)=="csuper" then
                scs_m_new.objs(i).model.rpar=do_versionxx(o.model.rpar)
            end
        end
    end
    edited=resume(%t)
endfunction


function model=scicos_model27(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13)
    //initialisation de model scicos_new
    if exists("sim","local")==0 then sim="",end
    if exists("in","local")==0 then in=[],end
    if exists("out","local")==0 then out=[],end
    if exists("evtin","local")==0 then evtin=[],end
    if exists("evtout","local")==0 then evtout=[],end
    if exists("state","local")==0 then state=[],end
    if exists("dstate","local")==0 then dstate=[],end
    if exists("rpar","local")==0 then rpar=[],end
    if exists("ipar","local")==0 then ipar=[],end
    if exists("blocktype","local")==0 then blocktype="c",end
    if exists("firing","local")==0 then firing=[],end
    if exists("dep_ut","local")==0 then dep_ut=[%f %f],end
    if exists("label","local")==0 then label="",end

    model=scicos_new(["model","sim","in","out","evtin","evtout","state","dstate",..
    "rpar","ipar","blocktype","firing","dep_ut","label"],..
    sim,in,out,evtin,evtout,state,dstate,..
    rpar,ipar,blocktype,firing,dep_ut,label)
endfunction

function graphics=scicos_graphics27(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10)
    //initialisation de graphics scicos_new
    if exists("orig","local")==0 then orig=[0 0],end
    if exists("sz","local")==0 then sz=[20 20],end
    if exists("exprs","local")==0 then exprs=[],end
    if exists("pin","local")==0 then pin=[],end
    if exists("pout","local")==0 then pout=[],end
    if exists("pein","local")==0 then pein=[],end
    if exists("peout","local")==0 then peout=[],end
    if exists("gr_i","local")==0 then gr_i=[],end
    if exists("id","local")==0 then id="",end
    if exists("style","local")==0 then style="",end

    graphics=scicos_new(["graphics","orig","sz","exprs","pin",..
    "pout","pein","peout","gr_i","id","style"],..
    orig,sz,flip,exprs,pin,pout,pein,peout,gr_i,id,style)
endfunction
