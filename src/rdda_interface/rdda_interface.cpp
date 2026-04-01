#include "rdda_interface.h"

using namespace std;

/* RDDNode constructor */
RDDNode::RDDNode(Rdda *rddaptr, std::string node_name) : Node(node_name){

    rdda = rddaptr;

    // rdda_packet_sub = 

    // rdda_packet_sub = this->create_subscription<rdda_interface::msg::RDDAPacket>(
    //     "/rdda_right_master_output", 
    //     1, 
    //     std::bind(&RDDNode::rddapacket_callback, this, std::placeholders::_1));

    rdda_packet_pub = this->create_publisher<rdda_interface::msg::RDDAPacket>(
        "/rdda_packet", 1);

    rdda_joint_state_pub = this->create_publisher<sensor_msgs::msg::JointState>(
        "/rdda_joint_states", 1);

    // if (node_type == "right_gripper") {
    //     rdda_packet_sub = node_.subscribe("/rdda_right_master_output", 1, &RDDNode::rddapacket_callback, this);
    //     rdda_packet_pub = node_.advertise<rdda_interface::msg::RDDAPacket>("/rdda_right_master_input", 1);
    //     rdda_joint_state_pub = node_.advertise<sensor_msgs::msg::JointState>("/right_gripper_joint_states", 1);
    // }
    // else if (node_type == "right_glove") {
    //     rdda_packet_sub = node_.subscribe("/rdda_right_master_input", 1, &RDDNode::rddapacket_callback, this);
    //     rdda_packet_pub = node_.advertise<rdda_interface::msg::RDDAPacket>("/rdda_right_master_output", 1);
    //     rdda_joint_state_pub = node_.advertise<sensor_msgs::msg::JointState>("/right_glove_joint_states", 1);
    // }
    // else if (node_type == "left_gripper") {
    //     rdda_packet_sub = node_.subscribe("/rdda_l_master_output", 1, &RDDNode::rddapacket_callback, this);
    //     rdda_packet_pub = node_.advertise<rdda_interface::msg::RDDAPacket>("/rdda_l_master_input", 1);
    //     rdda_joint_state_pub = node_.advertise<sensor_msgs::msg::JointState>("/left_gripper_joint_states", 1);
    // }
    // else if (node_type == "left_glove") {
    //     rdda_packet_sub = node_.subscribe("/rdda_l_master_input", 1, &RDDNode::rddapacket_callback, this);
    //     rdda_packet_pub = node_.advertise<rdda_interface::msg::RDDAPacket>("/rdda_l_master_output", 1);
    //     rdda_joint_state_pub = node_.advertise<sensor_msgs::msg::JointState>("/left_glove_joint_states", 1);
    //     rdda_packet_aux_pub = node_.advertise<rdda_interface::msg::RDDAPacket>("/rdda_left_glove_high_freq", 1);
    // }

    // else if (node_type == "remote") {
    //     rdda_packet_sub = node_.subscribe("/joint_cmds", 1, &RDDNode::subJointCommands_callback, this);
    //     rdda_packet_pub = node_.advertise<rdda_interface::msg::RDDAPacket>("/rdda_output", 1);
    // }

    // ROS_INFO("Node initialized");
    RCLCPP_INFO(this->get_logger(), "Node initialized");

//     rdda_maxvel_srv = nh_.advertiseService("set_max_vel", &RDDNode::setMaxVel, this);
//     rdda_maxeff_srv = nh_.advertiseService("set_max_eff", &RDDNode::setMaxEffort, this);
//     rdda_stiff_srv = nh_.advertiseService("set_stiff", &RDDNode::setStiffness, this);
}

RDDNode::~RDDNode() = default;

/* Initialize interface with ROS parameters. */
void RDDNode::initConfigParams() {
    // double freq, stiff[MOTOR_COUNT], max_vel[MOTOR_COUNT], max_eff[MOTOR_COUNT];
//    bool need_homing;

    // mutex_lock(&rdda->mutex);

    // for (int i=0; i<MOTOR_COUNT; ++i) {
    //     if (rclcpp::param::get("/stiff", stiff[i])) {
    //         rdda->motor[i].stiffness = stiff[i];
    //     }
    //     if (rclcpp::param::get("/max_vel", max_vel[i])) {
    //         rdda->motor[i].vel_sat = max_vel[i];
    //     }
    //     if (rclcpp::param::get("/max_eff", max_eff[i])) {
    //         rdda->motor[i].tau_sat = max_eff[i];
    //     }
    // }
    // if (rclcpp::param::get("/anti_alias_freq", freq)) {
    //     rdda->freq_anti_alias = freq;
    // }

    // mutex_unlock(&rdda->mutex);
    // ROS_INFO("Node config initialized");
    teleop_connection_index = false;

    // if (node_type == "right_glove" || "right_gripper") joint_names = {"right_index_flex_motor_joint", "right_thumb_flex_motor_joint", "right_thumb_swivel_motor_joint"};
    // else if (node_type == "left_glove" || "left_gripper") joint_names = {"left_index_flex_motor_joint", "left_thumb_flex_motor_joint", "left_thumb_swivel_motor_joint"};

    joint_names = {"input_z-axis", 
                "input_shoulder",
                "inout_elbow",
                "output_z-axis",
                "output_shoulder",
                "output_elbow"};

    // // Homing slave gripper
    // if (node_type == "right_gripper" || node_type == "left_gripper") {
    //     // homing_finger();
    //     // ROS_INFO("Slave gripper homed");
    // }

    // if (node_type == "remote") {
    //     // homing_finger();
    //     // ROS_INFO("Remote gripper homed");
    // }
    // rdda->ts.remote_stamp = double(this->now().seconds());

}

// // A dummy callback to signal completion of slave gripper homing
// bool RDDNode::initSlave(std_srvs::srv::Empty::Request &req, std_srvs::srv::Empty::Response &resp) {
//     return true;
// }

// // Homing slave gripper finger before actuation
// void RDDNode::homing_finger() {
//     rdda_interface::msg::RDDAPacket packet_msg;

//     double tau_upper_limit = 0.3;
//     double tau_lower_limit = -0.4;
//     double control_step = 0.05;
//     std::vector<double> pos_ref{rdda->motor[0].rddaPacket.pos_out, rdda->motor[1].rddaPacket.pos_out, rdda->motor[2].rddaPacket.pos_out};
//     rclcpp::Rate loop_rate(20);
//     std::vector<bool> opened{false, false};

//     // set homing stiffness to 5.0
//     for (int i = 0; i < MOTOR_COUNT; i ++) {
//         rdda->motor[i].rddaPacket.pos_ref = pos_ref[i];
//     }

//     // IMPORTANT!! For pos_ref to be updated
//     rclcpp::sleep_for(std::chrono::milliseconds(100));

//     for (int i = 0; i < MOTOR_COUNT; i ++) {
//         rdda->motor[i].stiffness = 5.0;
//     }

//     // Open two fingers to lower bound
//     while(rclcpp::ok() && (!opened[0] || !opened[1] || !opened[2])) {
//         for (int i = 0; i < MOTOR_COUNT; i ++) {
//             if (rdda->motor[i].rddaPacket.tau < tau_upper_limit) {
//                 // mutex_lock(&rdda->mutex);
//                 pos_ref[i] += control_step;
//                 rdda->motor[i].rddaPacket.pos_ref = pos_ref[i];
//                 std::cout << rdda->motor[0].rddaPacket.tau << " " << rdda->motor[1].rddaPacket.tau << "" 
//                 << rdda->motor[2].rddaPacket.tau << std::endl;
//                 // mutex_unlock(&rdda->mutex);
//             }
//             else {
//                 opened[i] = true;
//             }
//         }
//         loop_rate.sleep();
//     }

//     // ROS_INFO("Fingers opened to the max");

//     // Reset stiffness to 0
//     for (int i = 0; i < MOTOR_COUNT; i ++) {
//         rdda->motor[i].stiffness = 0.0;
//         rdda->motor[i].rddaPacket.tau_ref = 0.1;
//     }
    
//     // Wait for stiffness to be updated
//     rclcpp::sleep_for(std::chrono::milliseconds(100));


//     // Reset motor init position
//     for (int i = 0; i < MOTOR_COUNT; i ++) {
//         rdda->motor[i].init_pos = rdda->motor[i].motorIn.act_pos;
//     }

//     // init_srv = node_.advertiseService("/slave_initialized", &RDDNode::initSlave, this);
// }

/* Publish rdda joint msgs at 100Hz */
void RDDNode::publish_rdda_joint_state() {
    sensor_msgs::msg::JointState states;
    // double series_stiffness = 4.0;
    states.effort.resize(joint_names.size());
    states.name.resize(joint_names.size());
    states.position.resize(joint_names.size());
    states.velocity.resize(joint_names.size());
    states.header.stamp = this->now();
    mutex_lock(&rdda->mutex);
    for (size_t i = 0; i < joint_names.size(); i ++) {
        states.name[i] = joint_names[i];
        // states.position[i] = rdda->motor[i].rddaPacket.pos_out + rdda->motor[i].motorIn.act_pre / series_stiffness;
        // states.velocity[i] = rdda->motor[i].rddaPacket.vel_out;
        // states.effort[i] = -rdda->motor[i].motorIn.act_pre;
        states.position[i] = rdda->motor[i].motorIn.act_pos;
        states.velocity[i] = rdda->motor[i].motorIn.act_vel;
        states.effort[i] = -rdda->motor[i].motorIn.act_tau;
    }
    mutex_unlock(&rdda->mutex);
    rdda_joint_state_pub->publish(states);
}

/* Publish rdda packet through ROS */
void RDDNode::publish_rddapacket() {

    rdda_interface::msg::RDDAPacket packet_msg;
    packet_msg.pos.resize(MOTOR_COUNT);
    packet_msg.vel.resize(MOTOR_COUNT);
    packet_msg.tau.resize(MOTOR_COUNT);
    packet_msg.wave.resize(MOTOR_COUNT);
    // packet_msg.wave_aux.resize(MOTOR_COUNT);
    packet_msg.pressure.resize(MOTOR_COUNT);
    // packet_msg.contact_flag.resize(MOTOR_COUNT);
    // packet_msg.test.resize(MOTOR_COUNT);
    packet_msg.delay_energy_reservior.resize(MOTOR_COUNT);
    packet_msg.pos_d.resize(MOTOR_COUNT);
    packet_msg.energy.resize(MOTOR_COUNT);
    packet_msg.ct.resize(MOTOR_COUNT);

    mutex_lock(&rdda->mutex);

    for (int i = 0; i < MOTOR_COUNT; i ++) {
        packet_msg.pos[i] = rdda->motor[i].motorOut.tg_pos;
        packet_msg.vel[i] = rdda->motor[i].motorOut.vel_off;
        packet_msg.tau[i] = rdda->motor[i].motorOut.tau_off;

        // packet_msg.pos[i] = rdda->motor[i].rddaPacket.pos_out;
        // packet_msg.vel[i] = rdda->motor[i].rddaPacket.vel_out;
        // packet_msg.tau[i] = rdda->motor[i].rddaPacket.tau;
        // packet_msg.contact_flag[i] = rdda->motor[i].rddaPacket.contact_flag;
        // packet_msg.wave[i] = rdda->motor[i].rddaPacket.wave_out;
        // packet_msg.wave_aux[i] = rdda->motor[i].rddaPacket.wave_out_aux;
        // packet_msg.test[i] = rdda->motor[i].rddaPacket.test;
        // packet_msg.pressure[i] = rdda->motor[i].motorIn.act_pre;
        // packet_msg.delay_energy_reservior[i] = rdda->motor[i].rddaPacket.delay_energy_reservior;
        // packet_msg.pos_d[i] = rdda->motor[i].rddaPacket.pos_d_out;
        // packet_msg.energy[i] = rdda->motor[i].rddaPacket.energy_tdpa_out;
        // packet_msg.ct[i] = rdda->motor[i].rddaPacket.coupling_torque_out;
    }

    
    // packet_msg.error_signal = rdda->error_signal.error_out;
    packet_msg.local_stamp = double(this->now().seconds());
    // packet_msg.local_stamp = double(rclcpp::Time::now().seconds());
    // packet_msg.remote_stamp = rdda->ts.remote_stamp;
    // packet_msg.time_delay = rdda->ts.delay_cycle * 0.25e-3;

    
    mutex_unlock(&rdda->mutex);
    
    // if (rdda_packet_pub.getNumSubscribers() == 0)
    // {
    //     if (node_type == "right_gripper") {
    //         ROS_ERROR_STREAM("Connection lost, trying to reconnect...");
    //         rdda_packet_pub.shutdown();
    //         rdda_packet_pub = nh_.advertise<rdda_interface::RDDAPacket>("/rdda_right_master_input", 1);
    //     }
    //     else if (node_type == "right_glove") {
    //         ROS_ERROR_STREAM("Connection lost, trying to reconnect...");
    //         rdda_packet_pub.shutdown();
    //         rdda_packet_pub = nh_.advertise<rdda_interface::RDDAPacket>("/rdda_right_master_output", 1);
    //     }
    //     else if (node_type == "left_gripper") {
    //         ROS_ERROR_STREAM("Connection lost, trying to reconnect...");
    //         rdda_packet_pub.shutdown();
    //         rdda_packet_pub = nh_.advertise<rdda_interface::RDDAPacket>("/rdda_left_master_input", 1);
    //     }
    //     else if (node_type == "left_glove") {
    //         ROS_ERROR_STREAM("Connection lost, trying to reconnect...");
    //         rdda_packet_pub.shutdown();
    //         rdda_packet_pub = nh_.advertise<rdda_interface::RDDAPacket>("/rdda_left_master_output", 1);
    //     }
    // }
    
    packet_msg.header.stamp = this->now();

    rdda_packet_pub->publish(packet_msg);

    // ROS_INFO("Published RDDARead message");
}

// /* Publish rdda packet through ROS */
// void RDDNode::publish_rddapacket_delay() {

//     rdda_interface::msg::RDDAPacket packet_msg;
//     packet_msg.pos.resize(MOTOR_COUNT);
//     packet_msg.vel.resize(MOTOR_COUNT);
//     packet_msg.pressure.resize(MOTOR_COUNT);
//     packet_msg.wave.resize(MOTOR_COUNT);
//     packet_msg.pos_d.resize(MOTOR_COUNT);

//     mutex_lock(&rdda->mutex);

//     for (int i = 0; i < MOTOR_COUNT; i ++) {
//         pos_d[current_index][i] = rdda->motor[i].rddaPacket.pos_out;
//         vel_d[current_index][i] = rdda->motor[i].rddaPacket.vel_out;
//         pre_d[current_index][i] = rdda->motor[i].motorIn.act_pre;
//         wave_d[current_index][i] = rdda->motor[i].rddaPacket.wave_out;
//         pos_dd[current_index][i] = rdda->motor[i].rddaPacket.pos_d_out;
//     }

    
//     delay_index = current_index - added_delay;
//     if (delay_index < 0) delay_index += delay_max;
//     current_index++;
//     if (current_index >= delay_max) current_index = 0;

//     for (int i = 0; i < MOTOR_COUNT; i ++) {
//         packet_msg.pos[i] = pos_d[delay_index][i];
//         packet_msg.vel[i] = vel_d[delay_index][i];
//         packet_msg.pressure[i] = pre_d[delay_index][i];
//         packet_msg.wave[i] = wave_d[delay_index][i];
//         packet_msg.pos_d[i] = pos_dd[delay_index][i];
//     }

//     packet_msg.error_signal = rdda->error_signal.error_out;
//     packet_msg.local_stamp = double(this->now().seconds());
//     packet_msg.remote_stamp = rdda->ts.remote_stamp;
//     packet_msg.time_delay = rdda->ts.delay_cycle * 0.25e-3;

//     mutex_unlock(&rdda->mutex);

//     rdda_packet_pub->publish(packet_msg);
// }

// /* Publish rdda packet through ROS */
// void RDDNode::publish_rddapacket_aux() {

//     rdda_interface::msg::RDDAPacket packet_msg;
//     packet_msg.pos.resize(MOTOR_COUNT);
//     packet_msg.vel.resize(MOTOR_COUNT);
//     packet_msg.tau.resize(MOTOR_COUNT);
//     packet_msg.wave.resize(MOTOR_COUNT);
//     packet_msg.wave_aux.resize(MOTOR_COUNT);
//     packet_msg.pressure.resize(MOTOR_COUNT);
//     packet_msg.test.resize(MOTOR_COUNT);
//     packet_msg.delay_energy_reservior.resize(MOTOR_COUNT);

//     mutex_lock(&rdda->mutex);

//     for (int i = 0; i < MOTOR_COUNT; i ++) {
//         packet_msg.pos[i] = rdda->motor[i].rddaPacket.pos_out;
//         packet_msg.vel[i] = rdda->motor[i].rddaPacket.vel_out;
//         packet_msg.tau[i] = rdda->motor[i].rddaPacket.tau;
//         packet_msg.wave[i] = rdda->motor[i].rddaPacket.wave_out;
//         packet_msg.wave_aux[i] = rdda->motor[i].rddaPacket.wave_out_aux;
//         packet_msg.test[i] = rdda->motor[i].rddaPacket.test;
//         packet_msg.pressure[i] = rdda->motor[i].motorIn.act_pre;
//         packet_msg.delay_energy_reservior[i] = rdda->motor[i].rddaPacket.delay_energy_reservior;
//     }
//     packet_msg.time_delay = rdda->ts.delay_cycle * 0.25e-3;

//     mutex_unlock(&rdda->mutex);

//     rdda_packet_aux_pub->publish(packet_msg);
// }

// /* Subscriber callback */
// /* Comment out callback for remote test */
// void RDDNode::rddapacket_callback(const rdda_interface::msg::RDDAPacket::SharedPtr packet_msg) {

//     mutex_lock(&rdda->mutex);
//     for (int i = 0; i < MOTOR_COUNT; i ++) {
//         // Use this for data collection
//         // rdda->motor[i].rddaPacket.pos_in = packet_msg->pos[i];
//         // rdda->motor[i].rddaPacket.wave_in = packet_msg->wave[i];

//         rdda->motor[i].rddaPacket.pos_ref = -1.0 * packet_msg->pos[i]; // use this for policy rollout, add negative sign to synchronize the direction

//         // rdda->motor[i].rddaPacket.vel_in = packet_msg->vel[i];
//         // rdda->motor[i].rddaPacket.wave_in_aux = packet_msg->wave_aux[i];
//         // rdda->motor[i].rddaPacket.pos_d_in = packet_msg->pos_d[i];
//         // rdda->motor[i].rddaPacket.pre_in = packet_msg->pressure[i];
//         // rdda->motor[i].rddaPacket.energy_tdpa_in = packet_msg->energy[i];
//         // rdda->motor[i].rddaPacket.coupling_torque_in = packet_msg->ct[i];
//     }
//     rdda->error_signal.error_in = packet_msg->error_signal;
//     rdda->ts.remote_stamp = packet_msg->local_stamp;
//     double last_local_stamp = packet_msg->remote_stamp;
//     rclcpp::Time local_stamp = this->now();
//     rdda->ts.delay_cycle = int((local_stamp.seconds() - last_local_stamp) / 0.25e-3 / 2);

//     if (!teleop_connection_index) {
//         for (int i = 0; i < MOTOR_COUNT; i ++) {
//             rdda->motor[i].rddaPacket.tau_ref = 0.0;
//         }
//     }
//     mutex_unlock(&rdda->mutex);

//     teleop_connection_index = true;
//     // ROS_INFO_THROTTLE(1, "Write into rdda memory, message delay: %lf", (local_stamp.seconds() - last_local_stamp) / 2);

// }

// void RDDNode::subJointCommands_callback(const trajectory_msgs::msg::JointTrajectoryPoint::SharedPtr JointCommands_msg) {

//     mutex_lock(&rdda->mutex);

//     for (int i = 0; i < MOTOR_COUNT; i ++) {
//         rdda->motor[i].rddaPacket.pos_ref = JointCommands_msg->positions[i];
//     }

//     // ROS_INFO("Set position reference: [%lf, %lf, %lf]", JointCommands_msg->positions[0], JointCommands_msg->positions[1], JointCommands_msg->positions[2]);
//     mutex_unlock(&rdda->mutex);
// }

// /* Service functions */
// bool RDDNode::setMaxVel(rdda_interface::SetMaxVelocity::Request &req, rdda_interface::SetMaxVelocity::Response &res) {

//     mutex_lock(&rdda->mutex);

//     req.max_vel.resize(MOTOR_COUNT);

//     for (int i = 0; i < MOTOR_COUNT; i ++) {
//         rdda->motor[i].vel_sat = req.max_vel[i];
//     }

//     res.err = 0;
//     ROS_INFO("Request: vel_sat = [%lf, %lf, %lf]", (double)req.max_vel[0], (double)req.max_vel[1], (double)req.max_vel[2]);
//     ROS_INFO("Error indicator: %d", res.err);

//     mutex_unlock(&rdda->mutex);
//     return true;
// }

// bool RDDNode::setMaxEffort(rdda_interface::SetMaxEffort::Request &req, rdda_interface::SetMaxEffort::Response &res) {

//     mutex_lock(&rdda->mutex);

//     req.max_effort.resize(MOTOR_COUNT);

//     for (int i = 0; i < MOTOR_COUNT; i ++) {
//         rdda->motor[i].tau_sat = req.max_effort[i];
//     }

//     res.err = 0;
//     ROS_INFO("Request: tau_sat = [%lf, %lf, %lf]", (double)req.max_effort[0], (double)req.max_effort[1], (double)req.max_effort[2]);

//     mutex_unlock(&rdda->mutex);
//     return true;
// }

// bool RDDNode::setStiffness(rdda_interface::SetStiffness::Request &req, rdda_interface::SetStiffness::Response &res) {

//     mutex_lock(&rdda->mutex);

//     req.stiffness.resize(MOTOR_COUNT);

//     for (int i = 0; i < MOTOR_COUNT; i ++) {
//         rdda->motor[i].stiffness = req.stiffness[i];
//     }

//     res.err = 0;
//     ROS_INFO("Request: stiffness = [%lf, %lf, %lf]", (double)req.stiffness[0], (double)req.stiffness[1], (double)req.stiffness[2]);

//     mutex_unlock(&rdda->mutex);
//     return true;
// }

/* Run loop */
void RDDNode::run() {
    int teleop_freq = 2000;
    int joint_state_pub_freq = 10;
    int joint_state_pub_index = 0;
    rclcpp::Rate loop_rate(teleop_freq);
    while (rclcpp::ok()) {
	/* Publisher (wrap) */
        // publish_rddapacket_aux();
        publish_rddapacket();
        // if (node_type == "right_gripper" || node_type == "right_glove") publish_rddapacket();
        // else publish_rddapacket_delay();
        if (joint_state_pub_index >= int(teleop_freq/joint_state_pub_freq)) joint_state_pub_index = 0;
        if (joint_state_pub_index == 0) {publish_rdda_joint_state();}
        joint_state_pub_index++;
	    /* Subscriber callback loop */
	    rclcpp::spin_some(this->shared_from_this());
	    loop_rate.sleep();
    }
}

int main(int argc, char** argv) {

    /* Instanciate input-output data varibles */
    Rdda *rdda;

    /* Map data structs to shared memory */
    /* Open and obtain shared memory pointers for master-input data */
    // char *name = &argv[1][0];
    rdda = initRdda();
    if (rdda == nullptr) {
        fprintf(stderr, "Init rdda failed.\n");
        printf("shm_open error, errno(%d): %s\n", errno, strerror(errno));
        exit(1);
    }

    /* Initialise ROS node */
    rclcpp::init(argc, argv);
    // rclcpp::NodeHandle node("~");
    // auto node = rclcpp::Node::make_shared(rdda, "rdda_interface");
    auto node = std::make_shared<RDDNode>(rdda, "rdda_interface");    
    printf("Launch ros interface\n");


    // initialize a node with "master" or "slave" setting
    // RDDNode rdd(node, rdda, std::string(argv[1]));
    node->initConfigParams();
    // wait indefinitely until the service is on (slave finger homing completed)
    // rclcpp::service::waitForService("/slave_initialized", -1);
    // ROS_INFO("Node starts running");
    RCLCPP_INFO(node->get_logger(),"node started");
    node->run();
}
