#include "physics.hpp"

#include <cstdarg>

// Note: this file is heavily based on the Jolt Physics Hello World implementation, due to the simplicities of that program and this game
// https://github.com/jrouwe/JoltPhysics/blob/master/HelloWorld/HelloWorld.cpp

bool JPH_EnableDebug = true;

bool JPH_GetDebugStatus() {
    return JPH_EnableDebug;
}

void JPH_SetDebugStatus(bool enabled) {
    JPH_EnableDebug = enabled;
}

void DebugPrint(const char* fmt, ...) {
    if (!JPH_EnableDebug)
        return;

    va_list list;
    va_start(list, fmt);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, list);
    va_end(list);
    std::cout << buffer << std::endl;
}

void TraceImpl(const char* inFMT, ...) {
    // Pass down to DebugPrint
    va_list list;
    va_start(list, inFMT);
    DebugPrint(inFMT, list);
    va_end(list);
}

bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, uint inLine) {
    DebugPrint("%s:%u: (%s) %s", inFile, inLine, inExpression, (inMessage != nullptr ? inMessage : ""));
    return true;
};

bool ObjectLayerPairFilterImpl::ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const {
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

BPLayerInterfaceImpl::BPLayerInterfaceImpl() {
    // Create a mapping table from object to broad phase layer
    mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
    mObjectToBroadPhase[Layers::MOVING]     = BroadPhaseLayers::MOVING;
}

uint BPLayerInterfaceImpl::GetNumBroadPhaseLayers() const {
    return BroadPhaseLayers::NUM_LAYERS;
}

JPH::BroadPhaseLayer BPLayerInterfaceImpl::GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const {
    JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
    return mObjectToBroadPhase[inLayer];
}

bool ObjectVsBroadPhaseLayerFilterImpl::ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const {
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

JPH::ValidateResult ContactListenerImpl::OnContactValidate(const JPH::Body&, const JPH::Body&, JPH::RVec3Arg, const JPH::CollideShapeResult&) {
    DebugPrint("Contact validate callback");
    return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
}

void ContactListenerImpl::OnContactAdded(const JPH::Body&, const JPH::Body&, const JPH::ContactManifold&, JPH::ContactSettings&) {
    DebugPrint("A contact was added");
}

void ContactListenerImpl::OnContactPersisted(const JPH::Body&, const JPH::Body&, const JPH::ContactManifold&, JPH::ContactSettings&) {
    DebugPrint("A contact was persisted");
}

void ContactListenerImpl::OnContactRemoved(const JPH::SubShapeIDPair&) {
    DebugPrint("A contact was removed");
}

void BodyActivationListenerImpl::OnBodyActivated(const JPH::BodyID&, JPH::uint64) {
    DebugPrint("A body got activated");
}

void BodyActivationListenerImpl::OnBodyDeactivated(const JPH::BodyID&, JPH::uint64) {
    DebugPrint("A body went to sleep");
}

PhysicsEngine::PhysicsEngine() {
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

    _body_activation_listener = std::make_shared<BodyActivationListenerImpl>();
    _contact_listener         = std::make_shared<ContactListenerImpl>();

    _physics_system->SetBodyActivationListener(_body_activation_listener.get());
    _physics_system->SetContactListener(_contact_listener.get());
    _physics_system->SetGravity({0, cGravity, 0});

    _interface      = &_physics_system->GetBodyInterface();
    _lock_interface = &_physics_system->GetBodyLockInterface();
}

PhysicsEngine::~PhysicsEngine() {}

/** Indicate that the physics engine is ready to start computing. This should be called after an object has been added or removed, ideally. */
void PhysicsEngine::Ready() {
    _physics_system->OptimizeBroadPhase();
}

void PhysicsEngine::Update(double dt) {
    const int cCollisionSteps = 4;
    _physics_system->Update(dt, cCollisionSteps, _allocator.get(), _job_system.get());
};

JPH::BodyInterface* PhysicsEngine::Interface() const {
    return _interface;
}

const JPH::BodyLockInterface* PhysicsEngine::LockInterface() const {
    return _lock_interface;
}

std::shared_ptr<JPH::PhysicsSystem> PhysicsEngine::PhysicsSystem() {
    return _physics_system;
}