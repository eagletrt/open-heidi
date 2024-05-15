#include <cstring>
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "requests/requests.h"
#include "requests/openid_configuration.h"
#include "requests/device.h"

int main() {

  std::string discovery_url = "https://idp.eagletrt.it/realms/master/.well-known/openid-configuration";
  std::string client_id = "c3363912f2d069ed92e4b4b63612f4da";
  std::vector<std::string> scopes = {"openid", "email", "profile", "offline_access"};

  auto discovery_response = request_discover_openid_configuration(discovery_url);
  auto device_response = request_device_flow_init(
    std::string(discovery_response.body["device_authorization_endpoint"]),
    client_id,
    scopes
  );

  std::cout << std::string(device_response.body["user_code"]) << std::endl;
}

