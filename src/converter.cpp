#include "converter.h"
#include <bitset>
#include <iostream>

toml::value Converter::JsonToToml(const rapidjson::Document& document) {
  toml::value t;
  if(!document.IsNull()) {
    if (document.IsArray()) {
      t = toml::array();
      for (rapidjson::Value::ConstValueIterator itr = document.Begin(); itr != document.End(); ++itr) {
        t.as_array().push_back(JsonToToml(*itr));
      }
    } else {
      t = toml::table();
      for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr) {
        t.as_table().emplace(std::pair<toml::key, toml::value>(itr->name.GetString(), JsonToToml(itr->value)));
      }
    }
  } else {
    t = toml::table(); // return empty table
  }
  return t;
}

rapidjson::Document Converter::TomlToJson(const toml::value& toml_val) {
  auto serializer = JsonSerializer(); // This helps to converting toml to json.
  auto json_val = toml::visit(serializer, toml_val);
  rapidjson::Document ret;
  ret.CopyFrom(json_val, ret.GetAllocator());
  return ret;
}

toml::value Converter::JsonToToml(const rapidjson::Value& object) {
  toml::value ret;
  switch (object.GetType())
  {
  case rapidjson::kNumberType:
    if (object.IsDouble()) {
      ret = object.GetDouble();
    } else if (object.IsFloat()) {
      ret = object.GetFloat();
    } else if (object.IsInt()) {
      ret = object.GetInt();
    } else if (object.IsUint()) {
      ret = object.GetUint();
    } else if (object.IsInt64()) {
      ret = object.GetInt64();
    } else if (object.IsUint64()) {
      ret = object.GetUint64();
    }
    break;
  case rapidjson::kTrueType:
  case rapidjson::kFalseType:
    ret = object.GetBool();
    break;
  case rapidjson::kStringType:
    ret = object.GetString();
    break;
  case rapidjson::kArrayType:
    {
      toml::array arr;
      auto json_arr = object.GetArray();
      arr.reserve(json_arr.Size());
      for (auto& v : json_arr) {
        arr.emplace_back(JsonToToml(v));
      }
      ret = arr;
    }      
    break;
  case rapidjson::kObjectType:
    { 
      if (IsLocalTime(object)) {
        ret = ConvertLocalTime(object);
      } else if (IsLocalDate(object)) {
        ret = ConvertLocalDate(object);
      } else if (IsOffsetDateTime(object)) {
        ret = ConvertOffsetDateTime(object);
      } else if (IsLocalDateTime(object)) {
        ret = ConvertLocalDateTime(object);
      } else {
        toml::table t;
        for (rapidjson::Value::ConstMemberIterator itr = object.MemberBegin(); itr != object.MemberEnd(); ++itr) {
          t.emplace(std::pair<toml::key, toml::value>(itr->name.GetString(), JsonToToml(itr->value)));
        }
        ret = t;
      }        
    }      
    break;
  default:
    std::cerr << "Unknown json type " << object.GetType() << std::endl;
    break;
  }
  return ret;
}

bool Converter::IsLocalTime(const rapidjson::Value& v) {
  std::bitset<6> fulfil_checker;
  for (rapidjson::Value::ConstMemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr) {
    if (!strcmp(itr->name.GetString(), "hour")) {
      fulfil_checker.set(0);
    } else if (!strcmp(itr->name.GetString(), "minute")) {
      fulfil_checker.set(1);
    } else if (!strcmp(itr->name.GetString(), "second")) {
      fulfil_checker.set(2);
    } else if (!strcmp(itr->name.GetString(), "millisecond")) {
      fulfil_checker.set(3);
    } else if (!strcmp(itr->name.GetString(), "microsecond")) {
      fulfil_checker.set(4);
    } else if (!strcmp(itr->name.GetString(), "nanosecond")) {
      fulfil_checker.set(5);
    } else {
      fulfil_checker.reset();
      break;
    }
  }
  return fulfil_checker.all();
}

bool Converter::IsLocalDate(const rapidjson::Value& v) {
  std::bitset<3> fulfil_checker;
  for (rapidjson::Value::ConstMemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr) {
    if (!strcmp(itr->name.GetString(), "year")) {
      fulfil_checker.set(0);
    } else if (!strcmp(itr->name.GetString(), "month")) {
      fulfil_checker.set(1);
    } else if (!strcmp(itr->name.GetString(), "day")) {
      fulfil_checker.set(2);
    } else {
      fulfil_checker.reset();
      break;
    }
  }
  return fulfil_checker.all();
}

bool Converter::IsLocalDateTime(const rapidjson::Value& v) {
  std::bitset<9> fulfil_checker;
  for (rapidjson::Value::ConstMemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr) {
    if (!strcmp(itr->name.GetString(), "year")) {
      fulfil_checker.set(0);
    } else if (!strcmp(itr->name.GetString(), "month")) {
      fulfil_checker.set(1);
    } else if (!strcmp(itr->name.GetString(), "day")) {
      fulfil_checker.set(2);
    } else if (!strcmp(itr->name.GetString(), "hour")) {
      fulfil_checker.set(3);
    } else if (!strcmp(itr->name.GetString(), "minute")) {
      fulfil_checker.set(4);
    } else if (!strcmp(itr->name.GetString(), "second")) {
      fulfil_checker.set(5);
    } else if (!strcmp(itr->name.GetString(), "millisecond")) {
      fulfil_checker.set(6);
    } else if (!strcmp(itr->name.GetString(), "microsecond")) {
      fulfil_checker.set(7);
    } else if (!strcmp(itr->name.GetString(), "nanosecond")) {
      fulfil_checker.set(8);
    } else {
      fulfil_checker.reset();
      break;
    }
  }
  return fulfil_checker.all();
}

bool Converter::IsOffsetDateTime(const rapidjson::Value& v) {
  std::bitset<11> fulfil_checker;
  for (rapidjson::Value::ConstMemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr) {
    if (!strcmp(itr->name.GetString(), "year")) {
      fulfil_checker.set(0);
    } else if (!strcmp(itr->name.GetString(), "month")) {
      fulfil_checker.set(1);
    } else if (!strcmp(itr->name.GetString(), "day")) {
      fulfil_checker.set(2);
    } else if (!strcmp(itr->name.GetString(), "hour")) {
      fulfil_checker.set(3);
    } else if (!strcmp(itr->name.GetString(), "minute")) {
      fulfil_checker.set(4);
    } else if (!strcmp(itr->name.GetString(), "second")) {
      fulfil_checker.set(5);
    } else if (!strcmp(itr->name.GetString(), "millisecond")) {
      fulfil_checker.set(6);
    } else if (!strcmp(itr->name.GetString(), "microsecond")) {
      fulfil_checker.set(7);
    } else if (!strcmp(itr->name.GetString(), "nanosecond")) {
      fulfil_checker.set(8);
    } else if (!strcmp(itr->name.GetString(), "offset_hour")) {
      fulfil_checker.set(9);
    } else if (!strcmp(itr->name.GetString(), "offset_minute")) {
      fulfil_checker.set(10);
    } else {
      fulfil_checker.reset();
      break;
    }
  }
  return fulfil_checker.all();
}

toml::local_time Converter::ConvertLocalTime(const rapidjson::Value& v) {
  toml::local_time ret;
  for (rapidjson::Value::ConstMemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr) {
    if (!strcmp(itr->name.GetString(), "hour")) {
      ret.hour = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "minute")) {
      ret.minute = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "second")) {
      ret.second = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "millisecond")) {
      ret.millisecond = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "microsecond")) {
      ret.microsecond = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "nanosecond")) {
      ret.nanosecond = itr->value.GetUint();
    } 
  }
  return ret;
}

toml::local_date Converter::ConvertLocalDate(const rapidjson::Value& v) {
  toml::local_date ret;
  for (rapidjson::Value::ConstMemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr) {
    if (!strcmp(itr->name.GetString(), "year")) {
      ret.year = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "month")) {
      ret.month = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "day")) {
      ret.day = itr->value.GetUint();
    } else {
      break;
    }
  }
  return ret;
}

toml::local_datetime Converter::ConvertLocalDateTime(const rapidjson::Value& v) {
  toml::local_datetime ret;
  for (rapidjson::Value::ConstMemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr) {
    if (!strcmp(itr->name.GetString(), "year")) {
      ret.date.year = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "month")) {
      ret.date.month = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "day")) {
      ret.date.day = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "hour")) {
      ret.time.hour = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "minute")) {
      ret.time.minute = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "second")) {
      ret.time.second = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "millisecond")) {
      ret.time.millisecond = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "microsecond")) {
      ret.time.microsecond = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "nanosecond")) {
      ret.time.nanosecond = itr->value.GetUint();
    } else {
      break;
    }
  }
  return ret;
}

toml::offset_datetime Converter::ConvertOffsetDateTime(const rapidjson::Value& v) {
  toml::offset_datetime ret;
  for (rapidjson::Value::ConstMemberIterator itr = v.MemberBegin(); itr != v.MemberEnd(); ++itr) {
    if (!strcmp(itr->name.GetString(), "year")) {
      ret.date.year = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "month")) {
      ret.date.month = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "day")) {
      ret.date.day = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "hour")) {
      ret.time.hour = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "minute")) {
      ret.time.minute = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "second")) {
      ret.time.second = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "millisecond")) {
      ret.time.millisecond = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "microsecond")) {
      ret.time.microsecond = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "nanosecond")) {
      ret.time.nanosecond = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "offset_hour")) {
      ret.offset.hour = itr->value.GetUint();
    } else if (!strcmp(itr->name.GetString(), "offset_minute")) {
      ret.offset.minute = itr->value.GetUint();
    } else {
      break;
    }
  }
  return ret;
}

rapidjson::Value Converter::JsonSerializer::operator()(const toml::boolean& v) {
  return rapidjson::Value(v);
}

rapidjson::Value Converter::JsonSerializer::operator()(const toml::integer& v) {
  return rapidjson::Value(v);
}

rapidjson::Value Converter::JsonSerializer::operator()(const toml::floating& v) {
  return rapidjson::Value(v);
}

rapidjson::Value Converter::JsonSerializer::operator()(const toml::string& v) {
  // since toml11 automatically convert string to multiline string that is
  // valid only in TOML, we need to format the string to make it valid in
  // JSON.
  rapidjson::Value ret;
  std::string val = escape_string(v.str);
  ret.SetString(val.c_str(), val.length(), GetAllocator());
  return ret;
}

rapidjson::Value Converter::JsonSerializer::operator()(const toml::local_time& v) {
  rapidjson::Value ret;
  ret.SetObject();
  ret.AddMember("hour", v.hour, GetAllocator());
  ret.AddMember("minute", v.minute, GetAllocator());
  ret.AddMember("second", v.second, GetAllocator());
  ret.AddMember("millisecond", v.millisecond, GetAllocator());
  ret.AddMember("microsecond", v.microsecond, GetAllocator());
  ret.AddMember("nanosecond", v.nanosecond, GetAllocator());
  return ret;
}

rapidjson::Value Converter::JsonSerializer::operator()(const toml::local_date& v) {    
  rapidjson::Value ret;
  ret.SetObject();
  ret.AddMember("year", v.year, GetAllocator());
  ret.AddMember("month", v.month, GetAllocator());
  ret.AddMember("day", v.day, GetAllocator());
  return ret;
}

rapidjson::Value Converter::JsonSerializer::operator()(const toml::local_datetime& v) {
  rapidjson::Value ret;
  ret.SetObject();
  ret.AddMember("year", v.date.year, GetAllocator());
  ret.AddMember("month", v.date.month, GetAllocator());
  ret.AddMember("day", v.date.day, GetAllocator());
  ret.AddMember("hour", v.time.hour, GetAllocator());
  ret.AddMember("minute", v.time.minute, GetAllocator());
  ret.AddMember("second", v.time.second, GetAllocator());
  ret.AddMember("millisecond", v.time.millisecond, GetAllocator());
  ret.AddMember("microsecond", v.time.microsecond, GetAllocator());
  ret.AddMember("nanosecond", v.time.nanosecond, GetAllocator());
  return ret;
}

rapidjson::Value Converter::JsonSerializer::operator()(const toml::offset_datetime& v) {
  rapidjson::Value ret;
  ret.SetObject();
  ret.AddMember("year", v.date.year, GetAllocator());
  ret.AddMember("month", v.date.month, GetAllocator());
  ret.AddMember("day", v.date.day, GetAllocator());
  ret.AddMember("hour", v.time.hour, GetAllocator());
  ret.AddMember("minute", v.time.minute, GetAllocator());
  ret.AddMember("second", v.time.second, GetAllocator());
  ret.AddMember("millisecond", v.time.millisecond, GetAllocator());
  ret.AddMember("microsecond", v.time.microsecond, GetAllocator());
  ret.AddMember("nanosecond", v.time.nanosecond, GetAllocator());
  ret.AddMember("offset_hour", v.offset.hour, GetAllocator());
  ret.AddMember("offset_minute", v.offset.minute, GetAllocator());
  return ret;
}

rapidjson::Value Converter::JsonSerializer::operator()(const toml::array& v) {
  rapidjson::Value ret(rapidjson::kArrayType);
  ret.Reserve(v.size(), GetAllocator());
  for(const auto& elem : v) {
    ret.PushBack(toml::visit(*this, elem), GetAllocator());
  }
  return ret;
}

rapidjson::Value Converter::JsonSerializer::operator()(const toml::table& v) {
  rapidjson::Value ret(rapidjson::kObjectType);
  for(const auto& elem : v) {
    ret.AddMember(rapidjson::Value(escape_string(elem.first).c_str(), GetAllocator()), toml::visit(*this, elem.second), GetAllocator());
  }
  return ret;
}

std::string Converter::JsonSerializer::escape_string(const std::string& s) const {
  std::string retval;
  retval.reserve(s.length());
  for(const char c : s) {
    switch(c) {
      case '\\': {retval += "\\\\"; break;}
      case '\"': {retval += "\\\""; break;}
      case '\b': {retval += "\\b";  break;}
      case '\t': {retval += "\\t";  break;}
      case '\f': {retval += "\\f";  break;}
      case '\n': {retval += "\\n";  break;}
      case '\r': {retval += "\\r";  break;}
      default  : {retval += c;      break;}
    }
  }
  return retval;
}