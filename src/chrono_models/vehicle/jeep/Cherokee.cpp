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
// Authors: Radu Serban, Asher Elmquist, Rainer Gericke
// =============================================================================
//
// Wrapper classes for modeling an entire Jeep Cherokee 1997 vehicle assembly
// Vehicle Parameters taken from SAE Paper 1999-01-0121
// (including the vehicle itself, the powertrain, and the tires).
//
// =============================================================================

#include "Cherokee.h"

namespace chrono {
namespace vehicle {
namespace jeep {

// -----------------------------------------------------------------------------
Cherokee::Cherokee()
    : m_system(nullptr),
      m_vehicle(nullptr),
      m_contactMethod(ChContactMethod::NSC),
      m_collsysType(ChCollisionSystem::Type::BULLET),
      m_chassisCollisionType(CollisionType::NONE),
      m_fixed(false),
      m_brake_locking(false),
      m_brake_type(BrakeType::SHAFTS),
      m_tireType(TireModelType::TMEASY),
      m_tire_step_size(-1),
      m_initPos(ChCoordsys<>(ChVector3d(0, 0, 1), QUNIT)),
      m_initFwdVel(0),
      m_initOmega({0, 0, 0, 0}),
      m_apply_drag(false) {}

Cherokee::Cherokee(ChSystem* system)
    : m_system(system),
      m_vehicle(nullptr),
      m_contactMethod(ChContactMethod::NSC),
      m_collsysType(ChCollisionSystem::Type::BULLET),
      m_chassisCollisionType(CollisionType::NONE),
      m_fixed(false),
      m_brake_locking(false),
      m_brake_type(BrakeType::SHAFTS),
      m_tireType(TireModelType::TMEASY),
      m_tire_step_size(-1),
      m_initPos(ChCoordsys<>(ChVector3d(0, 0, 1), QUNIT)),
      m_initFwdVel(0),
      m_initOmega({0, 0, 0, 0}),
      m_apply_drag(false) {}

Cherokee::~Cherokee() {
    delete m_vehicle;
}

// -----------------------------------------------------------------------------
void Cherokee::SetAerodynamicDrag(double Cd, double area, double air_density) {
    m_Cd = Cd;
    m_area = area;
    m_air_density = air_density;

    m_apply_drag = true;
}

// -----------------------------------------------------------------------------
void Cherokee::Initialize() {
    // Create and initialize the Cherokee vehicle
    m_vehicle = m_system ? new Cherokee_Vehicle(m_system, m_fixed, m_brake_type, m_chassisCollisionType)
                         : new Cherokee_Vehicle(m_fixed, m_brake_type, m_contactMethod, m_chassisCollisionType);
    m_vehicle->SetCollisionSystemType(m_collsysType);
    m_vehicle->SetInitWheelAngVel(m_initOmega);
    m_vehicle->Initialize(m_initPos, m_initFwdVel);

    // If specified, enable aerodynamic drag
    if (m_apply_drag) {
        m_vehicle->GetChassis()->SetAerodynamicDrag(m_Cd, m_area, m_air_density);
    }

    // Create and initialize the powertrain system
    auto engine = chrono_types::make_shared<Cherokee_EngineShafts>("Engine");
    auto transmission = chrono_types::make_shared<Cherokee_AutomaticTransmissionShafts>("Transmission");
    auto powertrain = chrono_types::make_shared<ChPowertrainAssembly>(engine, transmission);
    m_vehicle->InitializePowertrain(powertrain);

    // Create the tires and set parameters depending on type.
    switch (m_tireType) {
        case TireModelType::RIGID_MESH:
        case TireModelType::RIGID: {
            bool use_mesh = (m_tireType == TireModelType::RIGID_MESH);

            auto tire_FL = chrono_types::make_shared<Cherokee_RigidTire>("FL", use_mesh);
            auto tire_FR = chrono_types::make_shared<Cherokee_RigidTire>("FR", use_mesh);
            auto tire_RL = chrono_types::make_shared<Cherokee_RigidTire>("RL", use_mesh);
            auto tire_RR = chrono_types::make_shared<Cherokee_RigidTire>("RR", use_mesh);

            m_vehicle->InitializeTire(tire_FL, m_vehicle->GetAxle(0)->m_wheels[LEFT], VisualizationType::NONE);
            m_vehicle->InitializeTire(tire_FR, m_vehicle->GetAxle(0)->m_wheels[RIGHT], VisualizationType::NONE);
            m_vehicle->InitializeTire(tire_RL, m_vehicle->GetAxle(1)->m_wheels[LEFT], VisualizationType::NONE);
            m_vehicle->InitializeTire(tire_RR, m_vehicle->GetAxle(1)->m_wheels[RIGHT], VisualizationType::NONE);

            m_tire_mass = tire_FL->GetMass();

            break;
        }
            /*

        case TireModelType::PAC02: {
            auto tire_FL = chrono_types::make_shared<Cherokee_Pac02Tire>("FL");
            auto tire_FR = chrono_types::make_shared<Cherokee_Pac02Tire>("FR");
            auto tire_RL = chrono_types::make_shared<Cherokee_Pac02Tire>("RL");
            auto tire_RR = chrono_types::make_shared<Cherokee_Pac02Tire>("RR");

            m_vehicle->InitializeTire(tire_FL, m_vehicle->GetAxle(0)->m_wheels[LEFT],
   VisualizationType::NONE); m_vehicle->InitializeTire(tire_FR, m_vehicle->GetAxle(0)->m_wheels[RIGHT],
   VisualizationType::NONE); m_vehicle->InitializeTire(tire_RL, m_vehicle->GetAxle(1)->m_wheels[LEFT],
   VisualizationType::NONE); m_vehicle->InitializeTire(tire_RR, m_vehicle->GetAxle(1)->m_wheels[RIGHT],
   VisualizationType::NONE);

            m_tire_mass = tire_FL->GetMass();

            break;
        }
        case TireModelType::TMEASY: {
            auto tire_FL = chrono_types::make_shared<Cherokee_TMeasyTire>("FL");
            auto tire_FR = chrono_types::make_shared<Cherokee_TMeasyTire>("FR");
            auto tire_RL = chrono_types::make_shared<Cherokee_TMeasyTire>("RL");
            auto tire_RR = chrono_types::make_shared<Cherokee_TMeasyTire>("RR");

            m_vehicle->InitializeTire(tire_FL, m_vehicle->GetAxle(0)->m_wheels[LEFT],
   VisualizationType::NONE); m_vehicle->InitializeTire(tire_FR, m_vehicle->GetAxle(0)->m_wheels[RIGHT],
   VisualizationType::NONE); m_vehicle->InitializeTire(tire_RL, m_vehicle->GetAxle(1)->m_wheels[LEFT],
   VisualizationType::NONE); m_vehicle->InitializeTire(tire_RR, m_vehicle->GetAxle(1)->m_wheels[RIGHT],
   VisualizationType::NONE);

            m_tire_mass = tire_FL->GetMass();

            break;
        }
*/
        default:
            std::cout << "Unsupported Tire Model Type! Switching to TMeasy.\n";
        case TireModelType::TMEASY: {
            auto tire_FL = chrono_types::make_shared<Cherokee_TMeasyTire>("FL");
            auto tire_FR = chrono_types::make_shared<Cherokee_TMeasyTire>("FR");
            auto tire_RL = chrono_types::make_shared<Cherokee_TMeasyTire>("RL");
            auto tire_RR = chrono_types::make_shared<Cherokee_TMeasyTire>("RR");

            m_vehicle->InitializeTire(tire_FL, m_vehicle->GetAxle(0)->m_wheels[LEFT], VisualizationType::NONE);
            m_vehicle->InitializeTire(tire_FR, m_vehicle->GetAxle(0)->m_wheels[RIGHT], VisualizationType::NONE);
            m_vehicle->InitializeTire(tire_RL, m_vehicle->GetAxle(1)->m_wheels[LEFT], VisualizationType::NONE);
            m_vehicle->InitializeTire(tire_RR, m_vehicle->GetAxle(1)->m_wheels[RIGHT], VisualizationType::NONE);

            m_tire_mass = tire_FL->GetMass();

            break;
        }
    }

    for (auto& axle : m_vehicle->GetAxles()) {
        for (auto& wheel : axle->GetWheels()) {
            if (m_tire_step_size > 0)
                wheel->GetTire()->SetStepsize(m_tire_step_size);
        }
    }

    m_vehicle->EnableBrakeLocking(m_brake_locking);

    // Recalculate vehicle mass, to properly account for all subsystems
    m_vehicle->InitializeInertiaProperties();
}

// -----------------------------------------------------------------------------
void Cherokee::Synchronize(double time, const DriverInputs& driver_inputs, const ChTerrain& terrain) {
    m_vehicle->Synchronize(time, driver_inputs, terrain);
}

// -----------------------------------------------------------------------------
void Cherokee::Advance(double step) {
    m_vehicle->Advance(step);
}

}  // namespace jeep
}  // end namespace vehicle
}  // end namespace chrono
