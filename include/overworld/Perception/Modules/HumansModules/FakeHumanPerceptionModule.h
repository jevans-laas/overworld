#ifndef OWDS_FAKEHUMANPERCEPTIONMODULE_H
#define OWDS_FAKEHUMANPERCEPTIONMODULE_H

#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2_ros/transform_listener.h>

#include "ontologenius/OntologiesManipulator.h"
#include "overworld/AgentPose.h"
#include "overworld/BasicTypes/BodyPart.h"
#include "overworld/Perception/Modules/PerceptionModuleBase.h"

namespace owds {

  class FakeHumanPerceptionModule : public PerceptionModuleRosBase<BodyPart, overworld::AgentPose>
  {
  public:
    FakeHumanPerceptionModule();

    virtual bool closeInitialization() override;

  private:
    bool perceptionCallback(const overworld::AgentPose& msg);

    Percept<BodyPart> createPercept(const std::string& human_name, const std::string& part_name);

    onto::OntologiesManipulator* ontologies_manipulator_;
    onto::OntologyManipulator* onto_;

    tf2_ros::Buffer tf_buffer_;
    tf2_ros::TransformListener tf2_listener_;
  };

} // namespace owds

#endif // OWDS_FAKEHUMANPERCEPTIONMODULE_H
