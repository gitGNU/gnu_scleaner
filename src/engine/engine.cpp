/*
Copyright (C) 2000 Yann Guidon (whygee@f-cpu.org)
Copyright (C) 2007 FROUIN Jean-Michel
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

#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <pwd.h>
#include <dirent.h>
#include <mntent.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <sys/utsname.h>
#include <interface/maininterface.h>
#include "engine.h"

CEngine::CEngine():
m_rootPlugin(0), m_asRoot(false), m_callback(0)
{
	m_FoldersList.push_back("/home");
}

CEngine::~CEngine()
{
	std::cout << "List contain : \n";
	std::list<std::string>::iterator l_it;
	for(l_it = m_FoldersList.begin(); l_it != m_FoldersList.end(); ++l_it)
	{
		std::cout << *l_it << '\n';
	}
}

int CEngine::loadPlugins(const std::string& _path)
{
	int l_ret=0;

	//Plugins manager
	m_pfm = CPluginManager::Instance();

	l_ret = m_pfm->loadPlugins(_path);
	return l_ret;
}


bool CEngine::loadInterface()
{
	bool l_ret = false;

	CMainInterface* l_Main = 0;
	l_Main = new CMainInterface(NULL, wxID_ANY, wxT(NAME), SYMBOL_MAININTERFACE_POSITION, SYMBOL_MAININTERFACE_SIZE);
	if(l_Main)
	{
		l_Main->Show(true);
		l_ret = true;
	}

	return l_ret;
}


bool CEngine::isRoot()
{
	bool l_ret;
	if((getuid() != 0) || (geteuid() != 0))
	{
		l_ret = false;
	}
	else
	{
		l_ret = true;
	}
	return l_ret;
}


bool CEngine::getKernelVersion(std::string& _version)
{
	bool l_ret = false;
	utsname l_temp;
	if(uname(&l_temp) != 0)
	{
		std::cerr << i8n("[ERR] CEngine::getKernelVersion error !\n");
	}
	else
	{
		_version += l_temp.release;

		l_ret = true;
	}

	return l_ret;
}


bool CEngine::callOutputPlugin(std::list<std::string>& _list, std::string& _name, const std::string& _path, IProgressbar* _callback)
{
	bool l_ret = false;
	std::map<std::string, IOutPlugin*>* l_OutputPlugs;
	l_OutputPlugs = m_pfm->getOutputListPtr();
	IOutPlugin* l_plug = (*l_OutputPlugs)[_name];
	if(l_plug != 0)
	{
		l_plug->processFileList(_list, _path, _callback);
		l_ret = true;
	}
	else
	{
		std::cout << i8n("[WNG] : null") << '\n';
	}
	return l_ret;
}




bool CEngine::getUsername(std::string& _username)
{
	bool l_ret = false;

	uid_t l_uid = geteuid();

	struct passwd* l_passwd = 0;
	l_passwd = getpwuid(l_uid);

	#if defined DEBUG
	std::cout << i8n("[DBG] CEngine::getUsername UID: ") << l_uid << '\n';
	#endif

	if (l_passwd)
	{
		_username = l_passwd->pw_name;
		#if defined DEBUG
		std::cout << i8n("[DBG] CEngine::getUsername Username: ") << _username << '\n';
		#endif
		l_ret = true;
	}
	else
	{
		l_ret = false;
	}

	return l_ret;
}


void CEngine::formatSize(double _size, std::string& _str)
{
	std::stringstream l_temp;

	if(_size > (1024*1024*1024))
	{
		l_temp << ROUND((_size / (1024*1024*1024))) << i8n("GB");
	}
	else
	{
		if(_size > (1024*1024))
		{
			l_temp << ROUND(_size / (1024*1024)) << i8n("MB");
		}
		else
		{
			if(_size > 1024)
			{
				l_temp << ROUND(_size / 1024) << i8n("KB");
			}
			else
			{
				if(_size == 0)
				{
					l_temp << i8n("null size");
				}
				else
				{
					l_temp << _size;
				}
			}
		}
	}
	_str += l_temp.str();
}


double CEngine::getFreeSpace(const std::string& _path, std::string& _used, std::string& _free, std::string& _total)
{
	double l_ret = 0;
	
	FILE* l_mountTable;
	struct mntent* l_mountEntry;

	l_mountTable = setmntent(FSTAB, "r");

	if (l_mountTable == 0) 
	{
		perror(FSTAB);
	}
	else
	{
		double l_free, l_used, l_total;
		l_free = l_used = l_total = 0;

		while ((l_mountEntry = getmntent(l_mountTable))) 
		{
			struct statfs l_stat;
			double l_blocks_used;
			double l_blocks_percent_used;

			if (statfs(l_mountEntry->mnt_dir, &l_stat) != 0) 
			{
				perror(l_mountEntry->mnt_dir);
			}
			else
			{
				if (l_stat.f_blocks > 0) 
				{
					l_blocks_used = l_stat.f_blocks - l_stat.f_bfree;
					l_blocks_percent_used = (double)(l_blocks_used * 100.0 / (l_blocks_used + l_stat.f_bavail) + 0.5);
					if (strcmp(l_mountEntry->mnt_fsname, "/dev/root") == 0) 
					{
						/* Adjusts l_mountEntry->mnt_fsname to be the real root l_mountEntry->mnt_fsname,
						 * or leaves l_mountEntry->mnt_fsname alone if it can't find it */
						//find_real_root_l_mountEntry->mnt_fsname_name( l_mountEntry->mnt_fsname);
					}
					else
					{
						l_free += l_stat.f_bavail*(l_stat.f_bsize);
						l_used += (l_stat.f_blocks - l_stat.f_bfree)*(l_stat.f_bsize);
						l_total += l_stat.f_blocks*(l_stat.f_bsize); 
					}
				}
			}
		}

			std::string l_size;
			formatSize(l_free, l_size);
			_free += l_size;
			l_size.clear();
			formatSize(l_total, l_size);
			_total += l_size;
			l_size.clear();
			formatSize(l_used, l_size);
			_used += l_size;
			endmntent(l_mountTable);
	}

	return l_ret;
}

bool CEngine::scanDisk(IProgressbar* _callback)
{
	bool l_ret = false;

	m_callback = _callback;

	std::list<std::string>::iterator l_itFolders;

	for(l_itFolders = m_FoldersList.begin(); l_itFolders != m_FoldersList.end(); ++l_itFolders)
	{
		//If launch as root
		if(isRoot())
		{
			std::map<std::string, IInPlugin*>* l_input = CEngine::Instance()->getPluginManager()->getInputListPtr();
			std::map<std::string, IInPlugin*>::iterator l_it;
			for(l_it = l_input->begin(); l_it != l_input->end(); ++l_it)
			{
				if(((*l_it).second)->needRoot())
				{
					IRootPlugin* l_root = dynamic_cast<IRootPlugin*>((*l_it).second);
					std::string l_dir;
					l_root->getDirectory(l_dir);
					_callback->updateProgress(l_dir, true);
					scanDirectory(l_dir, true, l_root);
				}
			}
		}
		else
		{
			std::string l_username;
			getUsername(l_username);
			(*l_itFolders) += "/" + l_username;
			#if defined DEBUG
			std::cout << i8n("[DBG] CEngine::scanDisk Path : ") << (*l_itFolders) << '\n';
			#endif
		}
		scanDirectory(*l_itFolders);
	}
	return l_ret;
}


void CEngine::addFolder(std::string _dir)
{
	m_FoldersList.push_back(_dir);
}

void CEngine::delFolder(std::string _dir)
{
	std::list<std::string>::iterator l_it;
	for(l_it = m_FoldersList.begin(); l_it != m_FoldersList.end(); ++l_it)
	{
		if((*l_it) == _dir)
		{
			m_FoldersList.erase(l_it);
		}
	}
}


int CEngine::FTW_callback(const char* _fpath, const struct stat* _stat, int _tflag, struct FTW* _ftwbuf)
{
	int l_ret = 0;
	std::string l_path(_fpath);

	//Check if it is a folder ?
	if (_stat->st_mode > 23420)
	{
		#if defined DEBUG
		std::cout << i8n("[DBG] FTW_callback : ") << l_path << '\n';
		#endif
		CEngine* l_eng = CEngine::Instance();
		if(l_eng->asRoot())
		{
			IRootPlugin* l_root = 0;
			l_root = l_eng->rootPlugin();
			if(l_root != 0)
			{
				l_root->processFile(l_path);
			}
		}
		else
		{
			int l_count = 0;
			std::map<std::string, IInPlugin*>* l_input = CEngine::Instance()->getPluginManager()->getInputListPtr();
			std::map<std::string, IInPlugin*>::iterator l_it;
			for(l_it = l_input->begin(); l_it != l_input->end(); ++l_it)
			{
				IProgressbar* l_prog = l_eng->getCallback();

				if(l_prog != 0)
				{
					if(l_count++ == 10)
					{
						std::cout << l_count << '\n';
						l_prog->updateProgress(l_path, true);
						l_count = 0;
					}
				}

				if(!((*l_it).second)->needRoot())
				{
					((*l_it).second)->processFile(l_path);
				}
			}
		}
	}

	return l_ret;				 // To tell nftw() to continue
}


bool CEngine::scanDirectory(const std::string& _path, bool _asRoot, IRootPlugin* _rootPlugin, bool _recursive)
{
	bool l_ret = false;

	int l_flags = FTW_PHYS;

	if(_asRoot)
	{
		m_asRoot = true;
		m_rootPlugin = _rootPlugin;
	}

	if( nftw(_path.c_str(), FTW_callback, 20, l_flags) == 0)
	{
		l_ret = true;
	}

	m_asRoot = false;

	return l_ret;
}

void CEngine::getTimestamp(std::string& _str)
{
	time_t l_tm;
	struct tm l_time;

	time(&l_tm);
	memcpy(&l_time, localtime(&l_tm), sizeof(l_time));

	//Append date: YYYYMMDD
	std::stringstream l_temp;
	//Append year
	l_temp << 1900 + l_time.tm_year;

	//Append month
	if(l_time.tm_mon + 1 < 10)
	{
		l_temp << 0 << l_time.tm_mon + 1;
	}
	else
	{
		l_temp << l_time.tm_mon + 1;
	}

	//Append day
	if(l_time.tm_mday + 1 < 10)
	{
		l_temp << 0 << l_time.tm_mday;
	}
	else
	{
		l_temp << l_time.tm_mday;
	}

	l_temp << "_";

	//Append hour
	if(l_time.tm_hour < 10)
	{
		l_temp << 0 << l_time.tm_hour;
	}
	else
	{
		l_temp << l_time.tm_hour;
	}

	//Append minute
	if(l_time.tm_min < 10)
	{
		l_temp << 0 << l_time.tm_min;
	}
	else
	{
		l_temp << l_time.tm_min;
	}

	//Append second
	if(l_time.tm_sec < 10)
	{
		l_temp << 0 << l_time.tm_sec;
	}
	else
	{
		l_temp << l_time.tm_sec;
	}

	_str += l_temp.str();

	#if defined DEBUG
	std::cout << i8n("[DBG] CEngine::getTimestamp Timestamp: ") << _str << '\n';
	#endif
}

#include <sys/stat.h>
#include <unistd.h> /* pour lstat() */
#include <stdio.h>  /* pour fprintf() */
#include <stdlib.h> /* pour malloc() */

#define CRC_POLY_REV 0xEDB88320 /* poly CRC32 inversé CRC32 classique : polynôme = 0x04C11DB7 (32,26,23,22,16,12,11,10,8,7,5,4,2,1,0) */

unsigned long int CRC32tab[256],
 CRC_val, CRCSeed = 0xffffffff;

#define CRC_BUFFER_CHUNK_SIZE 65536 /* peut être changé */
unsigned char *CRC_buffer=NULL;

/* calcule la table de CRC32 */
void mkCRCtab() {
  int i, j;
  unsigned long r;

  for (i=0; i<256; i++) {
    r = i;
    for (j=0; j<8; j++) {
      if ( r & 1 )
         r = (r >> 1) ^ CRC_POLY_REV;
      else
         r>>=1;
    }
    CRC32tab[i] = r;
  }
}

void CEngine::calcCRC32(const std::string& _filename){
  unsigned char *c;
  unsigned int i,taille;
  FILE *in;

  /* init_CRC(); */
  if (CRC_buffer==NULL) {
    /* initialisation du buffer */
    CRC_buffer=malloc(CRC_BUFFER_CHUNK_SIZE);
    if (CRC_buffer==NULL) {
      fprintf(stderr,"\nerreur de malloc() dans calcule_CRC\n");
      exit(6);
    }
  }
  CRC_val=CRCSeed;

  in=fopen(nom_fic,"rb");
  if (in==NULL) {
    fprintf(stderr,"CRC impossible, lecture de %s refusée           \n",nom_fic);
    return;
  }
  fprintf(stderr,"CRCing %s%c                        ",nom_fic,0xD);

  while ((taille=fread(CRC_buffer,1,CRC_BUFFER_CHUNK_SIZE,in))!=0) {
    c=CRC_buffer;
    while (taille--) {
      i=(CRC_val&255)^(*c++);
      CRC_val=CRC32tab[i]^(CRC_val>>8);
    }
  }
  fclose(in);
}

/* vi:set ts=4: */
