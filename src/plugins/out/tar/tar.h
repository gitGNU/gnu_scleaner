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
$Date: 2007-08-28 14:00:13 +0200 (mar, 28 aoû 2007) $
$Rev: 45 $
$Author: snoogie $
------------------------------------------------------

*/

/*! @page page3 Plugins documentations.
* - @subpage tar
*/

/*! @page tar tar plugin.
* @section desc Description
* This plugin allow user to create a <b>tarball</b> with all selected files from input plugins.
* <br>
* @section param Parameters
* @section pb Knows bugs
* @section todo Todo
*/

#ifndef _TAR_H_
#define _TAR_H_

#include <config.h>
#include <plugins/out_plugin.h>

class CtarPlugin : public IOutPlugin
{
    public:
        CtarPlugin();
        ~CtarPlugin();

		/*!
		*@brief From IPlugin.
		*/
		void processFileList(std::list<std::string>& _fl);
		const std::string description();
		const std::string author();
		const std::string version();
		eType Type();

};
#endif                           //_TAR_H_
/* vi:set ts=4: */