#include "pac/MacroCost.h"
#include "rsyn/io/parser/lef_def/LEFControlParser.h"
#include <cmath>
#include <iostream>
#include <algorithm>

namespace Rsyn
{

namespace {

constexpr double ALPHA = 1.0;
constexpr double BETA = 1.0;
constexpr double GAMA = 1.0;

std::string toUpper(const std::string& str) {
    std::string upperStr = str;
    std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
    return upperStr;
}

bool isPowerPin(const LefPinDscp& pin) {
    auto useName = toUpper(pin.clsPinUse);
    return useName == "POWER" || useName == "GROUND" || pin.clsPinName == "VSS" || pin.clsPinName == "VDD";
}

double calcSmallestBoundingBoxArea(const LefPinDscp& pin1, const LefPinDscp& pin2) {
    // Initialize a degenerate bounding box
    DoubleRectangle bbox;
    bbox.degenerate();

    // Helper function to stretch the bounding box to fit a rectangle
    auto stretchToFit = [&bbox](const DoubleRectangle& rect) {
        bbox.stretchToFit(rect[0]);
        bbox.stretchToFit(rect[1]);
    };

    // Iterate over all geometries of both pins and update the bounding box
    for (const auto& port : pin1.clsPorts) {
        for (const auto& geo : port.clsLefPortGeoDscp) {
            for (const auto& rect : geo.clsBounds) {
                stretchToFit(rect);
            }
        }
    }

    for (const auto& port : pin2.clsPorts) {
        for (const auto& geo : port.clsLefPortGeoDscp) {
            for (const auto& rect : geo.clsBounds) {
                stretchToFit(rect);
            }
        }
    }

    return bbox.computeArea();
}

double calcPinArea(const LefPinDscp& pin, int layer=0) {
    double area = 0;
    for (auto& port : pin.clsPorts)
    {
        for (auto& port_geo : port.clsLefPortGeoDscp)
        {
            for (auto& bound : port_geo.clsBounds)
            {
                area += bound.computeArea();
            }
        }
    }
    return area;
}

}

MacroCost::MacroCost(const LefMacroDscp& macro, double minWidth) : macro_{macro}, minWidth_{minWidth} {}

double MacroCost::calc() {
    return ALPHA*pec() + BETA*pac() + GAMA*prc();
}

double MacroCost::pec() {
    int num = 0;
    for (auto& pin : macro_.clsPins) {
        if (!isPowerPin(pin)) {
            ++num;
        }
    }
    return num;
}

double MacroCost::pac() {
    double cost = 0;
    for (auto& pin : macro_.clsPins)
    {
        if (isPowerPin(pin)) { continue; }
        
        double exponent = 2 - calcPinArea(pin) / minWidth_;
        cost += std::pow(2, exponent);
    }
    return cost;
}

double MacroCost::prc() {
    double cost = 0;
    auto size = macro_.clsPins.size();
    for (size_t i = 0; i < size - 1; ++i) {
        auto& pin1 = macro_.clsPins[i];
        if (isPowerPin(pin1)) { continue; }

        for (size_t j = i+1; j < size; ++j) {
            auto& pin2 = macro_.clsPins[j];            
            if (isPowerPin(pin2)) { continue; }

            double exponent = 2 - calcSmallestBoundingBoxArea(pin1, pin2) / (3*minWidth_);
            cost += std::pow(2, exponent);
        }
    }
    return cost;
}

} // namespace Rsyn
