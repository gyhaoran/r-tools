#include "reader/MacroCostReader.h"
#include "rsyn/io/parser/lef_def/LEFControlParser.h"
#include "rsyn/util/Stepwatch.h"
#include "rsyn/util/ScopeTimer.h"
#include "pac/MacroCost.h"
#include "pac.h"
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

namespace Rsyn
{

namespace
{

std::string calcMacroCost(const LefDscp& lef, double minWidth) {
    std::stringstream ss;
    ss << "[\n";
    for (auto& macro : lef.clsLefMacroDscps)
    {
        MacroCost macroCost(macro);
        auto cost = macroCost.calc();
        ss << "  {\"" << macro.clsMacroName << "\": " << cost << "}\n";
    }
    ss << "]";
    return ss.str();
}

} // namespace


bool MacroCostReader::load(const Rsyn::Json& params) {
	std::string path = params.value("path", "");
    minWidth_ = params.value<double>("min_width", 0.01);
    
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
            lefFiles_.push_back(path + std::string(file));
        }
    } else {
        lefFiles_.push_back(path + params.value("lefFiles", ""));
    }

	parsingFlow();
	return true;
}

void MacroCostReader::parsingFlow() {
	parseLefFiles();
    cost_ = calcMacroCost(lefDescriptor_, minWidth_);
}

void MacroCostReader::parseLefFiles() {
	LEFControlParser lefParser;

	for (int i = 0; i < lefFiles_.size(); i++) {
		if (!boost::filesystem::exists(lefFiles_[i])) {
			std::cout << "[WARNING] Failed to open file " << lefFiles_[i] << "\n";
			std::exit(1);
		}
		lefParser.parseLEF(lefFiles_[i], lefDescriptor_);
	}
}

std::string MacroCostReader::getResult() const {
    return cost_;
}

} // namespace Rsyn

std::string calcMacroScore(const std::string& input) {
    Rsyn::MacroCostReader reader{};
    if (reader.load(Rsyn::Json::parse(input))) {
        return reader.getResult();
    }
    return "";
}
