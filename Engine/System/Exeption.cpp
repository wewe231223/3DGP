#include "pch.h"
#include "Exeption.h"



Exeption::Exeption(HRESULT hErrorCode, const std::tstring& ctsFuctionName, const std::tstring& ctsFileName, const int nLineNumber)
	:m_errorCode(hErrorCode), m_funcName(ctsFuctionName), m_fileName(ctsFileName), m_lineNo(nLineNumber) {
}

Exeption::Exeption(const std::tstring& ctsMessage) : m_msg(ctsMessage) {

}

std::tstring Exeption::ToString() const {
	if (m_lineNo == -1) return m_msg;
	_com_error err(m_errorCode);
	std::tstring errmsg = err.ErrorMessage();
	return m_funcName + _T("\nFailed in : ") + m_fileName + _T("\nLine : ") + std::to_wstring(m_lineNo) + _T("\nError : ") + errmsg;
}