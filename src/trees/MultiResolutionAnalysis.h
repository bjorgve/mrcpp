/*
 * MRCPP, a numerical library based on multiresolution analysis and
 * the multiwavelet basis which provide low-scaling algorithms as well as
 * rigorous error control in numerical computations.
 * Copyright (C) 2020 Stig Rune Jensen, Jonas Juselius, Luca Frediani and contributors.
 *
 * This file is part of MRCPP.
 *
 * MRCPP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MRCPP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with MRCPP.  If not, see <https://www.gnu.org/licenses/>.
 *
 * For information on the complete list of contributors to MRCPP, see:
 * <https://mrcpp.readthedocs.io/>
 */

#pragma once

#include "BoundingBox.h"
#include "core/MWFilter.h"
#include "core/ScalingBasis.h"

#include "MRCPP/mrcpp_declarations.h"

namespace mrcpp {

/** @class MultiResolutionAnalysis
 *
 * @brief Class collecting computational domain and MW basis
 *
 * @details In order to combine different functions and operators in
 * mathematical operations, they need to be compatible. That is, they must
 * be defined on the same computational domain and constructed using the same
 * polynomial basis (order and type). This information constitutes an MRA,
 * which needs to be defined and passed as argument to all function and
 * operator constructors, and only functions and operators with compatible
 * MRAs can be combined in subsequent calculations.
 */

template <int D> class MultiResolutionAnalysis final {
public:
    MultiResolutionAnalysis(const BoundingBox<D> &bb, const ScalingBasis &sb, int depth = MaxDepth);
    MultiResolutionAnalysis(const MultiResolutionAnalysis<D> &mra);
    MultiResolutionAnalysis &operator=(const MultiResolutionAnalysis &mra) = delete;

    int getOrder() const { return this->basis.getScalingOrder(); }
    int getMaxDepth() const { return this->maxDepth; }
    int getMaxScale() const { return this->world.getScale() + this->maxDepth; }

    const MWFilter &getFilter() const { return *this->filter; }
    const ScalingBasis &getScalingBasis() const { return this->basis; }
    const BoundingBox<D> &getWorldBox() const { return this->world; }

    void setPeriodicOperatorReach(int reach) { this->periodic_operator_reach = reach; }
    int getPeriodicOperatorReach() const { return this->periodic_operator_reach; }

    void setPeriodicOperatorCutOff(int periodic_cut_off) { this->periodic_cut_off = periodic_cut_off; }
    int getPeriodicOperatorCutOff() const { return this->periodic_cut_off; }

    void setOperatorScale(int scale) { this->operator_scale = scale; }
    int getOperatorScale() const { return this->operator_scale; }

    void setStds(double stds) { this->n_gauss_stds = stds; }
    double getStds() { return this->n_gauss_stds; }

    MultiResolutionAnalysis<1> getKernelMRA() const;
    MultiResolutionAnalysis<2> getOperatorMRA() const;

    int getRootScale() const { return this->world.getScale(); }

    bool operator==(const MultiResolutionAnalysis<D> &mra) const;
    bool operator!=(const MultiResolutionAnalysis<D> &mra) const;

    void print() const;

protected:
    const int maxDepth;
    const ScalingBasis basis;
    const BoundingBox<D> world;
    int periodic_operator_reach{2};
    int periodic_cut_off{1};
    int operator_scale{0};
    double n_gauss_stds{4};
    MWFilter *filter;

    void setupFilter();
};

} // namespace mrcpp
