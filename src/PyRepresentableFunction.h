//
// Created by magnar on 16.03.18.
//

#pragma once
#include "pybind11/include/pybind11/pybind11.h"
#include "RepresentableFunction.h"


template <int D>
class PyRepresentableFunction : public mrcpp::RepresentableFunction<D> {
public:
    using mrcpp::RepresentableFunction<D>::RepresentableFunction;

    double evalf(const double *r) const override {
        PYBIND11_OVERLOAD_PURE(
                double,
                mrcpp::RepresentableFunction<D>,
                evalf,
                r
        );
    }

};
