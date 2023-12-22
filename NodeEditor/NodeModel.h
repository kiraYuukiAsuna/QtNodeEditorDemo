#pragma once

#include "nlohmann/json.hpp"
#include <vector>
#include "safe/safe.h"
#include "magic_enum/magic_enum.hpp"

namespace Flexible {
    enum class RepeatType {
        Single,
        Daily
    };

    enum class NodeType {
        Begin,
        End,
        PerformanceJob,
        FunctionJob
    };

    enum class InputStateType {
        Waitting,
        Ended,
        Ignore
    };

    enum class OutputStateType {
        Waitting,
        Running,
        Ended,
        Ignore
    };

    enum class ResultType {
        Success,
        Failure,
        Ignore
    };

    enum class ConditionType {
        Success,
        Failure,
        Finished
    };

    enum class FailureType {
        All,
        Function,
        Environment,
        Others
    };

    class BasicInfoModel{
    public:
        std::string uuid{};
        std::string jobKey{};
        std::string jobId{};
        std::string jobName{};
        std::string status{};
        RepeatType repeatType{RepeatType::Daily};
        std::string startTime{};
        std::string timeZone{};
        std::string description{};

        friend void to_json(nlohmann::json &json, const BasicInfoModel &object) {
            json["uuid"] = object.uuid;
            json["jobKey"] = object.jobKey;
            json["jobId"] = object.jobId;
            json["jobName"] = object.jobName;
            json["status"] = object.status;
            json["repeatType"] = magic_enum::enum_name(object.repeatType);
            json["startTime"] = object.startTime;
            json["timeZone"] = object.timeZone;
            json["description"] = object.description;
        }

        friend void from_json(const nlohmann::json &json, BasicInfoModel &object) {
            BasicInfoModel defaultObject;
            object.uuid = json.value("uuid", defaultObject.uuid);
            object.jobKey = json.value("jobKey", defaultObject.jobKey);
            object.jobId = json.value("jobId", defaultObject.jobId);
            object.jobName = json.value("jobName", defaultObject.jobName);
            object.status = json.value("status", defaultObject.status);
            object.repeatType = magic_enum::enum_cast<RepeatType>(json.value("repeatType", magic_enum::enum_name(object.repeatType))).value_or(RepeatType::Daily);
            object.startTime = json.value("startTime", defaultObject.startTime);
            object.timeZone = json.value("timeZone", defaultObject.timeZone);
            object.description = json.value("description", defaultObject.description);
        };

    };

    class NodeInputModel{
    public:
        std::string jobKey{};
        ConditionType condition{ConditionType::Failure};
        InputStateType state{InputStateType::Ignore};
        FailureType failureType{FailureType::All};

        friend void to_json(nlohmann::json &json, const NodeInputModel &object) {
            json["jobKey"] = object.jobKey;
            json["condition"] = magic_enum::enum_name(object.condition);
            json["state"] = magic_enum::enum_name(object.state);
            json["failureType"] = magic_enum::enum_name(object.failureType);
        }

        friend void from_json(const nlohmann::json &json, NodeInputModel &object) {
            NodeInputModel defaultObject;
            object.jobKey = json.value("jobKey", defaultObject.jobKey);
            object.condition = magic_enum::enum_cast<ConditionType>(json.value("condition", magic_enum::enum_name(defaultObject.condition))).value_or(ConditionType::Failure);
            object.state = magic_enum::enum_cast<InputStateType>(json.value("state", magic_enum::enum_name(defaultObject.state))).value_or(InputStateType::Ignore);
            object.failureType = magic_enum::enum_cast<FailureType>(json.value("failureType", magic_enum::enum_name(defaultObject.failureType))).value_or(FailureType::All);
        };

    };

    class NodeOutputModel{
    public:
        std::string jobKey{};
        ConditionType condition{ConditionType::Failure};
        OutputStateType state{OutputStateType::Ignore};
        FailureType failureType{FailureType::All};

        friend void to_json(nlohmann::json &json, const NodeOutputModel &object) {
            json["jobKey"] = object.jobKey;
            json["condition"] = magic_enum::enum_name(object.condition);
            json["state"] = magic_enum::enum_name(object.state);
            json["failureType"] = magic_enum::enum_name(object.failureType);
        }

        friend void from_json(const nlohmann::json &json, NodeOutputModel &object) {
            NodeOutputModel defaultObject;
            object.jobKey = json.value("jobKey", defaultObject.jobKey);
            object.condition = magic_enum::enum_cast<ConditionType>(json.value("condition", magic_enum::enum_name(defaultObject.condition))).value_or(ConditionType::Failure);
            object.state = magic_enum::enum_cast<OutputStateType>(json.value("state", magic_enum::enum_name(defaultObject.state))).value_or(OutputStateType::Ignore);
            object.failureType = magic_enum::enum_cast<FailureType>(json.value("failureType", magic_enum::enum_name(defaultObject.failureType))).value_or(FailureType::All);
        }

    };

    class NodeModel{
    public:
        std::string uuid{};
        NodeType nodeType{NodeType::PerformanceJob};
        std::string jobKey{};
        int level{0};
        int timeout{0};
        std::string state{};
        ResultType result{ResultType::Ignore};
        std::vector<NodeInputModel> inputs{};
        std::vector<NodeOutputModel> outputs{};

        friend void to_json(nlohmann::json &nlohmann_json_j, const NodeModel &nlohmann_json_t) {
            nlohmann_json_j["uuid"] = nlohmann_json_t.uuid;
            nlohmann_json_j["nodeType"] = magic_enum::enum_name(nlohmann_json_t.nodeType);
            nlohmann_json_j["jobKey"] = nlohmann_json_t.jobKey;
            nlohmann_json_j["level"] = nlohmann_json_t.level;
            nlohmann_json_j["timeout"] = nlohmann_json_t.timeout;
            nlohmann_json_j["state"] = nlohmann_json_t.state;
            nlohmann_json_j["result"] = magic_enum::enum_name(nlohmann_json_t.result);
            nlohmann_json_j["inputs"] = nlohmann_json_t.inputs;
            nlohmann_json_j["outputs"] = nlohmann_json_t.outputs;
        }

        friend void from_json(const nlohmann::json &nlohmann_json_j, NodeModel &nlohmann_json_t) {
            NodeModel nlohmann_json_default_obj;
            nlohmann_json_t.uuid = nlohmann_json_j.value("uuid", nlohmann_json_default_obj.uuid);
            nlohmann_json_t.nodeType = magic_enum::enum_cast<NodeType>(nlohmann_json_j.value("nodeType",magic_enum::enum_name(nlohmann_json_default_obj.nodeType))).value_or(NodeType::PerformanceJob);
            nlohmann_json_t.jobKey = nlohmann_json_j.value("jobKey", nlohmann_json_default_obj.jobKey);
            nlohmann_json_t.level = nlohmann_json_j.value("level", nlohmann_json_default_obj.level);
            nlohmann_json_t.timeout = nlohmann_json_j.value("timeout", nlohmann_json_default_obj.timeout);
            nlohmann_json_t.state = nlohmann_json_j.value("state", nlohmann_json_default_obj.state);
            nlohmann_json_t.result = magic_enum::enum_cast<ResultType>(nlohmann_json_j.value("result", magic_enum::enum_name(nlohmann_json_default_obj.result))).value_or(ResultType::Ignore);
            nlohmann_json_t.inputs = nlohmann_json_j.value("inputs", nlohmann_json_default_obj.inputs);
            nlohmann_json_t.outputs = nlohmann_json_j.value("outputs", nlohmann_json_default_obj.outputs);
        };

    };

    class FlexibleModel {
    public:
        BasicInfoModel basicInfo{};
        std::vector<NodeModel> nodes{};

        friend void to_json(nlohmann::json &nlohmann_json_j, const FlexibleModel &nlohmann_json_t) {
            nlohmann_json_j["basicInfo"] = nlohmann_json_t.basicInfo;
            nlohmann_json_j["nodes"] = nlohmann_json_t.nodes;
        }

        friend void from_json(const nlohmann::json &nlohmann_json_j, FlexibleModel &nlohmann_json_t) {
            FlexibleModel nlohmann_json_default_obj;
            nlohmann_json_t.basicInfo = nlohmann_json_j.value("basicInfo", nlohmann_json_default_obj.basicInfo);
            nlohmann_json_t.nodes = nlohmann_json_j.value("nodes", nlohmann_json_default_obj.nodes);
        };

    };

    using SafeBasicInfoModel = safe::Safe<BasicInfoModel>;
    using SafeNodeInputModel = safe::Safe<NodeInputModel>;
    using SafeNodeOutputModel = safe::Safe<NodeOutputModel>;
    using SafeNodeModel = safe::Safe<NodeModel>;
    using SafeFlexibleModel = safe::Safe<FlexibleModel>;

}
