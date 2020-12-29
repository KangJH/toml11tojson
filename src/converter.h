#pragma once
#include "rapidjson/document.h"
#include "toml.hpp"

class Converter {
public:
  static toml::value JsonToToml(const rapidjson::Document& document);
  static rapidjson::Document TomlToJson(const toml::value& toml_val);
private:
  static toml::value JsonToToml(const rapidjson::Value& object);
  static bool IsLocalTime(const rapidjson::Value& v);
  static bool IsLocalDate(const rapidjson::Value& v);
  static bool IsLocalDateTime(const rapidjson::Value& v);
  static bool IsOffsetDateTime(const rapidjson::Value& v);
  static toml::local_time ConvertLocalTime(const rapidjson::Value& v);
  static toml::local_date ConvertLocalDate(const rapidjson::Value& v);
  static toml::local_datetime ConvertLocalDateTime(const rapidjson::Value& v);
  static toml::offset_datetime ConvertOffsetDateTime(const rapidjson::Value& v);
  // Serializer for toml to json
  class JsonSerializer {
  public:
    rapidjson::Value operator()(const toml::boolean& v);
    rapidjson::Value operator()(const toml::integer& v);
    rapidjson::Value operator()(const toml::floating& v);
    rapidjson::Value operator()(const toml::string& v);
    rapidjson::Value operator()(const toml::local_time& v);
    rapidjson::Value operator()(const toml::local_date& v);
    rapidjson::Value operator()(const toml::local_datetime& v);
    rapidjson::Value operator()(const toml::offset_datetime& v);
    rapidjson::Value operator()(const toml::array& v);
    rapidjson::Value operator()(const toml::table& v);
  private:
    std::string escape_string(const std::string& s) const;
    rapidjson::Document::AllocatorType& GetAllocator() { return document_.GetAllocator(); }
    rapidjson::Document document_;
  };
};