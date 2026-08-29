//
// Created by zxzzsdb on 2026/8/25.
//配置读取类
//

#ifndef GATESERVER_CONFIGMGR_H
#define GATESERVER_CONFIGMGR_H
#include "const.h"
struct SectionInfo {
    SectionInfo();
    ~SectionInfo();

    SectionInfo(const SectionInfo& src);

    SectionInfo& operator = (const SectionInfo& src);

    std::map<std::string, std::string> _section_datas;
    std::string  operator[](const std::string  &key);
};
class ConfigMgr
{
public:
    ~ConfigMgr() {
        _config_map.clear();
    }
    SectionInfo operator[](const std::string& section) {
        if (_config_map.find(section) == _config_map.end()) {
            return SectionInfo();
        }
        return _config_map[section];
    }

//    ConfigMgr& operator=(const ConfigMgr& src) {
//        if (&src == this) {
//            return *this;
//        }
//
//        this->_config_map = src._config_map;
//        return *this;
//    };
//
//    ConfigMgr(const ConfigMgr& src) {
//        this->_config_map = src._config_map;
//    }
    ConfigMgr(const ConfigMgr& src) = delete;
    ConfigMgr& operator=(const ConfigMgr& src) = delete;
    static ConfigMgr& Inst() {
        static ConfigMgr cfg_mgr;
        return cfg_mgr;
    }

private:
    ConfigMgr();
    // 存储section和key-value对的map
    std::map<std::string, SectionInfo> _config_map;
};


#endif //GATESERVER_CONFIGMGR_H
