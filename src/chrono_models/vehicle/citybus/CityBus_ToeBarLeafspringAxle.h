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
// Authors: Radu Serban
// =============================================================================
//
// CityBus leafspring axle.
//
// =============================================================================

#ifndef CityBus_TOEBARLEAFSPRING_AXLE_H
#define CityBus_TOEBARLEAFSPRING_AXLE_H

#include "chrono_vehicle/wheeled_vehicle/suspension/ChToeBarLeafspringAxle.h"

#include "chrono_models/ChApiModels.h"

namespace chrono {
namespace vehicle {
namespace citybus {

/// @addtogroup vehicle_models_citybus
/// @{

class CH_MODELS_API CityBus_ToeBarLeafspringAxle : public ChToeBarLeafspringAxle {
  public:
    CityBus_ToeBarLeafspringAxle(const std::string& name);
    ~CityBus_ToeBarLeafspringAxle();

  protected:
    virtual const ChVector3d getLocation(PointId which) override;

    virtual double getAxleTubeMass() const override { return m_axleTubeMass; }
    virtual double getSpindleMass() const override { return m_spindleMass; }
    virtual double getKnuckleMass() const override { return m_knuckleMass; }
    virtual double getTierodMass() const override { return m_tierodMass; }
    virtual double getDraglinkMass() const override { return m_draglinkMass; }

    virtual double getAxleTubeRadius() const override { return m_axleTubeRadius; }
    virtual double getSpindleRadius() const override { return m_spindleRadius; }
    virtual double getSpindleWidth() const override { return m_spindleWidth; }
    virtual double getKnuckleRadius() const override { return m_knuckleRadius; }
    virtual double getTierodRadius() const override { return m_tierodRadius; }
    virtual double getDraglinkRadius() const override { return m_draglinkRadius; }

    virtual const ChVector3d getAxleTubeCOM() const override { return ChVector3d(0, 0, 0); }

    virtual const ChVector3d& getAxleTubeInertia() const override { return m_axleTubeInertia; }
    virtual const ChVector3d& getSpindleInertia() const override { return m_spindleInertia; }
    virtual const ChVector3d& getKnuckleInertia() const override { return m_knuckleInertia; }
    virtual const ChVector3d& getTierodInertia() const override { return m_tierodInertia; }
    virtual const ChVector3d& getDraglinkInertia() const override { return m_draglinkInertia; }

    virtual double getAxleInertia() const override { return m_axleShaftInertia; }

    virtual double getSpringRestLength() const override { return m_springRestLength; }
    /// Return the functor object for spring force.
    virtual std::shared_ptr<ChLinkTSDA::ForceFunctor> getSpringForceFunctor() const override { return m_springForceCB; }
    /// Return the functor object for shock force.
    virtual std::shared_ptr<ChLinkTSDA::ForceFunctor> getShockForceFunctor() const override { return m_shockForceCB; }

  private:
    std::shared_ptr<ChLinkTSDA::ForceFunctor> m_springForceCB;
    std::shared_ptr<ChLinkTSDA::ForceFunctor> m_shockForceCB;

    static const double m_axleShaftInertia;

    static const double m_axleTubeMass;
    static const double m_spindleMass;
    static const double m_knuckleMass;
    static const double m_tierodMass;
    static const double m_draglinkMass;

    static const double m_axleTubeRadius;
    static const double m_spindleRadius;
    static const double m_spindleWidth;
    static const double m_knuckleRadius;
    static const double m_tierodRadius;
    static const double m_draglinkRadius;

    static const ChVector3d m_axleTubeInertia;
    static const ChVector3d m_spindleInertia;
    static const ChVector3d m_knuckleInertia;
    static const ChVector3d m_tierodInertia;
    static const ChVector3d m_draglinkInertia;

    static const double m_springCoefficient;
    static const double m_springRestLength;
    static const double m_springDesignLength;
    static const double m_springMinLength;
    static const double m_springMaxLength;

    static const double m_damperCoefficient;
    static const double m_damperDegressivityExpansion;
    static const double m_damperDegressivityCompression;
};

/// @} vehicle_models_citybus

}  // end namespace citybus
}  // end namespace vehicle
}  // end namespace chrono

#endif
