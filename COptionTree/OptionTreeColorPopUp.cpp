// COptionTree
//
// License
// -------
// This code is provided "as is" with no expressed or implied warranty.
//
// Feel free to modify and/or distribute this file, but
// do not remove the headers in place.
// 
// You may use this code in a commercial product with or without acknowledgement.
// However you may not sell this code or any modification of this code.
//
// I would appreciate a notification of any bugs discovered or 
// improvements that could be made to help the control grow for everyone.
//
// History:
// --------
//	See License.txt for full history ifnormation.
//
//
// Copyright (c) 1999-2002 
// Matthew R. Miller 
// mattrmiller@computersmarts.net
//
// 
// Acknowledgements:
// -----------------
// The COptionTree is inspired by code, ideas, and submissions from the following: 
//
// CPropTree v1.0 - Property Tree Control - By Scott Ramsay (http://www.thecodeproject.com/treectrl/proptree.asp)
// A color picker button - By James White (http://www.thecodeproject.com/miscctrl/colorbutton.asp)


#include "stdafx.h"
#include <math.h>
#include "OptionTreeColorPopUp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


ColorTableEntry COptionTreeColorPopUp::m_crColors[] = 
{
    { RGB(0x00, 0x00, 0x00),    _T("Black")             },
    { RGB(0xA5, 0x2A, 0x00),    _T("Brown")             },
    { RGB(0x00, 0x40, 0x40),    _T("Dark Olive Green")  },
    { RGB(0x00, 0x55, 0x00),    _T("Dark Green")        },
    { RGB(0x00, 0x00, 0x5E),    _T("Dark Teal")         },
    { RGB(0x00, 0x00, 0x8B),    _T("Dark Blue")         },
    { RGB(0x4B, 0x00, 0x82),    _T("Indigo")            },
    { RGB(0x28, 0x28, 0x28),    _T("Gray-80%")			},

    { RGB(0x8B, 0x00, 0x00),    _T("Dark Red")          },
    { RGB(0xFF, 0x68, 0x20),    _T("Orange")            },
    { RGB(0x8B, 0x8B, 0x00),    _T("Dark Yellow")       },
    { RGB(0x00, 0x93, 0x00),    _T("Green")             },
    { RGB(0x38, 0x8E, 0x8E),    _T("Teal")              },
    { RGB(0x00, 0x00, 0xFF),    _T("Blue")              },
    { RGB(0x7B, 0x7B, 0xC0),    _T("Blue-Gray")         },
    { RGB(0x66, 0x66, 0x66),    _T("Gray-50%")			},

    { RGB(0xFF, 0x00, 0x00),    _T("Red")               },
    { RGB(0xFF, 0xAD, 0x5B),    _T("Light Orange")      },
    { RGB(0x32, 0xCD, 0x32),    _T("Lime")              }, 
    { RGB(0x3C, 0xB3, 0x71),    _T("Sea Green")         },
    { RGB(0x7F, 0xFF, 0xD4),    _T("Aqua")              },
    { RGB(0x7D, 0x9E, 0xC0),    _T("Light Blue")        },
    { RGB(0x80, 0x00, 0x80),    _T("Violet")            },
    { RGB(0x7F, 0x7F, 0x7F),    _T("Gray-40%")			},

    { RGB(0xFF, 0xC0, 0xCB),    _T("Pink")              },
    { RGB(0xFF, 0xD7, 0x00),    _T("Gold")              },
    { RGB(0xFF, 0xFF, 0x00),    _T("Yellow")            },    
    { RGB(0x00, 0xFF, 0x00),    _T("Bright Green")      },
    { RGB(0x40, 0xE0, 0xD0),    _T("Turquoise")         },
    { RGB(0xC0, 0xFF, 0xFF),    _T("Skyblue")           },
    { RGB(0x48, 0x00, 0x48),    _T("Plum")              },
    { RGB(0xC0, 0xC0, 0xC0),    _T("Gray-25%")			},

    { RGB(0xFF, 0xE4, 0xE1),    _T("Rose")              },
    { RGB(0xD2, 0xB4, 0x8C),    _T("Tan")               },
    { RGB(0xFF, 0xFF, 0xE0),    _T("Light Yellow")      },
    { RGB(0x98, 0xFB, 0x98),    _T("Pale Green ")       },
    { RGB(0xAF, 0xEE, 0xEE),    _T("Pale Turquoise")    },
    { RGB(0x68, 0x83, 0x8B),    _T("Pale Blue")         },
    { RGB(0xE6, 0xE6, 0xFA),    _T("Lavender")          },
    { RGB(0xFF, 0xFF, 0xFF),    _T("White")             }
};

/////////////////////////////////////////////////////////////////////////////
// COptionTreeColorPopUp

COptionTreeColorPopUp::COptionTreeColorPopUp()
{
    // Initialize
	Initialize();
}

COptionTreeColorPopUp::COptionTreeColorPopUp(CPoint pPoint, COLORREF crColor, CWnd* pParentWnd, LPCTSTR szDefaultText, LPCTSTR szCustomText)
{
    // Initialize
	Initialize();

	// Set variables
    m_crColor = m_crInitialColor = crColor;
    m_wndParent = pParentWnd;
    m_strDefaultText = (szDefaultText)? szDefaultText : _T("");
    m_strCustomText  = (szCustomText)?  szCustomText  : _T("");

    // Create
	COptionTreeColorPopUp::Create(pPoint, m_crColor, pParentWnd, szDefaultText, szCustomText);
}

void COptionTreeColorPopUp::Initialize()
{
	// Declare variables
	NONCLIENTMETRICS ncm;
	LOGPALETTE* pLogPalette;
    struct 
	{
        LOGPALETTE    LogPalette;
        PALETTEENTRY  PalEntry[OT_CPU_MAXCOLORS];
    } pal;

    // Get number of colors
	m_nNumColors = sizeof(m_crColors)/sizeof(ColorTableEntry);
    ASSERT(m_nNumColors <= OT_CPU_MAXCOLORS);
    if (m_nNumColors > OT_CPU_MAXCOLORS)
	{
        m_nNumColors = OT_CPU_MAXCOLORS;
	}

	// Set variables
    m_nNumColumns = 0;
    m_nNumRows = 0;
    m_nBoxSize = 18;
    m_nMargin = ::GetSystemMetrics(SM_CXEDGE);
    m_nCurrentSel = OT_CPU_INVALIDCOLOR;
    m_nChosenColorSel = OT_CPU_INVALIDCOLOR;
    m_wndParent = NULL;
    m_crColor = m_crInitialColor = RGB(0, 0, 0);
    m_bChildWindowVisible = FALSE;

    // Make sure the color square is at least 5 x 5;
    if (m_nBoxSize - 2 * m_nMargin - 2 < 5) 
	{
		m_nBoxSize = 5 + 2 * m_nMargin + 2;
	}

    // Create the font
    ncm.cbSize = sizeof(NONCLIENTMETRICS);
    VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));
    m_fFont.CreateFontIndirect(&(ncm.lfMessageFont));

    // Create the palette
    pLogPalette = (LOGPALETTE*) &pal;
    pLogPalette->palVersion = 0x300;
    pLogPalette->palNumEntries = (WORD) m_nNumColors; 
    for (int i = 0; i < m_nNumColors; i++)
    {
        pLogPalette->palPalEntry[i].peRed   = GetRValue(m_crColors[i].crColor);
        pLogPalette->palPalEntry[i].peGreen = GetGValue(m_crColors[i].crColor);
        pLogPalette->palPalEntry[i].peBlue  = GetBValue(m_crColors[i].crColor);
        pLogPalette->palPalEntry[i].peFlags = 0;
    }
    m_plPalette.CreatePalette(pLogPalette);
}

COptionTreeColorPopUp::~COptionTreeColorPopUp()
{
    // Delete variables
	if (m_fFont.GetSafeHandle() != NULL)
	{
		m_fFont.DeleteObject();
	}
	if (m_plPalette.GetSafeHandle() != NULL)
	{
		m_plPalette.DeleteObject();
	}
}

BOOL COptionTreeColorPopUp::Create(CPoint pPoint, COLORREF crColor, CWnd* pParentWnd, LPCTSTR szDefaultText, LPCTSTR szCustomText)
{
    // Declare variables
	CString strClassName;
	
	// Verify window
	ASSERT(pParentWnd && ::IsWindow(pParentWnd->GetSafeHwnd()));
  
    // Se variables
	m_wndParent = pParentWnd;
    m_crColor = m_crInitialColor = crColor;

    // Get the class name and create the window
    strClassName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, 0, (HBRUSH) (COLOR_BTNFACE+1), 0);

	// Create window
    if (!CWnd::CreateEx(0, strClassName, _T(""), WS_POPUP | WS_VISIBLE, pPoint.x, pPoint.y, 100, 100, pParentWnd->GetSafeHwnd(), 0, NULL))
	{
        return FALSE;
	}

	// Show window
	ShowWindow(SW_SHOWNA);

    // Store the Custom text
    if (szCustomText != NULL) 
	{
        m_strCustomText = szCustomText;
	}

    // Store the Default Area text
    if (szDefaultText != NULL) 
	{
        m_strDefaultText = szDefaultText;
	}
        
    // Set the window size
    SetWindowSize();

    // Create the tooltips
    CreateToolTips();

    // Find which cell (if any) corresponds to the initial color
    FindCellFromColor(crColor);

    // Capture all mouse events for the life of this window
    SetCapture();

    return TRUE;
}

BEGIN_MESSAGE_MAP(COptionTreeColorPopUp, CWnd)
    //{{AFX_MSG_MAP(COptionTreeColorPopUp)
    ON_WM_NCDESTROY()
    ON_WM_LBUTTONUP()
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_WM_KEYDOWN()
    ON_WM_QUERYNEWPALETTE()
    ON_WM_PALETTECHANGED()
	ON_WM_KILLFOCUS()
	ON_WM_ACTIVATEAPP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionTreeColorPopUp message handlers

// For tooltips
BOOL COptionTreeColorPopUp::PreTranslateMessage(MSG* pMsg) 
{
	// Relay tooltip
	if (m_ttToolTip.GetSafeHwnd() != NULL)
	{
		m_ttToolTip.RelayEvent(pMsg);
	}

    // Sometimes if the picker loses focus it is never destroyed
    if (GetCapture()->GetSafeHwnd() != m_hWnd)
	{
        SetCapture(); 
	}

    return CWnd::PreTranslateMessage(pMsg);
}


void COptionTreeColorPopUp::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// Declare variable
	int nRow, nCol;
    
	// Get row
	nRow = GetRow(m_nCurrentSel);
	
	// Get column
	nCol = GetColumn(m_nCurrentSel);

    // Down
	if (nChar == VK_DOWN) 
    {
        if (nRow == OT_CPU_DEFAULTBOXVALUE) 
		{
            nRow = nCol = 0; 
		}
        else if (nRow == OT_CPU_CUSTOMBOXVALUE)
        {
            if (m_strDefaultText.GetLength())
			{
                nRow = nCol = OT_CPU_DEFAULTBOXVALUE;
			}
            else
			{
                nRow = nCol = 0;
			}
        }
        else
        {
            nRow++;

            if (GetIndex(nRow, nCol) < 0)
            {
                if (m_strCustomText.GetLength())
				{
                    nRow = nCol = OT_CPU_CUSTOMBOXVALUE;
				}
                else if (m_strDefaultText.GetLength())
				{
                    nRow = nCol = OT_CPU_DEFAULTBOXVALUE;
				}
                else
				{
                    nRow = nCol = 0;
				}
            }
        }

        ChangeSelection(GetIndex(nRow, nCol));
    }

	// Up
    if (nChar == VK_UP) 
    {
        if (nRow == OT_CPU_DEFAULTBOXVALUE)
        {
            if (m_strCustomText.GetLength())
			{
                nRow = nCol = OT_CPU_CUSTOMBOXVALUE;
			}
            else
			{ 
                nRow = GetRow(m_nNumColors-1); 
                nCol = GetColumn(m_nNumColors-1); 
            }
        }
        else if (nRow == OT_CPU_CUSTOMBOXVALUE)
        { 
            nRow = GetRow(m_nNumColors-1); 
            nCol = GetColumn(m_nNumColors-1); 
        }
        else if (nRow > 0) 
		{
			nRow--;
		}
        else
        {
            if (m_strDefaultText.GetLength())
			{
                nRow = nCol = OT_CPU_DEFAULTBOXVALUE;
			}
            else if (m_strCustomText.GetLength())
			{
                nRow = nCol = OT_CPU_CUSTOMBOXVALUE;
			}
            else
            { 
                nRow = GetRow(m_nNumColors-1); 
                nCol = GetColumn(m_nNumColors-1); 
            }
        }

        ChangeSelection(GetIndex(nRow, nCol));
    }

	// Right
    if (nChar == VK_RIGHT) 
    {
        if (nRow == OT_CPU_DEFAULTBOXVALUE) 
		{
            nRow = nCol = 0; 
		}
        else if (nRow == OT_CPU_CUSTOMBOXVALUE)
        {
            if (m_strDefaultText.GetLength())
			{
                nRow = nCol = OT_CPU_DEFAULTBOXVALUE;
			}
            else
			{
                nRow = nCol = 0;
			}
        }
        else if (nCol < m_nNumColumns - 1) 
		{
            nCol++;
		}
        else 
        { 
            nCol = 0; nRow++;
        }

        if (GetIndex(nRow,nCol) == OT_CPU_INVALIDCOLOR)
        {
            if (m_strCustomText.GetLength())
			{
                nRow = nCol = OT_CPU_CUSTOMBOXVALUE;
			}
            else if (m_strDefaultText.GetLength())
			{
                nRow = nCol = OT_CPU_DEFAULTBOXVALUE;
			}
            else
			{
                nRow = nCol = 0;
			}
        }

        ChangeSelection(GetIndex(nRow, nCol));
    }

	// Left
    if (nChar == VK_LEFT) 
    {
        if (nRow == OT_CPU_DEFAULTBOXVALUE)
        {
            if (m_strCustomText.GetLength())
			{
                nRow = nCol = OT_CPU_CUSTOMBOXVALUE;
			}
            else
			{ 
                nRow = GetRow(m_nNumColors-1); 
                nCol = GetColumn(m_nNumColors-1); 
            }
        }
        else if (nRow == OT_CPU_CUSTOMBOXVALUE)
        { 
            nRow = GetRow(m_nNumColors-1); 
            nCol = GetColumn(m_nNumColors-1); 
        }
        else if (nCol > 0) 
		{
			nCol--;
		}
        else
        {
            if (nRow > 0) 
			{ 
				nRow--; 
				nCol = m_nNumColumns - 1; 
			}
            else 
            {
                if (m_strDefaultText.GetLength())
				{
                    nRow = nCol = OT_CPU_DEFAULTBOXVALUE;
				}
                else if (m_strCustomText.GetLength())
				{
                    nRow = nCol = OT_CPU_CUSTOMBOXVALUE;
				}
                else
                { 
                    nRow = GetRow(m_nNumColors - 1); 
                    nCol = GetColumn(m_nNumColors - 1); 
                }
            }
        }

        ChangeSelection(GetIndex(nRow, nCol));
    }

	// Escape
    if (nChar == VK_ESCAPE) 
    {
        m_crColor = m_crInitialColor;

        EndSelection(OT_CPU_SELENDCANCEL);

        return;
    }

	// Return or space
    if (nChar == VK_RETURN || nChar == VK_SPACE)
    {
        EndSelection(OT_CPU_SELENDOK);

        return;
    }

    CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

// auto-deletion
void COptionTreeColorPopUp::OnNcDestroy() 
{
    CWnd::OnNcDestroy();

    delete this;
}

void COptionTreeColorPopUp::OnPaint() 
{
    // Declare variables
	CPaintDC dc(this);
	CRect rcClient;

	// Get client rect
	GetClientRect(rcClient);

    // Draw the Default Area text
    if (m_strDefaultText.GetLength())
	{
        DrawCell(&dc, OT_CPU_DEFAULTBOXVALUE);
	}
 
    // Draw color cells
    for (int i = 0; i < m_nNumColors; i++)
	{
        DrawCell(&dc, i);
	}
    
    // Draw custom text
    if (m_strCustomText.GetLength())
	{
        DrawCell(&dc, OT_CPU_CUSTOMBOXVALUE);
	}

    // Draw raised window edge (ex-window style WS_EX_WINDOWEDGE is sposed to do this, but for some reason isn't
    dc.DrawEdge(rcClient, EDGE_RAISED, BF_RECT);
}

void COptionTreeColorPopUp::OnMouseMove(UINT nFlags, CPoint point) 
{
	// Declare variables
    int nNewSelection = OT_CPU_INVALIDCOLOR;

    // Translate points to be relative raised window edge
    point.x -= m_nMargin;
    point.y -= m_nMargin;

    // First check we aren't in text box
    if (m_strCustomText.GetLength() && m_rcCustomTextRect.PtInRect(point))
	{
        nNewSelection = OT_CPU_CUSTOMBOXVALUE;
	}
    else if (m_strDefaultText.GetLength() && m_rcDefaultTextRect.PtInRect(point))
	{
        nNewSelection = OT_CPU_DEFAULTBOXVALUE;
	}
    else
    {
        // -- Take into account text box
        if (m_strDefaultText.GetLength()) 
		{
            point.y -= m_rcDefaultTextRect.Height();  
		}

        // -- Get the row and column
        nNewSelection = GetIndex(point.y / m_nBoxSize, point.x / m_nBoxSize);

        // -- In range? If not, default and exit
        if (nNewSelection < 0 || nNewSelection >= m_nNumColors)
        {
            CWnd::OnMouseMove(nFlags, point);

            return;
        }
    }

    // OK - we have the row and column of the current selection (may be OT_CPU_CUSTOMBOXVALUE)
    // Has the row/col selection changed? If yes, then redraw old and new cells.
    if (nNewSelection != m_nCurrentSel)
	{
        ChangeSelection(nNewSelection);
	}

    CWnd::OnMouseMove(nFlags, point);
}

void COptionTreeColorPopUp::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// Declare variables
	DWORD dwPos;

    // Get osition
	dwPos = GetMessagePos();
    point = CPoint(LOWORD(dwPos), HIWORD(dwPos));

    if (m_rcWindowRect.PtInRect(point))
	{
        EndSelection(OT_CPU_SELENDOK);
	}
    else
	{
        EndSelection(OT_CPU_SELENDCANCEL);
	}

	// Handle left click
	if (::IsWindow(GetSafeHwnd()))
	{
		CWnd::OnLButtonUp(nFlags, point);
	}
}

int COptionTreeColorPopUp::GetIndex(int nRow, int nCol) const
{ 
	// Get index
    if ((nRow == OT_CPU_CUSTOMBOXVALUE || nCol == OT_CPU_CUSTOMBOXVALUE) && m_strCustomText.GetLength())
	{
        return OT_CPU_CUSTOMBOXVALUE;
	}
    else if ((nRow == OT_CPU_DEFAULTBOXVALUE || nCol == OT_CPU_DEFAULTBOXVALUE) && m_strDefaultText.GetLength())
	{
        return OT_CPU_DEFAULTBOXVALUE;
	}
    else if (nRow < 0 || nCol < 0 || nRow >= m_nNumRows || nCol >= m_nNumColumns)
	{
        return OT_CPU_INVALIDCOLOR;
	}
    else
    {
        if (nRow * m_nNumColumns + nCol >= m_nNumColors)
		{
            return OT_CPU_INVALIDCOLOR;
		}
        else
		{
            return nRow * m_nNumColumns + nCol;
		}
    }
}

int COptionTreeColorPopUp::GetRow(int nIndex) const               
{ 
	// Get row
    if (nIndex == OT_CPU_CUSTOMBOXVALUE && m_strCustomText.GetLength())
	{
        return OT_CPU_CUSTOMBOXVALUE;
	}
    else if (nIndex == OT_CPU_DEFAULTBOXVALUE && m_strDefaultText.GetLength())
	{
        return OT_CPU_DEFAULTBOXVALUE;
	}
    else if (nIndex < 0 || nIndex >= m_nNumColors)
	{
        return OT_CPU_INVALIDCOLOR;
	}
    else
	{
        return nIndex / m_nNumColumns; 
	}
}

int COptionTreeColorPopUp::GetColumn(int nIndex) const            
{ 
	// Get column
    if (nIndex == OT_CPU_CUSTOMBOXVALUE && m_strCustomText.GetLength())
	{
        return OT_CPU_CUSTOMBOXVALUE;
	}
    else if (nIndex == OT_CPU_DEFAULTBOXVALUE && m_strDefaultText.GetLength())
	{
        return OT_CPU_DEFAULTBOXVALUE;
	}
    else if (nIndex < 0 || nIndex >= m_nNumColors)
	{
        return OT_CPU_INVALIDCOLOR;
	}
    else
	{
        return nIndex % m_nNumColumns; 
	}
}

void COptionTreeColorPopUp::FindCellFromColor(COLORREF crColor)
{
	// Find cell
    if (crColor == CLR_DEFAULT && m_strDefaultText.GetLength())
    {
        m_nChosenColorSel = OT_CPU_DEFAULTBOXVALUE;
        return;
    }

    for (int i = 0; i < m_nNumColors; i++)
    {
        if (GetColor(i) == crColor)
        {
            m_nChosenColorSel = i;
            return;
        }
    }

    if (m_strCustomText.GetLength())
	{
        m_nChosenColorSel = OT_CPU_CUSTOMBOXVALUE;
	}
    else
	{
        m_nChosenColorSel = OT_CPU_INVALIDCOLOR;
	}
}


BOOL COptionTreeColorPopUp::GetCellRect(int nIndex, const LPRECT& rect)
{
	// Get cell rect
    if (nIndex == OT_CPU_CUSTOMBOXVALUE)
    {
        ::SetRect(rect, m_rcCustomTextRect.left,  m_rcCustomTextRect.top, m_rcCustomTextRect.right, m_rcCustomTextRect.bottom);
		
		return TRUE;
    }
    else if (nIndex == OT_CPU_DEFAULTBOXVALUE)
    {
        ::SetRect(rect, m_rcDefaultTextRect.left,  m_rcDefaultTextRect.top, m_rcDefaultTextRect.right, m_rcDefaultTextRect.bottom);

        return TRUE;
    }

    if (nIndex < 0 || nIndex >= m_nNumColors)
	{
        return FALSE;
	}

    rect->left = GetColumn(nIndex) * m_nBoxSize + m_nMargin;
    rect->top  = GetRow(nIndex) * m_nBoxSize + m_nMargin;

    // Move everything down if we are displaying a default text area
    if (m_strDefaultText.GetLength()) 
	{
        rect->top += (m_nMargin + m_rcDefaultTextRect.Height());
	}

    rect->right = rect->left + m_nBoxSize;
    rect->bottom = rect->top + m_nBoxSize;

    return TRUE;
}

void COptionTreeColorPopUp::SetWindowSize()
{
    // Declare variables
	CSize TextSize;

    // If we are showing a custom or default text area, get the font and text size.
    if (m_strCustomText.GetLength() || m_strDefaultText.GetLength())
    {
        CClientDC dc(this);
        CFont* pOldFont = (CFont*) dc.SelectObject(&m_fFont);

        // -- Get the size of the custom text (if there IS custom text)
        TextSize = CSize(0,0);
        if (m_strCustomText.GetLength())
            TextSize = dc.GetTextExtent(m_strCustomText);

        // -- Get the size of the default text (if there IS default text)
        if (m_strDefaultText.GetLength())
        {
            CSize DefaultSize = dc.GetTextExtent(m_strDefaultText);
            if (DefaultSize.cx > TextSize.cx) TextSize.cx = DefaultSize.cx;
            if (DefaultSize.cy > TextSize.cy) TextSize.cy = DefaultSize.cy;
        }

        dc.SelectObject(pOldFont);
        TextSize += CSize(2*m_nMargin,2*m_nMargin);

        // -- Add even more space to draw the horizontal line
        TextSize.cy += 2*m_nMargin + 2;
    }

    // Get the number of columns and rows
    m_nNumColumns = 8;
    m_nNumRows = m_nNumColors / m_nNumColumns;
    if (m_nNumColors % m_nNumColumns) 
	{
		m_nNumRows++;
	}

    // Get the current window position, and set the new size
    CRect rect;
    GetWindowRect(rect);

    m_rcWindowRect.SetRect(rect.left, rect.top, rect.left + m_nNumColumns*m_nBoxSize + 2*m_nMargin, rect.top  + m_nNumRows*m_nBoxSize + 2*m_nMargin);

    // If custom text, then expand window if necessary, and set text width as
    // window width
    if (m_strDefaultText.GetLength()) 
    {
        if (TextSize.cx > m_rcWindowRect.Width())
		{
            m_rcWindowRect.right = m_rcWindowRect.left + TextSize.cx;
		}
        TextSize.cx = m_rcWindowRect.Width()-2*m_nMargin;

        // -- Work out the text area
        m_rcDefaultTextRect.SetRect(m_nMargin, m_nMargin, m_nMargin+TextSize.cx, 2*m_nMargin+TextSize.cy);
        m_rcWindowRect.bottom += m_rcDefaultTextRect.Height() + 2*m_nMargin;
    }

    // If custom text, then expand window if necessary, and set text width as
    // window width
    if (m_strCustomText.GetLength()) 
    {
        if (TextSize.cx > m_rcWindowRect.Width())
		{
            m_rcWindowRect.right = m_rcWindowRect.left + TextSize.cx;
		}
        TextSize.cx = m_rcWindowRect.Width()-2*m_nMargin;

        // -- Work out the text area
        m_rcCustomTextRect.SetRect(m_nMargin, m_rcWindowRect.Height(), m_nMargin+TextSize.cx, m_rcWindowRect.Height()+m_nMargin+TextSize.cy);
		m_rcWindowRect.bottom += m_rcCustomTextRect.Height() + 2*m_nMargin;
   }

    // Need to check it'll fit on screen: Too far right?
    CSize ScreenSize(::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));
    if (m_rcWindowRect.right > ScreenSize.cx)
	{
        m_rcWindowRect.OffsetRect(-(m_rcWindowRect.right - ScreenSize.cx), 0);
	}

    // Too far left?
    if (m_rcWindowRect.left < 0)
	{
        m_rcWindowRect.OffsetRect( -m_rcWindowRect.left, 0);
	}

    // Bottom falling out of screen?
    if (m_rcWindowRect.bottom > ScreenSize.cy)
    {
        CRect rcParentRect;
        m_wndParent->GetWindowRect(rcParentRect);
        m_rcWindowRect.OffsetRect(0, -(rcParentRect.Height() + m_rcWindowRect.Height()));
    }

    // Set the window size and position
    MoveWindow(m_rcWindowRect, TRUE);
}

void COptionTreeColorPopUp::CreateToolTips()
{
    // Create the tool tip
    if (!m_ttToolTip.Create(this)) 
	{
		return;
	}

    // Add a tool for each cell
    for (int i = 0; i < m_nNumColors; i++)
    {
        CRect rect;
        if (!GetCellRect(i, rect)) 
		{
			continue;
		}
		m_ttToolTip.AddTool(this, GetColorName(i), rect, 1);
    }

	// Create inactive
	m_ttToolTip.Activate(TRUE);
}

void COptionTreeColorPopUp::ChangeSelection(int nIndex)
{
    CClientDC dc(this);

    if (nIndex > m_nNumColors)
	{
        nIndex = OT_CPU_CUSTOMBOXVALUE; 
	}

    if ((m_nCurrentSel >= 0 && m_nCurrentSel < m_nNumColors) || m_nCurrentSel == OT_CPU_CUSTOMBOXVALUE || m_nCurrentSel == OT_CPU_DEFAULTBOXVALUE)
    {
        // -- Set Current selection as invalid and redraw old selection (this way
        // the old selection will be drawn unselected)
        int OldSel = m_nCurrentSel;
        m_nCurrentSel = OT_CPU_INVALIDCOLOR;
        DrawCell(&dc, OldSel);
    }

    // Set the current selection as row/col and draw (it will be drawn selected)
    m_nCurrentSel = nIndex;
    DrawCell(&dc, m_nCurrentSel);

    // Store the current color
    if (m_nCurrentSel == OT_CPU_CUSTOMBOXVALUE)
	{
        m_wndParent->SendMessage(OT_CPU_SELCHANGE, (WPARAM) m_crInitialColor, 0);
	}
    else if (m_nCurrentSel == OT_CPU_DEFAULTBOXVALUE)
    {
        m_crColor = CLR_DEFAULT;
        m_wndParent->SendMessage(OT_CPU_SELCHANGE, (WPARAM) CLR_DEFAULT, 0);
    }
    else
    {
        m_crColor = GetColor(m_nCurrentSel);
        m_wndParent->SendMessage(OT_CPU_SELCHANGE, (WPARAM) m_crColor, 0);
    }
}

void COptionTreeColorPopUp::EndSelection(int nMessage)
{
	// Release capture
    ReleaseCapture();

    // If custom text selected, perform a custom color selection
    if (nMessage != OT_CPU_SELENDCANCEL && m_nCurrentSel == OT_CPU_CUSTOMBOXVALUE)
    {
        m_bChildWindowVisible = TRUE;

        CColorDialog dlg(m_crInitialColor, CC_FULLOPEN | CC_ANYCOLOR, this);

        if (dlg.DoModal() == IDOK)
		{
            m_crColor = dlg.GetColor();
		}
        else
		{
            nMessage = OT_CPU_SELENDCANCEL;
		}

        m_bChildWindowVisible = FALSE;
    } 

	// Send message
    m_wndParent->SendMessage(nMessage, (WPARAM) m_crColor, 0);

	// Get initial color
    if (nMessage == OT_CPU_SELENDCANCEL)
	{
        m_crColor = m_crInitialColor;
	}
    
    // Kill focus
    if (!m_bChildWindowVisible)
	{
        DestroyWindow();
	}
}

void COptionTreeColorPopUp::DrawCell(CDC* pDC, int nIndex)
{
    // For the Custom Text area
    if (m_strCustomText.GetLength() && nIndex == OT_CPU_CUSTOMBOXVALUE)
    {
        // -- The extent of the actual text button
        CRect TextButtonRect = m_rcCustomTextRect;
        TextButtonRect.top += 2*m_nMargin;

        // -- Fill background
        pDC->FillSolidRect(TextButtonRect, ::GetSysColor(COLOR_3DFACE));

        // -- Draw horizontal line
        pDC->FillSolidRect(m_rcCustomTextRect.left+2*m_nMargin, m_rcCustomTextRect.top,
                           m_rcCustomTextRect.Width()-4*m_nMargin, 1, ::GetSysColor(COLOR_3DSHADOW));
        pDC->FillSolidRect(m_rcCustomTextRect.left+2*m_nMargin, m_rcCustomTextRect.top+1,
                           m_rcCustomTextRect.Width()-4*m_nMargin, 1, ::GetSysColor(COLOR_3DHILIGHT));

        TextButtonRect.DeflateRect(1,1);

        // -- Fill background
        if (m_nChosenColorSel == nIndex && m_nCurrentSel != nIndex)
		{
            pDC->FillSolidRect(TextButtonRect, ::GetSysColor(COLOR_3DLIGHT));
		}
        else
		{
            pDC->FillSolidRect(TextButtonRect, ::GetSysColor(COLOR_3DFACE));
		}

        // -- Draw button
        if (m_nCurrentSel == nIndex) 
		{
            pDC->DrawEdge(TextButtonRect, BDR_RAISEDINNER, BF_RECT);
		}
        else if (m_nChosenColorSel == nIndex)
		{
            //pDC->DrawEdge(TextButtonRect, BDR_SUNKENOUTER, BF_RECT);
		}

        // -- Draw custom text
        CFont *pOldFont = (CFont*) pDC->SelectObject(&m_fFont);
        pDC->SetBkMode(TRANSPARENT);
        pDC->DrawText(m_strCustomText, TextButtonRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        pDC->SelectObject(pOldFont);

        return;
    }        

    // For the Default Text area
    if (m_strDefaultText.GetLength() && nIndex == OT_CPU_DEFAULTBOXVALUE)
    {
        // -- Fill background
        pDC->FillSolidRect(m_rcDefaultTextRect, ::GetSysColor(COLOR_3DFACE));

        // -- The extent of the actual text button
        CRect TextButtonRect = m_rcDefaultTextRect;
        TextButtonRect.DeflateRect(1,1);

        // -- Fill background
        if (m_nChosenColorSel == nIndex && m_nCurrentSel != nIndex)
		{
            pDC->FillSolidRect(TextButtonRect, ::GetSysColor(COLOR_3DLIGHT));
		}
        else
		{
            pDC->FillSolidRect(TextButtonRect, ::GetSysColor(COLOR_3DFACE));
		}

        // -- Draw thin line around text
        CRect LineRect = TextButtonRect;
        LineRect.DeflateRect(2*m_nMargin,2*m_nMargin);
        CPen pen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
        CPen* pOldPen = pDC->SelectObject(&pen);
        pDC->SelectStockObject(NULL_BRUSH);
        pDC->Rectangle(LineRect);
        pDC->SelectObject(pOldPen);

        // -- Draw button
        if (m_nCurrentSel == nIndex) 
		{
            pDC->DrawEdge(TextButtonRect, BDR_RAISEDINNER, BF_RECT);
		}
        else if (m_nChosenColorSel == nIndex)
		{
            pDC->DrawEdge(TextButtonRect, BDR_SUNKENOUTER, BF_RECT);
		}

        // -- Draw custom text
        CFont *pOldFont = (CFont*) pDC->SelectObject(&m_fFont);
        pDC->SetBkMode(TRANSPARENT);
        pDC->DrawText(m_strDefaultText, TextButtonRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        pDC->SelectObject(pOldFont);

        return;
    }        

    CRect rect;
    if (!GetCellRect(nIndex, rect)) return;

    // -- Select and realize the palette
    CPalette* pOldPalette = NULL;
    if (pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
    {
        pOldPalette = pDC->SelectPalette(&m_plPalette, FALSE);
        pDC->RealizePalette();
    }

    // -- Fill background
    if (m_nChosenColorSel == nIndex && m_nCurrentSel != nIndex)
	{
        pDC->FillSolidRect(rect, ::GetSysColor(COLOR_3DHILIGHT));
	}
    else
        pDC->FillSolidRect(rect, ::GetSysColor(COLOR_3DFACE));

    // Draw button
    if (m_nCurrentSel == nIndex) 
	{
        pDC->DrawEdge(rect, BDR_RAISEDINNER, BF_RECT);
	}
    else if (m_nChosenColorSel == nIndex)
	{
        pDC->DrawEdge(rect, BDR_SUNKENOUTER, BF_RECT);
	}

    CBrush brush(PALETTERGB(GetRValue(GetColor(nIndex)), 
                            GetGValue(GetColor(nIndex)), 
                            GetBValue(GetColor(nIndex)) ));
    CPen   pen;
    pen.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));

    CBrush* pOldBrush = (CBrush*) pDC->SelectObject(&brush);
    CPen*   pOldPen   = (CPen*)   pDC->SelectObject(&pen);

    // Draw the cell color
    rect.DeflateRect(m_nMargin+1, m_nMargin+1);
    pDC->Rectangle(rect);

    // restore DC and cleanup
    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
    brush.DeleteObject();
    pen.DeleteObject();

    if (pOldPalette && pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
	{
        pDC->SelectPalette(pOldPalette, FALSE);
	}
}

BOOL COptionTreeColorPopUp::OnQueryNewPalette() 
{
    // Force redraw
	Invalidate();    


    return CWnd::OnQueryNewPalette();
}

void COptionTreeColorPopUp::OnPaletteChanged(CWnd* pFocusWnd) 
{
    if (pFocusWnd->GetSafeHwnd() != GetSafeHwnd())
	{
        Invalidate();
	}

	CWnd::OnPaletteChanged(pFocusWnd);
}

void COptionTreeColorPopUp::OnKillFocus(CWnd* pNewWnd) 
{
	// Release capture
    ReleaseCapture();
    
	CWnd::OnKillFocus(pNewWnd);
}


void COptionTreeColorPopUp::OnActivateApp(BOOL bActive, HTASK hTask) 
{
	// If Deactivating App, cancel this selection
	if (!bActive)
	{
		 EndSelection(OT_CPU_SELENDCANCEL);
	}

	CWnd::OnActivateApp(bActive, hTask);
}
