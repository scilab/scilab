/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/

/*--------------------------------------------------------------------------*/
#include "TextToPrint.h"
#include "sci_malloc.h"
#include "charEncoding.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
static HDC PrinterHDC = NULL;
static char PrinterName[2048];
static char PrinterOrientation;
/*--------------------------------------------------------------------------*/
HFONT EzCreateFont (HDC hdc, TCHAR * szFaceName, int iDeciPtHeight, int iDeciPtWidth, int iAttributes, BOOL fLogRes)
{
    FLOAT cxDpi, cyDpi ;
    HFONT hFont ;
    LOGFONT lf ;
    POINT pt ;
    TEXTMETRIC tm ;
    SaveDC (hdc) ;
    SetGraphicsMode (hdc, GM_ADVANCED) ;
    ModifyWorldTransform (hdc, NULL, MWT_IDENTITY) ;
    SetViewportOrgEx (hdc, 0, 0, NULL) ;
    SetWindowOrgEx (hdc, 0, 0, NULL) ;
    if (fLogRes)
    {
        cxDpi = (FLOAT) GetDeviceCaps (hdc, LOGPIXELSX) ;
        cyDpi = (FLOAT) GetDeviceCaps (hdc, LOGPIXELSY) ;
    }
    else
    {
        cxDpi = (FLOAT) (25.4 * GetDeviceCaps (hdc, HORZRES) /
                         GetDeviceCaps (hdc, HORZSIZE)) ;
        cyDpi = (FLOAT) (25.4 * GetDeviceCaps (hdc, VERTRES) /
                         GetDeviceCaps (hdc, VERTSIZE)) ;
    }
    pt.x = (int) (iDeciPtWidth * cxDpi / 72) ;
    pt.y = (int) (iDeciPtHeight * cyDpi / 72) ;
    DPtoLP (hdc, &pt, 1) ;

    lf.lfHeight = - (int) (fabs (pt.y) / 10.0 + 0.5) ;
    lf.lfWidth = 0 ;
    lf.lfEscapement = 0 ;
    lf.lfOrientation = 0 ;
    lf.lfWeight = iAttributes & EZ_ATTR_BOLD ? 700 : 0 ;
    lf.lfItalic = iAttributes & EZ_ATTR_ITALIC ? 1 : 0 ;
    lf.lfUnderline = iAttributes & EZ_ATTR_UNDERLINE ? 1 : 0 ;
    lf.lfStrikeOut = iAttributes & EZ_ATTR_STRIKEOUT ? 1 : 0 ;
    lf.lfCharSet = DEFAULT_CHARSET ;
    lf.lfOutPrecision = 0 ;
    lf.lfClipPrecision = 0 ;
    lf.lfQuality = 0 ;
    lf.lfPitchAndFamily = 0 ;
    lstrcpy (lf.lfFaceName, szFaceName) ;
    hFont = CreateFontIndirect (&lf) ;
    if (iDeciPtWidth != 0)
    {
        hFont = (HFONT) SelectObject (hdc, hFont) ;
        GetTextMetrics (hdc, &tm) ;
        DeleteObject (SelectObject (hdc, hFont)) ;
        lf.lfWidth = (int) (tm.tmAveCharWidth *
                            fabs (pt.x) / fabs (pt.y) + 0.5) ;
        hFont = CreateFontIndirect (&lf) ;
    }
    RestoreDC (hdc, -1) ;
    return hFont ;
}
/*--------------------------------------------------------------------------*/
void PrintString(char *lines, char *Entete)
{
    HDC PrintDC;
    HFONT hFont, hOldFont;
    HDC hDCmem;
    DOCINFO di;
    int TextLength = 0;
    int i = 0;
    int Index1 = 0;
    int Index2 = 3;
    int numero = 1;
    // Extrait les informations sur la police
    TEXTMETRIC tm;
    int NbLigneParPage = 0;
    int HauteurCaractere = 0;
    int NombredeCaracteresparLignes = 0;


    PrintDC = GetPrinterDC();
    if (PrintDC != NULL)
    {
        hFont = EzCreateFont (PrintDC, TEXT ("Courier New"), 120, 0, 0, TRUE) ;

        hOldFont = SelectObject(PrintDC, hFont );

        hDCmem = CreateCompatibleDC(PrintDC);
        memset( &di, 0, sizeof( DOCINFO ) );
        di.cbSize      = sizeof( DOCINFO );
        di.lpszDocName = "Scilab Document";

        TextLength = (int)strlen(lines);

        GetTextMetrics(PrintDC, (TEXTMETRIC *)&tm);

        NombredeCaracteresparLignes = GetDeviceCaps(PrintDC, HORZRES) / (tm.tmMaxCharWidth + 1);
        // la valeur HauteurCaractere contient hauteur des caractéres + l'interligne
        HauteurCaractere = tm.tmHeight + tm.tmExternalLeading;
        NbLigneParPage = GetDeviceCaps(PrintDC, VERTRES) / HauteurCaractere;

        if (TextLength > 0)
        {
            if ( StartDoc( PrintDC, &di ) > 0 )
            {
                char *LignePrint = NULL;
                LignePrint = (char*)MALLOC((NombredeCaracteresparLignes + 1) * sizeof(char));

                StartPage(PrintDC);
                PageHeader(PrintDC, Entete);
                for (i = 0; i < TextLength; i++)
                {
                    LignePrint[Index1] = lines[i];
                    if ( Index1 == NombredeCaracteresparLignes )
                    {
                        Index2 ++;
                        LignePrint[Index1] = '\0';
                        if (LignePrint[Index1 - 1] == '\r')
                        {
                            LignePrint[Index1 - 1] = '\0';
                        }
                        TextOut (PrintDC, (tm.tmMaxCharWidth + 10), Index2 * HauteurCaractere, LignePrint, (int)strlen(LignePrint));
                        Index1 = 0;
                        if (LignePrint)
                        {
                            FREE(LignePrint);
                            LignePrint = NULL;
                        }
                        LignePrint = (char*)MALLOC((NombredeCaracteresparLignes + 1) * sizeof(char));
                    }
                    else if ( (lines[i] == '\n') )
                    {
                        Index2 ++;
                        if (Index1 > 0)
                        {
                            LignePrint[Index1] = '\0';
                            if (LignePrint[Index1 - 1] == '\r')
                            {
                                LignePrint[Index1 - 1] = '\0';
                            }
                            TextOut (PrintDC, (tm.tmMaxCharWidth + 10), Index2 * HauteurCaractere, LignePrint, (int)strlen(LignePrint));
                            Index1 = 0;
                        }
                        if (LignePrint)
                        {
                            FREE(LignePrint);
                            LignePrint = NULL;
                        }
                        LignePrint = (char*)MALLOC((NombredeCaracteresparLignes + 1) * sizeof(char));
                    }
                    else
                    {
                        Index1 ++;
                    }
                    if (Index2 == NbLigneParPage - 4)
                    {
                        Footer(PrintDC, numero);
                        EndPage (PrintDC);
                        StartPage(PrintDC);
                        numero++;
                        PageHeader(PrintDC, Entete);
                        Index2 = 3;
                    }
                }
                Index2 ++;
                LignePrint[Index1] = '\0';
                TextOut (PrintDC, (tm.tmMaxCharWidth + 10), Index2 * HauteurCaractere, LignePrint, (int) strlen(LignePrint));
                Footer(PrintDC, numero);
                EndPage (PrintDC);
                EndDoc (PrintDC);

                if (LignePrint)
                {
                    FREE(LignePrint);
                    LignePrint = NULL;
                }
            }
        }
        SelectObject(PrintDC, hOldFont );
    }

}
/*--------------------------------------------------------------------------*/
void PrintFile(char *filename)
{
#define MAXBUF 4096
    HDC PrintDC;
    HFONT hFont, hOldFont;
    HDC hDCmem;
    DOCINFO di;
    int Index2 = 3;
    int numero = 1;
    // Extrait les informations sur la police
    TEXTMETRIC tm;
    int NbLigneParPage = 0;
    int HauteurCaractere = 0;
    int NombredeCaracteresparLignes = 0;
    FILE * pFile;
    char  line[MAXBUF];

    PrintDC = GetPrinterDC();
    if (PrintDC != NULL)
    {

        hFont = EzCreateFont (PrintDC, TEXT ("Courier New"), 120, 0, 0, TRUE) ;
        hOldFont = SelectObject(PrintDC, hFont );

        hDCmem = CreateCompatibleDC(PrintDC);
        memset( &di, 0, sizeof( DOCINFO ) );
        di.cbSize      = sizeof( DOCINFO );
        di.lpszDocName = "Scilab Document";

        GetTextMetrics(PrintDC, (TEXTMETRIC *)&tm);

        NombredeCaracteresparLignes = GetDeviceCaps(PrintDC, HORZRES) / (tm.tmMaxCharWidth + 1);
        // la valeur HauteurCaractere contient hauteur des caractéres + l'interligne
        HauteurCaractere = tm.tmHeight + tm.tmExternalLeading;
        NbLigneParPage = GetDeviceCaps(PrintDC, VERTRES) / HauteurCaractere;

        wcfopen(pFile , filename, "rt");

        if (pFile)
        {
            if ( StartDoc( PrintDC, &di ) > 0 )
            {
                char *LignePrint = NULL;
                StartPage(PrintDC);
                PageHeader(PrintDC, filename);

                while (fgets (line, sizeof(line), pFile) != NULL)
                {
                    if (line[strlen(line) - 1] == '\n')
                    {
                        line[strlen(line) - 1] = '\0';    /* enleve le retour chariot */
                    }
                    if (line[strlen(line) - 2] == '\r')
                    {
                        line[strlen(line) - 2] = '\0';    /* enleve le retour chariot */
                    }

                    if ( strlen(line) > (unsigned int )NombredeCaracteresparLignes)
                    {
                        int i = 0;
                        int j = 0;
                        int subline = 0;
                        int restsubline = 0;
                        subline = (int)strlen(line) / NombredeCaracteresparLignes ;
                        restsubline = (int)strlen(line) % NombredeCaracteresparLignes ;

                        for (i = 0; i < subline; i++)
                        {
                            LignePrint = (char*)MALLOC( (NombredeCaracteresparLignes + 1) * sizeof(char));
                            for (j = 0; j < (NombredeCaracteresparLignes); j++)
                            {
                                if (line[(i * NombredeCaracteresparLignes) + j] == 9) /* == \t */
                                {
                                    LignePrint[j] = ' ';
                                }
                                else
                                {
                                    LignePrint[j] = line[(i * NombredeCaracteresparLignes) + j];
                                }

                            }
                            LignePrint[j] = '\0';

                            TextOut (PrintDC, (tm.tmMaxCharWidth + 10), Index2 * HauteurCaractere, LignePrint, (int)strlen(LignePrint));
                            if (LignePrint)
                            {
                                FREE(LignePrint);
                                LignePrint = NULL;
                            }
                            Index2 ++;
                            if (Index2 == NbLigneParPage - 4)
                            {
                                Footer(PrintDC, numero);
                                EndPage (PrintDC);
                                StartPage(PrintDC);
                                numero++;
                                PageHeader(PrintDC, filename);
                                Index2 = 3;
                            }
                        }
                        if (restsubline > 0)
                        {
                            LignePrint = (char*)MALLOC( (NombredeCaracteresparLignes + 1) * sizeof(char));
                            for (j = 0; j < (restsubline); j++)
                            {
                                if (line[(i * NombredeCaracteresparLignes) + j] == 9) /* == \t */
                                {
                                    LignePrint[j] = ' ';
                                }
                                else
                                {
                                    LignePrint[j] = line[(i * NombredeCaracteresparLignes) + j];
                                }
                            }
                            LignePrint[j] = '\0';
                            TextOut (PrintDC, (tm.tmMaxCharWidth + 10), Index2 * HauteurCaractere, LignePrint, (int)strlen(LignePrint));
                            if (LignePrint)
                            {
                                FREE(LignePrint);
                                LignePrint = NULL;
                            }
                            Index2 ++;
                            if (Index2 == NbLigneParPage - 4)
                            {
                                Footer(PrintDC, numero);
                                EndPage (PrintDC);
                                StartPage(PrintDC);
                                numero++;
                                PageHeader(PrintDC, filename);
                                Index2 = 3;
                            }
                        }
                    }
                    else
                    {
                        LignePrint = os_strdup(line);
                        TextOut (PrintDC, (tm.tmMaxCharWidth + 10), Index2 * HauteurCaractere, LignePrint, (int)strlen(LignePrint));
                        if (LignePrint)
                        {
                            FREE(LignePrint);
                            LignePrint = NULL;
                        }
                        Index2 ++;
                        if (Index2 == NbLigneParPage - 4)
                        {
                            Footer(PrintDC, numero);
                            EndPage (PrintDC);
                            StartPage(PrintDC);
                            numero++;
                            PageHeader(PrintDC, filename);
                            Index2 = 3;
                        }
                    }
                }
                fclose(pFile);

                Footer(PrintDC, numero);
                EndPage (PrintDC);
                EndDoc (PrintDC);
            }
        }
        SelectObject(PrintDC, hOldFont );
    }
}
/*--------------------------------------------------------------------------*/
void Footer(HDC hdc, int number)
{
    HPEN hPen, hPenOld;
    LOGBRUSH lb;
    int NombredeCaracteresparLignes = 0;
    TEXTMETRIC tm;
    int yChar = 0;
    char *ptrLine = NULL;

    int CySize = GetDeviceCaps(hdc, VERTRES);
    // Initialize the pen's brush.
    lb.lbStyle = BS_SOLID;
    lb.lbColor = RGB(0, 0, 0);
    lb.lbHatch = 0;

    GetTextMetrics (hdc, (TEXTMETRIC *) & tm);
    NombredeCaracteresparLignes = tm.tmMaxCharWidth + 10;
    yChar = tm.tmHeight + tm.tmExternalLeading ;
    ptrLine = (char*)MALLOC( (NombredeCaracteresparLignes + 1) * sizeof(char));

    hPen = ExtCreatePen(PS_SOLID, 1, &lb, 0, NULL);
    hPenOld = SelectObject(hdc, hPen);

    MoveToEx(hdc, (tm.tmMaxCharWidth + 10), CySize - (yChar * 3) - 10, NULL);
    LineTo(hdc, GetDeviceCaps(hdc, HORZRES) - (tm.tmMaxCharWidth + 10), CySize - (yChar * 3) - 10);

    SelectObject(hdc, hPenOld);
    DeleteObject(hPen);

    wsprintf(ptrLine, "Page : %d", number);
    TextOut(hdc, (tm.tmMaxCharWidth + 10), CySize - (yChar * 3), ptrLine, (int)strlen(ptrLine));
    FREE(ptrLine);
}
/*--------------------------------------------------------------------------*/
void PageHeader(HDC hdc, LPSTR Entete)
{
    HPEN hPen, hPenOld;
    LOGBRUSH lb;

    TEXTMETRIC tm;
    int NbLigneParPage = 0;
    int HauteurCaractere = 0;
    int NombredeCaracteresparLignes = 0;
    int NombredeLignesOccupeesparEntete = 1;

    char dbuffer [9];
    char tbuffer [9];
    char *ptrLine = NULL;

    // Initialize the pen's brush.
    lb.lbStyle = BS_SOLID;
    lb.lbColor = RGB(0, 0, 0);
    lb.lbHatch = 0;

    _strdate( dbuffer );
    _strtime( tbuffer );

    GetTextMetrics (hdc, (TEXTMETRIC *) & tm);

    NombredeCaracteresparLignes = GetDeviceCaps(hdc, HORZRES) / (tm.tmMaxCharWidth + 10);
    // la valeur HauteurCaractere contient hauteur des caractéres + l'interligne
    HauteurCaractere = tm.tmHeight + tm.tmExternalLeading;
    NbLigneParPage = GetDeviceCaps(hdc, VERTRES) / HauteurCaractere;

    ptrLine = (char*)MALLOC( (NombredeCaracteresparLignes + 1) * sizeof(char));
    wsprintf(ptrLine, "%s %s %s", dbuffer, tbuffer, Entete);

    TextOut(hdc, (tm.tmMaxCharWidth + 10), NombredeLignesOccupeesparEntete * HauteurCaractere, ptrLine, (int)strlen(ptrLine));
    NombredeLignesOccupeesparEntete++;

    hPen = ExtCreatePen(PS_SOLID, 1, &lb, 0, NULL);
    hPenOld = SelectObject(hdc, hPen);

    MoveToEx(hdc, (tm.tmMaxCharWidth + 10), NombredeLignesOccupeesparEntete * HauteurCaractere, NULL);
    LineTo(hdc, GetDeviceCaps(hdc, HORZRES) - (tm.tmMaxCharWidth + 10), NombredeLignesOccupeesparEntete * HauteurCaractere);

    SelectObject(hdc, hPenOld);
    DeleteObject(hPen);

    FREE(ptrLine);

}
/*--------------------------------------------------------------------------*/
void CutLineForDisplay(char *CutLine, char *Line, int NumberOfCharByLine)
{
    int LenLine = lstrlen(Line);

    if (LenLine > NumberOfCharByLine)
    {
        int NumberOfLines = lstrlen(Line) / NumberOfCharByLine;
        int i = 0;
        char *Buffer = NULL;

        Buffer = (char*)MALLOC((LenLine + NumberOfLines + 1) * sizeof(char));
        for (i = 0; i < NumberOfLines + 1; i++)
        {
            if ( i == 0)
            {
                lstrcpyn(Buffer, &Line[i * NumberOfCharByLine], NumberOfCharByLine);

            }
            else
            {
                char BufferCat[MAX_PATH];
                lstrcpyn(BufferCat, &Line[i * NumberOfCharByLine], NumberOfCharByLine);
                lstrcat(Buffer, BufferCat);

            }
            if (i != NumberOfLines)
            {
                lstrcat(Buffer, "\n");
            }
        }
        lstrcpy(CutLine, Buffer);

        FREE(Buffer);
    }
    else
    {
        wsprintf(CutLine, "%s", Line);
    }
}
/*--------------------------------------------------------------------------*/
BOOL ConfigurePrinterDialogBox(void)
{
    BOOL bOK = FALSE;

    PRINTDLG pd;

    if (PrinterHDC)
    {
        DeleteDC(PrinterHDC);
        PrinterHDC = NULL;
    }
    wsprintf(PrinterName, "%s", "EMPTY");
    PrinterOrientation = 'p';

    memset (&pd, 0, sizeof (PRINTDLG));
    pd.lStructSize = sizeof (PRINTDLG);
    pd.hwndOwner = NULL;
    pd.hDevMode = NULL;
    pd.hDevNames = NULL;
    pd.hDC = NULL;
    pd.Flags = PD_ALLPAGES | PD_COLLATE | PD_RETURNDC | PD_USEDEVMODECOPIESANDCOLLATE | PD_NOSELECTION | PD_HIDEPRINTTOFILE | PD_NONETWORKBUTTON;
    pd.nFromPage = 0;
    pd.nToPage = 0;
    pd.nMinPage = 0;
    pd.nMaxPage = 0;
    pd.nCopies = 1;
    pd.hInstance = NULL;
    pd.lCustData = 0L;
    pd.lpfnPrintHook = NULL;
    pd.lpfnSetupHook = NULL;
    pd.lpPrintTemplateName = NULL;
    pd.lpSetupTemplateName = NULL;
    pd.hPrintTemplate = NULL;
    pd.hSetupTemplate = NULL;
    if (PrintDlg (&pd) == FALSE)
    {
        wsprintf(PrinterName, "%s", "EMPTY");
        PrinterOrientation = 'p';
        PrinterHDC = NULL;
        bOK = FALSE;
    }
    else
    {
        LPDEVNAMES lpDev = (LPDEVNAMES)GlobalLock(pd.hDevNames);
        LPDEVMODE lpDevMode = (LPDEVMODE)GlobalLock(pd.hDevMode);
        LPCTSTR lpszDevice = (LPCTSTR)lpDev + lpDev->wDeviceOffset;

        wsprintf(PrinterName, "%s", lpszDevice);
        GlobalUnlock(pd.hDevNames);

        if (lpDevMode->dmOrientation == DMORIENT_PORTRAIT )
        {
            PrinterOrientation = 'p';
        }
        else
        {
            PrinterOrientation = 'l';
        }
        GlobalUnlock(pd.hDevMode);

        if (PrinterHDC)
        {
            DeleteDC(PrinterHDC);
            PrinterHDC = NULL;
        }
        PrinterHDC = pd.hDC;
        bOK = TRUE;
    }

    return bOK;
}
/*--------------------------------------------------------------------------*/
HDC GetPrinterDC(void)
{
    if (PrinterHDC == NULL)
    {
        PRINTDLG pd;
        LPDEVNAMES lpDev = NULL;
        LPDEVMODE lpDevMode = NULL;
        LPCTSTR lpszDevice = NULL;

        int   failed = 0;

        // Reset printdlg struct
        memset( &pd, 0, sizeof(PRINTDLG) );
        pd.Flags = PD_RETURNDEFAULT | PD_RETURNDC;
        pd.lStructSize = sizeof( PRINTDLG );


        failed = ( ! PrintDlg( &pd ) );
        if (failed)
        {
            ConfigurePrinterDialogBox();
        }

        lpDev = (LPDEVNAMES)GlobalLock(pd.hDevNames);
        lpDevMode = (LPDEVMODE)GlobalLock(pd.hDevMode);
        lpszDevice = (LPCTSTR)lpDev + lpDev->wDeviceOffset;

        wsprintf(PrinterName, "%s", lpszDevice);
        GlobalUnlock(pd.hDevNames);

        if (lpDevMode->dmOrientation == DMORIENT_PORTRAIT )
        {
            PrinterOrientation = 'p';
        }
        else
        {
            PrinterOrientation = 'l';
        }
        GlobalUnlock(pd.hDevMode);

        if (PrinterHDC)
        {
            DeleteDC(PrinterHDC);
            PrinterHDC = NULL;
        }
        PrinterHDC = pd.hDC;

    }

    return PrinterHDC;
}
/*--------------------------------------------------------------------------*/
char GetPrinterOrientation(void)
{
    return PrinterOrientation;
}
/*--------------------------------------------------------------------------*/
char* GetPrinterName(void)
{
    char *ReturnPrinterName = NULL;

    ReturnPrinterName = MALLOC(strlen(PrinterName) * sizeof(char));
    wsprintf(ReturnPrinterName, "%s", PrinterName);

    return ReturnPrinterName;
}
/*--------------------------------------------------------------------------*/
