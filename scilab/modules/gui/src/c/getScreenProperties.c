int GetScreenProperty(char *prop, char *value)
{
	
//  POINT Ecran = {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)} ;
//  HDC hdc=GetDC(NULL);
//  if(!strcmp(prop,"screensize_px"))
//    {
//      sprintf(value,"%f|%f|%f|%f",(float)1,(float)1,
//	      (float)Ecran.x,
//	      (float)Ecran.y);
//    }
//  else if(!strcmp(prop,"screensize_mm"))
//    {
//      sprintf(value,"%f|%f|%f|%f",(float)0,(float)0,
//	      (float)GetDeviceCaps(hdc,HORZSIZE),
//	      (float)GetDeviceCaps(hdc,VERTSIZE));
//    }
//  else if(!strcmp(prop,"screensize_cm"))
//    {
//      sprintf(value,"%f|%f|%f|%f",(float)0,(float)0,
//	      (float)GetDeviceCaps(hdc,HORZSIZE)/10,
//	      (float)GetDeviceCaps(hdc,VERTSIZE)/10);
//    }
//  else if(!strcmp(prop,"screensize_in"))
//    {
//      sprintf(value,"%f|%f|%f|%f",(float)0,(float)0,
//								  (float)GetDeviceCaps(hdc,HORZSIZE)/25.4,
//								  (float)GetDeviceCaps(hdc,VERTSIZE)/25.4);
//    }
//  else if(!strcmp(prop,"screensize_pt"))
//    {
//      sprintf(value,"%f|%f|%f|%f",(float)0,(float)0,
//								  (float)GetDeviceCaps(hdc,HORZSIZE)/25.4*72,
//								  (float)GetDeviceCaps(hdc,VERTSIZE)/25.4*72);
//    }
//  else if(!strcmp(prop,"screensize_norm"))
//    {
//      sprintf(value,"%f|%f|%f|%f",(float)0,(float)0,(float)1,(float)1);
//    }
//  else if(!strcmp(prop,"screendepth"))
//    {
//      sprintf(value,"%f",(float)GetDeviceCaps(hdc,BITSPIXEL));
//    }
//  else
//    {
//      return -1;
//    }
//  ReleaseDC (NULL, hdc);
  return 0;
}


/*--------------------------------------------------------------------------*/
/* Scilab get the DPI (root properties) */
/* F.Leray 08.03.05 */
/* return the x/y DPI */
int GetScreenDPI(int *ixres, int *iyres)
{
	*ixres = 0;
	*iyres = 0;
//  POINT Ecran = {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)} ;
//  HDC hdc=GetDC(NULL);
//
//  *ixres = GetDeviceCaps(hdc, LOGPIXELSX);
//  *iyres = GetDeviceCaps(hdc, LOGPIXELSY);
//  
//  ReleaseDC (NULL, hdc);
  return 0;
}

/*--------------------------------------------------------------------------*/
