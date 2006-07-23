function ge_do_information(GraphList)
//Copyright INRIA
//Author : Serge Steer 2002
// Shows the information about the graph

  r=x_choices('Information can be displayed or sent to file',..
	      list(list('',1,['Display','Send to text file','Send to TeX file'])))
  if r==1 then
    x_message_modeless(ge_make_text_info());
  elseif r==2 then
    path=xgetfile()
    if path<>'' then
      mputl(ge_make_text_info(),path)
    end
  elseif r==3 then
    path=xgetfile('*.tex')
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
  nn=size(GraphList.node_x,'*');
  txt=[txt;'Number of nodes : '+string(nn)];
  na=size(GraphList.tail,'*');
  txt=[txt;'Number of arcs  : '+string(na)];
  
  //Information on nodes
  txt=[txt;'';
       'Information on nodes'
       '--------------------';''];
  
  w='';
  edge_length=%f;edge_cost=%f;edge_min_cap=%f;edge_max_cap=%f;
  edge_q_weight=%f;  edge_q_orig=%f;edge_weigh=%f;edge_label=%f;
  
  c=['Numbers';
     '-------'
     string(1:nn)'];
  c=part(c,1:max(length(c)+1));
  w=w+'|'+c;
  
  c=['Names';
     '-----'
     GraphList.node_name(:)];
  c=part(c,1:max(length(c)+1));
  w=w+'|'+c;
  
  c=['Types';
     '-----'
     string(GraphList.node_type(:))];
  c=part(c,1:max(length(c)+1));
  w=w+'|'+c;
  
  c=['Demands';
     '-------'
     string(GraphList.node_demand(:))];
  c=part(c,1:max(length(c)+1));
  w=w+'|'+c;
  
  c=['Labels'
     '------'
     ;string(GraphList.node_label(:))];
  c=part(c,1:max(length(c)+1));
  w=w+'|'+c;
  
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
     GraphList.edge_name(:)];
  
  c=part(c,1:max(length(c)+1));
  w=w+'|'+c;
  
  if or(GraphList.edge_length<>0) then
    c=['Length';
       '------'
       string(GraphList.edge_length(:))];
    
    c=part(c,1:max(length(c)+1));
    w=w+'|'+c;
    edge_length=%t;
  end
  
  if or(GraphList.edge_cost<>0) then
    c=['Costs';
       '-----'
       string(GraphList.edge_cost(:))];
    c=part(c,1:max(length(c)+1));
    w=w+'|'+c;
    edge_cost=%t;
  end
  
  if or(GraphList.edge_min_cap<>0) then
    c=['Min Capacities';
       '--------------'
       string(GraphList.edge_min_cap(:))];
    c=part(c,1:max(length(c)+1));
    w=w+'|'+c;
    edge_min_cap=%t;
  end
  
  if or(GraphList.edge_max_cap<>0) then
    c=['Max Capacities';
       '--------------'
       string(GraphList.edge_max_cap(:))];
    c=part(c,1:max(length(c)+1));
    w=w+'|'+c;
    edge_max_cap=%t;
  end
  
  if or(GraphList.edge_q_weight<>0) then
    c=['Quadratic weights';
       '-----------------'
       string(GraphList.edge_q_weight(:))];
    c=part(c,1:max(length(c)+1));
    w=w+'|'+c;
    edge_q_weight=%t;
  end
  
  if or(GraphList.edge_q_orig<>0) then
    c=['Quadratic origins';
       '-----------------'
       string(GraphList.edge_q_orig(:))];
    c=part(c,1:max(length(c)+1));
    w=w+'|'+c;
    edge_q_orig=%t;
  end

  if or(GraphList.edge_weight<>0) then
    c=['Weights';
       '-------'
       string(GraphList.edge_weight(:))];
    c=part(c,1:max(length(c)+1));
    w=w+'|'+c;
    edge_weigh=%t;
  end


  if or(GraphList.edge_label<>'') then
    c=['Labels';
       '------'
       GraphList.edge_label(:)];
    c=part(c,1:max(length(c)+1));
    w=w+'|'+c;
    edge_label=%t;
  end
  w=w+'|'
  
  t=[edge_length;edge_cost;edge_min_cap;edge_max_cap;
     edge_q_weight;edge_q_orig;edge_weigh;edge_label];
  if ~and(t) then
    T=['Lengths';'Costs';'Min Capacities';'Max Capacities';
       'Quadratic weights';'Quadratic origins';'Weights';'Labels'];
    T=T(~t)
    if size(T,'*')==1 then
      txt=[txt;'';strcat(T,', ')+' is set to default values (0 or """")';''];
    else
      if size(T,'*')>4 then
	txt=[txt;'';
	     strcat(T(1:4),', ')+',';
	     strcat(T(5:$),', ')+' are set to default values (0 or """")';''];
      else
	txt=[txt;'';strcat(T,', ')+' are set to default values (0 or """")';''];
      end
    end
  end 
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
  nn=size(GraphList.node_x,'*');
  na=size(GraphList.tail,'*');
  txt=[txt;'{\bf Number of nodes:}  '+string(nn)+', {\bf Number of arcs:} '+string(na)];
  
  //Information on nodes
  txt=[txt;'';
       '\section{Information on nodes}';'']
  
  w='';
  edge_length=%f;edge_cost=%f;edge_min_cap=%f;edge_max_cap=%f;
  edge_q_weight=%f;  edge_q_orig=%f;edge_weigh=%f;edge_label=%f;
  
  c=['Numbers'
     string(1:nn)'];
  c=part(c,1:max(length(c)+1));
  w=w+c;
  
  c=['Names'
     GraphList.node_name(:)];
  c=part(c,1:max(length(c)+1));
  w=w+'&'+c;
  
  c=['Types';
     string(GraphList.node_type(:))];
  c=part(c,1:max(length(c)+1));
  w=w+'&'+c;
  
  c=['Demands';
     string(GraphList.node_demand(:))];
  c=part(c,1:max(length(c)+1));
  w=w+'&'+c;
  
  c=['Labels'
     ;string(GraphList.node_label(:))];
  c=part(c,1:max(length(c)+1));
  w=w+'&'+c;
  
  w=w+'\\ \hline'
  
  txt=[txt;
       '\begin{tabular}{|l|l|l|l|l|}\hline';
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
     GraphList.edge_name(:)];
  
  c=part(c,1:max(length(c)+1));
  w=w+'&'+c;
  
  if or(GraphList.edge_length<>0) then
    c=['Length';
       string(GraphList.edge_length(:))];
    
    c=part(c,1:max(length(c)+1));
    w=w+'&'+c;
    edge_length=%t;
  end
  
  if or(GraphList.edge_cost<>0) then
    c=['Costs';
       string(GraphList.edge_cost(:))];
    c=part(c,1:max(length(c)+1));
    w=w+'&'+c;
    edge_cost=%t;
  end
  
  if or(GraphList.edge_min_cap<>0) then
    c=['Min Capacities';
       string(GraphList.edge_min_cap(:))];
    c=part(c,1:max(length(c)+1));
    w=w+'&'+c;
    edge_min_cap=%t;
  end
  
  if or(GraphList.edge_max_cap<>0) then
    c=['Max Capacities';
       string(GraphList.edge_max_cap(:))];
    c=part(c,1:max(length(c)+1));
    w=w+'&'+c;
    edge_max_cap=%t;
  end
  
  if or(GraphList.edge_q_weight<>0) then
    c=['Quadratic weights';
       string(GraphList.edge_q_weight(:))];
    c=part(c,1:max(length(c)+1));
    w=w+'&'+c;
    edge_q_weight=%t;
  end
  
  if or(GraphList.edge_q_orig<>0) then
    c=['Quadratic origins';
       string(GraphList.edge_q_orig(:))];
    c=part(c,1:max(length(c)+1));
    w=w+'&'+c;
    edge_q_orig=%t;
  end

  if or(GraphList.edge_weight<>0) then
    c=['Weights';
       string(GraphList.edge_weight(:))];
    c=part(c,1:max(length(c)+1));
    w=w+'&'+c;
    edge_weigh=%t;
  end


  if or(GraphList.edge_label<>'') then
    c=['Labels';
       GraphList.edge_label(:)];
    c=part(c,1:max(length(c)+1));
    w=w+'&'+c;
    edge_label=%t;
  end
  w=w+'\\ \hline'

  t=[edge_length;edge_cost;edge_min_cap;edge_max_cap;
     edge_q_weight;edge_q_orig;edge_weigh;edge_label];
  d='|'+strcat('l'+emptystr(1,size(find(t),'*')+2),'|')+'|'
  w=['\begin{tabular}{'+d+'}\hline';
     w;
     '\end{tabular}'];
  if ~and(t) then
    T=['Lengths';'Costs';'Min Capacities';'Max Capacities';
       'Quadratic weights';'Quadratic origins';'Weights';'Labels'];
    T=T(~t)
    if size(T,'*')==1 then
      txt=[txt;'';strcat(T,', ')+' is set to default values';'\par'];
    else
      txt=[txt;'';strcat(T,', ')+' are set to default values';'\par'];
    end
  end 
  txt=[txt;w;'\end{document}'];
endfunction



