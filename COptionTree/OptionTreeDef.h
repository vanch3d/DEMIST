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



// Option Definitions
// -- Specifications
#define OT_INFOWINDOWHEIGHT					50						// Information window height
#define OT_DEFHEIGHT						21						// Default height of an item
#define OT_SPACE							5						// Default horizontal spacing
#define OT_EXPANDBOX						9						// Size of the expand box
#define OT_CHECKBOX							14						// Size of the check box
#define OT_EXPANDCOLUMN						16						// Width of the expand column
#define OT_PNINDENT							16						// Child level indent
#define OT_COLRNG							5						// Width of splitter
#define OT_EXPANDBOXHALF					(OT_EXPANDBOX / 2)		// Half of expand box
#define OT_DEFLABEL							_T("No Item Selected")	// Default label for no selected item in the info window
#define OT_DEFINFO							_T("Select an item to see its description.") // Default info for no selected item in the info window
#define OT_RIGHTRESIZELIMIT					100						// Resize limit for right of client so controls don't dissapear
#define OT_TREELIST_ID						30000
// -- Item Types
#define OT_ITEM_STATIC						1
#define OT_ITEM_CHECKBOX					2
#define OT_ITEM_COLOR						3
#define OT_ITEM_COMBOBOX					4
#define OT_ITEM_DATE						5
#define OT_ITEM_EDIT						6
#define OT_ITEM_ICON						7
#define OT_ITEM_RADIO						8
#define OT_ITEM_SPINNER						9
// -- Tree Options
#define OT_OPTIONS_SHOWINFOWINDOW			0x0001					// Show information window
#define OT_OPTIONS_NOTIFY					0x0002					// Send parent notifications		
#define OT_OPTIONS_DEFINFOTEXTNOSEL			0x0004					// Show default info text for no selected item, otherwise blank		
#define OT_OPTIONS_SHADEEXPANDCOLUMN		0x0008					// Shade the expand column
#define OT_OPTIONS_SHADEROOTITEMS			0x0010					// Shade the root items
// -- Edit Options
#define OT_EDIT_MLHEIGHT					75						// Multiline height
// -- Combo Box Options
#define OT_COMBO_DROPDOWNHEIGHT				100						// Drop down default height
// -- Check Box Options
#define OT_CHECKBOX_DEFCHECKTEXT			_T("Checked")			// Default checked text
#define OT_CHECKBOX_DEFUNCHECKTEXT			_T("UnChecked")			// Default un checked text
#define OT_CHECKBOX_SIZE					14.2					// Size of the check box
// -- Radio Options
#define OT_RADIO_VSPACE						2						// Vertical space between radios
#define OT_RADIO_SIZE						14.2					// Size of radio
// -- Spinner Options
#define OT_SPINNER_WIDTH					15						// Width of the spinner button
// -- Color Options
#define OT_COLOR_MORECOLORS					_T("More Colors...")	// Text for more colors
#define OT_COLOR_AUTOMATIC					_T("Automatic")			// Text for more automatic
#define OT_COLOR_SIZE						14.2					// The size for the color square
// -- Icon options
#define OT_ICON_MARGIN						10						// The margin for the popup window
#define OT_ICON_ICONSPACE					10						// The space between icons in the popup window
#define OT_ICON_NOSELECTION					_T("No icon selected.")	// Text for not icon selected

// NOTE: The following are highly important and should not need to be changed
// -- Hit test
#define OT_HIT_LABEL						(WM_USER + 1000)		// Label
#define OT_HIT_COLUMN						(WM_USER + 1001)		// Column
#define OT_HIT_EXPAND						(WM_USER + 1002)		// Expand
#define OT_HIT_ATTRIBUTE					(WM_USER + 1003)		// Attribute
#define OT_HIT_CLIENT						(WM_USER + 1004)		// Client
// -- Notification to user
#define OT_NOTIFY_FIRST						(0U-1100U)
#define OT_NOTIFY_INSERTITEM				(OT_NOTIFY_FIRST - 1)	// Insert item
#define OT_NOTIFY_DELETEITEM				(OT_NOTIFY_FIRST - 2)	// Delete item
#define OT_NOTIFY_DELETEALLITEMS			(OT_NOTIFY_FIRST - 3)	// Delete all items
#define OT_NOTIFY_ITEMCHANGED				(OT_NOTIFY_FIRST - 5)	// Item changed
#define OT_NOTIFY_ITEMBUTTONCLICK			(OT_NOTIFY_FIRST - 6)	// Item button click
#define OT_NOTIFY_SELCHANGE					(OT_NOTIFY_FIRST - 7)	// Selection changed
#define OT_NOTIFY_ITEMEXPANDING				(OT_NOTIFY_FIRST - 8)	// Item expanding
#define OT_NOTIFY_COLUMNCLICK				(OT_NOTIFY_FIRST - 9)	// Column click
#define OT_NOTIFY_PROPCLICK					(OT_NOTIFY_FIRST - 10)	// Property click
// -- Notication to controls
#define OT_NOTIFY_COMMITCHANGES				WM_USER + 0x0102		// Loosing focus
#define OT_NOTIFY_FORCEREDRAW				WM_USER + 0x0103		// Force redraw
#define OT_NOTIFY_UP						WM_USER + 0x0104		// Up key pressed
#define OT_NOTIFY_DOWN						WM_USER + 0x0105		// Down key pressed
// -- Menu Definitions
#define OT_MES_UNDO							_T("&Undo")				// Text for undo
#define OT_MES_CUT							_T("Cu&t")				// Text for cut
#define OT_MES_COPY							_T("&Copy")				// Text for copy
#define OT_MES_PASTE						_T("&Paste")			// Text for paste
#define OT_MES_DELETE						_T("&Delete")			// Text for delete
#define OT_MES_SELECTALL					_T("Select &All")		// Text for select all
#define OT_ME_SELECTALL						WM_USER + 0x7000		// Command for select all
// -- Color PopUp
#define OT_CPU_SELCHANGE					WM_USER + 1001			// Color picker selection change
#define OT_CPU_DROPDOWN						WM_USER + 1002			// Color picker drop down
#define OT_CPU_CLOSEUP						WM_USER + 1003			// Color picker close up
#define OT_CPU_SELENDOK						WM_USER + 1004			// Color picker end OK
#define OT_CPU_SELENDCANCEL					WM_USER + 1005			// Color picker end (cancelled)
#define OT_CPU_DEFAULTBOXVALUE				-3						// Default box value
#define OT_CPU_CUSTOMBOXVALUE				-2						// Custom box value
#define OT_CPU_INVALIDCOLOR					-1						// Invalid color value
#define OT_CPU_MAXCOLORS					100						// Maximum number of colors
// -- Icon PopUp
#define OT_ICON_CLOSE						WM_USER + 1001				// Icon picker window close
