function r=%i_m_s(a,b)

if b==[] then r=[],end
select inttype(a)
case 1 then
  r=a*int8(b)
case 2 then
  r=a*int16(b)
case 4 then
  r=a*int32(b)
end

