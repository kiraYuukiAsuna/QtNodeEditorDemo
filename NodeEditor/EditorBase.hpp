#pragma once

enum class EditorType{
    NodeEditor,

};

class EditorBase{
public:
    virtual EditorType getEditorType() = 0;

    virtual void save() = 0;
    virtual void refresh() = 0;

};
