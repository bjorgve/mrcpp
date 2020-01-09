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

#include "PoissonOperator.h"
#include "PoissonKernel.h"
#include "utils/Printer.h"

namespace mrcpp {

PoissonOperator::PoissonOperator(const MultiResolutionAnalysis<3> &mra, double pr)
        : ConvolutionOperator<3>(mra, pr) {
    int oldlevel = Printer::setPrintLevel(0);
    double epsilon = this->prec / 10.0;
    double r_min = calcMinDistance(mra, epsilon);
    double r_max = calcMaxDistance(mra);
    PoissonKernel poisson_kernel(epsilon, r_min, r_max);
    // Rescale for application in 3D
    poisson_kernel.rescale(3);
    initializeOperator(poisson_kernel);
    Printer::setPrintLevel(oldlevel);
}

} // namespace mrcpp
