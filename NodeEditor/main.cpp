#include <iostream>
#include "NodeModel.h"
#include <thread>


int main() {

}


void test() {
    // Flexible::FlexibleModel flexible_model;
    // flexible_model.basicInfo.description = "descriptionxxx";
    // flexible_model.basicInfo.status = "statusxxx";
    // flexible_model.basicInfo.uuid = "xxx";
    // flexible_model.basicInfo.jobId = "jobidxxx";
    // flexible_model.basicInfo.jobKey = "jobkeyxxx";
    // flexible_model.basicInfo.jobName = "jobNamexxx";
    // flexible_model.basicInfo.repeatType = Flexible::RepeatType::Single;
    // flexible_model.basicInfo.startTime = "startTimexxx";
    // flexible_model.basicInfo.timeZone = "timeZonexxx";
    //
    // Flexible::NodeModel node_model;
    // node_model.level = 10;
    // node_model.result = Flexible::ResultType::Success;
    // node_model.state = "OK";
    // node_model.timeout = 20;
    // node_model.uuid = "uuidxxx";
    // node_model.jobKey = "jobKeyxxx";
    // node_model.nodeType = Flexible::NodeType::Begin;
    //
    // node_model.inputs.push_back(Flexible::NodeInputModel{
    //     .jobKey = "jobkeyxxx",
    //     .condition = Flexible::ConditionType::Finished,
    //     .state = Flexible::InputStateType::Waitting,
    //     .failureType = Flexible::FailureType::Environment
    // });
    //
    // node_model.outputs.push_back(Flexible::NodeOutputModel{
    //     .jobKey = "jobkeyxxx",
    //     .condition = Flexible::ConditionType::Finished,
    //     .state = Flexible::OutputStateType::Running,
    //     .failureType = Flexible::FailureType::Environment
    // });
    //
    // flexible_model.nodes.push_back(node_model);
    //
    // nlohmann::json jx = flexible_model;
    // std::string fxstr = jx.dump(4);
    // std::cout << fxstr << std::endl;
    //
    // auto modelx = nlohmann::json::parse(fxstr);
    // std::cout << modelx.dump(4) << std::endl;
    // auto modelcc = modelx.get<Flexible::FlexibleModel>();

    //----------------------------------//

    // Flexible::NodeModel model1;
    // model1.nodeType = Flexible::NodeType::Begin;
    // model1.inputs.emplace_back();
    // model1.outputs.emplace_back();
    //
    // nlohmann::json jx = model1;
    // std::cout << jx.dump(4) << std::endl;
    // auto modelx = nlohmann::json::parse(jx.dump(4));
    //
    // auto modelcc = modelx.get<Flexible::NodeModel>();
    //
    // using SafeType = safe::Safe<Flexible::NodeModel>;
    // SafeType safeObject({},modelcc);
    // {
    //     auto value3 = safeObject.writeAccess(); // nicer, but only with C++17 and later
    //     std::cout << magic_enum::enum_name(value3->nodeType) << std::endl;
    // }
    // auto copy = safeObject.copy();
    //
    // nlohmann::json j = copy;
    // auto model2 = j.get<Flexible::NodeModel>();
    // std::cout << j.dump(4) << std::endl;
    //-------------------------------//
    // Flexible::SafeFlexibleModel safeModel;
    // safeModel.writeAccess()->basicInfo.description = "123";
    // safeModel.writeAccess()->basicInfo.description = "456";
    //
    // {
    //     auto access = safeModel.writeAccess();
    //     access->basicInfo.description = "789";
    // }
    //
    // try {
    //     std::thread t1([&]() {
    //         while (true) {
    //             auto access = safeModel.writeAccess();
    //             access->basicInfo.description = "1122";
    //         }
    //     });
    //
    //     std::thread t2([&]() {
    //         while (true) {
    //             auto access = safeModel.writeAccess();
    //            access->basicInfo.description = "3344";
    //         }
    //     });
    //
    //     t1.join();
    //     t2.join();
    // }catch (std::exception& e) {
    //     std::cerr<<e.what()<<"\n";
    // }
    //--------------------------------//
    // Flexible::SafeNodeModel safeModel;
    //
    // try {
    //     std::thread t1([&]() {
    //         for(int i=0;i<100;i++) {
    //             auto access = safeModel.writeAccess();
    //             access->timeout++;
    //             std::this_thread::sleep_for(std::chrono::milliseconds(50));
    //         }
    //         std::cout <<"++ end...\n";
    //     });
    //
    //     std::thread t2([&]() {
    //         for(int i=0;i<100;i++) {
    //             auto access = safeModel.writeAccess();
    //             access->timeout--;
    //             std::this_thread::sleep_for(std::chrono::milliseconds(50));
    //         }
    //         std::cout <<"-- end...\n";
    //     });
    //
    //     std::thread t3([&]() {
    //         for(int i=0;i<100;i++) {
    //             auto access = safeModel.readAccess();
    //             std::cout << "current value: " << access->timeout << "\n";
    //             std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //         }
    //     });
    //
    //     t1.join();
    //     t2.join();
    //     t3.join();
    // }
    // catch (std::exception&e) {
    //     std::cerr << e.what() << "\n";
    // }
}
