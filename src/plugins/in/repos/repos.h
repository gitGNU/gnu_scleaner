/**
 * This file is part of scleaner project.

 * Copyright (C) 2007, 2008 FROUIN Jean-Michel

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

/*! @page page3 Plugins documentations.
 * - @subpage repos
 */

/*! @page repos repos: Find repository stuff.
 * @section desc Description
 * Find bazar stuff: .bzr
 * Find cvs stuff: CVS
 * Find git stuff : .git
 * Find subversion stuff: .svn
 */

#ifndef __REPOS_H__
#define __REPOS_H__

#include <config.h>
#include <plugins/in/in_plugin.h>

class CreposPlugin : public Plugins::IInputPlugin
{
  public:
    CreposPlugin();
    ~CreposPlugin();

    bool GrabNullFile()
    {
      return false;
    }

    bool GetDefaultSelection()
    {
      return true;
    }

    /*!
     *@brief From Plugins::IInPlugin
     */
    void ProcessFile(const std::string& filename);
    void GetDirectory(std::string& path) { path = ""; }

    /*!
     *@brief From IPlugin
     */
    EType Type() { return eUserInput; }
    std::string Description();
};
#endif                           //_REPOS_H_
/* vi:set ts=4: */
