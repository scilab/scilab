function s=date()
w=getdate()
month=['Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec']
s=string(w(6))+'-'+month(w(2))+'-'+string(w(1))
endfunction
