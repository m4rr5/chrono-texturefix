// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2014 projectchrono.org
// All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Alessandro Tasora, Radu Serban
// =============================================================================

#ifndef CHASSEMBLYANALYSIS_H
#define CHASSEMBLYANALYSIS_H

#include "chrono/core/ChApiCE.h"
#include "chrono/timestepper/ChState.h"
#include "chrono/timestepper/ChIntegrable.h"

namespace chrono {

/// Enumerations for assembly level.
namespace AssemblyLevel {
enum Enum {
    POSITION = 1 << 0,      ///< satisfy constraints at position level
    VELOCITY = 1 << 1,      ///< satisfy constraints at velocity level
    ACCELERATION = 1 << 2,  ///< satisfy constraints at acceleration level
    FULL = 0xFFFF           ///< full assembly (position + velocity + acceleration)
};
}

/// Class for assembly analysis.
/// Assembly is performed by satisfying constraints at a position, velocity, and acceleration levels.
/// Assembly at position level involves solving a non-linear problem. Assembly at velocity level is
/// performed by taking a small integration step. Consistent accelerations are obtained through
/// finite differencing.
class ChApi ChAssemblyAnalysis {
  public:
    ChAssemblyAnalysis(ChIntegrableIIorder& mintegrable);

    ~ChAssemblyAnalysis() {}

    /// Perform the assembly analysis.
    /// Assembly is performed by satisfying constraints at position, velocity, and acceleration levels.
    /// Assembly at position level involves solving a non-linear problem. Assembly at velocity level is
    /// performed by taking a small integration step. Consistent accelerations are obtained through
    /// finite differencing.
    void AssemblyAnalysis(int action, double dt = 1e-7);

    /// Set the max number of Newton-Raphson iterations for the position assembly procedure.
    void SetMaxAssemblyIters(int mi) { max_assembly_iters = mi; }

    /// Get the max number of Newton-Raphson iterations for the position assembly procedure.
    int GetMaxAssemblyIters() { return max_assembly_iters; }

    /// Get the integrable object.
    ChIntegrable* GetIntegrable() { return integrable; }

    /// Access the Lagrange multipliers.
    const ChVectorDynamic<>& GetLagrangeMultipliers() const { return L; }

    /// Access the current position state vector.
    const ChState& GetStatePos() const { return X; }

    /// Access the current velocity state vector.
    const ChStateDelta& GetStateVel() const { return V; }

    /// Access the current acceleration state vector.
    const ChStateDelta& GetStateAcc() const { return A; }

  private:
    ChIntegrableIIorder* integrable;

    ChState X;
    ChStateDelta V;
    ChStateDelta A;
    ChVectorDynamic<> L;
    unsigned int max_assembly_iters;
};

}  // end namespace chrono

#endif
