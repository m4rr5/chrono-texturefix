// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2014 projectchrono.org
// All right reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Radu Serban
// =============================================================================
//
// Wrapper classes for modeling an entire G500 vehicle assembly
// (including the vehicle itself, the powertrain, and the tires).
//
// =============================================================================

#ifndef G500_H
#define G500_H

#include <array>
#include <string>

#include "chrono_models/ChApiModels.h"
#include "chrono_models/vehicle/gclass/G500_Vehicle.h"
#include "chrono_models/vehicle/gclass/G500_EngineSimpleMap.h"
#include "chrono_models/vehicle/gclass/G500_AutomaticTransmissionSimpleMap.h"
#include "chrono_models/vehicle/gclass/G500_RigidTire.h"
#include "chrono_models/vehicle/gclass/G500_TMeasyTire.h"

namespace chrono {
namespace vehicle {
namespace gclass {

/// @addtogroup vehicle_models_gclass
/// @{

/// Definition of the gclass assembly.
/// This class encapsulates a concrete wheeled vehicle model with parameters corresponding to
/// a gclass vehicle, the powertrain model, and the 4 tires.
class CH_MODELS_API G500 {
  public:
    G500();
    G500(ChSystem* system);

    ~G500();

    void SetContactMethod(ChContactMethod val) { m_contactMethod = val; }
    void SetCollisionSystemType(ChCollisionSystem::Type collsys_type) { m_collsysType = collsys_type; }

    void SetChassisFixed(bool val) { m_fixed = val; }
    void SetChassisCollisionType(CollisionType val) { m_chassisCollisionType = val; }

    void SetBrakeType(BrakeType brake_type) { m_brake_type = brake_type; }
    void SetEngineType(EngineModelType val) { m_engineType = val; }
    void SetTransmissionType(TransmissionModelType val) { m_transmissionType = val; }
    void SetTireType(TireModelType val) { m_tireType = val; }

    // void setSteeringType(SteeringTypeWV val) { m_steeringType = val; }

    void SetInitPosition(const ChCoordsys<>& pos) { m_initPos = pos; }
    void SetInitFwdVel(double fwdVel) { m_initFwdVel = fwdVel; }
    void SetInitWheelAngVel(const std::vector<double>& omega) { m_initOmega = omega; }

    void SetTireStepSize(double step_size) { m_tire_step_size = step_size; }

    void EnableBrakeLocking(bool lock) { m_brake_locking = lock; }

    ChSystem* GetSystem() const { return m_vehicle->GetSystem(); }
    ChWheeledVehicle& GetVehicle() const { return *m_vehicle; }
    std::shared_ptr<ChChassis> GetChassis() const { return m_vehicle->GetChassis(); }
    std::shared_ptr<ChBodyAuxRef> GetChassisBody() const { return m_vehicle->GetChassisBody(); }

    void Initialize();

    void LockAxleDifferential(int axle, bool lock) { m_vehicle->LockAxleDifferential(axle, lock); }
    void LockCentralDifferential(int which, bool lock) { m_vehicle->LockCentralDifferential(which, lock); }

    void SetAerodynamicDrag(double Cd, double area, double air_density);

    void SetChassisVisualizationType(VisualizationType vis) { m_vehicle->SetChassisVisualizationType(vis); }
    void SetSuspensionVisualizationType(VisualizationType vis) { m_vehicle->SetSuspensionVisualizationType(vis); }
    void SetSteeringVisualizationType(VisualizationType vis) { m_vehicle->SetSteeringVisualizationType(vis); }
    void SetWheelVisualizationType(VisualizationType vis) { m_vehicle->SetWheelVisualizationType(vis); }
    void SetTireVisualizationType(VisualizationType vis) { m_vehicle->SetTireVisualizationType(vis); }

    void Synchronize(double time, const DriverInputs& driver_inputs, const ChTerrain& terrain);
    void Advance(double step);

    void LogHardpointLocations() { m_vehicle->LogHardpointLocations(); }
    void DebugLog(int what) { m_vehicle->DebugLog(what); }

  protected:
    ChSystem* m_system;
    G500_Vehicle* m_vehicle;

    ChContactMethod m_contactMethod;
    ChCollisionSystem::Type m_collsysType;
    CollisionType m_chassisCollisionType;
    bool m_fixed;
    bool m_brake_locking;

    EngineModelType m_engineType;
    TransmissionModelType m_transmissionType;
    BrakeType m_brake_type;
    TireModelType m_tireType;

    double m_tire_step_size;

    SteeringTypeWV m_steeringType;

    ChCoordsys<> m_initPos;
    double m_initFwdVel;
    std::vector<double> m_initOmega;

    bool m_apply_drag;
    double m_Cd;
    double m_area;
    double m_air_density;

    double m_tire_mass;
};

/// @} vehicle_models_gclass

}  // end namespace gclass
}  // end namespace vehicle
}  // end namespace chrono

#endif
