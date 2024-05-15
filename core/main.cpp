#include <cstring>
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "requests/requests.h"
#include "requests/device.h"

int main() {

  std::string url = "https://idp.eagletrt.it/realms/master/protocol/openid-connect/auth/device";
  std::string client_id = "c3363912f2d069ed92e4b4b63612f4da";
  std::vector<std::string> scopes = {"openid", "email", "profile", "offline_access"};
 
  auto response = request_device_flow_init(url, client_id, scopes);

  std::cout << response.body["user_code"] << std::endl;
}

