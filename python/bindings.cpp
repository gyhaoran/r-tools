#include <pybind11/pybind11.h>
#include "pac.h"

namespace py = pybind11;

PYBIND11_MODULE(pacpy, m) {
    // Expose the verify function
    m.def("calc_pin_score", &calcPinScore, "Calc pin score of macro", py::arg("input"));
    m.def("calc_macro_socre", &calcMacroScore, "Calc macro score", py::arg("input"));
}
