////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - manipulator.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "manipulator.h"
#include "mesh.h"
#include "shader.h"
#include "diagnostic.h"
#include "input.h"

namespace
{
    const std::string MODEL_FOLDER(".\\Resources\\Models\\");
};

Manipulator::Manipulator(LPDIRECT3DDEVICE9 d3ddev, const RenderCallbacks& callbacks) :
    m_selectedTool(NONE),
    m_selectedAxis(NO_AXIS)
{
    m_tools.resize(MAX_TOOLS);
    m_tools[MOVE] = std::shared_ptr<Tool>(new Tool(d3ddev, "move", MOVE, callbacks));
    m_tools[ROTATE] = std::shared_ptr<Tool>(new Tool(d3ddev, "move", ROTATE, callbacks));
    m_tools[SCALE] = std::shared_ptr<Tool>(new Tool(d3ddev, "move", SCALE, callbacks));
}

Manipulator::Tool::Tool(LPDIRECT3DDEVICE9 d3ddev, const std::string& name, 
    int index, const RenderCallbacks& callbacks)
{
    auto toolshader = callbacks.getShader(ShaderManager::TOOL_SHADER);

    axis.resize(MAX_AXIS);
    std::generate(axis.begin(), axis.end(), 
        [&](){ return Manipulator::MeshPtr(new Mesh(callbacks)); });

    axis[X_AXIS]->Load(d3ddev, MODEL_FOLDER + name + "X.obj", toolshader, X_AXIS);
    axis[Y_AXIS]->Load(d3ddev, MODEL_FOLDER + name + "Y.obj", toolshader, Y_AXIS);
    axis[Z_AXIS]->Load(d3ddev, MODEL_FOLDER + name + "Z.obj", toolshader, Z_AXIS);
}

void Manipulator::ChangeTool(ToolType type)
{
    //toggle off tool if selecting twice
    m_selectedTool = (m_selectedTool == type ? NONE : type);
    m_selectedAxis = NO_AXIS;
}

std::string Manipulator::GetDescription(Manipulator::ToolAxis axis) const
{
    switch(axis)
    {
    case X_AXIS:
        return "X Axis";
    case Y_AXIS:
        return "Y Axis";
    case Z_AXIS:
        return "Z Axis";
    default:
        return "None";
    }
}

std::string Manipulator::GetDescription(Manipulator::ToolType type) const
{
    switch(type)
    {
    case MOVE:
        return "Move";
    case ROTATE:
        return "Rotate";
    case SCALE:
        return "Scale";
    case ANIMATE:
        return "Animate";
    case NONE:
    default:
        return "None";
    }
}

void Manipulator::MousePickTest(Picking& input)
{
    if(m_selectedTool != NONE)
    {
        ToolAxis selectedAxis = NO_AXIS;
        for(unsigned int i = 0; i < m_tools[m_selectedTool]->axis.size(); ++i)
        {
            if(m_tools[m_selectedTool]->axis[i]->MousePickingTest(input))
            {
                selectedAxis = static_cast<ToolAxis>(i);
            }
        }
        input.LockMesh(selectedAxis != NO_AXIS);
        m_selectedAxis = selectedAxis;
    }
}

void Manipulator::UpdateState(Manipulator::MeshPtr mesh, 
    const Input& input, const Transform& view, const Transform& projection)
{
    if(m_selectedTool != NONE && m_selectedAxis != NO_AXIS && input.IsMousePressed())
    {
        const D3DXVECTOR3 center(WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f, 0.0f);
        const D3DXVECTOR3 mouse(static_cast<float>(input.GetMouseX()),
            static_cast<float>(input.GetMouseY()), 0.0f);

        D3DXVECTOR3 direction(input.GetMouseDirection().x, input.GetMouseDirection().y, 0.0f);

        D3DXVECTOR3 axis;
        if(m_selectedAxis == X_AXIS)
        {
            axis = mesh->Right();
        }
        else if(m_selectedAxis == Y_AXIS)
        {
            axis = mesh->Up();
        }
        else if(m_selectedAxis == Z_AXIS)
        {
            axis = mesh->Forward();
        }

        D3DXVec3Normalize(&axis, &axis);

        // Project axis onto screen



        D3DXVec3Normalize(&direction, &direction);
        float dot = D3DXVec3Dot(&axis, &direction);
        const float speed = 1.0f;

        if(Diagnostic::AllowText())
        {
            Diagnostic::UpdateText("DOT", Diagnostic::WHITE, StringCast(dot));
            Diagnostic::UpdateLine("AXIS", Diagnostic::RED, D3DXVECTOR3(0,0,0), axis);
            Diagnostic::UpdateLine("DIR", Diagnostic::GREEN, D3DXVECTOR3(0,0,0), direction);
        }

        if(m_selectedAxis == X_AXIS)
        {
            mesh->Translate(-dot*speed, 0.0f, 0.0f);
        }
        else if(m_selectedAxis == Y_AXIS)
        {
            mesh->Translate(0.0f, dot*speed, 0.0f);
        }
        else if(m_selectedAxis == Z_AXIS)
        {
            mesh->Translate(0.0f, 0.0f, dot*speed);
        }
    }
}

void Manipulator::Render(const Transform& projection, const Transform& view,
        const D3DXVECTOR3& position, const Manipulator::MeshPtr& selectedMesh)
{
    if(Diagnostic::AllowText())
    {
        Diagnostic::UpdateText("SelectedTool", Diagnostic::WHITE,
            GetDescription(m_selectedTool));

        Diagnostic::UpdateText("SelectedAxis", Diagnostic::WHITE,
            GetDescription(m_selectedAxis));
    }

    if(m_selectedTool != NONE)
    {
        std::for_each(m_tools[m_selectedTool]->axis.begin(), 
            m_tools[m_selectedTool]->axis.end(), [&](const MeshPtr& mesh)
        {
            mesh->SetPosition(selectedMesh->Position());
            mesh->DrawMesh(position, projection, view);
        });
    }
}