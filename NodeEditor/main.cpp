#include <iostream>

#include "NodeModel.h"
#include "safe/safe.h"

int main() {
    NodeModel model1;
    model1.nodeType = "Start";
    model1.inputs.emplace_back();
    model1.outputs.emplace_back();

    using SafeType = safe::Safe<NodeModel>;
    SafeType safeObject({},model1);
    {
        auto value3 = safeObject.writeAccess(); // nicer, but only with C++17 and later
        std::cout << value3->nodeType << std::endl;
    }
    auto copy = safeObject.copy();

    nlohmann::json j = copy;
    auto model2 = j.template get<NodeModel>();
    std::cout << j.dump(4) << std::endl;
}
