#include "converter.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
  std::string filename = "C:\\Workspace\\toml11tojson\\build\\bin\\Debug\\example.toml";
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