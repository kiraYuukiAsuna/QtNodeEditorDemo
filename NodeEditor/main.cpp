#include <iostream>
#include "NodeModel.h"
#include <thread>


int main() {
    // 线程安全的model对象
    Flexible::SafeFlexibleModel safeModel;

    // 通过读写锁访问对象
    // 读取
    auto basicInfo = safeModel.readAccess()->basicInfo;
    Flexible::RepeatType repeatType = safeModel.readAccess()->basicInfo.repeatType;
    // 或者
    {
        auto readAccess = safeModel.readAccess();
        auto basicInfo2 = readAccess->basicInfo;
        Flexible::RepeatType repeatType = readAccess->basicInfo.repeatType;
    }

    // 写入
    safeModel.writeAccess()->basicInfo.description = "xxx";
    safeModel.writeAccess()->basicInfo.repeatType = Flexible::RepeatType::Daily;
    // 或者
    {
        auto writeAccess = safeModel.writeAccess();
        writeAccess->basicInfo.description = "xxx";
        writeAccess->basicInfo.repeatType = Flexible::RepeatType::Daily;
    }

    // 反序列化成json对象
    nlohmann::json jsonOject = *safeModel.readAccess();

    // json对象到字符串
    std::string jsonStr = jsonOject.dump(4);

    // 解析json字符串到json对象
    nlohmann::json jsonObject2 = nlohmann::json::parse(jsonStr);

    // json对象到线程安全的model对象(需先构造普通对象再构造线程安全对象)
    // 普通对象
    Flexible::FlexibleModel flexible_model = jsonObject2.get<Flexible::FlexibleModel>();
    // 构造线程安全对象
    Flexible::SafeFlexibleModel safe_flexible_model(flexible_model);

    // 至此完成 线程安全对象->json对象->json字符串->json对象->线程安全对象 的闭合

    // 一些有用的辅助功能
    // copy 用于拷贝线程安全对象的数据内容，因为线程安全对象带有mutex锁所以该对象不可复制，当然你可以选择使用引用或指针方式，或者使用智能指针做包装
    Flexible::FlexibleModel copy_model =  safe_flexible_model.copy();
    Flexible::SafeFlexibleModel& ref = safe_flexible_model;
    Flexible::SafeFlexibleModel* pointer = &safe_flexible_model;
    auto smart_pointer_wrapper = std::make_shared<Flexible::SafeFlexibleModel>();

    // assign 拷贝一个普通对象到线程安全对象
    safe_flexible_model.assign(copy_model);

    // unsafe 以不安全的方式访问线程安全对象里的数据
    auto basicInfo_unsafe = safe_flexible_model.unsafe().basicInfo;

    // mutex 获取线程安全对象的互斥量
    auto& mutex = safe_flexible_model.mutex();

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
