function GraphList=ge_new_graph()
// Returns a graph data structure with no nodes and no edges
  
//Copyright INRIA
//Author : Serge Steer 2002

//data structures fields names
  ttgraph = ['graph','version','name','directed','nodes','edges'];
  ttnodes=['nodes','number','graphics','data'];
  ttgnodes=['ngraphic','display','defaults','name','x','y','type','diam','border','colors','font','displaymode']
  ttdefnodes=['nodedefs','type','diam','border','colors','font']
  ttedges=['edges','tail','head','graphics','data'];
  ttgedges=['egraphic','display','defaults','profiles','name','width','foreground','font','profile_index','displaymode']
  ttdefedges=['edgedefs','width','foreground','font','profile_index'];
  
  //nodes default values
  if exists('name','local')==0 then name='Untitled',end
  version='5.0.1';
  diam=15;
  border=1;
  colors=[-1;-2];//[foreground; background]
  font=[3;8;-1];//[font size; font type; font foreground]
  nodetype=0;
  defnodes=tlist(ttdefnodes,nodetype,diam,border,colors,font);


  //nodes graphic data structure
  name=[];
  nodetype=[];
  x=[];
  y=[];
  diam=[];
  border=[];
  colors=[];
  font=[];
  display='nothing'
  displaymode='center'
  gnodes=mlist(ttgnodes,display,defnodes,name,x,y,nodetype,diam,border,colors,font,displaymode);
  //main nodes data_structure
  data=mlist(['nodedata'])  
  nodes=mlist(ttnodes,0,gnodes,data)
  
  //edges default values
  foreground=-1;
  width= 1;
  font=[3;8;-1];//[font size; font type; font foreground]
  profile_index=2;
  defedges=tlist(ttdefedges,width,foreground,font,profile_index);

  //edges graphic data structure
  name=[]
  foreground=[]
  width=[]
  font=[]
  profile_index=[]
  profiles=list(1.2,ge_default_edge_profile())
  display='nothing'
  displaymode='tangeant'
  gedges=mlist(ttgedges,display,defedges,profiles,name,width,foreground,font,profile_index,displaymode)
  data=mlist(['edgedata']);

  //Main edges data structure
  tail=[]
  head=[]
  edges=mlist(ttedges,tail,head,gedges,data)

  //graph data structure
  directed=1
  name='Untitled'
  GraphList=tlist(ttgraph,version,name,directed,nodes,edges)
endfunction
