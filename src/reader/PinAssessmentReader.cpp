#include "reader/PinAssessmentReader.h"
#include "rsyn/io/parser/lef_def/LEFControlParser.h"
#include "rsyn/util/Stepwatch.h"
#include "rsyn/util/ScopeTimer.h"
#include "pac/PinExpand.h"
#include "pac/MacroScore.h"
#include "pac/PinScore.h"
#include "pac.h"

#include <boost/filesystem.hpp>
#include <numeric>
#include <fstream>

namespace Rsyn {

namespace {

void printMacros(const LefMacroDscp& macro)
{
    std::cout << "Macro " << macro.clsMacroName << ": pin num: " << macro.clsPins.size() << '\n';
    for (auto& pin : macro.clsPins)
    {
        printf("  Pin %s, dir %s, use %s, port num %lu\n", pin.clsPinName.c_str(), 
                pin.clsPinDirection.c_str(), pin.clsPinUse.c_str(), pin.clsPorts.size());

        for (auto& port : pin.clsPorts)
        {
            for (auto& port_geo : port.clsLefPortGeoDscp)
            {
                printf("    Layer %s\n", port_geo.clsMetalName.c_str());
                for (auto& bound : port_geo.clsBounds)
                {
                    std::cout << "      " << bound << '\n';
                }
            }
        }
    }
}

std::string pinAccessCheck(const LefDscp& lef, bool needExpand) {
    std::string res = "[\n";
    for (auto& macro : lef.clsLefMacroDscps)
    {
        MacroScore macroScore(macro, needExpand);
        macroScore.calc();
        res += macroScore.toString() + ", \n";

        // printMacros(macro);
    }
    if (!lef.clsLefMacroDscps.empty())
    {
        res = res.substr(0, res.size() - 3);
    }

    res += "\n]";
    return res;
}

} // namespace

bool PinAssessmentReader::load(const Rsyn::Json& params) {
	std::string path = params.value("path", "");
    needExpand_ = params.value<bool>("expand", true);
    
    double minWidth = params.value<double>("min_width", 0.01);
    double minSpace = params.value<double>("min_space", 0.01);
    setDesignRules(minWidth, minSpace);

    if (path.back() != '/') {
        path += "/";
    }

    if (!params.count("lefFiles")) {
        std::cout << "[ERROR] at least one LEF file must be specified...\n";
        return false;
    }

    if (params["lefFiles"].is_array()) {
        const Rsyn::Json fileList = params["lefFiles"];
        for (const std::string file : fileList) {
            lefFiles.push_back(path + std::string(file));
        }
    } else {
        lefFiles.push_back(path + params.value("lefFiles", ""));
    }

	parsingFlow();
	return true;
}

void PinAssessmentReader::parsingFlow() {
	parseLefFiles();
    score_ = pinAccessCheck(lefDescriptor, needExpand_);
}

void PinAssessmentReader::parseLefFiles() {
	LEFControlParser lefParser;

	for (int i = 0; i < lefFiles.size(); i++) {
		if (!boost::filesystem::exists(lefFiles[i])) {
			std::cout << "[WARNING] Failed to open file " << lefFiles[i] << "\n";
			std::exit(1);
		}
		lefParser.parseLEF(lefFiles[i], lefDescriptor);
	}
}

std::string PinAssessmentReader::getResult() const {
    return score_;
}

} // namespace Rsyn

std::string calcPinScore(const std::string& input) { 
    Rsyn::PinAssessmentReader reader{};
    if (reader.load(Rsyn::Json::parse(input))) {
        return reader.getResult();
    }
    return "";
}
