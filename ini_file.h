#pragma once

#include <string>
#include <unordered_map>
using std::string;

namespace wuah
{
namespace iniparser
{

class Value
{
public:
    Value() = default;
    Value(const Value&) = default;
    Value& operator=(const Value&) = default;
    ~Value() = default;

    // 重载构造函数
    Value(int value);
    Value(double value);
    Value(bool value);
    Value(const char* value);
    Value(const string& value);

    // 重载赋值操作符
    Value& operator=(int value);
    Value& operator=(double value);
    Value& operator=(bool value);
    Value& operator=(const char* value);
    Value& operator=(const string& value);

    // 重载类型转换操作符
    operator int() const;
    operator double() const;
    operator bool() const;
    operator string() const;

    string& value(); // 返回值字符串

private:
    string m_value; // 值字符串
};

typedef std::unordered_map<string, Value> Section; // section 是一个 key-value 对的集合

class IniFile
{
public:
    IniFile() = default;    
    IniFile(const IniFile&) = default;
    IniFile& operator=(const IniFile&) = default;
    ~IniFile() = default;

    void trim(string& str); // 去除字符串两端的空白字符
    bool load_file(const string& filename); // 加载 ini 文件

    string file_to_string() const; // 将 IniFile 文件转换为字符串
    void show_all() const; // 显示所有 section 和 key-value 对
    bool save_file(const string& filename) const; // 保存 IniFile 文件

    bool has_section(const string& section) const;
    bool has_value(const string& section, const string& key) const;

    // 获取某个 section 下的某个 key 的值
    Value& get_value(const string& section, const string& key);
    // 获取某个 section 下的所有 key-value 对
    Section& operator[](const string& section);

    void set_value(const string& section, const string& key, const Value& value);

    void remove_section(const string& section);
    void remove_value(const string& section, const string& key);
    void clear();

private:
    string m_filename; // ini 文件名
    std::unordered_map<string, Section> m_sections; // section 集合
};

} // namespace iniparser
} // namespace wuah