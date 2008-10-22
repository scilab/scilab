
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ge_do_information(GraphList)
// Shows the information about the graph

  r=x_choices('Information can be displayed or sent to file',..
	      list(list('',1,['Display','Send to text file','Send to TeX file'])))
  if r==1 then
    x_message_modeless(ge_make_text_info());
  elseif r==2 then
    path=tk_savefile()
    if path<>'' then
      mputl(ge_make_text_info(),path)
    end
  elseif r==3 then
    path=tk_savefile('*.tex')
    if path<>'' then
      mputl(ge_make_tex_info(),path)
    end
  end
endfunction

function txt=ge_make_text_info()
  txt=['Information about graph : '+GraphList.name
       ' '
       ' ']
  if GraphList.directed==0 then
    txt=[txt;'Graph is not directed'];
  else
    txt=[txt;'Graph is  directed'];
  end
  txt=[txt;''];
  nn=size(GraphList.nodes);
  txt=[txt;'Number of nodes : '+string(nn)];
  na=size(GraphList.edges);
  txt=[txt;'Number of arcs  : '+string(na)];
  
  //Information on nodes
  txt=[txt;'';
       'Information on nodes'
       '--------------------';''];
  
  w='';
  
  c=['Numbers';
     '-------'
     string(1:nn)'];
  c=part(c,1:max(length(c)+1));
  w=w+'|'+c;
  
  c=['Names';
     '-----'
     GraphList.nodes.graphics.name(:)];
  c=part(c,1:max(length(c)+1));
  w=w+'|'+c;
  
  c=['Types';
     '-----'
     string(GraphList.nodes.graphics.type(:))];
  c=part(c,1:max(length(c)+1));
  w=w+'|'+c;
  
  F=getfield(1,GraphList.nodes.data);F=F(2:$);
  for f=F
    d=GraphList.nodes.data(f)';
    t=string(d)
    lt=max(length(t),'r')
    tt=emptystr(size(d,1),1)
    for k=1:size(d,2)-1,tt=tt+part(t(:,k),1:lt(k)+2),end
    tt=tt+part(t(:,$),1:lt($))

    c=[f;
       strcat('-'+emptystr(1,length(f)))
       tt]

    c=part(c,1:max(length(c)+1));
    w=w+'|'+c;
  end
  
  w=w+'|';
  txt=[txt;w];
  
  //Information on arcs
  txt=[txt;'';
       'Information on arcs'
       '-------------------';''];
  
  
  w='';
  c=['Numbers';
     '-------';
     string(1:na)'];
  
  c=part(c,1:max(length(c)+1));
  w=w+c;
  
  c=['Names';
     '-----';
     GraphList.edges.graphics.name(:)];
  
  c=part(c,1:max(length(c)+1));
  w=w+'|'+c;
  
  
  F=getfield(1,GraphList.edges.data);F=F(2:$);
  for f=F
    d=GraphList.edges.data(f)';
    t=string(d)
    lt=max(length(t),'r')
    tt=emptystr(size(d,1),1)
    for k=1:size(d,2)-1,tt=tt+part(t(:,k),1:lt(k)+2),end
    tt=tt+part(t(:,$),1:lt($))

    c=[f;
       strcat('-'+emptystr(1,length(f)))
       tt]

    c=part(c,1:max(length(c)+1));
    w=w+'|'+c;
  end
  w=w+'|';
  txt=[txt;w];

endfunction

///////////////////
function txt=ge_make_tex_info()
  txt=['\documentclass{article}'
       '\begin{document}'
       '\begin{center}'
       '{\large \bf Information about graph : '+GraphList.name+'}'
       '\end{center}'
       '\bigskip\bigskip'
       ' ']
  if GraphList.directed==0 then
    txt=[txt;'Graph is not directed\par'];
  else
    txt=[txt;'Graph is  directed\par'];
  end
  txt=[txt;''];
  nn=size(GraphList.nodes);
  na=size(GraphList.edges);
  txt=[txt;'{\bf Number of nodes:}  '+string(nn)+', {\bf Number of arcs:} '+string(na)];
  
  //Information on nodes
  txt=[txt;'';
       '\section{Information on nodes}';'']
  
  w='';
  
  c=['Numbers'
     string(1:nn)'];
  c=part(c,1:max(length(c)+1));
  w=w+c;
  
  c=['Names'
     GraphList.nodes.graphics.name(:)];
  c=part(c,1:max(length(c)+1));
  w=w+'&'+c;
  
  c=['Types';
     string(GraphList.nodes.graphics.type(:))];
  c=part(c,1:max(length(c)+1));
  w=w+'&'+c;

  F=getfield(1,GraphList.nodes.data);F=F(2:$);
  for f=F
    d=GraphList.nodes.data(f)';
    tt=string(d(:,1))
    nc=size(d,2)
    if nc==1 then
      c=[f;
	 tt]

    else
      for k=2:nc,tt=tt+'&'+string(d(:,k)),end
      c=['\multicolumn{'+string(nc)+'}{l}{'+f+'}';
	 tt]

    end

    w=w+'&'+c;
  end
  
  w=w+'\\ \hline'
  dels=strcat('l'+emptystr(1,3+size(F,'*')),'|')
  txt=[txt;
       '\begin{tabular}{|'+strcat('l'+emptystr(1,3+size(F,'*')),'|')+'|}\hline';
       w;
       '\end{tabular}'];
  
  //Information on arcs
  txt=[txt;'';
       '\section{Information on arcs}';'']
  
  w='';
  c=['Numbers';
     string(1:na)'];
  
  c=part(c,1:max(length(c)+1));
  w=w+c;
  
  c=['Names';
     GraphList.edges.graphics.name(:)];
  
  c=part(c,1:max(length(c)+1));
  w=w+'&'+c;
  
  F=getfield(1,GraphList.edges.data);F=F(2:$);
  for f=F
    d=GraphList.edges.data(f)';
    tt=string(d(:,1))
    nc=size(d,2)
    if nc==1 then
      c=[f;
	 tt]

    else
      for k=2:nc,tt=tt+'&'+string(d(:,k)),end
      c=['\multicolumn{'+string(nc)+'}{l}{'+f+'}';
	 tt]

    end

    w=w+'&'+c;
  end
  
  w=w+'\\ \hline'
  txt=[txt;
       '\begin{tabular}{|'+strcat('l'+emptystr(1,3+size(F,'*')),'|')+'|}\hline';
       w;
       '\end{tabular}'
       '\end{document}' ];
endfunction



