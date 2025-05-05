#include <iostream> // for cout, endl
#include <sstream> // for stringstream
#include <fstream> // for ifstream, ofstream
#include <algorithm> // for trim
#include "ini_file.h"
using std::cout;
using std::endl;
using std::to_string;
using namespace wuah::iniparser;


Value::Value(int value)
{
    *this = value;
}

Value::Value(double value)
{
    *this = value;
}

Value::Value(bool value)
{
    *this = value;
}

Value::Value(const char* value)
{
    *this = value;
}

Value::Value(const string& value)
{
    *this = value;
}

Value& Value::operator=(int value)
{
    m_value = to_string(value);
    return *this;
}   

Value& Value::operator=(double value)
{
    // double类型不支持stringstream，因此先转换为stringstream再转换为string
    std::stringstream ss;
    ss << value;
    m_value = ss.str();
    return *this;
}

Value& Value::operator=(bool value)
{
    m_value = value ? "true" : "false";
    return *this;
}
   
Value& Value::operator=(const char* value)
{
    if(value == nullptr)
    {
        m_value = "";
    }
    else
    {
        m_value = value;
    }
    return *this;
}
   
Value& Value::operator=(const string& value)
{
    m_value = value;
    return *this;
}

Value::operator int() const
{
    return stoi(m_value);
}

Value::operator double() const
{
    return stod(m_value);
}

Value::operator bool() const
{
    return (m_value == "true") ? true : false;
}

Value::operator string() const
{
    return m_value;
}

string& Value::value()
{
    return m_value;
}

void IniFile::trim(string& str)
{
    str.erase(0, str.find_first_not_of(" \t\n\r"));
    str.erase(str.find_last_not_of(" \t\n\r") + 1);
}

bool IniFile::load_file(const string& filename)
{
    m_filename = filename; // 保存文件名

    std::ifstream fileinput(filename);
    if(!fileinput.is_open())
    {
        cout << "Failed to open file: " << filename << endl;
        return false;
    }

    string line; // 读取每行内容
    string section_name; // 当前section名称
    while(std::getline(fileinput, line))
    {
        // 如果放在下面的if语句之后，会导致仅包含空格、制表符的行被trim之后，被当作section处理
        trim(line); // 去除行首尾的空白字符
        
        // 跳过空行、注释行和仅包含空格、制表符（经过trim后）的行
        if(line.empty() || line[0] == '#')
        {
            continue;
        }

        if(line[0] == '[')
        {
            // 处理section
            int pos = line.find(']');
            if(pos == string::npos)
            {
                cout << "Invalid section: " << line << endl;
                return false;
            }

            // 保存当前section名称
            section_name = line.substr(1, pos - 1);
            trim(section_name); // 去除section名称的空白字符

            // 如果section对象不存在，则创建
            if(m_sections.find(section_name) == m_sections.end())
            {
                Section section;
                m_sections[section_name] = section;
            }
        }
        else
        {
            // 处理key-value
            int pos = line.find('=');
            if(pos == string::npos)
            {
                cout << "Invalid key-value: " << line << endl;
                return false;
            }
            // 保存key-value
            string key = line.substr(0, pos);
            string value = line.substr(pos + 1);
            trim(key); // 去除key的空白字符
            trim(value); // 去除value的空白字符

            // 保存到section中
            m_sections[section_name][key] = value;
        }       
    }

    fileinput.close();
    return true;
}

string IniFile::file_to_string() const
{
    std::stringstream ss;
    for(auto it = m_sections.begin(); it != m_sections.end(); ++it)
    {
        ss << "[" << it->first << "]" << endl;
        for(auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
        {
            ss << it2->first << "=" << string(it2->second) << endl; // 把Value转为string类型
        }
        ss << endl;
    }
    return ss.str();
}

void IniFile::show_all() const
{
    cout << file_to_string() << endl;
}

bool IniFile::save_file(const string& filename) const
{
    std::ofstream fileoutput(filename); // 打开要保存的文件
    if(!fileoutput.is_open())
    {
        cout << "Failed to open file: " << filename << endl;
        return false;
    }

    fileoutput << file_to_string(); // 把file_to_string()的结果写入文件
    fileoutput.close();
    return true;
}

bool IniFile::has_section(const string& section) const
{
    return (m_sections.find(section) != m_sections.end());
}

bool IniFile::has_value(const string& section, const string& key) const
{
    std::unordered_map<string, Section>::const_iterator it = m_sections.find(section);
    if(it == m_sections.end()) // section不存在
    {
        return false;
    }
    return (it->second.find(key) != it->second.end());
}

Value& IniFile::get_value(const string& section, const string& key) 
{
    if(has_value(section, key))
    {
        return m_sections[section][key]; // 如果value存在，则返回value对象
    }
    else
    {
        throw std::out_of_range("Value not found");
    }
}

Section& IniFile::operator[](const string& section)
{
    if(has_section(section))
    {
        return m_sections[section]; // 如果section存在，则返回section对象
    }
    else
    {
        throw std::out_of_range("Section not found");
    }
}

void IniFile::set_value(const string& section, const string& key, const Value& value)
{
    if(has_value(section, key))
    {
        m_sections[section][key] = value; // 如果value存在，则更新value
    }
    else
    {
        throw std::out_of_range("Value not found");
    }
}

void IniFile::remove_section(const string& section)
{
    if(has_section(section))
    {
        m_sections.erase(section);
    }
}

void IniFile::remove_value(const string& section, const string& key)
{
    if(has_value(section, key))
    {
        m_sections[section].erase(key);
    }
}

void IniFile::clear()
{
    m_sections.clear();
}

