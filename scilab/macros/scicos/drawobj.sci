function drawobj(o)
// Copyright INRIA
if o(1)=='Block' then
   ierr=execstr(o(5)+'(''plot'',o)','errcatch')
   if ierr<>0 then 
    message(['Block '+o(5)+ ' not defined';
	'You must leave scicos and define it now.']),
   end
  elseif o(1)=='Link' then
  ct=o(7);c=ct(1),pos=o(6)
  if pos(2)>=0 then
    d=xget('dashes')
    thick=xget('thickness')
    t=maxi(pos(1),1)*maxi(pos(2),1)
    xset('thickness',t)
    xset('dashes',c)
    xpoly(o(2),o(3),'lines')
    xset('dashes',d)
    xset('thickness',thick)
  end
elseif o(1)=='Text' then
  execstr(o(5)+'(''plot'',o)')
end




