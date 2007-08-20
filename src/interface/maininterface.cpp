/*
Copyright (C) 2007 FROUIN Jean-Michel (jmfrouin@gmail.com)
------------------------------------------------------
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

------------------------------------------------------
Project : scleaner
------------------------------------------------------
$Date$
$Rev$
$Author$
------------------------------------------------------

*/

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <config.h>
#include <string>
#include <plugins/plugin_manager.h>
#include <plugins/iplugin.h>
#include "main.h"
#include "maininterface.h"

//GFX Elements
#include <gfx/empty.xpm>
#include <gfx/full.xpm>
#include <gfx/smile.xpm>

IMPLEMENT_CLASS( CMainInterface, wxFrame )

BEGIN_EVENT_TABLE( CMainInterface, wxFrame )
END_EVENT_TABLE()

CMainInterface::CMainInterface()
{
    Init();
}

CMainInterface::CMainInterface( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create( parent, id, caption, pos, size, style );
}

bool CMainInterface::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    Centre();
    return true;
}

CMainInterface::~CMainInterface()
{
    delete m_Icon;
#if defined(__WXCOCOA__)
    delete m_DockIcon;
#endif
}

void CMainInterface::Init()
{
	//Initialisation
	m_PluginsList = CPluginManager::Instance()->getPluginsListPtr();
	if(m_PluginsList == 0)
	{
		return;
	}

	//Tray icon !!
	m_Icon = new CTrayIcon();
#if defined(__WXCOCOA__)
  	m_DockIcon = new CTrayIcon(wxTaskBarIcon::DOCK);
#endif

	m_Icon->setParent(this);

  	if (!m_Icon->SetIcon(full_xpm, wxT(NAME)))
    {
		std::cout << "Could not set icon.";
	}
}

void CMainInterface::CreateControls()
{    
////@begin CMainInterface content construction
    CMainInterface* itemFrame1 = this;

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu3 = new wxMenu;
    menuBar->Append(itemMenu3, _("Menu"));
    itemFrame1->SetMenuBar(menuBar);

    wxToolBar* itemToolBar4 = CreateToolBar( wxTB_FLAT|wxTB_HORIZONTAL, ID_TOOLBAR1 );
    wxBitmap itemtool5Bitmap(wxNullBitmap);
    wxBitmap itemtool5BitmapDisabled;
    itemToolBar4->AddTool(ID_TOOL1, _T(""), itemtool5Bitmap, itemtool5BitmapDisabled, wxITEM_NORMAL, _T(""), wxEmptyString);
    wxBitmap itemtool6Bitmap(wxNullBitmap);
    wxBitmap itemtool6BitmapDisabled;
    itemToolBar4->AddTool(ID_TOOL2, _T(""), itemtool6Bitmap, itemtool6BitmapDisabled, wxITEM_NORMAL, _T(""), wxEmptyString);
    itemToolBar4->AddSeparator();
    wxBitmap itemtool8Bitmap(wxNullBitmap);
    wxBitmap itemtool8BitmapDisabled;
    itemToolBar4->AddTool(ID_TOOL3, _T(""), itemtool8Bitmap, itemtool8BitmapDisabled, wxITEM_NORMAL, _T(""), wxEmptyString);
    wxBitmap itemtool9Bitmap(wxNullBitmap);
    wxBitmap itemtool9BitmapDisabled;
    itemToolBar4->AddTool(ID_TOOL4, _T(""), itemtool9Bitmap, itemtool9BitmapDisabled, wxITEM_NORMAL, _T(""), wxEmptyString);
    itemToolBar4->Realize();
    itemFrame1->SetToolBar(itemToolBar4);

    wxSplitterWindow* itemSplitterWindow10 = new wxSplitterWindow( itemFrame1, ID_SPLITTERWINDOW1, wxDefaultPosition, wxSize(100, 100), wxSP_3DBORDER|wxSP_3DSASH|wxNO_BORDER );
    itemSplitterWindow10->SetMinimumPaneSize(0);

    //wxTreeCtrl* itemTreeCtrl11 = new wxTreeCtrl( itemSplitterWindow10, ID_TREECTRL1, wxDefaultPosition, wxSize(100, 100), wxTR_SINGLE );
    wxCheckTreeCtrl* itemTreeCtrl11 = new wxCheckTreeCtrl( itemSplitterWindow10, ID_TREECTRL1, wxDefaultPosition, wxSize(100, 100), wxTR_SINGLE | wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT);

	/* Happend plugins' name available */

	//wxTreeItemId l_root = itemTreeCtrl11->AddRoot(wxT("Input plugins"), wxTreeItemIcon_Expanded, wxTreeItemIcon_Expanded);
	wxTreeItemId l_root = itemTreeCtrl11->AddRoot(wxT("Input plugins :"));
    std::map<std::string, IPlugin*>::iterator _it;
    for(_it = m_PluginsList->begin(); _it != m_PluginsList->end(); ++_it)
    {
		std::cout << ":" << ((*_it).second)->getName() << '\n';
    	//itemTreeCtrl11->AppendItem(l_root, _(((*_it).second)->getName().c_str()), 2, 2/*, MyTreeCtrl::TreeCtrlIcon_File */ );
    	itemTreeCtrl11->AddCheckedItem(l_root, _(((*_it).second)->getName().c_str()), false /*, MyTreeCtrl::TreeCtrlIcon_File */ );
    }

    wxArrayString itemCheckListBox12Strings;
    itemCheckListBox12Strings.Add(_("dzdz"));
    itemCheckListBox12Strings.Add(_("dz"));
    itemCheckListBox12Strings.Add(_("dzdz"));
    itemCheckListBox12Strings.Add(_("d"));
    itemCheckListBox12Strings.Add(_("zd"));
    itemCheckListBox12Strings.Add(_("dz"));
    wxCheckListBox* itemCheckListBox12 = new wxCheckListBox( itemSplitterWindow10, ID_CHECKLISTBOX1, wxDefaultPosition, wxDefaultSize, itemCheckListBox12Strings, wxLB_SINGLE );

    itemSplitterWindow10->SplitVertically(itemTreeCtrl11, itemCheckListBox12, 200);

    wxStatusBar* itemStatusBar13 = new wxStatusBar( itemFrame1, ID_STATUSBAR1, wxST_SIZEGRIP|wxNO_BORDER );
    itemStatusBar13->SetFieldsCount(2);
    itemFrame1->SetStatusBar(itemStatusBar13);
}

bool CMainInterface::ShowToolTips()
{
    return true;
}

wxBitmap CMainInterface::GetBitmapResource( const wxString& name )
{
    wxUnusedVar(name);
    return wxNullBitmap;
}

wxIcon CMainInterface::GetIconResource( const wxString& name )
{
    wxUnusedVar(name);
    return wxNullIcon;
}
