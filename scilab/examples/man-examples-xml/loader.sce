
path=get_absolute_file_path('loader.sce');

myhelps=['helpdir1', "Title1";
	 'helpdir2', "Title2"]

for n=1:size(myhelps,'r'); 
  if grep(%helps(:,1),myhelps(n,1))== [] then 
    %helps=[%helps;myhelps(n,:)];
  end
end



	
	

