# toml11tojson
A json converter util class for toml11.

## Interface
- Converter::TomlToJson : Convert toml::value to rapidjson::Document
- Converter::JsonToToml : Convert rapidjson::Document to toml::value 

## How to use
1. Copy converter.cpp and converter.h into your project.
2. Copy rapidjson include files(external/rapidjson/include) into your project. 

## Dependency
[RapidJson](https://github.com/Tencent/rapidjson/)

## Example test code
**Code**
```
#include "converter.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
  std::string filename = "example.toml";
  if (argc > 1) {
    filename = argv[1];
  }
  auto data = toml::parse(filename);
  // width-dependent formatting
  std::cout << "Input toml" << std::endl;
  std::cout << std::setw(80) << data << std::endl;
  auto d = Converter::TomlToJson(data);
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  d.Accept(writer);
  std::cout << "Output Json" << std::endl;
  std::cout << buffer.GetString() << std::endl;
  
  return 0;
}
```
**Result**
```
Input toml
title = "TOML Example"
owner = {name="Tom Preston-Werner",dob=1979-05-27T07:32:00-08:00}
[database]
server = "192.168.1.1"
ports = [8001,8001,8002]
connection_max = 5000
enabled = true
[servers]
alpha = {ip="10.0.0.1",dc="eqdc10"}
beta = {ip="10.0.0.2",dc="eqdc10"}

[clients]
data = [["gamma","delta"],[1,2]]
hosts = ["alpha","omega"]


Output Json
{"title":"TOML Example","owner":{"name":"Tom Preston-Werner","dob":{"year":1979,"month":4,"day":27,"hour":7,"minute":32,"second":0,"millisecond":0,"microsecond":0,"nanosecond":0,"offset_hour":-8,"offset_minute":0}},"database":{"server":"192.168.1.1","ports":[8001,8001,8002],"connection_max":5000,"enabled":true},"servers":{"alpha":{"ip":"10.0.0.1","dc":"eqdc10"},"beta":{"ip":"10.0.0.2","dc":"eqdc10"}},"clients":{"data":[["gamma","delta"],[1,2]],"hosts":["alpha","omega"]}}

```
