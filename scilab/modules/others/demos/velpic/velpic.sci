function [vel,regionlist,linelist,seedlist,velolist]=velpic(nz,nx,sext)
//[vel,regionlist,linelist,seedlist]=velpic(nz,nx)
//Macro which interactively defines regions of a matrix.
//Closed surfaces inside the frame are not allowed.
// nz         :Number of columns in the matrix
// nx         :Number of rows in the matrix
// vel        :velocity matrix
// sext       :[smin,smax] gives the velocities min and max
// regionlist :list whose elements are matrices of dimension
//            :(2xN) containing the indices of the different regions
// linelist   :list whose elements are matrices of dimension (2xM)
//            :containing the indices of the different lines
// seedlist   :(2xK) vector containing positions of seeds
// velolist   :(1xK) vector containing velocities associated to seeds
//
//!
// author: C. Bunks     date: 12-NOV-90
// Copyright INRIA
[lhs,rhs]=argn(0)
//turn off the scilab function 'more'

   nrnc=lines();
   lines(0);
//
   qt='off'
//
   select rhs
   case 2 then
      smin=1500;smax=6000;
      rnd=100
   case 3 then
      smin=sext(1);smax=sext(2)
      rnd=10**int(-1+log(abs(smax-smin))/log(10))

   else error('velpic(nz,nx [,sext])')
   end
//make a suitable frame

   bnames=list('stop','help','undo','grill','quit');
   gopt='off';
   [buttons,slides]=makeframe(nz,nx,bnames);
   slide1=slides(1,:);

//seperate matrix into regions defined by lines

   linelist=makehorizons(nz,nx,bnames,buttons);
   if qt=='on' then return,end
//determine indices of matrix in each region

   write(%io(2),'Searching Regions'),
   regionlist=list();
   indexlist=[ones(1,nz).*.(1:nx);(1:nz).*.ones(1,nx)];

   region=0;
   while size(indexlist)<>[0 0],
      region=region+1;
      seed=indexlist(:,1);
      [inds,indexlist]=id_rgn(indexlist,linelist,seed);
      regionlist(region)=inds;
      write(%io(2),' Region Identified:  '+string(region)),
   end,

//sow a seed in each region

   [seedlist]=sow(nz,nx,slide1,bnames,buttons,regionlist,linelist);
   if qt=='on' then return,end
   [ssl,isl]=sort(seedlist(3,:));//sort seeds and velocities to regions
   [sr,sc]=size(seedlist);
   velolist=seedlist(4,isl(sc:-1:1));
   rolist=seedlist(3,isl(sc:-1:1));
   seedlist=seedlist(1:3,isl(sc:-1:1));

//Load velocity matrix with velocities by region.

   i2=0;
   vel=0*ones(1,nz*nx);
   nor=size(regionlist);
   nov=maxi(size(velolist));
   write(%io(2),'Building velocity matrix'),
   for k=1:nor,
      i1=i2+1;i2=i1-1;
      for j=1:nov,
         if rolist(j)==k then, i2=i2+1; end,
      end,
      rk=regionlist(k);
      rv=(rk(1,:)-ones(rk(1,:)))*nz+rk(2,:);
      if i1==i2 then,//constant velocity region
         vel(rv)=velolist(i1:i2)*ones(rv);
      else,//linearly varying velocity region
         vel(rv)=velcalc(rk,seedlist(:,i1:i2),velolist(i1:i2));
      end,
   end,
   vel=matrix(vel,nz,nx);
   vel=vel(nz:-1:1,:);

//re-establish the line counter

   lines(nrnc(2));

function []=helpme(msn)
//[]=helpme(msn)
//macro containing help messages
// msn :message number
//
//!
// author: C. Bunks     date: 12-NOV-90
// Copyright INRIA
hm1=[' ';
'Begin drawing a line by clicking the left';
'mouse button across a previously drawn line';
'segment or across a frame line segment. In';
'the second case the first mouse click must';
'be in the margin.';
' ';
'STOP:';
'  Stop drawing lines and start picking velocities';
' ';
'HELP:';
'  This help message';
' ';
'UNDO:';
'  Undo a previously drawn line';
' ';
'GRILL:';
'  Toggle grill on/off';
' '];

hm2=[' ';
'Terminate drawing a line by clicking the left';
'mouse button across a previously drawn line';
'segment or across a frame line segment.';
' ';
'STOP:';
'  Completely undo the current line and begin';
'  drawing a new line';
' ';
'HELP:';
'  This help messaage';
' ';
'UNDO:';
'  Undo a line segment';
' ';
'GRILL:';
'  Toggle grill on/off';
' '];

hm3=[' ';
'To sow a seed, choose a velocity by clicking';
'the left mouse button in the velocity slide.';
' ';
'The velocity value chosen on the slide';
'is echoed to the terminal.';
' ';
'To change the velocity value click the left';
'mouse button at another location on the slide.';
' ';
'When a suitable velocity value has been obtained';
'click the left mouse button somewhere in the frame';
'to plant the velocity seed';
' ';
'Multiple seeds in the same region yields linear';
'velocity gradients.  The linear gradients are';
'established between consecutive seed values';
'(following the order of placement)';
' ';
'STOP:';
'  Stop planting seeds and return velocity matrix';
' ';
'HELP:';
'  This help message';
' ';
'UNDO:';
'  Undo a seed placement';
' ';
'GRILL:';
'  Toggle grill on/off';
' '];

hmt=list(hm1,hm2,hm3);
x_message(hmt(msn));

function [seedlist]=sow(nz,nx,slide,bnames,buttons,regionlist,linelist);
//[seedlist]=sow(nz,nx,slide,bnames,buttons,regionlist,linelist);
//Interactively identify the various regions by clicking
//the mouse in each region
// nz         :number of rows of matrix
// nx         :number of columns of matrix
// nol        :number of lines (not including the boundary)
// slide      :slide information
// bnames     :button names
// buttons    :button positions
// regionlist :list of regions
// seedlist   :(4xN) matrix where each column contains (from top to
//            :bottom) the x and y coordinates of the seed, the region
//            :that the seed is in, and the velocity value associated
//            :to the seed
//
//!
// author: C. Bunks     date: 12-NOV-90
// Copyright INRIA
   sl1=slide(1);sl2=slide(2);sl3=slide(3);
   sl4=slide(4);sl5=slide(5);sl6=slide(6);
   seedlist=[];

//slide bar constants  

   vbar=[0 0 0 0 0;0 0 0 0 0];
   if nx<=nz then,
      sw=sl2-sl1;
      b1=sl1+.05*sw;b2=sl2-.05*sw;
      b3=.05*sw;b4=.05*sw;
   else,
      sw=sl4-sl3;
      b1=.05*sw;b2=.05*sw
      b3=sl3+.05*sw;b4=sl4-.05*sw;
   end,

//get velocity and place seed

   nol=size(linelist);
   nor=size(regionlist);

   sflag='on';
   while sflag=='on',
      write(%io(2),'Choose Velocity'),
      veloflag='on';
      vel=-1;
      while veloflag=='on',//loop until desired vel is obtained
          [i_i,v1,v2]=xclick();v=[v1;v2];

//check for a button

   [br,bc]=size(buttons);
   hflag='on';
   while hflag=='on',
      hflag='off';
      for bi=1:br,
         if buttons(bi,1)<v1 then, if v1<buttons(bi,2) then,
         if buttons(bi,3)<v2 then, if v2<buttons(bi,4) then,
            select bnames(bi)
            case 'help' then,
               helpme(3);
               [i_i,v1,v2]=xclick();v=[v1;v2];
               hflag='on';
            case 'stop' then,//stop sowing (all regions >= one seed)
               [sr,sc]=size(seedlist);
               rflag='off';
               for rk=1:nol,
                  srflag='off';
                  for sk=1:sc,
                     if seedlist(3,sk)==rk then, srflag='on'; end,
                  end,
                  if srflag=='off' then, rflag='on'; end,
               end,
               if rflag=='on' then,
                  write(%io(2),'Not every region has a seed'),
                  write(%io(2),'Choose Velocity'),
                  [i,v1,v2]=xclick();v=[v1;v2];
                  hflag='on';
               else,
                  sflag='off';
                  veloflag='off';
               end,
            case 'grill' then,
               if gopt=='on' then, 
                  gopt='off';
               else,
                  gopt='on';
               end,
               makegrill(nx,nz,gopt);
               [i,v1,v2]=xclick();v=[v1;v2];
               hflag='on';
            case 'undo' then,//undo line segment
               [sr,sc]=size(seedlist);
               toff=.05*maxi([nx,nz])/3;
               if sc>0 then,
                  s1=seedlist(1,sc);
                  s2=seedlist(2,sc);
                  text=string(seedlist(4,sc));
                  dess(36)=15;
                  xset("alufunction",6);
                    plot2d(s1',s2',[-3,0],"000");
                    xstring(s1+toff,s2+toff,text,0,0);
                  xset("alufunction",3);
                  seedlist=seedlist(:,1:sc-1);
               else,
                  write(%io(2),'Nothing to undo'),
                  write(%io(2),'Choose Velocity'),
               end,
               [i,v1,v2]=xclick();v=[v1;v2];
               hflag='on';
             case 'quit' then,
               qt=resume('on');
            end,
         end,end,
         end,end,
      end,
   end,

      if sflag=='on' then,//stop hasn't been signalled
//get velocity

         if sl1<=v1 then, if v1<=sl2 then,
         if sl3<=v2 then, if v2<=sl4 then,
//calculate velocity rounded to nearest rnd
//plot bar indicator and give velocity value
            xset("alufunction",6);           
            plot2d(vbar(1,:)',vbar(2,:)',[1],"000");
            xset("alufunction",3);           
            if nx<=nz then,
               vel=rnd*round((sl5+(sl6-sl5)*(v2-sl3)/(sl4-sl3))/rnd);
               vbar=[b1 b2 b2 b1 b1;v2-b3 v2-b3 v2+b4 v2+b4 v2-b3];
               plot2d(vbar(1,:)',vbar(2,:)',[1],"000"),
            else,
               vel=rnd*round((sl5+(sl6-sl5)*(v1-sl1)/(sl2-sl1))/rnd);
               vbar=[v1-b1 v1+b2 v1+b2 v1-b1 v1-b1;b3 b3 b4 b4 b3];
               plot2d(vbar(1,:)',vbar(2,:)',[1],"000"),
            end,
            write(%io(2),vel,'(f10)'),
         end,end,
         end,end,
         if vel<>-1 then,//check that a velocity was chosen
//plant seed
            if 1<=v1 then, if v1<=nx then,
            if 1<=v2 then, if v2<=nz then,

//find the region that v is in

               rflag='on';
               nr=0;
               while rflag=='on',//look for a region that contains seed
                  nr=nr+1;
                  rk=regionlist(nr);
                  rflag='off';
                  for nl=1:nol,//for this region check all lines
                     [testflag,bav]=testpt(v,rk(:,1),linelist(nl));
                     if testflag=='on' then, rflag='on'; end,
                  end,
                  if rflag=='off' then,//this region if no intersections
                     sregion=nr;   
                  end,
               end,

//plot seed and add to seedlist
               mrgn=.05*maxi([nx,nz])/3;
               plot2d(v1',v2',[-3,0],"000");
               xstring(v1+mrgn,v2+mrgn,string(vel),0,0);
               seedlist=[seedlist,[v;sregion;vel]];
               veloflag='off';
            end,end,
            end,end,
         end,
      end,
      end,
   end,


function [linelist]=makehorizons(nz,nx,bnames,buttons)
//[linelist]=makehorizons(nz,nx,bnames,buttons)
//macro which creates a frame with control buttons and
//interactively draws lines (i.e., horizons)
// nz       :Number of rows in frame
// nx       :Number of columns in frame
// bnames   :Button names
// buttons  :Button locations
// linelist :list whose elements are (2xN) matrices of line indices
//
//!
// author: C. Bunks     date: 12-NOV-90
// Copyright INRIA

//define outer perimeter as a line

   prt=.001;
   linelist=list([1-prt 1-prt  nx+prt nx+prt 1-prt;...
                  1-prt nz+prt nz+prt 1-prt  1-prt]);
   nol=size(linelist);

//start line drawing

   layer='true';
   yec=[];
   while layer=='true',
      nol=nol+1;

//Define layer by drawing a line

      write(%io(2),'Draw a Line'),
      [line,linelist,yec]=drawline(nz,nx,linelist,yec,bnames,buttons);
      if size(line)==[0,0] then,//if returned line is empty then stop
         layer='false'; 
      else if line==0 then,
         nol=nol-1;
      else,
         nol=size(linelist);
         nol=nol+1;
         linelist(nol)=line;
      end,
      end,
   end,


function [buttons,slides]=makeframe(nz,nx,btextlist);
//[buttons,slides]=makeframe(nz,nx,btextlist);
//macro which makes a frame and buttons
// nz        :Number of rows in frame
// nx        :Number of columns in frame
// btextlist :text for buttons
// buttons   :button information
// slides    :slide information
//
//!
// author: C. Bunks     date: 12-NOV-90
// Copyright INRIA

//setup of frame

//draw work box.
//note that the work box is placed in [1,nx] x [1,nz]
//and that the remaining elements are for centering properly
//the box (i.e., mrgn and the 1).

   maxx=maxi([nz,nx]);
   mrgn=.05*(maxx-1);
   xmin=1-mrgn-maxx+nx;xmax=maxx+4*mrgn;
   ymin=1-mrgn-maxx+nz;ymax=maxx+4*mrgn;
   if nx>nz+4*mrgn then,
      xmin=1-mrgn-maxx+nx;xmax=maxx+mrgn;
      ymin=1-mrgn-maxx+nz;ymax=maxx+mrgn;
   end,
   if nx<nz-4*mrgn
      xmin=1-mrgn-maxx+nx;xmax=maxx+4*mrgn;
      ymin=1-mrgn-maxx+nz;ymax=maxx+mrgn;
   end,
   rect=[xmin,ymin,xmax,ymax];
   plot2d(0,0,[1],"012",' ',rect);
   plot2d([1;1;nx;nx;1],[nz;1;1;nz;nz],[1],"000"),
//make buttons and slides
   dess4=10
   dess6=10
   dx=(xmax-xmin)/dess4
   dy=(ymax-ymin)/dess6

//make button bank

   bs=size(btextlist);//number of buttons
   bsi=1/(2*bs);
   buttons=[];
   if nz>=nx then,//in the right side margin
      bc=int(bs/(4+bsi))+1;//number of columns
      br=int(bs/(bc+bsi))+1;//number of rows
      bm=2*mrgn/(bc+1);
      bentry=0;
      for bj=1:bc,for bi=1:br,
         xbmin=nx+2*(bj-1)*bm+bm;xbmax=xbmin+2*bm;
         ybmax=nz-2*(bi-1)*bm;ybmin=ybmax-2*bm;
         bentry=bentry+1;
         if bentry<=bs then,
            text=btextlist(bentry);
         else,
            text='    ';
         end,
         makebutton(xbmin,xbmax,ybmin,ybmax,dx,dy,text);
         buttons=[buttons;xbmin,xbmax,ybmin,ybmax];
      end,end,
   else,//in the top margin
      br=int(bs/(4+bsi))+1;//number of rows
      bc=int(bs/(br+bsi))+1;//number of columns
      bm=2*mrgn/(br+1);
      bentry=0;
      for bi=1:br,for bj=1:bc,
         xbmin=1+2*(bj-1)*bm;xbmax=xbmin+2*bm;
         ybmin=nz+2*(bi-1)*bm+bm;ybmax=ybmin+2*bm;
         bentry=bentry+1;
         if bentry<=bs then,
            text=btextlist(bentry);
         else,
            text='    ';
         end,
         makebutton(xbmin,xbmax,ybmin,ybmax,dx,dy,text);
         buttons=[buttons;xbmin,xbmax,ybmin,ybmax];
      end,end,
   end,

//velocity slide

   if nz>=nx then,
      xbmin=nx+mrgn;xbmax=xbmin+2*mrgn;
      ybmin=1;ybmax=(1+nz)/2;
   else,
      xbmin=(1+nx)/2;xbmax=nx;
      ybmin=nz+mrgn;ybmax=ybmin+2*mrgn;
   end,
   text='VEL';
   dx=(xmax-xmin)/dess4;
   dy=(ymax-ymin)/dess6;
   makeslide(xbmin,xbmax,ybmin,ybmax,dx,dy,text,smin,smax);
   slides=[xbmin,xbmax,ybmin,ybmax,smin,smax];


function []=makebutton(xbmin,xbmax,ybmin,ybmax,dx,dy,text)
//[]=makebutton(xbmin,xbmax,ybmin,ybmax,dx,dy,text)
//make to make a button
// xbmin :min x coordinate of button
// xbmax :max x coordinate of button
// ybmin :min y coordinate of button
// ybmax :max y coordinate of button
// dx    :ratio of plot length to frame length: (xmax-xmin)/dess(4)
// dy    :ratio of plot height to frame height: (ymin-ymax)/dess(6)
// text  :text in button
//
//!
// author: C. Bunks     date: 12-NOV-90
// Copyright INRIA

//make button box
   xstringb(xbmin,ybmin,text,xbmax-xbmin,ybmax-ybmin);
   xrect(xbmin,ybmax,xbmax-xbmin,ybmax-ybmin);


function []=makeslide(xbmin,xbmax,ybmin,ybmax,dx,dy,text,smin,smax)
//[]=makeslide(xbmin,xbmax,ybmin,ybmax,dx,dy,text)
//make to make a button
// xbmin :min x coordinate of button
// xbmax :max x coordinate of button
// ybmin :min y coordinate of button
// ybmax :max y coordinate of button
// dx    :ratio of plot length to frame length: (xmax-xmin)/dess(4)
// dy    :ratio of plot height to frame height: (ymin-ymax)/dess(6)
// text  :text in button
// smin  :min value of slide
// smax  :max value of slide
//
//!
// author: C. Bunks     date: 12-NOV-90
// Copyright INRIA

//NOTE: The constant wcf is a 'wierd correction factor necessary
//for the correct positioning of the text in the button.  I don't
//understand why it is needed...

   wcf=1.3;

//make slide box

   xvec=[xbmin xbmin xbmax xbmax xbmin];//x button frame vector
   yvec=[ybmax ybmin ybmin ybmax ybmax];//y button frame vector
   delx=xbmax-xbmin;dely=ybmax-ybmin;

//label slide

   dess(27)=1;//soft caracters
   sl=length(text);
   if delx<dely then,
      cw=(xbmax-xbmin)/(dx*sl);//char.width=.9*(box width)/(text length)
   else,
      cw=(ybmax-ybmin)/(dx*sl);
   end,
   ch=cw/.82;
//   setcsize(cw,ch);
   textx=(xbmin+xbmax-dx*sl*cw/wcf)/2;//text x position
   texty=(ybmin+ybmax-dy*ch/wcf)/2;//text y position
   dess(51)=2;//turn labelling on
   dess(50)=textx;dess(52)=texty;//positions of text
   plot2d(xvec',yvec',[1],"000");
   xstring(textx,texty,text,0,0),

//label min value of slide

   text=string(smin);
   sl=length(text);
   if delx<dely then,
      cw=(xbmax-xbmin)/(dx*sl);
      ch=cw/.82;
      textx=(xbmin+xbmax-dx*sl*cw/wcf)/2;//text x position
      texty=ybmin+dy*ch/wcf/2;//text y position
   else,
      cw=(ybmax-ybmin)/(dx*sl);
      ch=cw/.82;
      textx=xbmin+dx*cw/wcf;//text x position
      texty=(ybmin+ybmax-dy*cw/wcf)/2;//text y position
   end,
//   setcsize(cw,ch);
   dess(51)=2;//turn labelling on
   dess(50)=textx;dess(52)=texty;//positions of text
   xstring(textx,texty,text,0,0),

//label max value of slide

   text=string(smax);
   sl=length(text);
   if delx<dely then,
      cw=(xbmax-xbmin)/(dx*sl);
      ch=cw/.82;
      textx=(xbmin+xbmax-dx*sl*cw/wcf)/2;//text x position
      texty=ybmax-3*dy*ch/wcf/2;//text y position
   else,
      cw=(ybmax-ybmin)/(dx*sl);
      ch=cw/.82;
      textx=xbmax-dx*(1+sl)*cw/wcf;//text x position
      texty=(ybmin+ybmax-dy*cw/wcf)/2;//text y position
   end,
//   setcsize(cw,ch);
   dess(51)=2;//turn labelling on
   dess(50)=textx;dess(52)=texty;//positions of text
   xstring(textx,texty,text,0,0),


function [line,linelist,yec]=drawline(nz,nx,linelist,yec,bnames,buttons)
//[line,linelist,yec]=drawline(nz,nx,linelist,yec,bnames,buttons)
//interactively draw a line
// nz       :number of columns of matrix
// nx       :number of rows of matrix
// linelist :list containing the matrices of indices for 
//          :each line
// yec      :extension list
// bnames   :Names of buttons
// buttons  :Button locations
// line     :new line
//
//!
// author: C. Bunks     date: 12-NOV-90
// Copyright INRIA

//to begin new line the first two clicks of the mouse must
//intersect an old line or the frame

   line=[];
   [i,x1,x2]=xclick();
   testflag='off';
   flag='on';

//check for a button

   [br,bc]=size(buttons);
   hflag='on';
   while hflag=='on',//while the mouse has been clicked in a button
      hflag='off';
      for bi=1:br,//check all the buttons
         if buttons(bi,1)<x1 then, if x1<buttons(bi,2) then,
         if buttons(bi,3)<x2 then, if x2<buttons(bi,4) then,
            select bnames(bi)
            case 'help' then,
               helpme(1);
               [i_i,x1,x2]=xclick();
               hflag='on';
            case 'stop' then,
               testflag='on';
               flag='off';
               yecl=0;
            case 'grill' then,
               if gopt=='on' then, 
                  gopt='off';
               else,
                  gopt='on';
               end,
               makegrill(nx,nz,gopt);
               [i_i,x1,x2]=xclick();
               hflag='on';
            case 'undo' then,//undo last line
               lls=size(linelist);
               if lls>1 then,//remove a line
                  [yer,yecc]=size(yec);
                  yf=yec(1,yecc);
                  yl=yec(2,yecc);
                  lk=linelist(lls);
                  [lkr,lkc]=size(lk);
                  xset("alufunction",6);
                  plot2d(lk(1,yf+1:lkc-yl)',lk(2,yf+1:lkc-yl)',[1],"000"),
                  xset("alufunction",3);
                  ltemp=list();
                  for k=1:lls-1,
                     ltemp(k)=linelist(k);
                  end,
                  linelist=ltemp;
                  yec=yec(:,1:yecc-1);
               else,
                  write(%io(2),'Nothing to undo'),
               end,
               [i_i,x1,x2]=xclick();
               hflag='on';
            case 'quit' then,
               qt=resume('on')
            end,
         end,end,
         end,end,
      end,
   end,

//start drawing line

   yext=[];
   while testflag=='off',   
      plot2d(x1',x2',[-3,-1],"000");//make a start circle
      [i_i,y1,y2]=xclick();

//check for a button

   [br,bc]=size(buttons);
   hflag='on';
   while hflag=='on'
      hflag='off';
      for bi=1:br,
         if buttons(bi,1)<y1 then, if y1<buttons(bi,2) then,
         if buttons(bi,3)<y2 then, if y2<buttons(bi,4) then,
            select bnames(bi)
            case 'help' then,
               helpme(1);
               [i_i,y1,y2]=xclick();
               hflag='on';
            case 'stop' then,
               write(%io(2),'Nothing to stop'),
               [i_i,y1,y2]=xclick();
               hflag='on';
            case 'grill' then,
               if gopt=='on' then, 
                  gopt='off';
               else,
                  gopt='on';
               end,
               makegrill(nx,nz,gopt);
               [i_i,y1,y2]=xclick();
               hflag='on';
            case 'undo' then,
               write(%io(2),'Nothing to undo'),
               [i_i,y1,y2]=xclick();
               hflag='on';
            case 'quit' then,
               qt=resume('on');
            end,
         end,end,
         end,end,
      end,
   end,

//if line segment defined by first two points intersects a
//previously defined line and y is in the frame then start a new line

      inflag='off';
      if 1<=y1 then, if y1<=nx then,//y in the frame
      if 1<=y2 then, if y2<=nz then,
         inflag='on';
         xsec=[];//list of intersections
         nol=size(linelist);
         for ln=1:nol,//test against all the lines for intersections
            [testflag,bav]=testpt([x1;x2],[y1;y2],linelist(ln));
            [br,bc]=size(bav);
            xsec=[xsec,[bav;ln*ones(1,bc)]];
         end,

//choose longest intersection which yields shortest segment
//and add on the necessary extensions

         [xr,xc]=size(xsec);
         if xc>0 then,
            testflag='on';
            btemp=xsec(1:2,:)-[x1;x2]*ones(1,xc);
            [val,in]=maxi([1 1]*(btemp.*btemp));//longest intersection
             xset("alufunction",6);
             plot2d(x1',x2',[-3,-1],"000");//undo start circle
             xset("alufunction",3);
            x=xsec(1:2,in);//new x
            x1=x(1);x2=x(2);
            ln=xsec(4,in);li=xsec(3,in);
            shortline=linelist(ln);//extension line
            [sr,sc]=size(shortline);
            if sc-li>li then,
               yext=shortline(:,1:li);
               [yer,yecf]=size(yext);//yec is useful for undoing
            else,
               yext=shortline(:,sc:-1:li+1);
               [yer,yecf]=size(yext);
            end,
         end,
      end,end,
      end,end,
      if inflag=='off' then,//case where y is not in the frame
             xset("alufunction",6);
             plot2d(x1',x2',[-3,-1],"000");//undo start circle
             xset("alufunction",3);
         x1=y1;x2=y2;
             plot2d(x1',x2',[-3,-1],"000");//undo start circle
      end,
      if testflag=='off' then,//case where x-y does not cross a line
             xset("alufunction",6);
             plot2d(x1',x2',[-3,-1],"000");//undo start circle
             xset("alufunction",3);
         x1=y1;x2=y2;
             plot2d(x1',x2',[-3,-1],"000");//undo start circle
      end,
   end,

   if flag=='on' then,
      plot2d([x1;y1],[x2;y2],[1,-1],"000"),
      line=[yext,[x1;x2],[y1;y2]];
      x1=y1;x2=y2;
   end,

//continue line

   while flag=='on',
      flag='off';
      [i_i,y1,y2]=xclick();

//check for a button
   
   [br,bc]=size(buttons);
   hflag='on';
   while hflag=='on'
      hflag='off';
      for bi=1:br,
         if buttons(bi,1)<y1 then, if y1<buttons(bi,2) then,
         if buttons(bi,3)<y2 then, if y2<buttons(bi,4) then,
            select bnames(bi)
            case 'help' then,//get help
               helpme(2);
               [i_i,y1,y2]=xclick();
               hflag='on';
            case 'stop' then,//totally undo current line
               [lr,lc]=size(line);
               xset("alufunction",6);
               plot2d(line(1,yecf+1:lc)',line(2,yecf+1:lc)',[1,-1],"000"),
               xset("alufunction",3);
               flag='off';
               yecl=0;
               line=0;
            case 'grill' then,//toggle grill
               if gopt=='on' then, 
                  gopt='off';
               else,
                  gopt='on';
               end,
               makegrill(nx,nz,gopt);
               [i_i,y1,y2]=xclick();
               hflag='on';
            case 'undo' then,//undo line segment
               [lr,lc]=size(line);
               xset("alufunction",6);
               plot2d(line(1,lc-1:lc)',line(2,lc-1:lc)',[1,-1],"000"),
               xset("alufunction",3);
               flag='off';
               if lc>yecf+2 then,
                  hflag='on';
                  line=line(:,1:lc-1);
                  x=line(:,lc-1);
                  x1=x(1);x2=x(2);
                  [i_i,y1,y2]=xclick();
                  flag='on';
               end,
               if flag=='off' then, yecl=0; line=0; end,
             case 'quit' then
               qt=resume('on')
            end,
         end,end,
         end,end,
      end,
   end,

      if line<>0 then,//line drawing has not been stopped

//test if line intersects itself

         [lr,lc]=size(line);
         if lc-1>yecf+1 then,
            [testflag,bav]=testpt([x1;x2],[y1;y2],line(:,yecf+1:lc-1));
         else,
            testflag='off';
         end,
         if testflag=='on' then,
            write(%io(2),' '),
            write(%io(2),'*********ERROR*********')
            write(%io(2),' Lines are not allowed'),
            write(%io(2),'to intersect themselves'),
            write(%io(2),' '),
            write(%io(2),'  Choose another point'),
            flag='on';
         else,

//test if point intersects a previously drawn line

         yext=[];
         flag='on';
         xsec=[];
         nol=size(linelist);
         for ln=1:nol,
            [testflag,bav]=testpt([x1;x2],[y1;y2],linelist(ln));
            [br,bc]=size(bav);
            xsec=[xsec,[bav;ln*ones(1,bc)]];
         end,

//check all the intersections and choose the shortest

         [br,bc]=size(xsec);
         if bc>0 then,
            flag='off';
            btemp=xsec(1:2,:)-[x1;x2]*ones(1,bc);
            [val,in]=mini([1 1]*(btemp.*btemp));
            y1=xsec(1,in);y2=xsec(2,in);

//find the extension of the intersected line by the 
//shortest segment of the intersecting line

            ln=xsec(4,in);li=xsec(3,in);
            shortline=linelist(ln);
            [sr,sc]=size(shortline);
            if sc-li>li then,
               yext=shortline(:,li:-1:1);
               [yer,yecl]=size(yext);
            else,
               yext=shortline(:,li+1:sc);
               [yer,yecl]=size(yext);
            end,
         end,

//plot line segment

         plot2d([x1;y1],[x2;y2],[1,-1],"000"),
         line=[line,[y1;y2],yext];
         x1=y1;x2=y2;
      end,
      end,
   end,
   if yecl<>0 then,
      yec=[yec,[yecf;yecl]];
   end,
   write(%io(2),'Done With Current Line'),
   

function [flag,bav]=testpt(p1,p2,line)
//[flag,bav]=testpt(p1,p2,line)
//macro which tests whether the line segment defined by the
//two points p1 and p2 intersects any of the line segments
//in line.
// p1   :2x1 matrix giving the indices of point number one
// p2   :2x1 matrix giving the indices of point number two
// line :2xN matrix giving the indices of a line
// flag :flag='on' indicates an intersection, flag='off' 
//      :indicates no intersection
// bav  :3xM matrix giving all the intersections found (if any)
//      :and the position in the line of the intersection
//
//!
// author: C. Bunks     date: 12-NOV-90
// Copyright INRIA

//set up arguments of fortran subprogram m45.f

   noi=0;
   nc=maxi(size(line));
   bav=0*ones(3,nc);
   flag=0;
   [flag,bav,noi]=fort('testpt',...
                                   p1,1,'r',...
                                   p2,2,'r',...
                                   nc,3,'i',...
                                   line,4,'r',...
                                   flag,5,'i',...
                                   noi,6,'i',...
                                   bav,7,'r',...
                                   'sort',...
                                   [1,1],5,'i',...
                                   [3,nc],7,'r',...
                                   [1,1],6,'i');

   bav=bav(:,1:noi);
   if flag==1 then, flag='on'; else, flag='off'; end,


function []=redraw(linelist,seedlist,velolist)
//[]=redraw(linelist[,seedlist[,velolist]])
//Macro which redraws the lines drawn by velpic
//and gives the locations and velocities chosen
// linelist :list containing the lines to be drawn
// seedlist :list containing the seed positions
// velolist :list containing the velocity values
//
//!
// author: C. Bunks     date: 12-NOV-90
// Copyright INRIA

   [lhs,rhs]=argn(0);

//determine the number of lines

   nol=size(linelist);
   l1=linelist(1);
   nr=maxi(l1(1,:));
   nc=maxi(l1(2,:));

//draw frame
    plot2d(0,0,[1],"012",' ',[1,1,nr,nc]);

//draw the lines (the first line from velpic is the 
//exterior line)

   for k=1:nol,
      lk=linelist(k);
      plot2d(lk(1,:)',lk(2,:)',[1],"000"),
   end,
   if rhs==2 then,
      plot2d(seedlist(1,:)',seedlist(2,:)',[-3,0],"000"),
   end,
   if rhs==3 then,
      [vr,vc]=size(velolist);
      toff=.05*maxi([nr,nc])/3;
      for k=1:vc,
         text=string(velolist(k));
         s1=seedlist(1,k);s2=seedlist(2,k);
         dess(50)=s1+toff;dess(52)=s2+toff;//positions of text
         plot2d(s1',s2',[-3,0],"000");
         xstring(s1+toff,s2+toff,text,0,0);
      end,
   end,


function [ind,indexlist]=id_rgn(indexlist,linelist,seed);
//[ind,indexlist]=id_region(indexlist,linelist,seed);
//Macro which determines all the indices of the matrix of dimension
//(nz X nx) which are in the region defined by the seed and the 
//linelist.  The elements which are in the same region as the seed
//are those which are on the same side of all the lines in the
//linelist.
// indexlist :(2xN) vector containing all the indices to be searched
// linelist  :list of lines
// seed      :pair of indices of the matrix identifying the region
// ind       :all indices of the matrix associated to the region
//           :defined by seed
//
//!
// author: C. Bunks     date: 12-NOV-90
// Copyright INRIA

   nlist=0*indexlist;
   ic=maxi(size(indexlist));
   nol=size(linelist);
   llist=[];noe=[1];
   for k=1:nol,
      noe=[noe,noe(k)+maxi(size(linelist(k)))];
      llist=[llist,linelist(k)];
   end,
   nolt=maxi(size(llist));
   noi=0;non=0;bav=0*ones(3,nolt);
   lln=0*ones(2,nolt);
   ind=0*indexlist;
   [ind,nlist,noi,non]=fort('idrgn',...
                              indexlist,1,'i',...
                              llist,2,'r',...
                              seed,3,'r',...
                              ind,4,'i',...
                              nol,5,'i',...
                              nolt,6,'i',...
                              ic,7,'i',...
                              noi,8,'i',...
                              noe,9,'i',...
                              nlist,10,'i',...
                              bav,11,'r',...
                              lln,12,'r',...
                              non,13,'i',...
                              'sort',...
                              [2,ic],4,'i',...
                              [2,ic],10,'i',...
                              [1,1],8,'i',...
                              [1,1],13,'i');
   ind=ind(:,1:noi);
   indexlist=nlist(:,1:non);



function []=makegrill(nx,nz,gopt)
//Plot a grill in the region [1 nx] x [1 nz]
// nx   :Number of x positions
// nz   :Number of z positions
// gopt :Plotting flag where 'on' means plot 
//      :and 'off means unplot
//
//!
// author: C. Bunks     date: 12-NOV-90
// Copyright INRIA
// Change JPC 2 mars 1992

   if gopt <>'on' then,xset("alufunction",6);end
   for k=2:nx-1, plot2d([k;k],[1;nz],[2],"000"), end,
   for k=2:nz-1, plot2d([1;nx],[k;k],[2],"000"), end
   xset("alufunction",3);

function [vi]=velcalc(indexlist,seedlist,velolist)
//Create velocity field for a region defined by indexlist
//where velocity varies linearly between control points
//defined by the seedlist sl and its associated velocities
//in vl.
// indexlist :list of indices of region
// seedlist  :indices of velocity seed locations
// velolist  :velocity control points
// vi        :velocities for region
//
//!
// author: C. Bunks     date: 12-NOV-90
// Copyright INRIA

   [vr,vc]=size(velolist);
   [ir,ic]=size(indexlist);
   vi=velolist(1)*ones(1,ic);
   for k=1:vc-1,
      s1=seedlist(1:2,k);s2=seedlist(1:2,k+1);
      v1=velolist(k);v2=velolist(k+1);
      x=indexlist(1,:);y=indexlist(2,:);
      x1=s1(1);y1=s1(2);
      x2=s2(1);y2=s2(2);
      dx=x2-x1;dy=y2-y1;dv=v2-v1;
      r2=dx*dx+dy*dy;
      xv=x-x1*ones(x);
      yv=y-y1*ones(y);
      gr=(xv*dx+yv*dy)/r2;
      vi=vi+mini(maxi(gr,0*gr),ones(gr))*dv;
   end,


