#include "overworld/Physics/Bullet3/Actor.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "overworld/Helper/BitCast.h"
#include "overworld/Physics/Bullet3/Context.h"

namespace owds::bullet3 {
  Actor::Actor(owds::bullet3::Context& ctx, const owds::Shape& shape)
    : owds::Actor(shape),
      ctx_(ctx)
  {}

  Actor::~Actor() noexcept
  {
    ctx_.bt_scene_->removeRigidBody(bt_actor_.get());
  }

  void Actor::setup()
  {
    std::visit([this](auto& elem) { setup(elem); }, shape_);

    const auto default_mass = 1.f;

    btRigidBody::btRigidBodyConstructionInfo info(default_mass, nullptr, bt_geometry_.get(), btVector3(0, 0, 0));
    bt_actor_ = std::make_unique<btRigidBody>(info);

    setMass(0.5);
    setRestitution(0.5);

    ctx_.bt_scene_->addRigidBody(bt_actor_.get());
  }

  void Actor::remove()
  {
    ctx_.actors_.erase(this);
  }

  void Actor::setPhysicsEnabled(const bool enabled)
  {
    if(enabled)
    {
      // todo: logic for re-enabling physics
    }
    else
    {
      bt_actor_->setMassProps(0, btVector3(0, 0, 0));
      bt_actor_->setCollisionFlags(bt_actor_->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
      bt_actor_->setLinearVelocity(btVector3(0, 0, 0));
      bt_actor_->setAngularVelocity(btVector3(0, 0, 0));
    }
  }

  void Actor::setSimulationEnabled(const bool enabled)
  {
    if(is_simulation_enabled_ == enabled)
    {
      return;
    }

    if(enabled)
    {
      ctx_.bt_scene_->addRigidBody(bt_actor_.get());
    }
    else
    {
      ctx_.bt_scene_->removeRigidBody(bt_actor_.get());
    }
  }

  void Actor::setMass(const float mass_kg)
  {
    btVector3 bt_local_inertia(0, 0, 0);
    bt_geometry_->calculateLocalInertia(mass_kg, bt_local_inertia);
    bt_actor_->setMassProps(mass_kg, bt_local_inertia);
  }

  void Actor::setStaticFriction(const float coefficient)
  {
    bt_actor_->setFriction(coefficient);
  }

  void Actor::setDynamicFriction(const float coefficient)
  {
    // uhhhh...
  }

  void Actor::setRestitution(const float coefficient)
  {
    bt_actor_->setRestitution(coefficient);
  }

  void Actor::setPositionAndOrientation(const std::array<float, 3>& position, const std::array<float, 3>& orientation)
  {
    const auto orientation_quat = glm::quat(owds::BitCast<glm::vec3>(orientation));
    const auto bt_transform = btTransform(
      btQuaternion(
        static_cast<btScalar>(orientation_quat.x),
        static_cast<btScalar>(orientation_quat.y),
        static_cast<btScalar>(orientation_quat.z),
        static_cast<btScalar>(orientation_quat.w)),
      btVector3(
        static_cast<btScalar>(position[0]),
        static_cast<btScalar>(position[1]),
        static_cast<btScalar>(position[2])));

    bt_actor_->setWorldTransform(bt_transform);
  }

  std::array<float, 16> Actor::getModelMatrix() const
  {
    const auto bt_transform = bt_actor_->getWorldTransform();
    const auto bt_origin = bt_transform.getOrigin();
    const auto bt_orientation = bt_transform.getRotation();

    const auto translation_mat =
      glm::translate(
        glm::mat4(1),
        glm::vec3(
          bt_origin.x(),
          bt_origin.y(),
          bt_origin.z()));

    const auto rotation_mat =
      glm::mat4_cast(
        glm::quat(
          bt_orientation.w(),
          bt_orientation.x(),
          bt_orientation.y(),
          bt_orientation.z()));

    return owds::BitCast<std::array<float, 16>>(translation_mat * rotation_mat);
  }

  std::pair<std::array<float, 3>, std::array<float, 3>> Actor::getPositionAndOrientation() const
  {
  }

  void Actor::setup(const owds::ShapeBox& shape)
  {
    bt_geometry_ = std::make_unique<btBoxShape>(btVector3(
      static_cast<btScalar>(shape.half_extents_[0]),
      static_cast<btScalar>(shape.half_extents_[1]),
      static_cast<btScalar>(shape.half_extents_[2])));
  }

  void Actor::setup(const owds::ShapeCapsule& shape)
  {
    bt_geometry_ = std::make_unique<btCapsuleShape>(
      static_cast<btScalar>(shape.radius_),
      static_cast<btScalar>(shape.height_));
  }

  void Actor::setup(const owds::ShapeCustomMesh& shape)
  {
    // todo: handle custom meshes
  }

  void Actor::setup(const owds::ShapeCylinder& shape)
  {
    bt_geometry_ = std::make_unique<btCylinderShape>(btVector3(
      static_cast<btScalar>(shape.radius_),
      static_cast<btScalar>(shape.height_),
      static_cast<btScalar>(shape.radius_)));
  }

  void Actor::setup(const owds::ShapeSphere& shape)
  {
    bt_geometry_ = std::make_unique<btSphereShape>(
      static_cast<btScalar>(shape.radius_));
  }
} // namespace owds::bullet3