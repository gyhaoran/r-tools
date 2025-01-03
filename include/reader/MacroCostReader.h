#ifndef A723C2E5_DD55_4F7F_8D9E_EEA612D1B11B
#define A723C2E5_DD55_4F7F_8D9E_EEA612D1B11B

#include "rsyn/phy/util/LefDescriptors.h"
#include "rsyn/model/timing/types.h"
#include "rsyn/util/Json.h"

namespace Rsyn {

class MacroCostReader {
public:
    MacroCostReader() = default;
    bool load(const Rsyn::Json& params);
    std::string getResult() const;

private:
    std::vector<std::string> lefFiles_;	
    LefDscp lefDescriptor_;
    double minWidth_{0.01};
    std::string cost_{""};

    void parsingFlow();
    void parseLefFiles();
};

} // namespace Rsyn

#endif /* A723C2E5_DD55_4F7F_8D9E_EEA612D1B11B */
