#include <string>
#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <unistd.h>

#include "requests/requests.h"
#include "requests/openid_configuration.h"
#include "requests/device.h"
#include "requests/token.h"

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

  std::cout << std::string(device_response.body["verification_uri_complete"]) << std::endl;

  // get int from json with key "interval"
  int interval = device_response.body["interval"];

  device_flow_params_t device_params {
    std::string(device_response.body["device_code"])
  };

  response_t token_response;
  
  do {
    std::cout << "Waiting for user to authenticate..." << std::endl;
    sleep(interval);
    token_response = request_token(
      std::string(discovery_response.body["token_endpoint"]),
      client_id,
      device_params
    );
  } while (token_response.code != 200);

  std::cout << "User authenticated!" << std::endl;

  response_t refresh_response;
  size_t token_expiration = token_response.body["expires_in"];
  
  do {
    sleep(token_expiration - 10);

    refresh_flow_params_t refresh_params {
      std::string(token_response.body["refresh_token"])
    };

    refresh_response = request_token(
      std::string(discovery_response.body["token_endpoint"]),
      client_id,
      refresh_params
    );

    if (refresh_response.code == 200) {
      std::cout << "Token refreshed!" << std::endl;
      token_response = refresh_response;
      token_expiration = token_response.body["expires_in"];
    } 

  } while (refresh_response.code == 200);

  std::cout << refresh_response.body << std::endl;

  std::cout << "Token expired!" << std::endl;
}

