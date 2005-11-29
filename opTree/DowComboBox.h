class CDowComboBox : public CTreeOptionsCombo
{
public:
	CDowComboBox();
	virtual ~CDowComboBox();

protected:
	//{{AFX_VIRTUAL(CDowComboBox)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDowComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

  DECLARE_DYNCREATE(CDowComboBox)
};

