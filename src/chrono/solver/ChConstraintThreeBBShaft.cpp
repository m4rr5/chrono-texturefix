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

#include "chrono/solver/ChConstraintThreeBBShaft.h"

namespace chrono {

// Register into the object factory, to enable run-time dynamic creation and persistence
CH_FACTORY_REGISTER(ChConstraintThreeBBShaft)

ChConstraintThreeBBShaft::ChConstraintThreeBBShaft(ChVariablesBody* mvariables_a,
                                                   ChVariablesBody* mvariables_b,
                                                   ChVariables* mvariables_c) {
    assert(mvariables_c->GetDOF() == 1);
    SetVariables(mvariables_a, mvariables_b, mvariables_c);
}

ChConstraintThreeBBShaft::ChConstraintThreeBBShaft(const ChConstraintThreeBBShaft& other) : ChConstraintThree(other) {
    Cq_a = other.Cq_a;
    Cq_b = other.Cq_b;
    Cq_c = other.Cq_c;
    Eq_a = other.Eq_a;
    Eq_b = other.Eq_b;
    Eq_c = other.Eq_c;
}

ChConstraintThreeBBShaft& ChConstraintThreeBBShaft::operator=(const ChConstraintThreeBBShaft& other) {
    if (&other == this)
        return *this;

    // copy parent class data
    ChConstraintThree::operator=(other);

    Cq_a = other.Cq_a;
    Cq_b = other.Cq_b;
    Cq_c = other.Cq_c;
    Eq_a = other.Eq_a;
    Eq_b = other.Eq_b;
    Eq_c = other.Eq_c;

    this->variables_a = other.variables_a;
    this->variables_b = other.variables_b;
    this->variables_c = other.variables_c;

    return *this;
}

void ChConstraintThreeBBShaft::SetVariables(ChVariables* mvariables_a,
                                            ChVariables* mvariables_b,
                                            ChVariables* mvariables_c) {
    assert(dynamic_cast<ChVariablesBody*>(mvariables_a));
    assert(dynamic_cast<ChVariablesBody*>(mvariables_b));
    assert(dynamic_cast<ChVariablesBody*>(mvariables_c));

    if (!mvariables_a || !mvariables_b || !mvariables_c) {
        SetValid(false);
        return;
    }

    SetValid(true);
    variables_a = mvariables_a;
    variables_b = mvariables_b;
    variables_c = mvariables_c;
    Cq_a.setZero();
    Cq_b.setZero();
    Cq_c.setZero();
}

void ChConstraintThreeBBShaft::Update_auxiliary() {
    // 1- Assuming jacobians are already computed, now compute
    //   the matrices [Eq_a]=[invM_a]*[Cq_a]'  etc
    if (variables_a->IsActive()) {
        variables_a->ComputeMassInverseTimesVector(Eq_a, Cq_a.transpose());
    }
    if (variables_b->IsActive()) {
        variables_b->ComputeMassInverseTimesVector(Eq_b, Cq_b.transpose());
    }
    if (variables_c->IsActive()) {
        variables_c->ComputeMassInverseTimesVector(Eq_c, Cq_c.transpose());
    }

    // 2- Compute g_i = [Cq_i]*[invM_i]*[Cq_i]' + cfm_i
    g_i = 0;
    if (variables_a->IsActive()) {
        g_i += Cq_a * Eq_a;
    }
    if (variables_b->IsActive()) {
        g_i += Cq_b * Eq_b;
    }
    if (variables_c->IsActive()) {
        g_i += Cq_c * Eq_c;
    }

    // 3- adds the constraint force mixing term (usually zero):
    if (cfm_i)
        g_i += cfm_i;
}

double ChConstraintThreeBBShaft::ComputeJacobianTimesState() {
    double ret = 0;

    if (variables_a->IsActive()) {
        ret += Cq_a * variables_a->State();
    }

    if (variables_b->IsActive()) {
        ret += Cq_b * variables_b->State();
    }

    if (variables_c->IsActive()) {
        ret += Cq_c(0) * variables_c->State()(0);
    }

    return ret;
}

void ChConstraintThreeBBShaft::IncrementState(double deltal) {
    if (variables_a->IsActive()) {
        variables_a->State() += Eq_a * deltal;
    }

    if (variables_b->IsActive()) {
        variables_b->State() += Eq_b * deltal;
    }

    if (variables_c->IsActive()) {
        variables_c->State()(0) += Eq_c(0) * deltal;
    }
}

void ChConstraintThreeBBShaft::AddJacobianTimesVectorInto(double& result, ChVectorConstRef vect) const {
    if (variables_a->IsActive()) {
        result += Cq_a * vect.segment(variables_a->GetOffset(), 6);
    }

    if (variables_b->IsActive()) {
        result += Cq_b * vect.segment(variables_b->GetOffset(), 6);
    }

    if (variables_c->IsActive()) {
        result += vect(variables_c->GetOffset()) * Cq_c(0);
    }
}

void ChConstraintThreeBBShaft::AddJacobianTransposedTimesScalarInto(ChVectorRef result, double l) const {
    if (variables_a->IsActive()) {
        result.segment(variables_a->GetOffset(), 6) += Cq_a.transpose() * l;
    }

    if (variables_b->IsActive()) {
        result.segment(variables_b->GetOffset(), 6) += Cq_b.transpose() * l;
    }

    if (variables_c->IsActive()) {
        result(variables_c->GetOffset()) += Cq_c(0) * l;
    }
}

void ChConstraintThreeBBShaft::PasteJacobianInto(ChSparseMatrix& mat, unsigned int start_row, unsigned int start_col) const {
    if (variables_a->IsActive())
        PasteMatrix(mat, Cq_a, start_row, variables_a->GetOffset() + start_col);
    if (variables_b->IsActive())
        PasteMatrix(mat, Cq_b, start_row, variables_b->GetOffset() + start_col);
    if (variables_c->IsActive())
        PasteMatrix(mat, Cq_c, start_row, variables_c->GetOffset() + start_col);
}

void ChConstraintThreeBBShaft::PasteJacobianTransposedInto(ChSparseMatrix& mat, unsigned int start_row, unsigned int start_col) const {
    if (variables_a->IsActive())
        PasteMatrix(mat, Cq_a.transpose(), variables_a->GetOffset() + start_row, start_col);
    if (variables_b->IsActive())
        PasteMatrix(mat, Cq_b.transpose(), variables_b->GetOffset() + start_row, start_col);
    if (variables_c->IsActive())
        PasteMatrix(mat, Cq_c.transpose(), variables_c->GetOffset() + start_row, start_col);
}

void ChConstraintThreeBBShaft::ArchiveOut(ChArchiveOut& archive_out) {
    // version number
    archive_out.VersionWrite<ChConstraintThreeBBShaft>();

    // serialize the parent class data too
    ChConstraintThree::ArchiveOut(archive_out);

    // serialize all member data:
    // NOTHING INTERESTING TO SERIALIZE (the Cq jacobians are not so
    // important to waste disk space.. they may be recomputed run-time,
    // and pointers to variables must be rebound in run-time.)
    // mstream << Cq_a;
    // mstream << Cq_b;
}

void ChConstraintThreeBBShaft::ArchiveIn(ChArchiveIn& archive_in) {
    // version number
    /*int version =*/archive_in.VersionRead<ChConstraintThreeBBShaft>();

    // deserialize the parent class data too
    ChConstraintThree::ArchiveIn(archive_in);

    // deserialize all member data:
    // NOTHING INTERESTING TO SERIALIZE (the Cq jacobians are not so
    // important to waste disk space.. they may be recomputed run-time,
    // and pointers to variables must be rebound in run-time.)
    // mstream << Cq_a;
    // mstream << Cq_b;
}

}  // end namespace chrono
