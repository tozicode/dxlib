#pragma once

#include <vector>
#include <fstream>

#include "util.h"


namespace dxlib
{

/** バイナリファイル上で文字列を読み書きする際の、文字列長を格納する型。 */
using _str_size_t = unsigned short;


/** ファイル読み込みを行うクラス。 */
class file_reader_t
{
public:
    file_reader_t(const filepath_t &p) : m_handle(FileRead_open(p.c_str())) {}
    ~file_reader_t() { FileRead_close(m_handle); }

    /** 指定した型の変数をバイナリファイルから読み込んで返す。 */
    template <class T> T read() const
    {
        T x;
        FileRead_read(&x, sizeof(T), m_handle);
        return x;
    }

    /** 文字列をバイナリファイルから読み込んで返す。 */
    template <> tstring read<tstring>() const
    {
        _str_size_t size = read<_str_size_t>() + 1;
        std::vector<tchar> str;
        str.assign(size, _T('\0'));
        FileRead_read(&str[0], sizeof(tchar) * size, m_handle);
        return tstring(&str[0]);
    }

    template <> bool read<bool>() const
    {
        char c = read<char>();
        return (c != 0);
    }

    inline tstring readline()
    {
        tchar buf[4096];
        FileRead_gets(buf, 4096, m_handle);
        return tstring(buf);
    }

    inline int getc() const { return FileRead_getc(m_handle); }

    inline bool eof() const { return FileRead_eof(m_handle) != 0; }
    inline bool good() const { return m_handle != 0; }
    inline bool bad() const { return not good(); }

private:
    int m_handle;
};


/** ファイルの書き込みをするためのクラス。 */
class binary_writer_t : public std::ofstream
{
public:
    binary_writer_t(const filepath_t &s)
        : std::ofstream(string(s).c_str(), mode()) {}

    /** 変数 x の内容をバイナリファイルに書き込む。 */
    template <class T> size_t write(const T &x)
    {
        std::ofstream::write((char*)&x, sizeof(T));
        return sizeof(T);
    }

    /** 文字列をバイナリファイルに書き込む。 */
    template <> size_t write<tstring>(const tstring &x)
    {
        _str_size_t size = static_cast<_str_size_t>(x.size());
        write<_str_size_t>(size);
        std::ofstream::write((char*)x.c_str(), sizeof(tchar) * size);
        return sizeof(_str_size_t) + sizeof(tchar) * size;
    }

    /** ブール値をバイナリファイルに書き込む。 */
    template <> size_t write<bool>(const bool &x)
    {
        return write<char>(x ? 1 : 0);
    }

private:
    static int mode() { return (std::ios::out | std::ios::trunc | std::ios::binary); }
};

}