#pragma once
#include <map>
#include <string>
#include <vector>

class UUIDServer {
  private:
  std::map<std::string, std::vector<std::string>> uuids;
  std::string PATH = "assets/program_data/enc.enc";
  UUIDServer();
  ~UUIDServer();

  public:
  static UUIDServer &Instance() {
    static UUIDServer inst = UUIDServer();
    return inst;
  }

  void registerGroup(std::string group);
  bool isRegistered(std::string group);
  std::string getUUID(std::string group);
};