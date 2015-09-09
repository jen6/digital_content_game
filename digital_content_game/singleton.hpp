#pragma once
#include <mutex>
#include <memory>
#include <assert.h>

template <class T>
class CppSingleton
{
public:
	static T* Instance() {
		if (!m_pInstance) {
			m_bInitialized = true;
			m_pInstance = new T;
		}
		assert(m_pInstance != NULL);
		return m_pInstance;
	}

	static void destroyInstance() {
		if (m_pInstance) {
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	/**
	* @note
	* 유일한 인스턴스를 보장하기 위한 값
	* new <class T> 를 막기 위한 flag
	*/
	static bool m_bInitialized;

protected:
	CppSingleton() {
		assert(!m_pInstance);
		assert(m_bInitialized);
	};
private:
	static T* m_pInstance;
	CppSingleton(CppSingleton const&);            // 복사 숨김
	CppSingleton& operator=(CppSingleton const&); // 할당 숨김
};

template <class T> T* CppSingleton<T>::m_pInstance = NULL;
template <class T> bool CppSingleton<T>::m_bInitialized = false;
