function r=mtlb_eval(%s1,%s2)
[lhs,rhs]=argn()
if exists('m2scilib')==0 then load('SCI/macros/m2sci/lib'),end
%prot=funcprot();funcprot(0)
//
deff('r=isanmfile(s)','r=%f');
res_path=[];logfile=-1;batch=%t;fnam='%fun';dquote='''';lcount=1;
level=[0,0];sciexp=0;
sciparam();[vnms,vtps]=m2sci_init()

//
deff('%fun()',%s1)
%txt=ins2sci(mmodlst(macr2lst(%fun)),4,vnms,vtps)
%txt(find(part(%txt,1:2)=='//'))=[]
%txt(1)=[];%txt($-1:$)=[];disp(%txt)

//

%r=0
%nold=size(who('get'),'*')
%r=execstr(%txt,'errcatch')

if %r<>0&rhs==2 then
  deff('%fun()',%s2)
  %txt=ins2sci(mmodlst(macr2lst(%fun)),4,vnms,vtps)
  %txt(find(part(%txt,1:2)=='//'))=[]
  %txt(1)=[];%txt($-1:$)=[];disp(%txt)
  %nold=size(who('get'),'*')
  %r=execstr(%txt,'errcatch')
else
  %nold=%nold+1
end
nams=who('get');
if size(nams,'*')<=%nold then
  r=0
  return
end
nams=nams(1:$-%nold)
%r=funcprot(%prot);

if nams=='ans' then
  r=ans
else
  r=[];
  execstr(lhsargs(nams)+'=resume'+rhsargs(nams))
end

function [vnms,vtps]=m2sci_init()

vnms=[],vtps=list()
vnms=[vnms;['%i','i']],
vtps($+1)=list('1','1','1',0)
vnms=[vnms;['%i','j']],
vtps($+1)=list('1','1','1',0)
vnms=[vnms;['%nan','NaN']],
vtps($+1)=list('1','1','1',0)
vnms=[vnms;['%nan','nan']],
vtps($+1)=list('1','1','1',0)
vnms=[vnms;['%inf','Inf']],
vtps($+1)=list('1','1','1',0)
vnms=[vnms;['%inf','inf']],
vtps($+1)=list('1','1','1',0)
vnms=[vnms;['%pi','pi']],
vtps($+1)=list('1','1','1',0)
vnms=[vnms;['%eps','eps']],

