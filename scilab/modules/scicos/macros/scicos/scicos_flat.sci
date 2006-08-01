function  [cor,corinv,links_table,cur_fictitious,ok]=scicos_flat(scs_m,ksup)
//This function takes a hierarchical Scicos diagram and computes the
//"flat" equivalent, removing "non computational" blocs like splits.
//S. Steer, R. Nikoukhah 2003. Copyright INRIA
  if argn(2)<=1 then ksup=0;end //used for recursion
  
  if ksup==0 then   // main scheme
    MaxBlock=countblocks(scs_m);
    //last created fictitious block (clock split,clock sum,super_blocks, superbloc))
    cur_fictitious=MaxBlock
    path=[];       // for delete_unconnected 
    scs_m_s=scs_m ;// for delete_unconnected 
  end
  //-------------- suppress blocks with an unconnected regular port -------------- 
  scs_m=delete_unconnected(scs_m);
  
  //list of blocks with are not retained in the final block list
  blocks_to_remove=['CLKSPLIT_f' 'SPLIT_f' 'IMPSPLIT_f' 'CLKSOM_f' 'CLKSOMV_f' 'NRMSOM_f']
  port_blocks=['IN_f','INIMPL_f','OUT_f','OUTIMPL_f','CLKIN_f','CLKINV_f','CLKOUT_f','CLKOUTV_f']
  n=lstsize(scs_m.objs) //number of "objects" in the data structure
  //-------------- initialize outputs --------------
  nb=0;
  links_table=[]; // 
  corinv=list();
  cor=list();for k=1:n, cor(k)=0;end

  ok=%t;
  Links=[] //to memorize links position in the data structure
  
  //-------------- Analyse blocks --------------

  for k=1:n //loop on all objects
    o=scs_m.objs(k);
    x=getfield(1,o);
    cor(k)=0
    if x(1)=='Block' then
      if or(o.gui==blocks_to_remove) then
	cur_fictitious=cur_fictitious+1;
	cor(k)=cur_fictitious
      elseif o.gui=='SUM_f'|o.gui=='SOM_f' then
	nb=nb+1;
	corinv(nb)=k;
	cor(k)=nb
	//scs_m=adjust_sum(scs_m,k)
      elseif or(o.gui==port_blocks) then
	//here we suppose to be inside a superblock
	//may be we can handle this blocks just as blocks_to_remove
	if ksup==0 then 
	  scs_m=scs_m_s
	  hilite_path([path,k],'Port blocks must be only used in a Super Block',%f)
	  ok=%f;return
	end
	connected=get_connected(scs_m,k)
	if connected==[] then
	  scs_m=scs_m_s
	  hilite_path([path,k],'This Super Block Input port is unconnected',%t)
	  ok=%f;return
	end
	if or(o.gui==['IN_f','INIMPL_f']) then
	  pind=Pind(1)
	elseif or(o.gui==['OUT_f','OUTIMPL_f']) then
	  pind=Pind(2)
	elseif or(o.gui==['CLKIN_f','CLKINV_f']) then
	  pind=Pind(3)
	elseif or(o.gui==['CLKOUT_f','CLKOUTV_f']) then
	  pind=Pind(4)
	end 
	//connect the link to the fictitious bloc replacing the superblock
	if scs_m.objs(connected).from(1)==k then
	  scs_m.objs(connected).from(1)=-(pind+o.model.ipar)
	end
	if scs_m.objs(connected).to(1)==k then
	  scs_m.objs(connected).to(1)=-(pind+o.model.ipar)
	end

      elseif o.model.sim=='super'|o.model.sim=='csuper' then
	path=[path k] //superbloc path in the hierarchy
	//replace superbloc by a set of fictitious blocks (one per port)
        //and reconnect links connected to the superblock to these
        //ficitious blocks
	Pinds=[];if exists('Pind') then Pinds=Pind,end
	Pind=[] //base of ports numbering
	//mprintf("entering superblock at level '+string(size(path,'*'))+"\r\n")
	for port_type=['pin','pout','pein','peout']
	  Pind=[Pind cur_fictitious]
	  ip=scs_m.objs(k).graphics(port_type);ki=find(ip>0)
	  for kk=ki
	    kc=ip(kk)
	    if scs_m.objs(kc).to(1)==k then  // a link going to the superblock
	      scs_m.objs(kc).to(1)=-(cur_fictitious+scs_m.objs(kc).to(2));
	      scs_m.objs(kc).to(2)=1
	    end
	    if scs_m.objs(kc).from(1)==k then  // a link coming from the superblock
	      scs_m.objs(kc).from(1)=-(cur_fictitious+scs_m.objs(kc).from(2));
	      scs_m.objs(kc).from(2)=1
	    end
	  end
	  cur_fictitious=cur_fictitious+size(ip,'*')
	end


	//Analyze the superblock contents

        [cors,corinvs,lt,cur_fictitious,ok]=scicos_flat(o.model.rpar,cur_fictitious)
	if ~ok then return,end
	nbs=size(corinvs) 
	
	//catenate superbloc data with current data

	f=find(lt(:,1)>0&lt(:,1)<=nbs);if f<>[] then lt(f,1)=lt(f,1)+nb,end
	links_table=[links_table;lt]
	
	for kk=1:nbs, corinv(nb+kk)=[k,corinvs(kk)];end
	cors=shiftcors(cors,nb)
	cor(k)=cors

	nb=nb+nbs
	Pind=Pinds
	path($)=[]
      else//standard blocks
	nb=nb+1
	corinv(nb)=k
	//[model,ok]=build_block(o.model)
	cor(k)=nb
      end
    elseif x(1)=='Deleted'|x(1)=='Text' then
	//this objects are ignored
    else //links
      Links=[Links k] // memorize their position for use during links analysis
    end
  end //end of loop on objects
  
  if ksup==0&nb==0 then
    message('Empty diagram')
    ok=%f
    return
  end

  //-------------- Analyse  links -------------- 
  for k=Links
    o=scs_m.objs(k);
    f=0
    if o.from(1)<0|o.from(1)>MaxBlock then //Link coming from a superblock input port
    else
      o.from(1)=cor(o.from(1));
    end
    if o.to(1)<0 |o.to(1)>MaxBlock then //Link going to a superblock output port
    else
      o.to(1)=cor(o.to(1)),
    end

    if o.ct(2)==2 //implicit links
      //if abs(o.from(1))==125|abs(o.to(1))==125 then pause,end
      links_table=[links_table
		   o.from(1:3)    o.ct(2) 
		   o.to(1:3)      o.ct(2) ]
    else //regular or event links
      links_table=[links_table
		   o.from(1:2)  -1  o.ct(2) //outputs are tagged with -1 
		   o.to(1:2)    1   o.ct(2) ] //inputs are tagged with 1
    end
  end
endfunction
