#ifndef __LIB_FILESYSTEM_H__
#define __LIB_FILESYSTEM_H__

#include <string>
#include <vector>

namespace filesystem {

// string type representing path
typedef std::string string_type;
typedef std::string::value_type char_type;

struct separator {
#if defined (_WIN32)
	static const char_type value = '\\';
#elif (defined (__GNUG__) && defined (__GLIBC__)) || defined(__ANDROID__) || defined(__linux__) || defined(__BADA__) || defined (__APPLE__)
	static const char_type value = '/';
#else
#	error unsupported platform
#endif
};

struct dot {
	static const char_type value = '.';
};

struct colon {
	static const char_type value = ':';
};

enum file_type {
	// regular file
	file_regular = 1,
	// directory file
	file_directory = 2,
	// temporary (WIN32 specyfic)
	file_temporary = 4,
	// character device file (Unix specyfic)
	file_chardev = 8,
	// block device file (Unix specyfic)
	file_blockdev = 16,
	// socket file (Unix specyfic)
	file_socket = 32,
	// named pipe (Unix specyfic)
	file_fifo = 64,
	// symbolic link file (Unix specyfic)
	file_symlink = 128,
	// file type cannot be determined (permissions)
	file_unknown = 256,
	// file not found special status
	file_not_found = 512,
	status_unknown = 0
};

// represents basic file status
class file_status {
public:
	file_status(int ft = status_unknown) : m_ft(ft) {}
	int type() const { return m_ft; }
private:
	int m_ft;
};

// represents path
class path {
public:
	path() {}
	path(const string_type & str_path) {
		append_normalized(str_path.c_str());
	}
	path(const char_type * str_path) {
		append_normalized(str_path);
	}

	path operator / (const char_type * str) const {
		path ret(*this);
		size_t s = ret.m_p.size();
		if (s > 0 && ret.m_p[s - 1] != separator::value)
			ret.m_p.push_back(separator::value);
		ret.append_normalized(str);
		return ret;
	}
	path operator / (const string_type & str_path) const {
		return this->operator / (str_path.c_str());
	}
	// returns directory part of path
	string_type file_directory() const;
	// returns filename part of path (with extension if there is one)
	string_type file_name() const;
	// returns filename extension part of path
	string_type file_ext() const;
	// returns filename without extension
	string_type base_name() const;
	// replaces extension with new one or append one if none is present.
	void replace_ext(const string_type & ext);
	// replaces filename part of path (with extension)
	void replace_name(const string_type & name);

	bool empty() const { return m_p.empty(); }
	const string_type & string() const { return m_p; }

    path parent() const { return path(this->file_directory()); }

private:
	void append_normalized(const char_type * name);
	string_type m_p;
};

// root directory reference
path root();

// file status query functions
file_status status(const path & p);
file_status link_status(const path & p);

// status info functions
inline bool is_known(const file_status & s) { return s.type() != status_unknown; }
inline bool exists(const file_status & s) { return is_known(s) && s.type() != file_not_found; }
inline bool is_file(const file_status & s) { return is_known(s) && (s.type() & file_regular) == file_regular; }
inline bool is_directory(const file_status & s) { return is_known(s) && (s.type() & file_directory) == file_directory; }
inline bool is_temporary(const file_status & s) { return is_known(s) && (s.type() & file_temporary) == file_temporary; }
inline bool is_symlink(const file_status & s) { return is_known(s) && (s.type() & file_symlink) == file_symlink; }

// shortcut status functions
bool exists(const path & fpath);
bool is_directory(const path & fpath);
bool is_file(const path & fpath);
bool is_temporary(const path & fpath);
bool is_symlink(const path & fpath);

// file operations

// file copy
int copy(const path & from, const path & to);
// file rename/move
int move(const path & from, const path & to);
// file delete
int remove(const path & fpath);
// recursive delete
int remove_all(const path & fpath);
// dir creation
int create_directory(const path & fpath);
// recussive dir creation
int create_directories(const path & fpath);


struct directory_entry {
string_type	name;
file_status	status;
};

typedef std::vector<directory_entry> directory_list;
typedef directory_list::const_iterator directory_iterator;

int find(const path & dir, const string_type & glob, directory_list & list);
void sort(directory_list & list);

} // namespace filesystem

#endif // __LIB_FILESYSTEM_H__
