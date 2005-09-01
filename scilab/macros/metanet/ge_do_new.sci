function [ok,GraphList]=ge_do_new()
//Copyright INRIA
//Author : Serge Steer 2002
   if EGdata.Edited then
     if x_message(['Current graph is modified'
		'Do you really want to erase it'],['yes','no'])==2 then
       ok=%f
       graphlist=[]
       return
     end
   end
   ok=%t
   GraphList=editgraph_diagram()
endfunction

