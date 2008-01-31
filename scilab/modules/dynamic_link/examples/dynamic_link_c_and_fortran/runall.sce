examples = [ 'ext1c.sce' ; 'ext1f.sce' ;
             'ext2c.sce' ; 'ext2f.sce' ;
             'ext3c.sce' ; 'ext3f.sce' ;
             'ext4c.sce' ; 'ext4f.sce' ;
             'ext5c.sce' ; 'ext5f.sce' ;
             'ext6c.sce' ; 'ext6f.sce' ;
             'ext7c.sce' ; 'ext7f.sce' ;
             'ext8c.sce' ; 'ext8f.sce' ;
             'ext9c.sce' ; 'ext9f.sce' ;
             'ext10c.sce' ; 'ext10f.sce' ;
             'ext11c.sce' ; 'ext12c.sce' ;
             'ext13c.sce' ; 'ext14c.sce' ];
             
for i=1:size(examples,'*')
   disp('-----------------------------------');
   disp('Example : ' + examples(i));
   disp('-----------------------------------');
   exec(examples(i));
   
end             









