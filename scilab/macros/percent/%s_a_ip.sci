function r=%s_a_ip(s,ip)
// s+ip
if size(s,'*')<>1 then error(10),end
r=(s+ip(1)):(s+ip(2)):(s+ip(3))
endfunction
