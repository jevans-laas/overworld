#ifndef OWDS_ROSSENDER_H
#define OWDS_ROSSENDER_H

#include "SharedMemory/PhysicsClientC_API.h"
#include "overworld/BasicTypes/Entity.h"
#include <visualization_msgs/MarkerArray.h>

#include <geometry_msgs/TransformStamped.h>
#include <tf2_ros/transform_broadcaster.h>

#include <image_transport/image_transport.h>

#include <unordered_map>

namespace owds {
class ROSSender
{
  public:
    ROSSender(ros::NodeHandle* n);

    template<typename T> void sendEntitiesToTF(const std::vector<T*>& entities);
    template<typename T> void sendEntitiesToRViz(const std::string& topic_name, const std::vector<T*>& entities);
    template<typename T> void sendEntitiesToTFAndRViz(const std::string& rviz_topic_name, const std::vector<T*>& entities);
    template<typename T> void sendEntitiesToTF(const std::map<std::string, T*>& entities);
    template<typename T> void sendEntitiesToRViz(const std::string& topic_name, const std::map<std::string, T*>& entities);
    template<typename T> void sendEntitiesToTFAndRViz(const std::string& rviz_topic_name, const std::map<std::string, T*>& entities);
    void sendImage(const std::string& topic_name, const b3CameraImageData& image);

  protected:
    ros::NodeHandle* n_;
    tf2_ros::TransformBroadcaster tf_broadcaster_;
    image_transport::ImageTransport it_;

    std::unordered_map<std::string, ros::Publisher> rviz_publishers_;
    std::unordered_map<std::string, image_transport::Publisher> image_publishers_;
};

template<typename T>
void ROSSender::sendEntitiesToTF(const std::vector<T*>& entities)
{
    static_assert(std::is_base_of<Entity,T>::value, "T must be derived from Entity");
    std::vector<geometry_msgs::TransformStamped> transforms;
    for (const auto& entity : entities)
    {
        if (entity->isLocated())
        {
            transforms.push_back(entity->toTfTransform());
        }
    }
    tf_broadcaster_.sendTransform(transforms);
}

template<typename T>
void ROSSender::sendEntitiesToRViz(const std::string& topic_name, const std::vector<T*>& entities)
{
    static_assert(std::is_base_of<Entity,T>::value, "T must be derived from Entity");
    if (rviz_publishers_.count(topic_name) == 0)
    {
        rviz_publishers_.insert(std::make_pair(topic_name, n_->advertise<visualization_msgs::MarkerArray>(topic_name, 1, false)));
    }
    ros::Publisher& pub = rviz_publishers_.at(topic_name);
    visualization_msgs::MarkerArray markers;
    size_t id = 0;
    for (const auto& entity : entities)
    {
        if (entity->isLocated())
        {
            markers.markers.push_back(entity->toMarker(id++, 1.0, "marker"));
        }
    }
    pub.publish(markers);
}

template<typename T>
void ROSSender::sendEntitiesToTFAndRViz(const std::string& rviz_topic_name, const std::vector<T*>& entities)
{
    static_assert(std::is_base_of<Entity,T>::value, "T must be derived from Entity");
    if (rviz_publishers_.count(rviz_topic_name) == 0)
    {
        rviz_publishers_.insert(std::make_pair(rviz_topic_name, n_->advertise<visualization_msgs::MarkerArray>(rviz_topic_name, 1, false)));
    }
    ros::Publisher& pub = rviz_publishers_.at(rviz_topic_name);
    std::vector<geometry_msgs::TransformStamped> transforms;
    visualization_msgs::MarkerArray markers;
    size_t id = 0;
    for (const auto& entity : entities)
    {
        if (entity->isLocated())
        {
            markers.markers.push_back(entity->toMarker(id++, 1.0, "marker"));
            transforms.push_back(entity->toTfTransform());
        }
    }
    pub.publish(markers);
    tf_broadcaster_.sendTransform(transforms);
}

template<typename T>
void ROSSender::sendEntitiesToTF(const std::map<std::string, T*>& entities)
{
    static_assert(std::is_base_of<Entity,T>::value, "T must be derived from Entity");
    std::vector<geometry_msgs::TransformStamped> transforms;
    for (const auto& entity : entities)
    {
        if (entity.second->isLocated())
        {
            transforms.push_back(entity.second->toTfTransform());
        }
    }
    tf_broadcaster_.sendTransform(transforms);
}

template<typename T>
void ROSSender::sendEntitiesToRViz(const std::string& topic_name, const std::map<std::string, T*>& entities)
{
    static_assert(std::is_base_of<Entity,T>::value, "T must be derived from Entity");
    if (rviz_publishers_.count(topic_name) == 0)
    {
        rviz_publishers_.insert(std::make_pair(topic_name, n_->advertise<visualization_msgs::MarkerArray>(topic_name, 1, false)));
    }
    ros::Publisher& pub = rviz_publishers_.at(topic_name);
    visualization_msgs::MarkerArray markers;
    size_t id = 0;
    for (const auto& entity : entities)
    {
        if (entity.second->isLocated())
        {
            markers.markers.push_back(entity.second->toMarker(id++, 1.0, "marker"));
        }
    }
    pub.publish(markers);
}

template<typename T>
void ROSSender::sendEntitiesToTFAndRViz(const std::string& rviz_topic_name, const std::map<std::string, T*>& entities)
{
    static_assert(std::is_base_of<Entity,T>::value, "T must be derived from Entity");
    if (rviz_publishers_.count(rviz_topic_name) == 0)
    {
        rviz_publishers_.insert(std::make_pair(rviz_topic_name, n_->advertise<visualization_msgs::MarkerArray>(rviz_topic_name, 1, false)));
    }
    ros::Publisher& pub = rviz_publishers_.at(rviz_topic_name);
    std::vector<geometry_msgs::TransformStamped> transforms;
    visualization_msgs::MarkerArray markers;
    size_t id = 0;
    for (const auto& entity : entities)
    {
        if (entity.second->isLocated())
        {
            markers.markers.push_back(entity.second->toMarker(id++, 1.0, "marker"));
            transforms.push_back(entity.second->toTfTransform());
        }
    }
    pub.publish(markers);
    tf_broadcaster_.sendTransform(transforms);
}

} // namespace owds

#endif /* OWDS_ROSSENDER_H */
