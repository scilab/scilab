
function Cmenu=check_edge(o,Cmenu,%pt)
  if windows(find(%win==windows(:,2)),1)==100000 then
    return // navigator
  end
  %xc=%pt(1);%yc=%pt(2)
  if typeof(o)=='Block' then
    orig=o.graphics.orig;sz=o.graphics.sz;
    orig=orig(:);sz=sz(:);
    eps=sz/5
    orig=orig+eps;sz=sz-2*eps;
    data=[(orig(1)-%xc)*(orig(1)+sz(1)-%xc),..
	  (orig(2)-%yc)*(orig(2)+sz(2)-%yc)]
    if data(1)<0&data(2)<0 then
      // we have cliked inside the block so it is probably not a link
    else
      //find output port location projected on the block
      [%xout,%yout,typout]=getoutputports(o)
      if %xout<>[] then
	%xxyymax=o.graphics.orig(:)+o.graphics.sz(:)
	%xout=max(min(%xout,%xxyymax(1)),o.graphics.orig(1))
	%yout=max(min(%yout,%xxyymax(2)),o.graphics.orig(2))
	// how close to the port is considered a link (/5)
	%center=orig+sz/2;
	if or((%xc-%xout).^2+(%yc-%yout).^2 <(%xc-%center(1)).^2+...
	      (%yc-%center(2)).^2) then
	  //we have clicked near a port
	  Cmenu='Link'
	end
      end
    end
  end
  
endfunction
