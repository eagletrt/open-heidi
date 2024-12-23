#include <curl/curl.h>
#include <unistd.h>

#include <cstdlib>
#include <format>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

#include "requests/device.h"
#include "requests/openid_configuration.h"
#include "requests/requests.h"
#include "requests/token.h"
#include "server.h"

/*
int main() {
  std::string discovery_url =
      "https://idp.eagletrt.it/realms/master/.well-known/openid-configuration";
  std::string client_id = "c3363912f2d069ed92e4b4b63612f4da";
  std::vector<std::string> scopes = {"openid", "email", "profile",
                                     "offline_access"};

  auto discovery_response =
      request_discover_openid_configuration(discovery_url);
  auto device_response = request_device_flow_init(
      std::string(discovery_response.body["device_authorization_endpoint"]),
      client_id, scopes);

  std::cout << std::string(device_response.body["verification_uri_complete"])
            << std::endl;
  system(
      ("open " + std::string(device_response.body["verification_uri_complete"]))
          .c_str());

  // get int from json with key "interval"
  int interval = device_response.body["interval"];

  device_flow_params_t device_params{
      std::string(device_response.body["device_code"])};

  response_t token_response;
  do {
    std::cout << "Waiting for user to authenticate..." << std::endl;
    sleep(interval);
    token_response =
        request_token(std::string(discovery_response.body["token_endpoint"]),
                      client_id, device_params);
  } while (token_response.code != 200);

  std::cout << "User authenticated!" << std::endl;

  response_t refresh_response;
  size_t token_expiration = token_response.body["expires_in"];

  do {
    sleep(token_expiration - 10);

    refresh_flow_params_t refresh_params{
        std::string(token_response.body["refresh_token"])};

    refresh_response =
        request_token(std::string(discovery_response.body["token_endpoint"]),
                      client_id, refresh_params);

    if (refresh_response.code == 200) {
      std::cout << "Token refreshed!" << std::endl;
      token_response = refresh_response;
      token_expiration = token_response.body["expires_in"];
    }

  } while (refresh_response.code == 200);

  std::cout << refresh_response.body << std::endl;

  std::cout << "Token expired!" << std::endl;
}
*/

int main() {
  CallbackResponse params;
  std::shared_ptr<CallbackResponse> ptr =
      std::make_shared<CallbackResponse>(params);

  std::string client_id =
      "63544120325-iheof77sq00leincpqpsh6ude30slcf1.apps.googleusercontent.com";

  AuthCallbackServer srv(client_id);
  std::string url = srv.start();

  std::string command = std::format("open \"{}\"", url);

  std::cout << "Using command \"" << command << "\"" << std::endl;

  system(command.c_str());

  std::cout << "Server started" << std::endl;

  CallbackResponse response;
  try {
    response = srv.wait_response(10);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    srv.stop();
    return 1;
  }

  srv.stop();

  std::cout << "Response received" << std::endl;
  std::cout << "Scope: " << response.scope << std::endl;
  std::cout << "State: " << response.state << std::endl;
  std::cout << "Code: " << response.code << std::endl;

  std::cout << "Server stopped" << std::endl;
}
