/*
 *  \date Mar 21, 2018
 *  \author Magnar Bjørgve <magnar.bjorgve@uit.no> \n
 *          Hylleraas Centre for Quantum Molecular Sciences \n
 *          UiT - The Arctic University of Norway
 */


#include <pybind11/pybind11.h>

#include "ABGVOperator.h"
#include "PoissonOperator.h"
#include "ConvolutionOperator.h"

#include "MultiResolutionAnalysis.h"

using namespace mrcpp;
namespace py = pybind11;


template<int D>
void init_pyOperators(py::module &m) {

    std::stringstream ABGVOperatorName;
    ABGVOperatorName << "ABGVOperator" << D << "D";
    py::class_ <ABGVOperator<D>> (m, ABGVOperatorName.str().data())
        .def(py::init< MultiResolutionAnalysis<D> &, double, double >());


    std::stringstream ConvOperatorName;
    ConvOperatorName << "ConvolutionOperator" << D << "D";
    py::class_ <ConvolutionOperator<D>> convop(m, ConvOperatorName.str().data());
    convop
        .def(py::init<MultiResolutionAnalysis<D> &, double>());

    if (D==3){
        py::class_ <PoissonOperator> (m, "PoissonOperator", convop)
            .def(py::init<const MultiResolutionAnalysis<3> &, double >());
    }

}

template void init_pyOperators<1>(py::module &m);
template void init_pyOperators<2>(py::module &m);
template void init_pyOperators<3>(py::module &m);

