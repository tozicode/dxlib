#include <cstring>
#include <limits>
#include <Windows.h>

#include "util.h"


namespace dxlib
{

string_t string_t::slice(int i, int j) const
{
    while (i < 0) i += size();
    while (j <= 0) j += size();
    assert(i < j);

    return substr(i, j - i);
}


std::vector<string_t> string_t::split(const tchar *sep, const int MAX_NUM) const
{
    auto _find_split_index = [this](const tchar *sep, int begin) -> index_t
    {
        for (size_t i = begin; i < size(); ++i)
        {
#ifdef UNICODE
            if (wcschr(sep, at(i)) != NULL)
#else
            if (strchr(sep, at(i)) != NULL)
#endif
                return static_cast<int>(i);
        }
        return -1;
    };

    std::vector<string_t> out;
    int idx(0);

    while (idx < static_cast<int>(size()))
    {
        int idx2(_find_split_index(sep, idx));

        if (idx2 < 0)
            idx2 = static_cast<int>(size());

        if (idx2 - idx > 0)
        {
            if (MAX_NUM > 0 and static_cast<int>(out.size()) >= MAX_NUM)
                idx2 = static_cast<int>(size());
            out.push_back(substr(idx, idx2 - idx));
        }

        idx = idx2 + 1;
    }

    return out;
}


string_t string_t::replace(const tstring &from, const tstring &to) const
{
    size_t pos(0);
    string_t out(*this);

    if (from.empty()) return out;

    while ((pos = out.find(from, pos)) != std::string::npos)
    {
        out.tstring::replace(pos, from.length(), to);
        pos += to.length();
    }

    return out;
}


bool string_t::startswith(const tstring &s) const
{
    if (size() < s.size()) return false;

    for (size_t i = 0; i < s.size(); ++i)
        if (at(i) != s.at(i))
            return false;

    return true;
}


bool string_t::endswith(const tstring &s) const
{
    if (size() < s.size()) return false;

    for (size_t i = 1; i <= s.size(); ++i)
        if (at(size() - i) != s.at(s.size() - i))
            return false;

    return true;
}


filepath_t filepath_t::module_file_path()
{
    tchar path[4096];
    if (GetModuleFileName(NULL, path, 4096))
    {
        return filepath_t(path);
    }
    else
    {
        ErrorLogAdd(_T("ERROR: GetModuleFileName was failed!"));
        return _T("");
    }
}


filepath_t filepath_t::module_file_dir()
{
    return module_file_path().dir();
}


filepath_t filepath_t::dir() const
{
    auto i = rfind(_T('\\'));
    return (i != tstring::npos) ? substr(0, i) : _T(".");
}


filepath_t filepath_t::extension() const
{
    auto i = rfind(_T('.'));
    return (i != tstring::npos) ? substr(i + 1) : _T("");
}


filepath_t filepath_t::relative_path_from(const filepath_t &p) const
{
    return replace(p, _T("."));
}


filepath_t filepath_t::absolute_path_with(const filepath_t &p) const
{
    if (front() == _T('.'))
        return p + substr(1);
    else
        return (*this);
}


const position_t NPOS = position_t(INT_MIN, INT_MIN);


}
