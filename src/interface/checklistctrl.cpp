/**
 * This file is part of scleaner project.

 * Copyright (C) 2007 FROUIN Jean-Michel

 * Visit scleaner website : http://www.scleaner.org
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
#include "checklistctrl.h"

namespace GUI
{
  /*
   * wxCheckListCtrl
   */

  IMPLEMENT_CLASS(wxCheckListCtrl, wxListCtrl)

    BEGIN_EVENT_TABLE(wxCheckListCtrl, wxListCtrl)
    EVT_MOUSE_EVENTS(wxCheckListCtrl::OnMouseEvent)
    EVT_CHAR(wxCheckListCtrl::OnKeyDown)
    EVT_CONTEXT_MENU(wxCheckListCtrl::OnContextMenu)
    EVT_MENU(ID_CHECKLIST_SELECT_ALL, wxCheckListCtrl::OnSelectAll)
    EVT_MENU(ID_CHECKLIST_UNSELECT_ALL, wxCheckListCtrl::OnUnselectAll)
    EVT_MENU(ID_CHECKLIST_INVERT_SELECTION, wxCheckListCtrl::OnInvertSelection)
    END_EVENT_TABLE()

    wxCheckListCtrl::wxCheckListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pt,
    const wxSize& sz, long style):
  wxListCtrl(parent, id, pt, sz, style)
  {
    LoadIcons();
  }

  /// Load the icons
  bool wxCheckListCtrl::LoadIcons()
  {
    //FIXME : Manual delete need
    fImageList = new wxImageList(16, 16, true);
    AssignImageList(fImageList, wxIMAGE_LIST_SMALL);

    fImageList->Add(wxIcon(checked_xpm));
    fImageList->Add(wxIcon(unchecked_xpm));
    fImageList->Add(wxIcon(folder_xpm));

    return true;
  }

  wxCheckListCtrl::~wxCheckListCtrl()
  {
  }

  /// Set the appropriate icon
  bool wxCheckListCtrl::SetIcon(long& item)
  {
    bool Ret = true;

    wxListItem Item;
    Item.SetId(item);
    GetItem(Item);
    wxString Text = Item.GetText();

    if(Item.GetImage() == 0)
    {
      Engine::CEngine::Instance()->SetUnselectedInputPlugs(std::string(Text.ToAscii()));
      SetItemImage(item, 1);
    }
    else
      SetItemImage(item, 0);

    return Ret;
  }

  void wxCheckListCtrl::OnMouseEvent(wxMouseEvent& event)
  {
    int l_flags = 0;
    long l_subitem;
    long Item = HitTest(wxPoint(event.GetX(), event.GetY()), l_flags, &l_subitem);

    if (event.LeftDown())
      if (l_flags & wxLIST_HITTEST_ONITEMICON)
        SetIcon(Item);

    event.Skip();
  }

  void wxCheckListCtrl::OnKeyDown(wxKeyEvent& event)
  {
    int l_flags = 0;
    long l_subitem;
    long Item = HitTest(wxPoint(event.GetX(), event.GetY()), l_flags, &l_subitem);

    if (event.GetKeyCode() == WXK_SPACE)
      SetIcon(Item);
    else
      event.Skip();
  }

  void wxCheckListCtrl::OnContextMenu(wxContextMenuEvent& event)
  {
    wxPoint point = event.GetPosition();
    // If from keyboard
    if (point.x == -1 && point.y == -1)
    {
      wxSize size = GetSize();
      point.x = size.x / 2;
      point.y = size.y / 2;
    }
    else
      point = ScreenToClient(point);
    ShowContextMenu(point);
  }

  void wxCheckListCtrl::ShowContextMenu(const wxPoint& pos)
  {
    wxMenu menu;

    menu.Append(ID_CHECKLIST_SELECT_ALL, wxString(i8n("&Select all files"), wxConvUTF8));
    menu.Append(ID_CHECKLIST_UNSELECT_ALL, wxString(i8n("&Unselect all files"), wxConvUTF8));
    menu.Append(ID_CHECKLIST_INVERT_SELECTION, wxString(i8n("&Invert selection"), wxConvUTF8));

    PopupMenu(&menu, pos.x, pos.y);
  }

  void wxCheckListCtrl::OnSelectAll(wxCommandEvent& WXUNUSED(event))
  {
    long Item = -1;
    if(GetSelectionCount() == 1)
    {
      //If user has selected only 1 item, that apply to all files
      for ( ;; )
      {
        Item = GetNextItem(Item);
        if(Item == -1)
          break;
        SetItemImage(Item, 0);
      }
    }
    else
    {
      //If user has selected more than 1 item, that apply to selected files
      for ( ;; )
      {
        Item = GetNextItem(Item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if(Item == -1)
          break;
        SetItemImage(Item, 0);
      }
    }
  }

  void wxCheckListCtrl::OnUnselectAll(wxCommandEvent& WXUNUSED(event))
  {
    std::cout << "OnUnselectAll\n";
    long Item = -1;
    if(GetSelectionCount() == 1)
    {
      //If user has selected only 1 item, that apply to all files
      for ( ;; )
      {
        Item = GetNextItem(Item);
        if(Item == -1)
          break;
        SetItemImage(Item, 1);
      }
    }
    else
    {
      //If user has selected more than 1 item, that apply to selected files
      for ( ;; )
      {
        Item = GetNextItem(Item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if(Item == -1)
          break;
        SetItemImage(Item, 1);
      }
    }
  }

  void wxCheckListCtrl::OnInvertSelection(wxCommandEvent& event)
  {
    long Item = -1;
    if(GetSelectionCount() == 1)
    {
      //If user has selected only 1 item, that apply to all files
      for ( ;; )
      {
        Item = GetNextItem(Item);
        if(Item == -1)
          break;

        wxListItem LItem;
        LItem.SetId(Item);
        GetItem(LItem);

        if(LItem.GetImage())
          SetItemImage(Item, 0);
        else
          SetItemImage(Item, 1);
      }
    }
    else
    {
      //If user has selected more than 1 item, that apply to selected files
      for ( ;; )
      {
        Item = GetNextItem(Item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if(Item == -1)
          break;

        wxListItem LItem;
        LItem.SetId(Item);
        GetItem(LItem);

        if(LItem.GetImage())
          SetItemImage(Item, 0);
        else
          SetItemImage(Item, 1);
      }
    }
  }

  int wxCheckListCtrl::GetSelectionCount()
  {
    int Count = 0;
    long Item = -1;
    for(;;)
    {
      Item = GetNextItem(Item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
      if(Item == -1)
        break;
      else
        Count++;
    }
    return Count;
  }

}
