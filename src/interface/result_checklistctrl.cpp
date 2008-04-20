/**
 * This file is part of scleaner project.

 * Copyright (C) 2008 FROUIN Jean-Michel

 * Visit scleaner website : http://www.scleaner.fr
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

// Include XPM icons
#include <gfx/checked.xpm>
#include <gfx/unchecked.xpm>
#include <gfx/folder.xpm>

#include <wx/menu.h>
#include <def.h>
#include <engine/engine.h>
#include "result_checklistctrl.h"

namespace GUI
{
    IMPLEMENT_CLASS(ResultCheckListCtrl, wxListCtrl)

    BEGIN_EVENT_TABLE(ResultCheckListCtrl, wxListCtrl)
        EVT_MOUSE_EVENTS(ResultCheckListCtrl::OnMouseEvent)
        EVT_CHAR(ResultCheckListCtrl::OnKeyDown)
        EVT_CONTEXT_MENU(ResultCheckListCtrl::OnContextMenu)
        EVT_MENU(ID_SELECT_ALL, ResultCheckListCtrl::OnSelectAll)
        EVT_MENU(ID_UNSELECT_ALL, ResultCheckListCtrl::OnUnselectAll)
    END_EVENT_TABLE()

    ResultCheckListCtrl::ResultCheckListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pt,
    const wxSize& sz, long style):
    wxListCtrl(parent, id, pt, sz, style)
    {
        LoadIcons();
    }

    bool ResultCheckListCtrl::LoadIcons()
    {
        //FIXME : Manual delete need
        m_imageList = new wxImageList(16, 16, true);
        AssignImageList(m_imageList, wxIMAGE_LIST_SMALL);

        m_imageList->Add(wxIcon(checked_xpm));
        m_imageList->Add(wxIcon(unchecked_xpm));
        m_imageList->Add(wxIcon(folder_xpm));

        return true;
    }

    ResultCheckListCtrl::~ResultCheckListCtrl()
    {
    }

    bool ResultCheckListCtrl::SetIcon(long& item)
    {
        bool l_ret = true;

        wxListItem l_item;
        l_item.SetId(item);
        GetItem(l_item);
        wxString l_text = l_item.GetText();

        if(l_item.GetImage() == 0)
        {
            Engine::CEngine::Instance()->SetUnselectedInputPlugs(std::string(l_text.ToAscii()));
            SetItemImage(item, 1);
        }
        else
        {
            SetItemImage(item, 0);
        }

        return l_ret;
    }


    void ResultCheckListCtrl::OnMouseEvent(wxMouseEvent& event)
    {
        int l_flags = 0;
        long l_subitem;
        long l_item = HitTest(wxPoint(event.GetX(), event.GetY()), l_flags, &l_subitem);

        if (event.LeftDown())
        {
            if (l_flags & wxLIST_HITTEST_ONITEMICON)
            {
                SetIcon(l_item);
            }
        }

        event.Skip();
    }


    void ResultCheckListCtrl::OnKeyDown(wxKeyEvent& event)
    {
        int l_flags = 0;
        long l_subitem;
        long l_item = HitTest(wxPoint(event.GetX(), event.GetY()), l_flags, &l_subitem);

        if (event.GetKeyCode() == WXK_SPACE)
        {
            SetIcon(l_item);
        }
        else
        {
            event.Skip();
        }
    }

    void ResultCheckListCtrl::OnContextMenu(wxContextMenuEvent& event)
    {
        wxPoint point = event.GetPosition();
        //std::cout << "Right click detected\n";
        // If from keyboard
        if (point.x == -1 && point.y == -1) {
            wxSize size = GetSize();
            point.x = size.x / 2;
            point.y = size.y / 2;
        } else {
            point = ScreenToClient(point);
        }
        ShowContextMenu(point);
    }


    void ResultCheckListCtrl::ShowContextMenu(const wxPoint& pos)
    {
        wxMenu menu;

        menu.Append(ID_SELECT_ALL, wxString(i8n("&Select all files"), wxConvUTF8));
        menu.Append(ID_UNSELECT_ALL, wxString(i8n("&Unselect all files"), wxConvUTF8));
        //m_lbox->SetSelection(-1);
        menu.Append(ID_SELECT_FROM_SAME_FOLDER, wxString(i8n("&Select all files from same folder"), wxConvUTF8));
        menu.Append(ID_UNSELECT_FROM_SAME_FOLDER, wxString(i8n("&Unselect all files from same folder"), wxConvUTF8));
        menu.Append(ID_SELECT_FROM_SAME_EXTENSION, wxString(i8n("&Select all files with same extension"), wxConvUTF8));
        menu.Append(ID_UNSELECT_FROM_SAME_EXTENSION, wxString(i8n("&Unselect all files with same extension"), wxConvUTF8));

        PopupMenu(&menu, pos.x, pos.y);
    }

    void ResultCheckListCtrl::OnSelectAll(wxCommandEvent& WXUNUSED(event))
    {

    }

    void ResultCheckListCtrl::OnUnselectAll(wxCommandEvent& WXUNUSED(event))
    {

    }
}