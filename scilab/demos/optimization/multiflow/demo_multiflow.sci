function demo_multiflow(gfile)
//              The Graph
  //demo_help demo_multiflot
  grs=get('old_style');set('old_style','on')
  win=edit_graph(editgraph_diagram(),1,[700,616])
  xset("wpos",600,16);xset("wdim",600*0.9,400*0.9);
  ge_set_nodes_id('Label')
  ge_set_arcs_id('Label')
  
  G=load_graph(gfile);
  G.default_font_size=14;
  As=full(graph_2_mat(G));  //Node-Edge incidence Matrix
  p=size(As,1);       //Number of nodes
  n=size(As,2);       //Number of arcs

  //              Image The Multiflow problem on the graph
  K=2;            //number of flows           
  sources=[1,10];  // sources nodes index
  sinks=[6,3];    // sink nodes index
  colors=[3,10];  // color of sources and sink for each flows 
  values=[5,4];   // values of each flow


  // set node source and sink nodes types in Graph data structure
  source=2;sink=1;
  G.node_type=zeros(1,p);
  G.node_type(sources)=source;
  G.node_type(sinks)=sink;

  // set node source and sink nodes colors in Graph data structure
  G.node_color(sources)=colors;
  G.node_color(sinks)=colors;

  //add label to the nodes
  G.node_label=emptystr(1,p);  
  G.node_label(sources)=string(values)
  G.node_label(sinks)=string(-values)

  show_graph(G)

  //add a title
  xstringb(50,600,'Oriented graph with sources and sinks',600, ...
	   80,'fill')
  realtimeinit(0.1);for k=1:50,realtime(k),end // wait a little
  //              Definition of the LP problem (As,B,c,U)
  //let X(i,k) the flow of commodity k on arc i and x=matrix(X,K*n;1)
  //Min Cost'*x    
  //diag(As,As)*x = B = [b1;b2;...];
  //[I,I]*x <= U


  //Form the Linear Problem matrices 

  //Column k of B = rhs vector for flow k.

  B=zeros(p,K);
  for k=1:K
    B(sources(k),k)=values(k);
    B(sinks(k),k)=-values(k);
  end



  //Arcs costs 
  rand('seed',0);
  c=rand(n,1); // random cost between 0 and 1
  
  //Arcs capacities
  umax=2;
  U=umax*ones(n,1); // each arc as the same max capacity 2


  //Min Cost'*x    
  //diag(As,As)*x = B = [b1;b2;...];
  //[I,I]*x <= U

  //Cost [c;c;...]
  Cost=ones(K,1).*.c;

  //diag(As,As,...)*x = B = [b1;b2;...];

  bigA=eye(K,K).*.As;

  //[I,I,...]*x <= U
  Capacity=ones(1,K).*.eye(n,n);


  Rhs=[B(:);U];lbounds=zeros(K*n,1);ubounds=10^30*ones(K*n,1);


  A=full([bigA;Capacity]);mi=K*p;

  [x,l,obj]=linpro(Cost,A,Rhs,lbounds,ubounds,mi);

  X=matrix(x,n,K);   
  //Entry (i,k) of X = amount of commodity k circulating on edge i.

  X=clean(X,0.0001);

  x1=X(1:n);x2=X(n+1:$);
  x1=clean(x1,0.0001);x2=clean(x2,0.000001);
  I1=sign(x1);I2=sign(x2);
  I=sign(X);

  w1=find(I(:,1)==1&sum(I,2)~=2);
  w2=find(I(:,2)==1&sum(I,2)~=2);
  w3=find(sum(I,2)==2);

  G.edge_color(w1)=colors(1);
  G.edge_color(w2)=colors(2);

  G.edge_label(n)=' ';
  G.edge_label(w1)=part(string(x1(w1)),1:2);
  G.edge_label(w2)=part(string(x2(w2)),1:2);

  G.edge_label(w3)='SAT';
  G.edge_label=G.edge_label';
  ge_set_arcs_id('Label')
  show_graph(G);
  xstringb(50,600,'Computed flows',600,  80,'fill');
  ;;
  realtimeinit(0.1);for k=1:30,realtime(k),end // wait a little
  ge_do_quit(%f)
  xdel(win)
 
  set('old_style',stripblanks(grs))
endfunction


