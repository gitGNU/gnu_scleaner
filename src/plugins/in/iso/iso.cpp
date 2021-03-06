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

#include <iostream>
#include <plugins/in/in_plugin_initializer.h>
#include "iso.h"
#include <sys/stat.h>            ///Get file size.
#include <leak/leak_detector.h>
#include <engine/engine.h>

Plugins::CInPluginInitializer<CisoPlugin> gCoders;

CisoPlugin::CisoPlugin()
{
  SetName("iso");
}


CisoPlugin::~CisoPlugin()
{
}


void CisoPlugin::ProcessFile(const std::string& filename)
{
  if( filename.find(".iso", 0) == (filename.length()-4) )
    fFL.push_back(filename);
}


std::string CisoPlugin::Description()
{
  return "Find disc image files (*.iso files)";
}


/* vi:set ts=4: */
