function setFontSize(vp)

	dim = xget('wdim');
	dim = dim.*vp(3:4);

	if or(dim <= 400)   // font size, depending on
	  fontsize=1;     // subplot state and window size
	else
	  fontsize=2;
	end

	xset('font',2,fontsize);
