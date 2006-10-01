function [pk]=snapshot(pt,view,frames,pmode)
//[pk]=snapshot(pt[,view][,frames][,pmode])
//Plot sequence of matrices from a cube (representing snapshots) 
//where the structure of the cube is represented by a matrix 
//one big matrix of dimension mN x N, i.e.:
//
//                      | p1 |
//                      | p2 |
//                  pt= |  . |  and  pk is an NxN matrix
//                      |  . |
//                      | pm |
//
//
//                   ___________
//                  /|         /|                   view='top'
//         p1 -----/ |        / |                
//                /  |       /--------------------- first matrix 
//               /_________ /   |                            |
//               |   |     |    |-- first matrix             |
//               |   |_____|____|             /              |
//               |  /      |   /             /               V
//         pm ---|-/       |  /-------------/------ last matrix
//               |/        |-- last matrix /
//               |_________|/  
//                            view='front'
//
//
// pt     :matrix of dimension mN x N
// view   :character chain taking values 'top' or 'front'.
//        :values of view are used as described in above figure.
//        :default value of view is 'top'.
// frames :frames is either of the form frames=fi:fstep:ff 
//        :or frames=fstep (in which case frames=1:fstep:m) and 
//        :indicates which subsequence of the collection of 
//        :matrices is to be displayed. Default is frames=1:1:m.
// pmode  :pmode='3d' for 3d plots and pmode='c' for contour plots
//        :default is '3d'
// pk     :last frame plotted by snapshot
//
//!
//author: C. Bunks     date: 1-NOV-90

// Copyright INRIA
[pr,pc]=size(pt);
   kmax=pr/pc;
   if ent(kmax)<>kmax then,
      write(%io(2),'Give matrix column length:')
      pc=read(%io(1),1,1);
      kmax=pr/pc;
   end,
   write(%io(2),'Total Number of Frames: '+string(kmax)),

//default evaluation

   [lhs,rhs]=argn(0);
   if rhs==1 then, view='top'; frames=1:kmax; pmode='3d'; end,
   if rhs==2 then, 
      if type(view)==10 then,
         if view=='top' then,
            frames=1:kmax; pmode='3d';
         else,
            if view=='front' then,
               frames=1:kmax; pmode='3d';
            else,
               pmode=view; view='top'; frames1:kmax;
            end,
         end,
      else, 
         frames=view; view='top'; pmode='3d';
         if maxi(size(frames))==1 then, 
            if frames>0 then,
               frames=1:frames:kmax; 
            else,
               frames=kmax:frames:1;
            end,
         end,
      end,
   end,
   if rhs==3 then,
      if type(view)==10 then,
         if view=='top' then,
            if type(frames)==10 then
               pmode=frames; frames=1:kmax;
            else,
               pmode='3d';
            end,
         else,
            if view=='front' then,
               if type(frames)==10 then
                  pmode=frames; frames=1:kmax;
               else,
                  pmode='3d';
               end,
            else,
               pmode='3d';
            end,
         end,
      else, 
         pmode=frames; frames=view; view='top';
         if maxi(size(frames))==1 then, 
            if frames>0 then,
               frames=1:frames:kmax; 
            else,
               frames=kmax:frames:1;
            end,
         end,
      end,
   end,
   if maxi(size(frames))==1 then, 
      if frames>0 then,
         frames=1:frames:kmax; 
      else,
         frames=kmax:frames:1;
      end,
   end,

//plot 

   for k=frames,
      if view=='top' then,
         pk=pt((k-1)*pc+1:k*pc,:);
      else,
         pk=pt(k:pc:pr,:);
      end,
      if pmode=='3d' then,
         plotdb(pk,'agc',[0,%pi/4],'x'),pause,xbasc();
      else,
         contour(1:pc,1:pc,pk,10),pause,xbasc();
      end,
    end;
