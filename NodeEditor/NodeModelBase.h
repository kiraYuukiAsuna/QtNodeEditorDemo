#pragma once

#include <nlohmann/json.hpp>
#include <vector>
#include <mutex>

//struct BasicInfo{
//    std::string jobKey;
//    std::string jobId;
//    std::string uuid;
//    std::string jobName;
//    std::string status;
//    std::string repeatType;
//    std::string startTime;
//    std::string timeZone;
//    std::string description;
//};

class NodeInput{
public:
    friend void to_json(nlohmann::json &json, const NodeInput &object) {
        json["jobKey"] = object.jobKey;
        json["condition"] = object.condition;
        json["state"] = object.state;
        json["failureType"] = object.failureType;
    }

    friend void from_json(const nlohmann::json &json, NodeInput &object) {
        NodeInput defaultObject;
        object.jobKey = json.value("jobKey", defaultObject.jobKey);
        object.condition = json.value("condition", defaultObject.condition);
        object.state = json.value("state", defaultObject.state);
        object.failureType = json.value("failureType", defaultObject.failureType);
    };

    void setJobKey() {

    }

private:
    std::string jobKey;
    std::string condition;
    std::string state;
    std::string failureType;
};

struct NodeOutput{
    std::string jobKey;
    std::string condition;
    std::string state;
    std::string failureType;

    friend void to_json(nlohmann::json &json, const NodeOutput &object) {
        json["jobKey"] = object.jobKey;
        json["condition"] = object.condition;
        json["state"] = object.state;
        json["failureType"] = object.failureType;
    }

    friend void from_json(const nlohmann::json &json, NodeOutput &object) {
        NodeOutput defaultObject;
        object.jobKey = json.value("jobKey", defaultObject.jobKey);
        object.condition = json.value("condition", defaultObject.condition);
        object.state = json.value("state", defaultObject.state);
        object.failureType = json.value("failureType", defaultObject.failureType);
    };
};

struct NodeModelBase{
    std::string nodeType;
    std::string jobKey;
    std::string level;
    std::string timeout;
    std::string state;
    std::string result;
    std::vector<NodeInput> inputs;
    std::vector<NodeOutput> outputs;

    friend void to_json(nlohmann::json &nlohmann_json_j, const NodeModelBase &nlohmann_json_t) {
        nlohmann_json_j["nodeType"] = nlohmann_json_t.nodeType;
        nlohmann_json_j["jobKey"] = nlohmann_json_t.jobKey;
        nlohmann_json_j["level"] = nlohmann_json_t.level;
        nlohmann_json_j["timeout"] = nlohmann_json_t.timeout;
        nlohmann_json_j["state"] = nlohmann_json_t.state;
        nlohmann_json_j["result"] = nlohmann_json_t.result;
        nlohmann_json_j["inputs"] = nlohmann_json_t.inputs;
        nlohmann_json_j["outputs"] = nlohmann_json_t.outputs;
    }

    friend void from_json(const nlohmann::json &nlohmann_json_j, NodeModelBase &nlohmann_json_t) {
        NodeModelBase nlohmann_json_default_obj;
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
