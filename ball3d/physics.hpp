#pragma once

#include <Jolt/Jolt.h>

// Jolt includes
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Constraints/FixedConstraint.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/RegisterTypes.h>

#include <iostream>
#include <memory>

bool JPH_GetDebugStatus();
void JPH_SetDebugStatus(bool enabled);

void TraceImpl(const char* inFMT, ...);
bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, uint inLine);

namespace Layers {
static constexpr JPH::ObjectLayer NON_MOVING = 0;
static constexpr JPH::ObjectLayer MOVING     = 1;
static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
};  // namespace Layers

class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter {
   public:
    virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override;
};

namespace BroadPhaseLayers {
static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
static constexpr JPH::BroadPhaseLayer MOVING(1);
static constexpr uint                 NUM_LAYERS(2);
};  // namespace BroadPhaseLayers

class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface {
   private:
    JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];

   public:
    BPLayerInterfaceImpl();
    virtual uint                 GetNumBroadPhaseLayers() const override;
    virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;
};

class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter {
   public:
    virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override;
};

class ContactListenerImpl : public JPH::ContactListener {
   public:
    virtual JPH::ValidateResult OnContactValidate(const JPH::Body&, const JPH::Body&, JPH::RVec3Arg, const JPH::CollideShapeResult&) override;
    virtual void                OnContactAdded(const JPH::Body&, const JPH::Body&, const JPH::ContactManifold&, JPH::ContactSettings&) override;
    virtual void                OnContactPersisted(const JPH::Body&, const JPH::Body&, const JPH::ContactManifold&, JPH::ContactSettings&) override;
    virtual void                OnContactRemoved(const JPH::SubShapeIDPair&) override;
};

class BodyActivationListenerImpl : public JPH::BodyActivationListener {
   public:
    virtual void OnBodyActivated(const JPH::BodyID&, JPH::uint64) override;
    virtual void OnBodyDeactivated(const JPH::BodyID&, JPH::uint64) override;
};

class PhysicsEngine {
   private:
    std::shared_ptr<JPH::TempAllocatorImpl>            _allocator;
    std::shared_ptr<JPH::JobSystemThreadPool>          _job_system;
    std::shared_ptr<BPLayerInterfaceImpl>              _broad_phase_layer_interface;
    std::shared_ptr<ObjectVsBroadPhaseLayerFilterImpl> _object_vs_broadphase_layer_filter;
    std::shared_ptr<ObjectLayerPairFilterImpl>         _object_vs_object_layer_filter;
    std::shared_ptr<JPH::PhysicsSystem>                _physics_system;
    std::shared_ptr<BodyActivationListenerImpl>        _body_activation_listener;
    std::shared_ptr<ContactListenerImpl>               _contact_listener;

    JPH::BodyInterface*           _interface;
    const JPH::BodyLockInterface* _lock_interface;

   public:
    PhysicsEngine();
    ~PhysicsEngine();

    /** Indicate that the physics engine is ready to start computing. This should be called after an object has been added or removed, ideally. */
    void                                Ready();
    void                                Update(double dt);
    JPH::BodyInterface*                 Interface() const;
    const JPH::BodyLockInterface*       LockInterface() const;
    std::shared_ptr<JPH::PhysicsSystem> PhysicsSystem();
};