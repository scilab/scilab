/** missing defines in gcwin32 **/

#ifndef GNUC_MISS
#define GNUC_MISS

#ifdef __GNUC__XXX
/**** XXXX  was necessary with b17 
typedef  METAFILEPICT *LPMETAFILEPICT;
***/ 
#define  SelectFont(hdc, hfont)  ((HFONT)SelectObject((hdc), (HGDIOBJ)(HFONT)(hfont)))
/* tmPitchAndFamily flags */
#define TMPF_FIXED_PITCH    0x01
#define TMPF_VECTOR             0x02
#define TMPF_DEVICE             0x08
#define TMPF_TRUETYPE       0x04
typedef LOGPEN *LPLOGPEN;
/**** XXXX  was necessary with b17 
typedef DRAWITEMSTRUCT *LPDRAWITEMSTRUCT;
*******/
#define HFILE_ERROR ((HFILE)-1)
#define     GetStockFont(i)      ((HFONT)GetStockObject(i))
#define     DeletePen(hpen)      DeleteObject((HGDIOBJ)(HPEN)(hpen))
#define     SelectPen(hdc, hpen)    ((HPEN)SelectObject((hdc), (HGDIOBJ)(HPEN)(hpen)))
#define     GetStockPen(i)       ((HPEN)GetStockObject(i))
#define     DeleteBrush(hbr)     DeleteObject((HGDIOBJ)(HBRUSH)(hbr))
#define     SelectBrush(hdc, hbr)   ((HBRUSH)SelectObject((hdc), (HGDIOBJ)(HBRUSH)(hbr)))
#define     GetStockBrush(i)     ((HBRUSH)GetStockObject(i))
#define     DeleteFont(hfont)            DeleteObject((HGDIOBJ)(HFONT)(hfont))
#define     SelectBitmap(hdc, hbm)  ((HBITMAP)SelectObject((hdc), (HGDIOBJ)(HBITMAP)(hbm)))

#endif /**  __GNUC__ **/
#endif /**  GNUC_MISS **/
