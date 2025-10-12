#pragma once

// Jolt Physics implementation taken from https://github.com/jrouwe/JoltPhysics/blob/master/HelloWorld/HelloWorld.cpp
// TODO rewrite this to fit the needs of the program better

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

// Callback for traces, connect this to your own trace function if you have one
inline void TraceImpl(const char* inFMT, ...) {
    // Format the message
    va_list list;
    va_start(list, inFMT);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), inFMT, list);
    va_end(list);

    // Print to the TTY
    std::cout << buffer << std::endl;
}

// Callback for asserts, connect this to your own assert handler if you have one
inline bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, uint inLine) {
    // Print to the TTY
    std::cout << inFile << ":" << inLine << ": (" << inExpression << ") " << (inMessage != nullptr ? inMessage : "") << std::endl;

    // Breakpoint
    return true;
};
// Layer that objects can be in, determines which other objects it can collide with
// Typically you at least want to have 1 layer for moving bodies and 1 layer for static bodies, but you can have more
// layers if you want. E.g. you could have a layer for high detail collision (which is not used by the physics simulation
// but only if you do collision testing).
namespace Layers {
static constexpr JPH::ObjectLayer NON_MOVING = 0;
static constexpr JPH::ObjectLayer MOVING     = 1;
static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
};  // namespace Layers

/// Class that determines if two object layers can collide
class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter {
   public:
    virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override {
        switch (inObject1) {
            case Layers::NON_MOVING:
                return inObject2 == Layers::MOVING;  // Non moving only collides with moving
            case Layers::MOVING:
                return true;  // Moving collides with everything
            default:
                JPH_ASSERT(false);
                return false;
        }
    }
};

// Each broadphase layer results in a separate bounding volume tree in the broad phase. You at least want to have
// a layer for non-moving and moving objects to avoid having to update a tree full of static objects every frame.
// You can have a 1-on-1 mapping between object layers and broadphase layers (like in this case) but if you have
// many object layers you'll be creating many broad phase trees, which is not efficient. If you want to fine tune
// your broadphase layers define JPH_TRACK_BROADPHASE_STATS and look at the stats reported on the TTY.
namespace BroadPhaseLayers {
static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
static constexpr JPH::BroadPhaseLayer MOVING(1);
static constexpr uint                 NUM_LAYERS(2);
};  // namespace BroadPhaseLayers

// BroadPhaseLayerInterface implementation
// This defines a mapping between object and broadphase layers.
class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface {
   public:
    BPLayerInterfaceImpl() {
        // Create a mapping table from object to broad phase layer
        mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
        mObjectToBroadPhase[Layers::MOVING]     = BroadPhaseLayers::MOVING;
    }

    virtual uint GetNumBroadPhaseLayers() const override {
        return BroadPhaseLayers::NUM_LAYERS;
    }

    virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override {
        JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
        return mObjectToBroadPhase[inLayer];
    }

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    virtual const char* GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const override {
        switch ((BroadPhaseLayer::Type)inLayer) {
            case (BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:
                return "NON_MOVING";
            case (BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:
                return "MOVING";
            default:
                JPH_ASSERT(false);
                return "INVALID";
        }
    }
#endif  // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

   private:
    JPH::BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
};

/// Class that determines if an object layer can collide with a broadphase layer
class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter {
   public:
    virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override {
        switch (inLayer1) {
            case Layers::NON_MOVING:
                return inLayer2 == BroadPhaseLayers::MOVING;
            case Layers::MOVING:
                return true;
            default:
                JPH_ASSERT(false);
                return false;
        }
    }
};

// An example contact listener
class MyContactListener : public JPH::ContactListener {
   public:
    // See: ContactListener
    virtual JPH::ValidateResult OnContactValidate(const JPH::Body&, const JPH::Body&, JPH::RVec3Arg, const JPH::CollideShapeResult&) override {
        std::cout << "Contact validate callback" << std::endl;

        // Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
        return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
    }

    virtual void OnContactAdded(const JPH::Body&, const JPH::Body&, const JPH::ContactManifold&, JPH::ContactSettings&) override {
        std::cout << "A contact was added" << std::endl;
    }

    virtual void OnContactPersisted(const JPH::Body&, const JPH::Body&, const JPH::ContactManifold&, JPH::ContactSettings&) override {
        std::cout << "A contact was persisted" << std::endl;
    }

    virtual void OnContactRemoved(const JPH::SubShapeIDPair&) override {
        std::cout << "A contact was removed" << std::endl;
    }
};

// An example activation listener
class MyBodyActivationListener : public JPH::BodyActivationListener {
   public:
    virtual void OnBodyActivated(const JPH::BodyID&, JPH::uint64) override {
        std::cout << "A body got activated" << std::endl;
    }

    virtual void OnBodyDeactivated(const JPH::BodyID&, JPH::uint64) override {
        std::cout << "A body went to sleep" << std::endl;
    }
};

class PhysicsEngine {
   private:
    std::shared_ptr<JPH::TempAllocatorImpl>            _allocator;
    std::shared_ptr<JPH::JobSystemThreadPool>          _job_system;
    std::shared_ptr<BPLayerInterfaceImpl>              _broad_phase_layer_interface;
    std::shared_ptr<ObjectVsBroadPhaseLayerFilterImpl> _object_vs_broadphase_layer_filter;
    std::shared_ptr<ObjectLayerPairFilterImpl>         _object_vs_object_layer_filter;
    std::shared_ptr<JPH::PhysicsSystem>                _physics_system;
    std::shared_ptr<MyBodyActivationListener>          _body_activation_listener;
    std::shared_ptr<MyContactListener>                 _contact_listener;

    JPH::BodyInterface*           _interface;
    const JPH::BodyLockInterface* _lock_interface;

   public:
    PhysicsEngine() {
        _allocator  = std::make_shared<JPH::TempAllocatorImpl>(10 * 1024 * 1024);
        _job_system = std::make_shared<JPH::JobSystemThreadPool>(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);

        const uint  cMaxBodies             = 1024;
        const uint  cNumBodyMutexes        = 0;
        const uint  cMaxBodyPairs          = 1024;
        const uint  cMaxContactConstraints = 1024;
        const float cGravity               = -10.0f;

        _broad_phase_layer_interface       = std::make_shared<BPLayerInterfaceImpl>();
        _object_vs_broadphase_layer_filter = std::make_shared<ObjectVsBroadPhaseLayerFilterImpl>();
        _object_vs_object_layer_filter     = std::make_shared<ObjectLayerPairFilterImpl>();
        _physics_system                    = std::make_shared<JPH::PhysicsSystem>();
        _physics_system->Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, *_broad_phase_layer_interface, *_object_vs_broadphase_layer_filter, *_object_vs_object_layer_filter);

        _body_activation_listener = std::make_shared<MyBodyActivationListener>();
        _contact_listener         = std::make_shared<MyContactListener>();

        _physics_system->SetBodyActivationListener(_body_activation_listener.get());
        _physics_system->SetContactListener(_contact_listener.get());
        _physics_system->SetGravity({0, cGravity, 0});

        _interface      = &_physics_system->GetBodyInterface();
        _lock_interface = &_physics_system->GetBodyLockInterface();
    }

    ~PhysicsEngine() {}

    /** Indicate that the physics engine is ready to start computing. This should be called after an object has been added or removed, ideally. */
    void Ready() {
        _physics_system->OptimizeBroadPhase();
    }

    void Update(double dt) {
        const int cCollisionSteps = 1;
        _physics_system->Update(dt, cCollisionSteps, _allocator.get(), _job_system.get());
    };

    JPH::BodyInterface* Interface() const {
        return _interface;
    }

    const JPH::BodyLockInterface* LockInterface() const {
        return _lock_interface;
    }

    std::shared_ptr<JPH::PhysicsSystem> PhysicsSystem() {
        return _physics_system;
    }
};