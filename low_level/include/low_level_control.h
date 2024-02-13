/* create low level control class that 
allows for direct control of joint states
i.e. joint1 = 90 degrees so joint1 will move to 
90 degrees */

// use joint_states to get current joint states
// use joint_trajectory_controller to set joint states similar to cartgoal

#ifndef LOW_LEVEL_CONTROL_H
#define LOW_LEVEL_CONTROL_H

#include <ros/ros.h>
#include <vector>
#include <control_msgs/FollowJointTrajectoryAction.h>
#include <trajectory_msgs/JointTrajectoryPoint.h>
#include <elfin_robot_msgs/SetInt16.h>
#include <elfin_robot_msgs/SetFloat64.h>
#include <std_msgs/Empty.h>
#include <std_srvs/SetBool.h>
#include <std_msgs/Int64.h>
#include <std_msgs/Empty.h>
#include <tf/transform_listener.h>
#include <tf_conversations/tf_eigen.h>

namespace low_level_control {
    
class LowLevelControl {
    public:
        LowLevelControl(ros::NodeHandle &nh);
        ~LowLevelControl();
        void JointCmdCB(const std_msgs::Int64ConstPtr &msg);
        void StopCB(const std_msgs::EmptyConstPtr &msg);

        void setVelocityScaling(double data);
        void setRefFrames(std::string ref_link);
        void setEndFrames(std::string end_link);

        bool jointMove_cb(elfin_robot_msgs::SetInt16::Request &req, elfin_robot_msgs::SetInt16::Response &resp);
        bool stop_cb(std::srvs::SetBool::Request &req, std_srvs::SetBool::Response &resp);

        void PoseStampedRotation(geometry_msgs::PoseStamped &pose_stamped, const tf::Vector3 &axis, double angle);

    private:
        ros::NodeHandle nh_;
        ros::Publisher joint_cmd_pub_;
        ros::Publisher stop_pub_;
        ros::ServiceServer joint_move_srv_;
        ros::ServiceServer stop_srv_;
        ros::ServiceClient set_velocity_scaling_srv_;
        ros::ServiceClient set_ref_frames_srv_;
        ros::ServiceClient set_end_frames_srv_;
        ros::Subscriber joint_cmd_sub_;
        ros::Subscriber stop_sub_;

        std::string joint_cmd_topic_;
        std::string stop_topic_;
        std::string joint_move_srv_name_;
        std::string stop_srv_name_;
        std::string set_velocity_scaling_srv_name_;
        std::string set_ref_frames_srv_name_;
        std::string set_end_frames_srv_name_;
        std::string ref_link_;
        std::string end_link_;

        double velocity_scaling_;
        bool stop_flag_;
        bool joint_cmd_flag_;
        int joint_cmd_data_;
        std::vector<double> joint_cmd_data_vec_;
        double joint_step_;
        double joint_duration_ns_;
        double joint_speed_default_;
        double resolution_angle_;
        double resolution_linear_;

        double joint_speed;
        double cart_duration;

        std::string end_link_;
        std::string ref_link_;
        std::string default_tip_link_;
        std::string root_link_;

        tf::TransformListener tf_listener_;
        tf::StampedTransform transform_rootToRef_;
        tf::StampedTransform transform_rootToEnd_;
    };

}

#endif // LOW_LEVEL_CONTROL_H