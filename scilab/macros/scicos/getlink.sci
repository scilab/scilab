function [scs_m,needcompile] = getlink(%pt,scs_m,needcompile)
// Copyright INRIA
// edition of a link from an output block to an input  block

//** BEWARE: "d9" state is not yet tested after Replot removal
//** 24/07/07: Al@n's patch for rotation of blocks

  outin=['out','in']
  //----------- get link origin --------------------------------------
  //------------------------------------------------------------------
  win = %win;
  xc1 = %pt(1);
  yc1 = %pt(2);

  [kfrom,wh] = getblocklink(scs_m,[xc1;yc1])

  if kfrom<>[] then
    o1 = scs_m.objs(kfrom)
  else
    return
  end

  //kfrom  is the number of selected block or link
  scs_m_save = scs_m;
  nc_save    = needcompile

  //** new graphics
  gh_curwin = gh_current_window ; //** acquire the current window handler

  if typeof(o1)=='Link' then  // add a split block
    pt=[xc1;yc1]
    [xx,yy,ct,from,to]=(o1.xx,o1.yy,o1.ct,o1.from,o1.to);
    if (-wh==size(xx,'*')) then
      wh=-(wh+1)
      // avoid with clicking at the end-point of link
    end

    // get split type
    [xout,yout,typout]=getoutputports(scs_m.objs(from(1)))
    clr=ct(1)
    [m,kp1]=mini((yc1-yout)^2+(xc1-xout)^2)
    k=kp1
    typo=ct(2)

    if typo==-1 then //old link comes from an event output port
      typp='evtout'
    else //old link comes from an regular output or input (implicit) port
      typp=outin(from(3)+1)
    end

    //get port size
    szout=getportsiz(scs_m.objs(from(1)),from(2),typp)
    //get port data type
    if typp=='out'|typp=='in' then
     szouttyp=getporttyp(scs_m.objs(from(1)),from(2),typp)
    end

    // get initial split position
    wh=wh(1)
    if wh>0 then
      d=projaff(xx(wh:wh+1),yy(wh:wh+1),pt)
    else // a corner
      wh=-wh
      d=[xx(wh);yy(wh)]
    end
    // Note : creation of the split block and modifications of links are
    //        done later, the sequel assumes that the split block is added
    //        at the end of scs_m
    ks=kfrom; //selected link number
    kfrom=size(scs_m.objs)+1;port_number=2; //to be created split block number

    fromsplit=%t
    //to be created link from origin

    from=[kfrom,port_number,0] 
    xo=d(1);yo=d(2)
    xl=d(1);yl=d(2)

  else //connection comes from a block

    graphics1=o1.graphics
    orig  = graphics1.orig
    sz    = graphics1.sz
    theta = graphics1.theta
    io    = graphics1.flip
    op    = graphics1.pout
    impi  = graphics1.pin
    cop   = graphics1.peout
    [xout,yout,typout]=getoutputports(o1)

    i_ImplIndx=find(graphics1.in_implicit=='I')

    if xout==[] then
        hilite_obj(kfrom)
        message('This block has no output port')
        unhilite_obj(kfrom)
      return
    end

    xxx=rotate([xout;yout],...
               theta*%pi/180,...
               [orig(1)+sz(1)/2;orig(2)+sz(2)/2]);
    xout=xxx(1,:);
    yout=xxx(2,:);

    [m,kp1]=mini((yc1-yout)^2+(xc1-xout)^2)
    k=kp1
    xo=xout(k);yo=yout(k);typo=typout(k)

    // Check if selected port is already connected and get port type ('in' or 'out')

    if typo==1  then //regular output port
      port_number=k
      if op(port_number)<>0 then
          hilite_obj(kfrom)
          message(['Selected port is already connected.';..
                   'To start a link off another link, place the cursor';..
                   'on the split point and double click, or type l.'])
          unhilite_obj(kfrom)
        return
      end
      typpfrom='out'
    elseif (typo==2 & k<=size(op,'*')) then //implicit  output port
      port_number=k
      if op(port_number)<>0 then
          hilite_obj(kfrom)
          message(['Selected port is already connected.';..
                   'To start a link off another link, place the cursor';..
                   'on the split point and double click, or type l.'])
          unhilite_obj(kfrom)
        return
      end
      typpfrom='out'
    elseif (typo==2 & k>size(op,'*')+size(cop,'*')) then //implicit  input port
      typpfrom='in' 
      k=k-size(op,'*')-size(cop,'*')
//      port_number=k,//out port
      port_number=i_ImplIndx(k)
      if impi(port_number)<>0 then
          hilite_obj(kfrom)
          message(['Selected port is already connected.';..
                   'To start a link off another link, place the cursor';..
                   'on the split point and double click, or type l.'])
          unhilite_obj(kfrom)
        return
      end
      typpfrom='in'
    else //event output port
      port_number=k-  size(op,'*')        //prod(size(find(typout==1)))
      if cop(port_number)<>0 then
          hilite_obj(kfrom)
          message(['Selected port is already connected.';..
                   'To start a link off another link, place the cursor';..
                   'on the split point and double click, or type l.'])
          unhilite_obj(kfrom)
        return
      end
      typpfrom='evtout'
    end
    fromsplit=%f
    clr=default_color(typo)

    //get port size
    szout=getportsiz(o1,port_number,typpfrom)
    //get port data type
    if typpfrom=='out'|typpfrom=='in' then
      szouttyp=getporttyp(o1,port_number,typpfrom)
    end

    //to be created link from origin 
    from=[kfrom,port_number,bool2s(typpfrom=='in'|typpfrom=='evtin')]
    xl=xo
    yl=yo

  end

  //----------- get link path ----------------------------------------
  //------------------------------------------------------------------

  //** new graphics
  drawlater()           ;         // draw later mode
  o_size = size(gh_curwin.children.children) ; //** o_size(1) is the number of compound object
  p_size = o_size ;

  while %t do //loop on link segments
    xe = xo; ye = yo ; //** o > origin ---- e > end
    //** the first step is the the creation of a dummy graphic object (a link of ZERO leght)
    //** and store this handler to modify it later 
    xpoly([xo;xe] , [yo;ye], 'lines')        ; //** draw the first 'dummy' object
    gh_link = gh_curwin.children.children(1) ; //** the last object is the link

    rep(3)=-1; //** initialization
    drawlater() ;
    while 1 do //** infinite loop
      //** for positive event exit from the loop

      //** Any event on the [right] button end the inner loop
      if or(rep(3)==[0,2,3,5,-5,-100]) then break,end

      //** otherwise ... get a new point

      //** This mode is prone to some errors
      //** mouse event queque is not cleared
      //** rep = xgetmouse(0,[%t,%t])

      //** mouse event queque is cleared (15 Mar 2007 bugfix)
      rep = xgetmouse([%t,%t]) ; //** looks better :)

      gh_figure = gcf();
      if gh_figure.figure_id<>curwin | rep(3)==-100 then
        //active window has been closed
        [%win, Cmenu] = resume(curwin,'Quit')
      end

      //** any rigth mouse event OR [Esc] OR [d] key : I want to disengage the current Link action
      if or(rep(3)==[2 5 12 65307 100]) then
          p_size = size(gh_curwin.children.children)
          d_size = p_size(1)-o_size(1);
          if d_size > 0 then
            gh_compound_delete = glue(gh_curwin.children.children(1:d_size) );
            delete (gh_compound_delete); //** delete the object
            draw(gh_curwin.children);
            show_pixmap(); //** display the buffer
          end

          if %scicos_debug_gr then
            disp("d1"); //** Debug
          end
          return; //** -----> Exit from the function
      end //**

      //plot new position of last link segment
      xe = rep(1); ye = rep(2) ;
      gh_link.data =  [xo yo ; xe ye ] ;   //** put the coordinate here
      gh_link.foreground = clr             //** put the color here ( 5 = red )
      draw(gh_curwin.children);
      show_pixmap(); //** display the buffer

    end
    //** ----------------- end of last segment while loop ------------------------------

    if %scicos_debug_gr then
      disp("-->"); //** debug only
    end

    //** ---------- YOU ARE STILL IN THE "Link" Block-to-Block MAIN LOOP ----------------
    //** The last segment end with [xe ye] coordinate:
    //**
    //** look for a block with a valid (good) input

    kto = getblock(scs_m,[xe;ye]) ;
    if kto<>[] then //new point designs the "to block"
      o2 = scs_m.objs(kto);
      graphics2 = o2.graphics;
      orig  = graphics2.orig
      sz    = graphics2.sz
      theta = graphics2.theta
      ip    = graphics2.pin
      impo  = graphics2.pout
      cip   = graphics2.pein
      [xin,yin,typin] = getinputports(o2)

      o_ImplIndx=find(graphics2.out_implicit=='I')

      //check connection
      if xin==[] then
         hilite_obj(kto)
         message('This block has no input port.');
         p_size = size(gh_curwin.children.children);
         d_size = p_size(1) - o_size(1);
         if d_size > 0 then
            gh_compound_delete = glue(gh_curwin.children.children(1:d_size));
             delete (gh_compound_delete); //** delete the object
         end
         if %scicos_debug_gr then
           disp("d2");   //** Debug
         end
         unhilite_obj(kto)
         return;      //** EXIT point from the function
      end

      xxx=rotate([xin;yin],...
                 theta*%pi/180,...
                 [orig(1)+sz(1)/2;orig(2)+sz(2)/2]);
      xin=xxx(1,:);
      yin=xxx(2,:);

      [m,kp2] = mini((ye-yin)^2+(xe-xin)^2)

      k = kp2

      xc2 = xin(k); yc2 = yin(k); typi = typin(k);

      if typo<>typi
        hilite_obj(kto)
        message(['Selected ports don''t have the same type'
                 'The port at the origin of the link has type '+string(typo);
                 'the port at the end has type '+string(typin(k))+'.'])
          p_size = size(gh_curwin.children.children)
          d_size = p_size(1)-o_size(1);
          if d_size > 0 then
             gh_compound_delete = glue(gh_curwin.children.children(1:d_size));
             delete (gh_compound_delete); //** delete the object
          end
          if %scicos_debug_gr then
             disp("d3"); //** Debug
          end
          unhilite_obj(kto)
          return; //** EXIT point from the function
      end

      //**------------------------------------------------------------------------
      //** check if is a normal regular input port (not an event input port)
      if typi==1  then // regular input port
        port_number = k ;
        if ip(port_number)<>0 then
             hilite_obj(kto)
             message(['Selected port is already connected.';..
                      'To start a link off another link, place the cursor';..
                      'on the split point and double click, or type l.']),
             p_size = size(gh_curwin.children.children)
             d_size = p_size(1)-o_size(1);
             if d_size > 0 then
               gh_compound_delete = glue(gh_curwin.children.children(1:d_size) );
               delete (gh_compound_delete); //** delete the object
             end
             if %scicos_debug_gr then
               disp("d4");//** Debug
             end
             unhilite_obj(kto)
           return
        end

        typpto='in'

        szin = getportsiz(o2,port_number,'in')
        need_warning = %f;
        if (szin(1)<>szout(1)) & mini([szin(1) szout(1)])>0 then
          need_warning=%t
        end

        //check for different number of dimension
        szout2 = []; szin2 = [];
        if size(szout,'*')==1 then
           szout2=1;
        else
           szout2=szout(2);
        end

        if size(szin,'*')==1 then
           szin2=1;
        else
           szin2=szin(2);
        end

        if (szin2<>szout2) & mini([szin2 szout2])>0 then
          need_warning=%t
        end

        if need_warning then
            hilite_obj(kto)
            message(['Warning :';
                     'Selected ports don''t have the same size';
                     'The port at the origin of the link has size '+sci2exp(szout);
                     'the port at the end has size '+sci2exp(szin)+'.'])
            unhilite_obj(kto)
        end

        //get port data type
        //and say warning if doesn't match with szouttyp
        szintyp=getporttyp(o2,port_number,'in')
        if (szintyp>0 & szouttyp>0) then //if-then-else, event-select blocks case and all the -1 intyp/outtyp
          if szintyp<>szouttyp then
            tt_typ=['double';'complex';'int32';'int16';
                    'int8';'uint32';'uint16';'uint8']

            hilite_obj(kto)
            message(['Warning :';
                     'Selected ports don''t have the same data type';
                     'The port at the origin of the link has datatype '+...
                      tt_typ(szouttyp)+' ('+sci2exp(szouttyp)+')';
                     'the port at the end has datatype '+...
                      tt_typ(szintyp)+' ('+sci2exp(szintyp)+')'+'.'])
            unhilite_obj(kto)
          end
        end

      //TODO
      elseif typi==2 & k<=size(ip,'*') then //implicit "input" port
        port_number = k
        if ip(port_number)<>0 then
           message(['Selected port is already connected.';..
                    'To start a link off another link, place the cursor';..
                    'on the split point and double click, or type l.']),
           p_size = size(gh_curwin.children.children)
           d_size = p_size(1)-o_size(1);
           if d_size > 0 then
               gh_compound_delete = glue(gh_curwin.children.children(1:d_size) );
               delete (gh_compound_delete); //** delete the object
           end
           if %scicos_debug_gr then
             disp("d5");//** Debug
           end
           return ; //** Exit point
        end

        typpto='in'
        szin=getportsiz(o2,port_number,'in')

        if szin<>szout & mini([szin szout])>0 then
          message(['Warning :';
                   'Selected ports don''t have the same size';
                   'The port at the origin of the link has size '+string(szout);
                   'the port at the end has size '+string(szin)])
        end

      //TODO
      elseif (typi==2 & k>size(ip,'*')+size(cip,'*')) then //implicit "output" port

        k=k-size(ip,'*')-size(cip,'*')
        typpto='out'
        //port_number=k
        port_number=o_ImplIndx(k)  //RN: explicit outputs are excluded
                                   //in the computation of k
        if impo(port_number)<>0 then
           message(['Selected port is already connected.';..
                    'To start a link off another link, place the cursor';..
                    'on the split point and double click, or type l.']),
           p_size = size(gh_curwin.children.children)
           d_size = p_size(1)-o_size(1);
           if d_size > 0 then
             gh_compound_delete = glue(gh_curwin.children.children(1:d_size) );
             delete (gh_compound_delete); //** delete the object
           end
           if %scicos_debug_gr then
             disp("d6");//** Debug
           end
           return; //** Exit point
        end
        typpto='out'
        szin=getportsiz(o2,port_number,'out')

        if szin<>szout & mini([szin szout])>0 then
          message(['Warning :';
                   'Selected ports don''t have the same  size';
                   'The port at the origin of the link has size '+string(szout);
                   'the port at the end has size '+string(szin)+'.'])
        end

      //** otherwise is an event input port
      else //event input port

        port_number=k-size(ip,'*')  //prod(size(find(typin==1)))

        if cip(port_number)<>0 then
            hilite_obj(kto)
            message(['Selected port is already connected.';..
                     'To start a link off another link, place the cursor';..
                     'on the split point and double click, or type l.']),
            p_size = size(gh_curwin.children.children)
            d_size = p_size(1)-o_size(1);
            if d_size > 0 then
              gh_compound_delete = glue(gh_curwin.children.children(1:d_size) );
              delete (gh_compound_delete); //** delete the object
            end
            if %scicos_debug_gr then
              disp("d7");//** Debug
            end
            unhilite_obj(kto)
            return; //** Exit point
        end

        typpto='evtin'
        szin=getportsiz(o2,port_number,'evtin')
        if szin<>szout & mini([szin szout])>0 then
          //-Alan-ONPASSEPARLA??
          message(['Warning :';
                   'Selected ports don''t have the same  size'
                   'The port at the origin of the link has size '+string(szout);
                   'the port at the end has size '+string(szin)+'.'])
        end

      end
      //**********************************
      //fin

      if %scicos_debug_gr then
        disp("|->>")
      end

        //** ------------- VERY DANGEROUS CODE HERE ------------------------------------------
        //** There is the serious possibility of residual "xor-mode" code 

      drawlater ;

      //** Alan's fix : 17/10/07 : remove that pieces of code.
      //** Directly treated below in the case nx=1
//       if kto==kfrom&size(xl,'*')==1 then
//         //direct link between two port of the same block (add a point)
//         xl=[xl;(xl+xc2)/2]
//         yl=[yl;(yl+yc2)/2]
//       end

      break;

    else //(kto==[]) new point ends current line segment

      if xe<>xo|ye<>yo then //to avoid null length segments
        xc2=xe;yc2=ye

        // xpoly([xo;xc2],[yo;yc2],'lines')

        if abs(xo-xc2)<abs(yo-yc2) then
          xc2=xo
        else
          yc2=yo
        end

        // xpoly([xo;xc2],[yo;yc2],'lines')
        gh_link.data =  [xo yo ; xc2 yc2 ] ; //** temp
        gh_link.foreground = clr           ; //** put the color here
        draw(gh_link.parent) ;
        show_pixmap();           //** display the buffer
        if %scicos_debug_gr then
          disp("d8");//** Debug
        end

        xl=[xl;xc2]
        yl=[yl;yc2]
        xo=xc2
        yo=yc2
      end

    end

  end //loop on link segments

  gh_figure = gcf();
  if gh_figure.figure_id<>curwin | rep(3)==-100 then
      //active window has been closed
      [%win,Cmenu]=resume(curwin,'Quit')
  end

  //make last segment horizontal or vertical
  typ=typo
  to=[kto,port_number,bool2s(typpto=='in'|typpto=='evtin')]
  nx=prod(size(xl))

  if from==to then
      message(['Selected port is already connected.';..
               'To start a link off another link, place the cursor';..
               'on the split point and double click, or type l.']),
      p_size = size(gh_curwin.children.children)
      d_size = p_size(1)-o_size(1);
      if d_size > 0 then
        gh_compound_delete = glue(gh_curwin.children.children(1:d_size) );
        delete (gh_compound_delete); //** delete the object
        draw(gh_curwin.children) ;
        show_pixmap();  //** display the buffer
      end
      if %scicos_debug_gr then
        disp("d9");//** Debug
      end
      //** ... to be removed
      //** Replot_(); //** force a replot (obsolete)
      return //** exit point
  end

  //**-----------------------------------------------------------------------------------
  //** the link is avalid link

  if nx==1 then //1 segment link

    if fromsplit&(xl<>xc2|yl<>yc2) then
      //try to move split point

      if xx(wh)==xx(wh+1) then //split is on a vertical link

        if (yy(wh)-yc2)*(yy(wh+1)-yc2)<0 then
          //erase last segment
          //**xpoly([xl;xc2],[yl;yc2],'lines')
          yl = yc2 ;
          //draw last segment
          //**xpoly([xl;xc2],[yl;yc2],'lines')
          gh_link.data =  [xl yl ; xc2 yc2] ; //** put the coordinate here
          draw(gh_link.parent);
          show_pixmap(); //** display the buffer
        end

      elseif yy(wh)==yy(wh+1) then //split is on a horizontal link

        if (xx(wh)-xc2)*(xx(wh+1)-xc2)<0 then
          //erase last segment
          //xpoly([xl;xc2],[yl;yc2],'lines')
          xl = xc2,
          //draw last segment
          // xpoly([xl;xc2],[yl;yc2],'lines')
          gh_link.data = [xl yl ; xc2 yc2 ] ; //** put the coordinate here
          draw(gh_link.parent);
           show_pixmap(); //** display the buffer
        end

      end
      d=[xl,yl]
    //** Alan's fix, 17/10/07
    //** create a point in the middle of the link
    //** in the case of a direct link between two port
    //** of the same block
    elseif kto==kfrom then
      xl=[xl;(xl+xc2)/2]
      yl=[yl;(yl+yc2)/2]
    end
    //form link datas
    xl=[xl;xc2];yl=[yl;yc2]
  else
    if xl(nx)==xl(nx-1) then
      //previous segment is vertical

      //erase last and previous segments
      // xpoly([xl(nx-1);xl(nx);xo;xc2],[yl(nx-1);yl(nx);yo;yc2],'lines')
      //draw last 2 segments
      // xpoly([xl(nx-1);xl(nx);xc2],[yl(nx-1);yc2;yc2],'lines')
      //drawlater ;
        nx = prod(size(xl)) ;
        gh_link_del = gh_curwin.children.children(1) ;
        delete( gh_link_del );
        gh_link_del = gh_curwin.children.children(1) ;
        delete( gh_link_del );
        xpoly([xl(nx-1) ; xl(nx) ; xc2] , [yl(nx-1) ; yc2 ; yc2] ,'lines');
        gh_link = gh_curwin.children.children(1) ;
        gh_link.foreground = clr
        draw(gh_link.parent) ;
        show_pixmap(); //** display the buffer
      //form link datas
      xl=[xl;xc2];yl=[yl(1:nx-1);yc2;yc2]

    //** ---- Previous segment is horizontal
    elseif yl(nx)==yl(nx-1) then
      //previous segment is horizontal
      //erase last and previous segments
      // xpoly([xl(nx-1);xl(nx);xo;xc2],[yl(nx-1);yl(nx);yo;yc2],'lines')
      //draw last 2 sgements
      //xpoly([xl(nx-1);xc2;xc2],[yl(nx-1);yl(nx);yc2],'lines')
      //drawlater ;
        nx = prod(size(xl)) ;
        gh_link_del = gh_curwin.children.children(1) ;
        delete( gh_link_del );
        gh_link_del = gh_curwin.children.children(1) ;
        delete( gh_link_del );
        xpoly([xl(nx-1);xc2;xc2],[yl(nx-1);yl(nx);yc2],'lines')
        gh_link = gh_curwin.children.children(1) ;
        gh_link.foreground = clr
        draw(gh_link.parent);
        show_pixmap(); //** display the buffer

      //form link datas
      xl=[xl(1:nx-1);xc2;xc2];yl=[yl;yc2]
    else //previous segment is oblique
      //nothing particular is done
      xl=[xl;xc2];yl=[yl;yc2]
    end
  end
  lk=scicos_link(xx=xl,yy=yl,ct=[clr,typ],from=from,to=to)
  gh_blk = drawobj(lk); //** <-- draw the link :)

  //** ----> Put the graphical handling update here :)

  //**---- Mr. Clean :) -----------------------------------------------------------------------
  p_size = size(gh_curwin.children.children) ; //** p_size(1) is the number of compound object
  d_size = p_size(1) - o_size(1) ;             //** at the and of this "Link" operation
  drawlater() ;
  if d_size > 0 then
    gh_compound_delete = glue(gh_curwin.children.children(1:d_size) );
    delete (gh_compound_delete); //** delete the object
  end
  draw(gh_curwin.children);
  show_pixmap(); //** display the buffer
  //**------------------------------------------------------------------------------------------

  //----------- update objects structure -----------------------------
  //------------------------------------------------------------------

  if fromsplit then //link comes from a split
    nx=size(scs_m.objs)+1
    //split old link
    from1=o1.from
    to1=o1.to
    link1=o1;
    link1.xx   = [xx(1:wh);d(1)];
    link1.yy   = [yy(1:wh);d(2)];
    link1.to   = [nx,1,1]

    link2=o1;
    link2.xx   = [d(1);xx(wh+1:size(xx,1))];
    link2.yy   = [d(2);yy(wh+1:size(yy,1))];
    link2.from = [nx,1,0];


    // create split block
    if typo==1 then
      sp=SPLIT_f('define')
      sp.graphics.orig = d;
      sp.graphics.pin  = ks;
      sp.graphics.pout = [nx+1;nx+2];

      SPLIT_f('plot',sp)
    elseif typo==2 then
      sp=IMPSPLIT_f('define')
      sp.graphics.orig = d;
      sp.graphics.pin  = ks;
      sp.graphics.pout = [nx+1;nx+2];
      inoutfrom='out' 
      IMPSPLIT_f('plot',sp)
    else
      sp=CLKSPLIT_f('define')
      sp.graphics.orig  = d;
      sp.graphics.pein  = ks;
      sp.graphics.peout = [nx+1;nx+2];
      CLKSPLIT_f('plot',sp)
    end

    glue(gh_curwin.children.children(1) ); //** create the compound
    //** be very careful: here the graphics datastructure has ONE more element than the
    //** "scs_m.objs" ;)
    //---------------------------
      scs_m.objs(ks)   = link1 ; //** adjust the data of the first half of the old "splitted" link

      //gh_ks = o_size(1) + 1 - ks + 1 ; //** I need to compensate for the last entry
      gh_ks = get_gri(ks,o_size(1)) + 1

      gh_curwin.children.children(gh_ks).children.data = [ link1.xx , link1.yy]  ; //** update the graphics datastructure
      link1_color = gh_curwin.children.children(gh_ks).children.foreground       ; //** save the color

      //---------------------------

      scs_m.objs(nx)  = sp    ; //** the graphics datastucture is already up to date in "nx" position

      //---------------------------

      scs_m.objs(nx+1) = link2 ;

      xpoly (link2.xx , link2.yy) ;
      gh_curwin.children.children(1).foreground = link1_color ;
      glue(gh_curwin.children.children(1) ); //** create the compound :)

    //---------------------------

    //----------------------------//
    // scs_m.objs(ks)=link1;      //
    // scs_m.objs(nx)=sp          //
    // scs_m.objs(nx+1)=link2;    //
    //----------------------------//
    //change link connected to the to block (before it was ks now it is
    //nx+1 (link2)
    //disp('scs_m.objs(to1(1))');pause
    scs_m.objs(to1(1))=mark_prt(scs_m.objs(to1(1)),to1(2),outin(to1(3)+1),typ,nx+1)

  end

//**----------------------------------------------------------------------------------

  //add new link in objects structure
  nx = size(scs_m.objs)+1 ;
  scs_m.objs($+1) = lk ;

  // drawlater ;
  drawobj(lk) ;
  draw(gh_curwin.children);
  show_pixmap(); //** display the buffer
  //update connected blocks
  scs_m.objs(kfrom)=mark_prt(scs_m.objs(kfrom),from(2),outin(from(3)+1),typ,nx)
  scs_m.objs(kto)=mark_prt(scs_m.objs(kto),to(2),outin(to(3)+1),typ,nx)

  needcompile=4

  [scs_m_save,nc_save,enable_undo,edited]=resume(scs_m_save,nc_save,%t,%t)

endfunction
