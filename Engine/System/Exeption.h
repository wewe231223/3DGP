#pragma once 

class Exeption {
public:
	Exeption() = default;
	Exeption(HRESULT ,const std::tstring&,const std::tstring&, const int);
	Exeption(const std::tstring& );

private:
	HRESULT m_errorCode{ S_OK };
	std::tstring m_funcName{};
	std::tstring m_fileName{};
	std::tstring m_msg{};
	int m_lineNo{ -1 };
public:
	std::tstring ToString() const;
};


#define CheckFailed(hr) {														\
	HRESULT hr_ = (hr);															\
	if (FAILED(hr_)) { throw Exeption(hr_, L#hr, _T(__FILE__), __LINE__); }	\
}


