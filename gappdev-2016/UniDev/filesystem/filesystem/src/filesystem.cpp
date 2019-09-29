#include "filesystem.h"
#include <algorithm>

#if defined (_WIN32)
#	define WIN32_LEAN_AND_MEAN
#	define NOMINMAX
#	include <windows.h>

#if !defined (INVALID_FILE_ATTRIBUTES)
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif

namespace filesystem {

	std::wstring to_utf16(const std::string & s) {
		std::wstring ret;
		wchar_t cbuf[MAX_PATH];
		int cs = ::MultiByteToWideChar(CP_UTF8, 0, s.c_str(), static_cast<int>(s.size()), cbuf, MAX_PATH);
		if (cs) {
			ret.assign(cbuf, cbuf + cs);
		}
		return ret;		
	}
	std::string from_utf16(const std::wstring & s) {
		std::string ret;
		char cbuf[MAX_PATH];
		int cs = ::WideCharToMultiByte(CP_UTF8, 0, s.c_str(), static_cast<int>(s.size()), cbuf, MAX_PATH, 0, 0);
		if (cs) {
			ret.assign(cbuf, cbuf + cs);
		}
		return ret;
	}

	file_status status_from_api(DWORD attrs) {
		if (attrs == INVALID_FILE_ATTRIBUTES) {
			attrs = ::GetLastError();
			switch (attrs) {
			case ERROR_FILE_NOT_FOUND:
			case ERROR_PATH_NOT_FOUND:
			case ERROR_INVALID_DRIVE:
			case ERROR_BAD_NETPATH:
				return file_status(file_not_found);
			case ERROR_ACCESS_DENIED:
				return file_status(file_unknown);
			default:
				return file_status();
			}
		} else {
			int pstat = status_unknown;
			if (attrs & FILE_ATTRIBUTE_DIRECTORY) pstat |= file_directory;
			// not directory, so regular file
			else if (attrs & FILE_ATTRIBUTE_NORMAL) pstat |= file_regular;
			else if (attrs & FILE_ATTRIBUTE_ARCHIVE) pstat |= file_regular;
			else if (attrs & FILE_ATTRIBUTE_READONLY) pstat |= file_regular;
			// this status is orthogonal
			if (attrs & FILE_ATTRIBUTE_TEMPORARY) pstat |= file_temporary;
			return file_status(pstat);
		}
	}

	file_status status(const path & p) {
		return status_from_api(::GetFileAttributes(to_utf16(p.string()).c_str()));
	}
	file_status link_status(const path & p) {
		// no symlink status for WIN32. CreateSymbolicLink is supported on Vista
		// and highier. FILE_ATTRIBUTE_REPARSE_POINT attribute is attached to
		// symlinks, then FindFirstFile must be called and field dwReserved0
		// must be checked for IO_REPARSE_TAG_SYMLINK value. then file must
		// be opened and GetFinalPathNameByHandle called on file handle to reach
		// symlink target. symlink support added for unix compatibility,
		// designed the way no one want to use it.
		// GetFileAttributesEx was not good enough?
		return file_status();
	}

	int copy(const path & from, const path & to) {
		std::wstring f = to_utf16(from.string());
		std::wstring t = to_utf16(to.string());
		if (::CopyFile(f.c_str(), t.c_str(), FALSE) == TRUE) {
			return 0;
		} else {
			return ::GetLastError();
		}
	}
	int move(const path & from, const path & to) {
		std::wstring f = to_utf16(from.string());
		std::wstring t = to_utf16(to.string());
		if (::MoveFile(f.c_str(), t.c_str()) == TRUE) {
			return 0;
		} else {
			return ::GetLastError();
		}
	}
	int remove(const path & fpath) {
		file_status s = status(fpath);
		std::wstring f = to_utf16(fpath.string());
		if (is_directory(s)) {
			if (::RemoveDirectory(f.c_str()) == TRUE) {
				return 0;
			} else {
				return ::GetLastError();
			}
		} else {
			if (::DeleteFile(f.c_str()) == TRUE) {
				return 0;
			} else {
				return ::GetLastError();
			}
		}
	}

	int create_directory(const path & fpath) {
		int ret = 0;
		std::wstring f = to_utf16(fpath.string());
		if (::CreateDirectory(f.c_str(), 0) == FALSE) {
			if ((ret = ::GetLastError()) == ERROR_ALREADY_EXISTS)
				if (status(fpath).type() == file_directory)
					ret = 0;
		}
		return ret;
	}

	int find(const path & dir, const string_type & glob, directory_list & list) {
		WIN32_FIND_DATA fd;
		HANDLE h = FindFirstFile(to_utf16((path(dir) / glob).string()).c_str(), &fd);
		if (h != INVALID_HANDLE_VALUE) {
			do {
				list.push_back(directory_entry());
				list.back().name = from_utf16(fd.cFileName);
				list.back().status =  status_from_api(fd.dwFileAttributes);
			} while (FindNextFile(h, &fd) == TRUE);
			FindClose(h);
			return 0;
		} else {
			return ::GetLastError();
		}
	}

} // namespace filesystem

#elif (defined (__GNUG__) && defined (__GLIBC__)) || defined(__ANDROID__) || defined(__linux__) || defined (__APPLE__)
#	include <stdio.h>
#	include <string.h>
#	include <unistd.h>
#	include <stdlib.h>
#	include <stddef.h>
#	include <errno.h>
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <fcntl.h>
#	include <dirent.h>
#	include <fnmatch.h>

#if defined(__ANDROID__) && !defined(DTTOIF)
// TODO: verify if this macro is ok!
#	define DTTOIF(dirtype) ((dirtype) << 12)
#endif

namespace filesystem {

	typedef int (*stat_function)(const char *path, struct stat *buf);

	file_status status_from_api(mode_t mode) {
		int pstat = status_unknown;
		if (S_ISREG(mode)) pstat |= file_regular;
		else if (S_ISDIR(mode)) pstat |= file_directory;
		else if (S_ISCHR(mode)) pstat |= file_chardev;
		else if (S_ISBLK(mode)) pstat |= file_blockdev;
		else if (S_ISFIFO(mode)) pstat |= file_fifo;
		else if (S_ISLNK(mode)) pstat |= file_symlink;
		else if (S_ISSOCK(mode)) pstat |= file_socket;
		return file_status(pstat);
	}

	file_status status_impl(const string_type & p, stat_function f = stat) {
		struct stat st;
		if (f(p.c_str(), &st) == 0) {
			return status_from_api(st.st_mode);
		} else {
			switch (errno) {
			case ENOENT:
				return file_status(file_not_found);
			default:
				return file_status(file_unknown);
			}
		}
	}

	file_status status(const path & p) {
		return status_impl(p.string(), stat);
	}

	file_status link_status(const path & p) {
		return status_impl(p.string(), lstat);
	}

	int move(const path & from, const path & to) {
		const string_type & cfrom = from.string();
		const string_type & cto = to.string();
		if (rename(cfrom.c_str(), cto.c_str()) == 0) {
			return 0;
		} else {
			return errno;
		}
	}

	int copy(const path & from, const path & to) {
		int ret = 0;
		const string_type & cfrom = from.string();
		const string_type & cto = to.string();
		int ifd = open(cfrom.c_str(), O_RDONLY);
		if (ifd != -1) {
			// perm: drw-r--r--
			int ofd = open(cto.c_str(), O_CREAT | O_WRONLY | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			if (ifd != -1) {
				char buf[16384];
				ssize_t nr;
				while ((nr = read(ifd, buf, 16384)) > 0) {
					if (write(ofd, buf, nr) != nr) {
						ret = errno;
						break;
					}
				}
				if (nr == -1) {
					ret = errno;
				}
				close(ofd);
			} else {
				ret = errno;
			}
			close(ifd);
		} else {
			ret = errno;
		}
		return ret;
	}

	int remove(const path & fpath) {
		const string_type & tmp = fpath.string();
		// use implementaion directly (one conversion less)
		file_status s = status_impl(tmp.c_str());
		if (is_directory(s)) {
			if (rmdir(tmp.c_str()) == 0) {
				return 0;
			} else {
				return errno;
			}
		} else {
			if (unlink(tmp.c_str()) == 0) {
				return 0;
			} else {
				return errno;
			}
		}
	}

	int create_directory(const path & fpath) {
		int ret = 0;
		const string_type & tmp = fpath.string();
		// perm: drwxr-xr-x
		if (mkdir(tmp.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) != 0) {
			if ((ret = errno) == EEXIST)
				if (status_impl(tmp.c_str()).type() == file_directory)
					ret = 0;
		}
		return 0;
	}

	int find(const path & dir, const string_type & glob, directory_list & list) {
		DIR * d = opendir(dir.string().c_str());
		if (d) {
			struct dirent ent;
			struct dirent *e;
			while (true) {
				readdir_r(d, &ent, &e);
				if (e == 0) break;
				if (fnmatch(glob.c_str(), e->d_name, 0) == 0) {
					list.push_back(directory_entry());
					list.back().name = e->d_name;
					if (e->d_type != DT_UNKNOWN) {
						list.back().status = status_from_api(DTTOIF(e->d_type));
					} else {
						// some filesystems does not set it.
						list.back().status = link_status(dir / e->d_name);
					}
				}
			}
			closedir(d);
			return 0;
		} else {
			return errno;
		}
	}

} // namespace filesystem

#elif defined (__BADA__)
#include <string.h>
#include <FBase.h>
#include <FIo.h>

namespace filesystem {

	file_status status_from_api(Osp::Io::FileAttributes &attr)
	{
		int pstat = status_unknown;
		if (attr.IsHidden()) pstat |= file_regular;
		else if(attr.IsReadOnly()) pstat |= file_regular;
		else if (attr.IsDirectory()) pstat |= file_directory;
		return file_status(pstat);
	}

	file_status status_impl(const string_type & p)
	{
		Osp::Io::FileAttributes attr;
		if (Osp::Io::File::GetAttributes(p.c_str(), attr) == E_SUCCESS) {
			return status_from_api(attr);
		} else
			return file_status(file_unknown);
	}

	file_status status(const path & p)
	{
		return status_impl(p.string());
	}

	file_status link_status(const path & p)
	{
		return status_impl(p.string());
	}

	int move(const path & from, const path & to)
	{
		const Osp::Base::String cfrom(from.string().c_str());
		const Osp::Base::String cto(to.string().c_str());
		file_status s = status_impl(from.string());
		if (is_directory(s))
			return Osp::Io::Directory::Rename(cfrom,cto);
		else
			return Osp::Io::File::Move(cfrom,cto);
	}

	int copy(const path & from, const path & to)
	{
		const Osp::Base::String cfrom(from.string().c_str());
		const Osp::Base::String cto(to.string().c_str());
		return Osp::Io::File::Copy(cfrom,cto,false);
	}

	int remove(const path & fpath)
	{
		const Osp::Base::String tmp(fpath.string().c_str());
		file_status s = status_impl(fpath.string());
		if (is_directory(s))
			return Osp::Io::Directory::Remove(tmp,true);
		else
			return Osp::Io::File::Remove(tmp);
	}

	int create_directory(const path & fpath)
	{
		const Osp::Base::String tmp(fpath.string().c_str());
		return Osp::Io::Directory::Create(tmp);
	}

	int find(const path & dir, const string_type & glob, directory_list & list)
	{
		Osp::Io::Directory dirr;
		Osp::Io::DirEnumerator *pDirEnum = null;
		result r = E_SUCCESS;

		// Opens the directory
		r = dirr.Construct(dir.string().c_str());
		if(r != E_SUCCESS)
			return r;

		// Reads all the directory entries
		pDirEnum = dirr.ReadN();
		if(!pDirEnum)
			return E_FAILURE;

		Osp::Base::String pattern(glob.c_str());
		// Loops through all the directory entries
		while(pDirEnum->MoveNext() == E_SUCCESS)
		{
			Osp::Io::DirEntry dirEntry = pDirEnum->GetCurrentDirEntry();

			Osp::Base::Utility::RegularExpression regex;
			regex.Construct(pattern, Osp::Base::Utility::REGEX_CASELESS
					| Osp::Base::Utility::REGEX_MULTI_LINE
					| Osp::Base::Utility::REGEX_EXTENDED);
			if(regex.Match(dirEntry.GetName(), false))
			{
				list.push_back(directory_entry());
				Osp::Base::ByteBuffer *pBuf = Osp::Base::Utility::StringUtil::StringToUtf8N(dirEntry.GetName());
				if(pBuf)
				{
					static const int maxPathLen = 256;
					char pp[maxPathLen];
					memcpy(pp,pBuf->GetPointer(),std::min(maxPathLen,pBuf->GetCapacity()));
					list.back().name = string_type(pp);
					string_type s = dir.string() + "//" + pp;
					list.back().status = status(s);
				}
			}
		}

		// Deletes the enumerator
		delete pDirEnum;
		return r;
	}
} // namespace filesystem
#else
#	error unsupported platform
#endif

namespace filesystem {

string_type path::file_directory() const {
	string_type ret;
	size_t p = m_p.rfind(separator::value);
	if (p != string_type::npos) {
		if (p > 0 && p == m_p.size() - 1) {			// "/path/to/something/"
			p = m_p.rfind(separator::value, p - 1);
		}
		ret.append(m_p.begin(), m_p.begin() + p);
	}
	return ret;
}

string_type path::file_name() const {
	string_type ret;
	size_t p = m_p.rfind(separator::value);
	if (p != string_type::npos) {
		ret.append(m_p.begin() + p + 1, m_p.end());
	} else {
		ret.append(m_p);
	}
	return ret;
}

string_type path::base_name() const {
	string_type ret;
	string_type fn = file_name();
	size_t dp = fn.rfind(dot::value);
	if (dp == string_type::npos) {
		dp = fn.size();
	}
	ret.append(fn.begin(), fn.begin() + dp);
	return ret;
}

string_type path::file_ext() const {
	string_type ret;
	string_type fn = file_name();
	size_t dp = fn.rfind(dot::value);
	if (dp != string_type::npos) {
		ret.append(fn.begin() + dp + 1, fn.end());
	}
	return ret;
}

void path::replace_ext(const string_type & ext) {
	// find last dot, but there must be no separator after it
	size_t dp = m_p.rfind(dot::value);
	size_t sp = m_p.rfind(separator::value);
	bool do_app = (dp == string_type::npos) || (sp != string_type::npos && sp > dp);
	if (do_app) {
		m_p.push_back(dot::value);
	} else {
		m_p.erase(m_p.begin() + dp + 1, m_p.end());
	}
	const char * e = ext.c_str();
	if (*e == dot::value) e++;	// if ext starts with dot, skip it
	append_normalized(e);
}

void path::replace_name(const string_type & name) {
	size_t p = m_p.rfind(separator::value);
	if (p != string_type::npos) {
		m_p.erase(m_p.begin() + p + 1, m_p.end());
	} else {
		m_p.clear();
	}
	append_normalized(name.c_str());
}

void path::append_normalized(const char_type * name) {
	// last nonseparator entry
	int se = static_cast<int>(strlen(name));
	int e = se - 1;
	while (e >= 0) {
		if (name[e] != separator::value) break;
		--e;
	}
	if (se > 0) {
		int i = 0;
		if (m_p.empty()) {
			m_p.push_back(name[i]);
			++i;
		}
		while (i <= e) {
			if (!(*m_p.rbegin() == separator::value && name[i] == separator::value))
				m_p.push_back(name[i]);
			++i;
		}
	}
}


bool exists(const path & fpath) {
	file_status s = status(fpath);
	return exists(s);
}
bool is_directory(const path & fpath) {
	file_status s = status(fpath);
	return is_directory(s);
}
bool is_file(const path & fpath) {
	file_status s = status(fpath);
	return is_file(s);
}
bool is_temporary(const path & fpath) {
	file_status s = status(fpath);
	return is_temporary(s);
}
bool is_symlink(const path & fpath) {
	file_status s = status(fpath);
	return is_symlink(s);
}

int create_directories(const path & fpath) {
	path base = fpath.file_directory();
	int ret = 0;
	file_status s = status(base);
	if (!exists(s)) {
		ret = create_directories(base);
	}
	if (ret == 0) {
		ret = create_directory(fpath);
	}
	return ret;
}

int remove_all(const path & fpath) {
	int r = 0;
	directory_list l;
	r = find(fpath, "*", l);
	if (r == 0) {
		for (directory_list::const_iterator it = l.begin(); it != l.end(); ++it) {
			if ((*it).status.type() == file_directory) {
				if ((*it).name != "." && (*it).name != "..") {
					r = remove_all(path(fpath) / (*it).name);
				}
			} else {
				r = remove(path(fpath) / (*it).name);
			}
			if (r != 0) break;	// break at first failure
		}
		if (r == 0) r = remove(fpath);
	}
	return r;
}

	path root() {
		static const char root_str[] = { separator::value, 0 };
#if defined (_WIN32) && ! defined (_WIN32_WCE)
		std::string root;
		wchar_t b[256];
		if (::GetCurrentDirectory(256, b) != 0) {
			std::string cd = from_utf16(b);
			size_t p = cd.find(colon::value);
			if (p != std::string::npos) {
				root.append(cd.c_str(), p + 1);
			}
		}
		root.append(root_str);
		return path(root);
#else
		return path(root_str);
#endif
	}

    bool filesortfunc(const directory_entry &file1, const directory_entry &file2) {
        return file1.name < file2.name;
    }

    void sort(directory_list & list) {
        if(list.empty()) return ;
        std::sort(list.begin(),list.end(),filesortfunc);
    }

} // namespace filesystem
