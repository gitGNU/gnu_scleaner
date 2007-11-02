/**
 * This file is part of scleaner project.

 * Copyright (C) 2007 FROUIN Jean-Michel

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


#include <iostream>
#include <plugins/inplugin_initializer.h>
#include "trash.h"
#include <sys/stat.h>			 ///Get file size.
#include <leak/leak_detector.h>
#include <engine/engine.h>

Plugins::CPluginInitializerIn<CtrashPlugin> g_trash;

CtrashPlugin::CtrashPlugin()
{
	setName("trash");
}


CtrashPlugin::~CtrashPlugin()
{
}


Plugins::IPlugin::eType CtrashPlugin::Type()
{
	eType l_ret;
	l_ret = eInput;
	return l_ret;
}


void CtrashPlugin::processFile(const std::string& _filename)
{
	m_fl.push_back(_filename);
}


bool CtrashPlugin::needRoot()
{
	bool l_ret;
	l_ret = false;				 // This plugin will work in both (user/root) mode.
	return l_ret;
}


std::string CtrashPlugin::Description()
{
	return "Find files in your trashbin";
}
/* vi:set ts=4: */
