#ifndef RDDA_NODE_H
#define RDDA_NODE_H

/* C++ headers */
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <vector>
#include <algorithm>

/* ROS headers */
// #include <ros/ros.h>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float64.hpp>
#include <std_srvs/srv/empty.hpp>

#include <sensor_msgs/msg/joint_state.hpp>
#include <trajectory_msgs/msg/joint_trajectory_point.hpp>
//#include "rdda_interface/JointCommands.h"
//#include "rdda_interface/JointStates.h"
#include "rdda_interface/msg/control_state.hpp"
#include "rdda_interface/msg/rdda_packet.hpp"

// #include "rdda_interface/set_max_velocity.hpp"
// #include "rdda_interface/set_max_effort.hpp"
// #include "rdda_interface/set_stiffness.hpp"
// #include "rdda_interface/homing.hpp"


/* C headers */
extern "C" {
#include "shm_data.h"
#include "shm.h"
};

class RDDNode : public rclcpp::Node {
 public:
    explicit RDDNode(Rdda *rdda, std::string node_name);

    ~RDDNode();

    void run();
    void initConfigParams();

 private:
   //  rclcpp::Node node_;

   //  rclcpp::Subscription<rdda_interface::msg::RDDAPacket>::SharedPtr rdda_packet_sub;

    rclcpp::Publisher<rdda_interface::msg::RDDAPacket>::SharedPtr rdda_packet_pub;
    rclcpp::Publisher<rdda_interface::msg::RDDAPacket>::SharedPtr rdda_packet_aux_pub;
    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr rdda_joint_state_pub;

   //  rclcpp::ServiceServer rdda_maxvel_srv;
   //  rclcpp::ServiceServer rdda_maxeff_srv;
   //  rclcpp::ServiceServer rdda_stiff_srv;
   //  rclcpp::ServiceServer init_srv;

    Rdda *rdda;
    std::vector<std::string> joint_names;

    std::string node_type;

    void publish_rddapacket();
    void publish_rddapacket_aux();
    void publish_rdda_joint_state();
    void homing_finger();
   //  void rddapacket_callback(const rdda_interface::msg::RDDAPacket::SharedPtr msg);
   //  void rddapacket_callback(const rdda_interface::msg::RDDAPacket::ConstPtr &msg);

   //  void subJointCommands_callback(const trajectory_msgs::msg::JointTrajectoryPoint::SharedPtr msg);
   //  void subJointCommands_callback(const trajectory_msgs::msg::JointTrajectoryPoint::ConstPtr &msg);

   //  bool setMaxVel(rdda_interface::SetMaxVelocity::Request &req, rdda_interface::SetMaxVelocity::Response &res);
   //  bool setMaxEffort(rdda_interface::SetMaxEffort::Request &req, rdda_interface::SetMaxEffort::Response &res);
   //  bool setStiffness(rdda_interface::SetStiffness::Request &req, rdda_interface::SetStiffness::Response &res);
   //  bool initSlave(std_srvs::srv::Empty::Request &req, std_srvs::srv::Empty::Response &res);
   //  bool initSlave(std_srvs::srv::Empty::Request &req, std_srvs::srv::Empty::Response &res);

    bool teleop_connection_index;

    int delay_max = 2000*10;
    double pos_d[20000][MOTOR_COUNT];
    double vel_d[20000][MOTOR_COUNT];
    double pre_d[20000][MOTOR_COUNT];
    double wave_d[20000][MOTOR_COUNT];
    double pos_dd[20000][MOTOR_COUNT];
    int added_delay = 2000 * 3;
    int current_index = 0;
    int delay_index;
    void publish_rddapacket_delay();
};

#endif /* RDDA_NODE */
