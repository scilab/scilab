/* Copyright ENPC */
/* wmessage.c
 * Scilab 
 *   Jean-Philipe Chancelier 
 *   Allan CORNET 2004
 *   Bugs and mail : Scilab@inria.fr 
 */
#include <windows.h>
#include "wmen_scilab.h"
#include "../wsci/resource.h"
extern HINSTANCE hdllInstance;
extern MADialog MAD;


#define SizePageCol 5
#define SizePageLin 5

//int IDC_Array[SizePageCol][SizePageLin];

int IDC_Array[]={IDC_EDITX0Y0,IDC_EDITX1Y0,IDC_EDITX2Y0,IDC_EDITX3Y0,IDC_EDITX4Y0,
				 IDC_EDITX0Y1,IDC_EDITX1Y1,IDC_EDITX2Y1,IDC_EDITX3Y1,IDC_EDITX4Y1,
				 IDC_EDITX0Y2,IDC_EDITX1Y2,IDC_EDITX2Y2,IDC_EDITX3Y2,IDC_EDITX4Y2,
				 IDC_EDITX0Y3,IDC_EDITX1Y3,IDC_EDITX2Y3,IDC_EDITX3Y3,IDC_EDITX4Y3,
				 IDC_EDITX0Y4,IDC_EDITX1Y4,IDC_EDITX2Y4,IDC_EDITX3Y4,IDC_EDITX4Y4};
/*-----------------------------------------------------------------------------------*/
BOOL CALLBACK MatDialogProc(HWND hwnd,UINT Message, WPARAM wParam, LPARAM lParam)
{
  
  switch (Message)
  {
	case WM_INITDIALOG:
		{
			int i,j,b;
			if ( MAD.nv < (SizePageLin*SizePageCol) )
			{
				//SendDlgItemMessage(hdlg, IDC_LISTCHOOSEASCILAB, LB_ADDSTRING, 0, (LPARAM)((LPSTR)ListScilabName[i] ));
			}

			b=0;
			for (i=0;i<SizePageCol;i++)
			{
				for(j=0;j<SizePageLin;j++)
				{if ( (MAD.nl < SizePageLin) && (i == MAD.nl) ) i++;
					if ( (MAD.nc < SizePageCol) && (j == MAD.nc) ) j++;
					b=i*j;
					SetDlgItemText(hwnd, IDC_Array[b], (LPSTR)MAD.data[b] );
					
				}
			}
			
		}
		
		break;
  case WM_COMMAND:
    switch (LOWORD(wParam))
	{
    case IDOK:
    	  EndDialog(hwnd, IDOK);
	  return TRUE;
	case IDCANCEL:
    	  EndDialog(hwnd, IDCANCEL);
	  return TRUE;
     }
    break;
  }
  return FALSE;
}
/*-----------------------------------------------------------------------------------*/
int MatrixDialogWindow()
{
  /*
  HWND hwndOwner=NULL ;
  int ValReturn=0;

  ValReturn=DialogBox(hdllInstance,(LPCSTR)(IDD_MDIALOG),hwndOwner,(DLGPROC)MatDialogProc);
*/
  /*if (ValReturn  == IDOK)
    /*return(TRUE);*/
	/*return(FALSE);
  else 
    return(FALSE);*/
  sciprint("Sorry : Matdialog not implemented . See \"help editvar\" \r\n");
  return(FALSE);
}

/*-----------------------------------------------------------------------------------*/
