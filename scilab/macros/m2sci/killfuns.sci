function fptr=killfuns()
// Copyright INRIA
funs=['list','tlist','exp','cond','xgrid','type','format',..
        'qr','lu','maxi','mini','max','min','size','degree']

fptr=[]
kfuns=[]
for f=funs
  fp=funptr(f)
  if fp<>0 then
    fptr=[fptr,funptr(f)]
    clearfun(f)
    kfuns=[kfuns,f]
  end
end
fptr=list(kfuns,fptr)


function restorefuns(fptr)
[funs,fptr]=fptr(1:2)
for k=1:size(funs,'*')
  newfun(funs(k),fptr(k))
end
