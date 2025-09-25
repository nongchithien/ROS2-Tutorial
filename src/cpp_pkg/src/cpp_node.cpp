#include "rclcpp/rclcpp.hpp"

class MyNode : public rclcpp::Node
{
public:
    MyNode() : Node("greeting"), counter_(0)
    {
        RCLCPP_INFO(get_logger(),"Hello world");
        timer_ =create_wall_timer(std::chrono::seconds(1),
                                  std::bind(&MyNode::TimerCallback, this));
    

    }
private:
    void TimerCallback(){
        RCLCPP_INFO(get_logger(),"counting %d", counter_);
        counter_ ++ ;
    }
    rclcpp::TimerBase::SharedPtr timer_;
    int counter_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

    /// Do some work here
    auto node = std::make_shared<MyNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}