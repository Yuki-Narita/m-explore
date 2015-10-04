#ifndef COSTMAP_CLIENT_
#define COSTMAP_CLIENT_

#include <ros/ros.h>
#include <costmap_2d/costmap_2d.h>
#include <nav_msgs/OccupancyGrid.h>
#include <map_msgs/OccupancyGridUpdate.h>
#include <geometry_msgs/PolygonStamped.h>
#include <tf/tf.h>
#include <tf/transform_listener.h>

namespace explore {

class Costmap2DClient {
public:
  /**
   * @brief Contructs client and start listening
   * @details Constructor will block until first map update is received and 
   * map is ready to use, also will block before trasformation
   * robot_base_frame <-> global_frame is available.
   * 
   * @param param_nh node hadle to retrieve parameters from
   * @param subscription_nh node hadle where topics will be subscribed
   * @param tf_listener Will be used for transformation of robot pose.
   */
	Costmap2DClient(
    ros::NodeHandle& param_nh,
    ros::NodeHandle& subscription_nh,
    const tf::TransformListener* tf_listener
  );
	/**
   * @brief Get the pose of the robot in the global frame of the costmap
   * @param global_pose Will be set to the pose of the robot in the global frame of the costmap
   * @return True if the pose was set successfully, false otherwise
   */
  bool getRobotPose(tf::Stamped<tf::Pose>& global_pose) const;

  /**
   * @brief Return a pointer to the "master" costmap which receives updates from all the layers.
   *
   * This pointer will stay the same for the lifetime of Costmap2DClient object.
   */
  costmap_2d::Costmap2D* getCostmap()
    {
      return &costmap_;
    }

  /**
   * @brief Return a pointer to the "master" costmap which receives updates from all the layers.
   *
   * This pointer will stay the same for the lifetime of Costmap2DClient object.
   */
  const costmap_2d::Costmap2D* getCostmap() const
    {
      return &costmap_;
    }

  /**
   * @brief  Returns the global frame of the costmap
   * @return The global frame of the costmap
   */
  const std::string& getGlobalFrameID() const
    {
      return global_frame_;
    }

  /**
   * @brief  Returns the local frame of the costmap
   * @return The local frame of the costmap
   */
  const std::string& getBaseFrameID() const
    {
      return robot_base_frame_;
    }

  const std::vector<geometry_msgs::Point>& getFootprint() const { return footprint_; }
  double getCircumscribedRadius() const { return circumscribed_radius_; }
  double getInscribedRadius() const { return inscribed_radius_; }

protected:
  void updateFullMap(const nav_msgs::OccupancyGrid::ConstPtr& msg);
  void updatePartialMap(const map_msgs::OccupancyGridUpdate::ConstPtr& msg);
  void updateFootPrint(const geometry_msgs::PolygonStamped::ConstPtr& msg);

	costmap_2d::Costmap2D costmap_;

  const tf::TransformListener* const tf_;  ///< @brief Used for transforming point clouds
  std::string global_frame_;  ///< @brief The global frame for the costmap
  std::string robot_base_frame_;  ///< @brief The frame_id of the robot base
  double transform_tolerance_;  ///< timeout before transform errors

  std::vector<geometry_msgs::Point> footprint_;
  double circumscribed_radius_, inscribed_radius_;
private:
  // will be unsubscribed at destruction
  ros::Subscriber costmap_sub_;
  ros::Subscriber costmap_updates_sub_;
  ros::Subscriber footprint_sub_;
};

} // namespace explore

#endif
