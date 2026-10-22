#include "menuinc.h"
#include "LISTVIEW_Private.h"

static void _DrawRoundedRect(int x0, int y0, int x1, int y1, int r, int wipe_xa, int wipe_xb)
{
#if defined(GUI_OPTIMIZE_ROUNDING)

    I32 rr;
    int y, yt, yb, x, xs, xe, l, xOld, MinSize;

    MinSize = ((y1 - y0 + 1) < (x1 - x0 + 1)) ? y1 - y0 + 1 : x1 - x0 + 1;

    if (MinSize < ((r - 2) << 1)) {
        r = (MinSize >> 1) + 1;
    }

    rr = (I32)r * r;
    xOld = -1;

    if (r > 0) {
        for (y = r - 1; y; y--) {
            x = GUI__sqrt32(rr - (I32)y * y);
            xs = x0 + r - x - 1;
            xe = x1 - r + x + 1;
            yt = y0 + (r - y) - 1;
            yb = y1 - (r - y) + 1;
            l = x - xOld;

            if (l > 1) {
                LCD_DrawHLine(xs,         yt, xs + l - 1);
                LCD_DrawHLine(xe - l + 1, yt, xe        );
                LCD_DrawHLine(xs,         yb, xs + l - 1);
                LCD_DrawHLine(xe - l + 1, yb, xe        );
            } else {
                LCD_DrawPixel(xs, yt);
                LCD_DrawPixel(xe, yt);
                LCD_DrawPixel(xs, yb);
                LCD_DrawPixel(xe, yb);
            }

            xOld = x;
        }
    }

    LCD_DrawHLine(x0 + r, y0, wipe_xa);
    LCD_DrawHLine(wipe_xb, y0, x1 - r);
    LCD_DrawHLine(x0 + r, y1, x1 - r);
    LCD_DrawVLine(x0, y0 + r - 1, y1 - r + 1);
    LCD_DrawVLine(x1, y0 + r - 1, y1 - r + 1);

#else

    I32 rr;
    int i, y, ye, x, xs, xe, l, xOld;

    if (((y1 - y0 + 1) >= (r << 1)) && ((x1 - x0 + 1) >= (r << 1))) {
        rr = (I32)r * r;
        y  = r;
        xOld = -1;

        for (i = y0; i <= (y0 + r); i++, y--, xOld = x) {
            x = GUI__sqrt32(rr - (I32)y * y);
            xs = x0 + r - x;
            xe = x1 - r + x;
            ye = y1 - (i - y0);
            l = x - xOld;

            if (l > 1) {
                LCD_DrawHLine(xs,          i, xs + l - 1);
                LCD_DrawHLine(xe - l + 1,  i, xe        );
                LCD_DrawHLine(xs,         ye, xs + l - 1);
                LCD_DrawHLine(xe - l + 1, ye, xe        );
            } else {
                LCD_DrawPixel(xs,  i);
                LCD_DrawPixel(xe,  i);
                LCD_DrawPixel(xs, ye);
                LCD_DrawPixel(xe, ye);
            }
        }

        LCD_DrawHLine(x0 + r + 1, y0, wipe_xa);
        LCD_DrawHLine(wipe_xb, y0, x1 - r - 1);

        LCD_DrawHLine(x0 + r + 1, y1, x1 - r - 1);
        LCD_DrawVLine(x0, y0 + r + 1, y1 - r - 1);
        LCD_DrawVLine(x1, y0 + r + 1, y1 - r - 1);
    }

#endif
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_DrawRoundedRect
*/


static LISTVIEW_CELL_INFO * _GetpCellInfo(LISTVIEW_Handle hObj, unsigned Column, unsigned Row, unsigned int Index) {
  LISTVIEW_CELL_INFO * pCellInfo = 0;
  if (hObj) {
    GUI_ARRAY RowArray;
    GUI_ARRAY CellArray;
    unsigned NumColumns, NumRows;
    LISTVIEW_Obj * pObj;
    LISTVIEW_ROW  * pRow;
    LISTVIEW_CELL * pCell;
    WM_HMEM hCellInfo;
    int i;

    pObj = LISTVIEW_LOCK_H(hObj);
    NumColumns = LISTVIEW__GetNumColumns(pObj);
    NumRows    = LISTVIEW__GetNumRows(pObj);
    RowArray   = pObj->hRowArray;
    GUI_UNLOCK_H(pObj);
    if (Index < GUI_COUNTOF(pCellInfo->aTextColor)) {
      if ((Column < NumColumns) && (Row < NumRows)) {
        pRow  = (LISTVIEW_ROW  *)GUI_ARRAY_GetpItemLocked(RowArray,  Row);
        if (pRow) {
          CellArray = pRow->hCellArray;
          GUI_UNLOCK_H(pRow);
          pCell = (LISTVIEW_CELL *)GUI_ARRAY_GetpItemLocked(CellArray, Column);
          if (pCell) {
            hCellInfo = pCell->hCellInfo;
            GUI_UNLOCK_H(pCell);
            if (hCellInfo == 0) {
              hCellInfo = GUI_ALLOC_AllocZero(sizeof(LISTVIEW_CELL_INFO));
              if (hCellInfo) {
                pCell = (LISTVIEW_CELL *)GUI_ARRAY_GetpItemLocked(CellArray, Column);
                pCell->hCellInfo = hCellInfo;
                GUI_UNLOCK_H(pCell);
                pCellInfo = (LISTVIEW_CELL_INFO *)GUI_LOCK_H(hCellInfo);
                for (i = 0; i < GUI_COUNTOF(pCellInfo->aTextColor); i++) {
                  pCellInfo->aTextColor[i] = LISTVIEW_GetTextColor(hObj, i);
                  pCellInfo->aBkColor[i]   = LISTVIEW_GetBkColor  (hObj, i);
                }
              }
            } else {
              pCellInfo = (LISTVIEW_CELL_INFO *)GUI_LOCK_H(hCellInfo);
            }
          }
        }
      }
    }
  }
  return pCellInfo;
}

void My_DrawRoundedRect(int x0, int y0, int x1, int y1, int r, int wipe_xa, int wipe_xb)
{
#if (GUI_WINSUPPORT)
    GUI_RECT Rect;
#endif
    GUI_LOCK();
#if (GUI_WINSUPPORT)
    WM_ADDORG(x0, y0);
    WM_ADDORG(x1, y1);
    Rect.x0 = x0;
    Rect.x1 = x1;
    Rect.y0 = y0;
    Rect.y1 = y1;
    WM_ITERATE_START(&Rect);
    {
#endif
        _DrawRoundedRect(x0, y0, x1, y1, r, wipe_xa, wipe_xb);
#if (GUI_WINSUPPORT)
    }
    WM_ITERATE_END();
#endif
    GUI_UNLOCK();
}


void MyLISTVIEW__InvalidateRow(LISTVIEW_Handle hObj, LISTVIEW_Obj * pObj, int Sel, int Cnum) {
	
  const GUI_FONT * FontType = LISTVIEW_GetFont(hObj);
  unsigned char FontSize = FontType->YSize / 2;
	  
  if (Sel >= 0) {
    GUI_RECT Rect;
    int HeaderHeight, RowDistY;
    HeaderHeight = HEADER_GetHeight(pObj->hHeader);
    RowDistY     = LISTVIEW__GetRowDistY(pObj);
    WM_GetInsideRectExScrollbar(hObj, &Rect);
    Rect.y0 += HeaderHeight + (Sel - pObj->ScrollStateV.v) * RowDistY;
    Rect.y1  = Rect.y0 + RowDistY - 1;

	Rect.x0 += FontSize * Cnum;
    WM_InvalidateRect(hObj, &Rect);
  }
}

void MyLISTVIEW_SetItemTextColor(LISTVIEW_Handle hObj, unsigned Column, unsigned Row, unsigned int Index, GUI_COLOR Color, int Cnum) {
  LISTVIEW_Obj * pObj;
  LISTVIEW_CELL_INFO * pCellInfo;
  	
  if (hObj) {
    WM_LOCK();
    pCellInfo = _GetpCellInfo(hObj, Column, Row, Index);
    if (pCellInfo) {
      pObj = LISTVIEW_LOCK_H(hObj);
      pCellInfo->aTextColor[Index] = Color;
      pCellInfo->Flags |= LISTVIEW_CELL_INFO_COLORS;
      MyLISTVIEW__InvalidateRow(hObj, pObj, Row, Cnum);
      GUI_UNLOCK_H(pObj);
    }
    if (pCellInfo) {
      GUI_UNLOCK_H(pCellInfo);
    }
    WM_UNLOCK();
  }
}


