/*****************************************************************************
    COPYRIGHT (C) 2000, Ken Bertelson <kenbertelson@hotmail.com>


*****************************************************************************/
#include "stdafx.h"
#include "TreeColumn.h"

#include "GridTreeCell.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTreeColumn::CTreeColumn()
{
    // tree cell data initialization
    m_nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
    m_crBkClr = CLR_DEFAULT;
    m_crFgClr = CLR_DEFAULT;
    m_lParam  = 0;
    m_plfFont = NULL;

    // tree properties initialization
    m_pGrid = NULL;
    m_iColumnWithTree = -1; // no column draws the tree
    m_bTreeUsesImages = FALSE;
    m_bShowTreeLines = TRUE;

    m_iFixedRowCount = -1;
    m_iRowCount = -1;
    m_bAllowDraw = FALSE;   // prevents crash during reset
    m_crTreeLineColour = ::GetSysColor(COLOR_3DSHADOW);
    m_nDefTreeIndent = 10;
    m_pEditWnd = NULL;
}

CTreeColumn::~CTreeColumn()
{
    delete m_plfFont;
}

/*****************************************************************************
Sets up a column to be drawn with + - tree controls w/ indenting.  Text
is automatically indented.  Rows expand and collapse as appropriate

*****************************************************************************/
BOOL CTreeColumn::TreeSetup(  CGridCtrl* apGridCtrl,  // tree acts on a column in this grid
                            int aiColumn,       // which column has tree
                            int aiTotalRows,    // total number of rows if tree totally expanded
                            int aiFixedRowCount,// Set fixed row count now, too
                            const unsigned char* apucTreeLevelAry,    // Tree Level data array --
                                                            //  must have aiTotalRows of entries
                            BOOL abShowTreeLines,   // T=show tree (not grid) lines; F=no tree lines
                            BOOL abUseImages,   // T=use 1st 3 images from already set image list
                                                //  to display folder graphics
                            CRuntimeClass* apRuntimeClass)   // can use your special
                                                //  CGridTreeCellBase-derived class
// returns:  success / fail
{
    ASSERT( apGridCtrl != NULL);
    ASSERT( aiColumn >= apGridCtrl->GetFixedColumnCount()
            && aiColumn < apGridCtrl->GetColumnCount());

    if( apRuntimeClass == NULL)  // default can hold data
       apRuntimeClass = RUNTIME_CLASS(CGridTreeCell);
    if (!apRuntimeClass->IsDerivedFrom(RUNTIME_CLASS(CGridTreeCellBase)))
    {
        ASSERT( FALSE);
        return FALSE;
    }

    m_pGrid = apGridCtrl;
    m_bAllowDraw = FALSE;   // prevents crash during reset
    BOOL bRC = TRUE;

    m_iFixedRowCount = aiFixedRowCount;
    m_iRowCount = aiTotalRows + aiFixedRowCount;
    m_iColumnWithTree = aiColumn;

    m_pGrid->SetFixedRowCount( m_iFixedRowCount);
    m_pGrid->SetRowCount( m_iRowCount);

    if( aiTotalRows > 0)
    {
        ASSERT( apucTreeLevelAry != NULL);

        // retain old cell properties:  store one time in CTreeColumn
        CGridTreeCell GridCellCopy;
        GridCellCopy.SetTreeColumn( this);
        CGridCellBase* pCurrCell = m_pGrid->GetCell( m_iFixedRowCount, m_iColumnWithTree);
        if (pCurrCell)
            GridCellCopy = *pCurrCell;

        // copy all data while replacing cells to tree cell type
        for( int i1=0; i1 < aiTotalRows; i1++)
        {
            int iCellRow = m_iFixedRowCount + i1;


            if( !m_pGrid->SetCellType(   iCellRow,
                                m_iColumnWithTree,
                                apRuntimeClass ) )
            {
                bRC = FALSE;
                break;
            }

            CGridTreeCell* pGridTreeCell = (CGridTreeCell*)m_pGrid->GetCell( iCellRow, m_iColumnWithTree);
            if( pGridTreeCell != NULL)
            {
                pGridTreeCell->SetTreeColumn( this);
                pGridTreeCell->SetLevelAndHide( *apucTreeLevelAry );
            }

            apucTreeLevelAry++;
        }

        SetTreeUsesImages( abUseImages);   // T=use images
        SetTreeLines( abShowTreeLines);    // T=show tree lines
    }

    m_bAllowDraw = TRUE;
    m_pGrid->Invalidate();
    return bRC;
}

void CTreeColumn::SetTreeUsesImages( BOOL abUseImages)   // T=use images
{
#ifdef _DEBUG
    ASSERT( m_iColumnWithTree >= 0);
    ASSERT( m_pGrid != NULL);

    if( abUseImages
        && m_pGrid->GetImageList() == NULL)
        ASSERT( FALSE);     // must call SetImageList() first
#endif

    m_bTreeUsesImages = abUseImages;

    if( abUseImages)
    {
        // make sure that items have an image associated with it for
        //  autosize column width calculations
        ASSERT( m_iRowCount > 0); // call SetRowCount() before this
        ASSERT( m_iColumnWithTree >= 0);
        for( int i1=m_pGrid->GetFixedColumnCount(); i1 < m_iRowCount; i1++)
        {
            m_pGrid->SetItemImage( i1, m_iColumnWithTree, 0);
        }
    }
}

void CTreeColumn::SetTreeLines( BOOL abShowTreeLines)   // T=show tree lines
{
    ASSERT( m_iColumnWithTree >= 0);
    m_bShowTreeLines = abShowTreeLines;
}

unsigned char CTreeColumn::GetTreeLevel(  int aiRow)  // row
// returns:  tree level, =0 if invalid input
{
    ASSERT( m_pGrid != NULL);
    ASSERT( aiRow >= 0
            && aiRow < m_iRowCount );
    ASSERT( m_iColumnWithTree >= 0);

    CGridTreeCell* pGridTreeCell = (CGridTreeCell*)m_pGrid->GetCell( aiRow, m_iColumnWithTree);
    if( pGridTreeCell != NULL)
        return pGridTreeCell->GetLevel();

    return 0;
}

BOOL CTreeColumn::IsTreeRowDisplayed(  int aiRow)  // row
// returns:  T=tree row is displayed
{
    ASSERT( m_pGrid != NULL);
    ASSERT( aiRow >= 0
            && aiRow < m_iRowCount );
    ASSERT( m_iColumnWithTree >= 0);

    CGridTreeCell* pGridTreeCell = (CGridTreeCell*)m_pGrid->GetCell( aiRow, m_iColumnWithTree);
    if( pGridTreeCell != NULL)
        return pGridTreeCell->IsViewable();

    return FALSE;
}

unsigned char CTreeColumn::GetLevelAndHide( int aiRow)
// returns:  tree level, =0 if invalid input
{
    ASSERT( m_pGrid != NULL);
    ASSERT( aiRow >= 0
            && aiRow < m_iRowCount );
    ASSERT( m_iColumnWithTree >= 0);

    CGridTreeCell* pGridTreeCell = (CGridTreeCell*)m_pGrid->GetCell( aiRow, m_iColumnWithTree);
    if( pGridTreeCell != NULL)
        return pGridTreeCell->GetLevelAndHide();

    return 0;
}

/*****************************************************************************
Examines saved array show / hide bit to determine if a row should display.
There is a one-for-one mapping of these bits to total rows

*****************************************************************************/
void CTreeColumn::TreeRefreshRows()
{
    ASSERT( m_pGrid != NULL);
    ASSERT( m_iColumnWithTree >= 0);

    // hide / show each row
    for( int i1=m_iFixedRowCount; i1 < m_iRowCount; i1++)
    {
        CGridTreeCell* pGridTreeCell = (CGridTreeCell*)m_pGrid->GetCell( i1, m_iColumnWithTree);
        if( pGridTreeCell == NULL)
            return;

        if( pGridTreeCell->IsViewable() )
        {
            if( m_pGrid->GetRowHeight( i1) < m_pGrid->GetDefCellHeight() )
                m_pGrid->SetRowHeight( i1, m_pGrid->GetDefCellHeight() );
        }
        else
        {
            if( m_pGrid->GetRowHeight( i1) > 0 )
                m_pGrid->SetRowHeight( i1, 0);
        }

    }

    if (::IsWindow( m_pGrid->m_hWnd) && m_bAllowDraw)
    {
        m_pGrid->Invalidate();
    }

}
/*****************************************************************************
Callable by class consumer to prepare one's own data array of tree info to
display something reasonable.   High order Bits of the tree display data are
modified.

*****************************************************************************/
void CTreeColumn::TreeDataPrepOutline(unsigned char aucLevel, // level to display >= 0x80 displays all
                                    int aiIndex,            // Index to tree display data to modify
                                    int aiNbrElements)      // nbr of elements in tree display data
{
    ASSERT( aucLevel > 0);
    ASSERT( m_iColumnWithTree >= 0);
    ASSERT( aiIndex >= 0 );
    ASSERT( aiNbrElements > 0);
    ASSERT( m_pGrid != NULL);
    ASSERT( aiIndex + aiNbrElements <= m_iRowCount - m_iFixedRowCount );

    int i1;

    if( aucLevel >= 0x80)
    {
        for( i1=0; i1 < aiNbrElements; i1++)
        {
            int iCellRow = m_iFixedRowCount + aiIndex + i1;

            CGridTreeCell* pGridTreeCell = (CGridTreeCell*)m_pGrid->GetCell( iCellRow, m_iColumnWithTree);
            if( pGridTreeCell == NULL)
                return;

            pGridTreeCell->SetViewable( TRUE);
        }
    }
    else
    {
        for( i1=0; i1 < aiNbrElements; i1++)
        {
            int iCellRow = m_iFixedRowCount + aiIndex + i1;

            CGridTreeCell* pGridTreeCell = (CGridTreeCell*)m_pGrid->GetCell( iCellRow, m_iColumnWithTree);
            if( pGridTreeCell == NULL)
                return;

            if( pGridTreeCell->GetLevel() <= aucLevel)
                pGridTreeCell->SetViewable( TRUE);
            else
                pGridTreeCell->SetViewable( FALSE);
        }
    }
}

/*****************************************************************************
Like MS-Word's outline feature.  Can show 1st level of outline all the
way up to 126 levels or all

*****************************************************************************/
void CTreeColumn::TreeDisplayOutline( unsigned char aucLevel )    // level to display >= 0x80 displays all
{
    ASSERT( aucLevel > 0);
    ASSERT( m_pGrid != NULL);
    ASSERT( m_iColumnWithTree >= 0);
    if( m_iColumnWithTree < 0)
        return;

    TreeDataPrepOutline(aucLevel,                   // level to display >= 0x80 displays all
                        0,                          // Index to tree display data to modify
                        m_iRowCount - m_iFixedRowCount);    // nbr of elements in tree display data

    TreeRefreshRows();

}

/*****************************************************************************
Expands a branch 1 and only 1 level -- like when a user clicks on "+" box
in windows explorer tree.  Note that it doesn't check whether adjacent nodes
are properply represented, it just does the expand

*****************************************************************************/
void CTreeColumn::TreeDataExpandOneLevel( int aiGridRow)
{
    ASSERT( m_iColumnWithTree >= 0);
    ASSERT( m_pGrid != NULL);

    ASSERT( aiGridRow >= m_iFixedRowCount
            && aiGridRow < m_iRowCount);

    // display desired node and save its level and make visible
    CGridTreeCell* pGridTreeCell = (CGridTreeCell*)m_pGrid->GetCell( aiGridRow, m_iColumnWithTree);
    if( pGridTreeCell == NULL)
        return;

    unsigned char ucLevelCurrent = pGridTreeCell->GetLevel();
    if( ucLevelCurrent <= 0)
        return; // don't expand or collapse items not part of the tree
    pGridTreeCell->SetViewable( TRUE);

    unsigned char ucLevel;
    int i1;
    for( i1 = aiGridRow + 1; i1 < m_iRowCount; i1++)
    {
        CGridTreeCell* pGridTreeCell = (CGridTreeCell*)m_pGrid->GetCell( i1, m_iColumnWithTree);
        if( pGridTreeCell == NULL)
            return;

        ucLevel = pGridTreeCell->GetLevel();
        if( ucLevel <= ucLevelCurrent)
            break;

        if( ucLevel == ucLevelCurrent + 1)
            pGridTreeCell->SetViewable( TRUE);
        else
            pGridTreeCell->SetViewable( FALSE);

    }

}

/*****************************************************************************
Collapses all levels of a branch -- not just 1 level.   Using this in
conjunction with TreeDataExpandOneLevel will get a tree grid that performs like
VC++ watch window.   Windows explorer tree differs:  it "remembers" last
expansion.

Note that it doesn't check whether adjacent nodes are properply represented, it
just does the collapse.

*****************************************************************************/
void CTreeColumn::TreeDataCollapseAllSubLevels( int aiGridRow)
{
    ASSERT( m_iColumnWithTree >= 0);
    ASSERT( m_pGrid != NULL);

    ASSERT( aiGridRow >= m_iFixedRowCount
            && aiGridRow < m_iRowCount);

    // display desired node and save its level and make visible
    CGridTreeCell* pGridTreeCell = (CGridTreeCell*)m_pGrid->GetCell( aiGridRow, m_iColumnWithTree);
    if( pGridTreeCell == NULL)
        return;

    unsigned char ucLevelCurrent = pGridTreeCell->GetLevel();
    if( ucLevelCurrent <= 0)
        return; // don't expand or collapse items not part of the tree
    pGridTreeCell->SetViewable( TRUE);

    unsigned char ucLevel;
    int i1;
    for( i1 = aiGridRow + 1; i1 < m_iRowCount; i1++)
    {
        CGridTreeCell* pGridTreeCell = (CGridTreeCell*)m_pGrid->GetCell( i1, m_iColumnWithTree);
        if( pGridTreeCell == NULL)
            return;

        ucLevel = pGridTreeCell->GetLevel();
        if( ucLevel <= ucLevelCurrent)
            break;

        pGridTreeCell->SetViewable( FALSE);

    }
}

/*****************************************************************************
Toggles tree expansion and collapsing.  Intended to be connected to a user
event

*****************************************************************************/
void CTreeColumn::TreeExpandCollapseToggle( int aiGridRow) // Grid row of node to toggle
{
    ASSERT( m_pGrid != NULL);
    ASSERT( aiGridRow >= m_iFixedRowCount
            && aiGridRow < m_iRowCount);
    ASSERT( m_bAllowDraw);
    ASSERT( m_iColumnWithTree >= 0);
    if( m_iColumnWithTree < 0)
        return;

    // if last element, forget it
    if( aiGridRow + 1 >= m_iRowCount)
        return;

    // see if it needs to be expanded by looking at hide/show state of first neighbor.
    //  If first neighbor is hidden, should expand
    CGridTreeCell* pGridTreeCell = (CGridTreeCell*)m_pGrid->GetCell( aiGridRow + 1, m_iColumnWithTree);
    if( pGridTreeCell == NULL)
        return;

    if( pGridTreeCell->IsViewable() )
    {
        TreeDataCollapseAllSubLevels( aiGridRow);
    }
    else
    {
        TreeDataExpandOneLevel( aiGridRow);
    }
    TreeRefreshRows();

}

/*****************************************************************************
Should the cell identified by the row have a "+" or "-" graphic?

*****************************************************************************/
BOOL CTreeColumn::TreeCellHasPlusMinus(   int aiRow,          // row of Cell to check
                                        BOOL* apbIsPlus,    // returns:  T=Is a plus
                                        BOOL* apbIsMinus,   // returns:  T=Is a minus
                                        BOOL* apbIsLastLeaf)// returns:  T=Is Last Leaf
// returns:  T=cell has a plus or minus;  F=not
{
    ASSERT( m_pGrid != NULL);
    ASSERT( aiRow >= m_iFixedRowCount
            && aiRow < m_iRowCount);
    ASSERT( m_iColumnWithTree >= 0);
    ASSERT( apbIsPlus != NULL);
    ASSERT( apbIsMinus != NULL);
    ASSERT( apbIsLastLeaf != NULL);

    *apbIsPlus = FALSE;
    *apbIsMinus = FALSE;
    *apbIsLastLeaf = FALSE;

    int iStartPt = aiRow + 1;
    int i1;

    // get current level
    CGridTreeCell* pGridTreeCell = (CGridTreeCell*)m_pGrid->GetCell( aiRow, m_iColumnWithTree);
    if( pGridTreeCell == NULL)
        return FALSE;
    unsigned char ucLevelCurrent = pGridTreeCell->GetLevel();
    if( ucLevelCurrent <= 0)
        return FALSE;     // no tree

    BOOL bIsNextShowing = FALSE;
    unsigned char ucLevelNext = 0;


    for( i1=iStartPt; i1 < m_iRowCount; i1++)
    {
        pGridTreeCell = (CGridTreeCell*)m_pGrid->GetCell( i1, m_iColumnWithTree);
        if( pGridTreeCell == NULL)
            return FALSE;

        bIsNextShowing = pGridTreeCell->IsViewable();
        ucLevelNext = pGridTreeCell->GetLevel();

        if( ucLevelCurrent >= ucLevelNext )
        {
            break;
        }

        if( !bIsNextShowing
            && ucLevelNext == ucLevelCurrent + 1)
        {
            *apbIsPlus = TRUE;
        }

    }

    // final attribute setting now that enough data has been examined
    if( i1 > iStartPt
        && !*apbIsPlus)
    {
        *apbIsMinus = TRUE;
    }

    if( !bIsNextShowing
        || ucLevelCurrent > ucLevelNext
        || i1 >= m_iRowCount )  // hit last element
        *apbIsLastLeaf = TRUE;

    return *apbIsMinus || *apbIsPlus;
}

BOOL CTreeColumn::GetTreeUsesNbrImages() const
{
    return CGridTreeCellBase::TREE_IMAGE_FENCE;
}
