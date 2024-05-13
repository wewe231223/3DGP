#pragma once 

class GameObject {
public:
	GameObject();
	GameObject(const std::tstring& name);
	virtual ~GameObject();
private:
	std::tstring m_name{};
	
};