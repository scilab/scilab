function lightenedSurface=lighten(theSurf,vect)

global currentSurf currentLightVect

currentSurf=theSurf;
currentLightVect=vect(:);

[nArgOut,vectInput]=check3dFun('lighten',theSurf,rand(5,1),rand(5,1));

if nArgOut==3

deff('[x,y,z]=lightenedSurface(u,v)',['global currentSurf currentLightVect';...
'[x,y,z]=currentSurf(u,v)';
'[xu,yu,zu,xv,yv,zv]=parametricDiffDeriv(currentSurf,u,v,x,y,z)';
'[n]=parametricNormals(xu(:)'',yu(:)'',zu(:)'',xv(:)'',yv(:)'',zv(:)'')';
'z=z(:)+%i*computeLight(-n,currentLightVect)']);
// Sign problem ? Where ? //
else

deff('[z]=lightenedSurface(u,v)',['global currentSurf currentLightVect';...
'[z]=currentSurf(u,v)';
'[zu,zv]=nonParametricDiffDeriv(currentSurf,u,v,z)';
'[n]=nonParametricNormals(zu(:)'',zv(:)'')';
'z=z(:)+%i*computeLight(n,currentLightVect)']);

end
