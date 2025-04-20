#include "utils/PS4AuthProvider.h"

namespace Dancecon::Config::PS4Auth {

const Utils::PS4AuthProvider::Config config = {false,        // Enabled
                                               {},           // Serial
                                               {},           // Signature
                                               R"pem()pem"}; // Pem

} // namespace Dancecon::Config::PS4Auth