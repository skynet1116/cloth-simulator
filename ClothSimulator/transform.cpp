////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - transform.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "transform.h"
#include "common.h"

Transform::Transform():
    m_yaw(0.0f),
    m_pitch(0.0f),
    m_roll(0.0f),
    m_position(0.0f, 0.0f, 0.0f),
    m_fullUpdateFn(nullptr),
    m_positionalUpdateFn(nullptr)
{ 
    D3DXMatrixIdentity(&m_scale);
    D3DXMatrixIdentity(&m_rotation);
}

void Transform::SetObserver(UpdateFn fullUpdate, UpdateFn positionalUpdate)
{
    m_fullUpdateFn = fullUpdate;
    m_positionalUpdateFn = positionalUpdate;
}

void Transform::Update(bool fullupdate)
{
    m_matrix = m_scale * m_rotation;
    m_matrix._41 = m_position.x;
    m_matrix._42 = m_position.y;
    m_matrix._43 = m_position.z;

    if(fullupdate)
    {
        if(m_fullUpdateFn != nullptr)
        {
            m_fullUpdateFn();
        }
    }
    else
    {
        if(m_positionalUpdateFn != nullptr)
        {
            m_positionalUpdateFn();
        }
    }
}

void Transform::SetScale(float scale)
{
    SetScale(scale, scale, scale);
}

void Transform::SetScale(float x, float y, float z)
{
    m_scale._11 = x;
    m_scale._22 = y;
    m_scale._33 = z;
    Update(true);
}

void Transform::Scale(float x, float y, float z)
{
    const float minimum = 0.1f;
    m_scale._11 = max(minimum, m_scale._11 + x);
    m_scale._22 = max(minimum, m_scale._22 + y);
    m_scale._33 = max(minimum, m_scale._33 + z);
    Update(true);
}

void Transform::RotateAroundAxis(float radians, D3DXVECTOR3 axis)
{
    D3DXMATRIX rotation;
    D3DXVec3Normalize(&axis, &axis);
    D3DXMatrixRotationAxis(&rotation, &axis, radians);
    m_rotation *= rotation;
    Update(true);
}

void Transform::SetPosition(const D3DXVECTOR3& position)
{
    m_position.x = position.x;
    m_position.y = position.y;
    m_position.z = position.z;
    Update(false);
}

void Transform::SetPosition(float x, float y, float z)
{
    m_position.x = x;
    m_position.y = y;
    m_position.z = z;
    Update(false);
}

void Transform::Translate(D3DXVECTOR3 position)
{
    D3DXVec3TransformCoord(&position, &position, &m_rotation);
    m_position += position;
    Update(false);
}

void Transform::Translate(float x, float y, float z)
{ 
    Translate(D3DXVECTOR3(x, y, z));
}

void Transform::MakeIdentity()
{
    m_yaw = 0.0f;
    m_roll = 0.0f;
    m_pitch = 0.0f;
    m_position.x = 0.0f;
    m_position.y = 0.0f;
    m_position.z = 0.0f;
    D3DXMatrixIdentity(&m_scale);
    D3DXMatrixIdentity(&m_rotation);
    Update(true);
}

D3DXVECTOR3 Transform::GetScale() const
{
    return D3DXVECTOR3(m_scale._11, m_scale._22, m_scale._33); 
}
