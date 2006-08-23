      subroutine xawelm
c     =============================================================
c     Primitives for scilab menus 
c     =============================================================
c     Copyright ENPC 
      include '../stack.h'
      external setmen, unsmen
      integer irep
      
      call withgui(irep)
      if (irep .eq. 0) then
        buf='interface disabled in -nogui mode.'
        call error(999)
        return
      endif
      
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' xawelm '//buf(1:4))
      endif
c     
      if(fin.eq.1) then
         call scidialog("x_dialog")
      endif
      if(fin.eq.2) then
         call scimess("x_message")
      endif
      if(fin.eq.3) then
         call scichoose("x_choose")
      endif
      if(fin.eq.4) then
         call scimdial("x_mdialog")
      endif
      if(fin.eq.7) then
         call scichoice("choices")
      endif
      if (fin .eq. 8) then
         call sciaddm("addmenu_old")
      endif
      if (fin .eq. 9) then
         call scidellm("delmenu_old")
      endif
      if (fin .eq. 10 ) then 
         call scisetum("setmenu_old",setmen)
      endif
      if (fin.eq.11) then 
         call scisetum("unsetmenu_old",unsmen)
      endif
      if (fin.eq.12) then
         call intsxgetfile("xgetfile")
      endif
      if (fin.eq.13) then
         call scimess1("x_message_modeless")
      endif
 9999 return
      end

 


 
 

