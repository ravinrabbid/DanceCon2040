#ifndef PS4AUTHCONFIGURATION_H_
#define PS4AUTHCONFIGURATION_H_

#include "utils/PS4AuthProvider.h"

namespace Dancecon::Config::PS4Auth {

const Utils::PS4AuthProvider::Config config = {.enabled = false,        //
                                               .serial = {},            //
                                               .signature = {},         //
                                               .key_pem = R"pem()pem"}; //

} // namespace Dancecon::Config::PS4Auth

#endif // PS4AUTHCONFIGURATION_H_