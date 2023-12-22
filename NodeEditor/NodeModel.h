#pragma once

#include "nlohmann/json.hpp"
#include <vector>
#include "safe/safe.h"

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

class FlexibleBasicInfoModel{
public:
    std::string uuid{};
    std::string jobKey{};
    std::string jobId{};
    std::string jobName{};
    std::string status{};
    std::string repeatType{};
    std::string startTime{};
    std::string timeZone{};
    std::string description{};

    friend void to_json(nlohmann::json &json, const FlexibleBasicInfoModel &object) {
        json["uuid"] = object.uuid;
        json["jobKey"] = object.jobKey;
        json["jobId"] = object.jobId;
        json["jobName"] = object.jobName;
        json["status"] = object.status;
        json["repeatType"] = object.repeatType;
        json["startTime"] = object.startTime;
        json["timeZone"] = object.timeZone;
        json["description"] = object.description;
    }

    friend void from_json(const nlohmann::json &json, FlexibleBasicInfoModel &object) {
        FlexibleBasicInfoModel defaultObject;
        object.uuid = json.value("uuid", defaultObject.uuid);
        object.jobKey = json.value("jobKey", defaultObject.jobKey);
        object.jobId = json.value("jobId", defaultObject.jobId);
        object.jobName = json.value("jobName", defaultObject.jobName);
        object.status = json.value("status", defaultObject.status);
        object.repeatType = json.value("repeatType", defaultObject.repeatType);
        object.startTime = json.value("startTime", defaultObject.startTime);
        object.timeZone = json.value("timeZone", defaultObject.timeZone);
        object.description = json.value("description", defaultObject.description);
    };

};

class NodeInputModel{
public:
    std::string jobKey{};
    std::string condition{};
    std::string state{};
    std::string failureType{};

    friend void to_json(nlohmann::json &json, const NodeInputModel &object) {
        json["jobKey"] = object.jobKey;
        json["condition"] = object.condition;
        json["state"] = object.state;
        json["failureType"] = object.failureType;
    }

    friend void from_json(const nlohmann::json &json, NodeInputModel &object) {
        NodeInputModel defaultObject;
        object.jobKey = json.value("jobKey", defaultObject.jobKey);
        object.condition = json.value("condition", defaultObject.condition);
        object.state = json.value("state", defaultObject.state);
        object.failureType = json.value("failureType", defaultObject.failureType);
    };

};

class NodeOutputModel{
public:
    std::string jobKey{};
    std::string condition{};
    std::string state{};
    std::string failureType{};

    friend void to_json(nlohmann::json &json, const NodeOutputModel &object) {
        json["jobKey"] = object.jobKey;
        json["condition"] = object.condition;
        json["state"] = object.state;
        json["failureType"] = object.failureType;
    }

    friend void from_json(const nlohmann::json &json, NodeOutputModel &object) {
        NodeOutputModel defaultObject;
        object.jobKey = json.value("jobKey", defaultObject.jobKey);
        object.condition = json.value("condition", defaultObject.condition);
        object.state = json.value("state", defaultObject.state);
        object.failureType = json.value("failureType", defaultObject.failureType);
    };

};

class NodeModel{
public:
    std::string uuid{};
    std::string nodeType{};
    std::string jobKey{};
    std::string level{};
    std::string timeout{};
    std::string state{};
    std::string result{};
    std::vector<NodeInputModel> inputs{};
    std::vector<NodeOutputModel> outputs{};

    friend void to_json(nlohmann::json &nlohmann_json_j, const NodeModel &nlohmann_json_t) {
        nlohmann_json_j["uuid"] = nlohmann_json_t.uuid;
        nlohmann_json_j["nodeType"] = nlohmann_json_t.nodeType;
        nlohmann_json_j["jobKey"] = nlohmann_json_t.jobKey;
        nlohmann_json_j["level"] = nlohmann_json_t.level;
        nlohmann_json_j["timeout"] = nlohmann_json_t.timeout;
        nlohmann_json_j["state"] = nlohmann_json_t.state;
        nlohmann_json_j["result"] = nlohmann_json_t.result;
        nlohmann_json_j["inputs"] = nlohmann_json_t.inputs;
        nlohmann_json_j["outputs"] = nlohmann_json_t.outputs;
    }

    friend void from_json(const nlohmann::json &nlohmann_json_j, NodeModel &nlohmann_json_t) {
        NodeModel nlohmann_json_default_obj;
        nlohmann_json_t.uuid = nlohmann_json_j.value("uuid", nlohmann_json_default_obj.uuid);
        nlohmann_json_t.nodeType = nlohmann_json_j.value("nodeType", nlohmann_json_default_obj.nodeType);
        nlohmann_json_t.jobKey = nlohmann_json_j.value("jobKey", nlohmann_json_default_obj.jobKey);
        nlohmann_json_t.level = nlohmann_json_j.value("level", nlohmann_json_default_obj.level);
        nlohmann_json_t.timeout = nlohmann_json_j.value("timeout", nlohmann_json_default_obj.timeout);
        nlohmann_json_t.state = nlohmann_json_j.value("state", nlohmann_json_default_obj.state);
        nlohmann_json_t.result = nlohmann_json_j.value("result", nlohmann_json_default_obj.result);
        nlohmann_json_t.inputs = nlohmann_json_j.value("inputs", nlohmann_json_default_obj.inputs);
        nlohmann_json_t.outputs = nlohmann_json_j.value("outputs", nlohmann_json_default_obj.outputs);
    };

};

class FlexibleModel {
public:
    FlexibleBasicInfoModel basicInfo{};
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
