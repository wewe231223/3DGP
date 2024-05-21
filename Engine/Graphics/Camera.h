

class Camera {
public:
    Camera();
    void FocusUpdate(float dx, float dy);
    void PositionUpdate(class Input* input);
    void UpdateViewMatrix();

    DirectX::XMMATRIX GetViewMatrix() const;

private:
    DirectX::XMVECTOR m_position;
    DirectX::XMVECTOR m_target;
    DirectX::XMVECTOR m_up;
    DirectX::XMVECTOR m_right;
    DirectX::XMVECTOR m_forward;

    DirectX::XMFLOAT4X4 m_viewMatrix;
    float m_pitch;
    float m_yaw;

};



