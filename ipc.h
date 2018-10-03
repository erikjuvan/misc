#pragma once

#include <Windows.h>
#include <iostream>

template <typename T>
class IPC {
private:
	int const m_mem_size;
	char const *m_mem_name;
	HANDLE m_hMapFile;
	T* m_data;

public:
	IPC(bool create, int mem_size = sizeof(T), char const* mem_name = "FileMappingObject") :
		m_mem_size(mem_size), m_mem_name(mem_name), m_data(nullptr), m_hMapFile(nullptr) {

		if (create) {
			m_hMapFile = CreateFileMapping(
				INVALID_HANDLE_VALUE,    // use paging file
				NULL,                    // default security
				PAGE_READWRITE,          // read/write access
				0,                       // maximum object size (high-order DWORD)
				m_mem_size,              // maximum object size (low-order DWORD)
				m_mem_name);             // name of mapping object			
		}
		else { // open
			m_hMapFile = OpenFileMapping(
				FILE_MAP_ALL_ACCESS,   // read/write access
				FALSE,                 // do not inherit the name
				m_mem_name);               // name of mapping object
		}

		if (m_hMapFile == nullptr) {
			std::cout << "Could not create file mapping object " << GetLastError() << std::endl;
		}
		else {
			m_data = static_cast<T*>(MapViewOfFile(m_hMapFile,   // handle to map object
				FILE_MAP_ALL_ACCESS, // read/write permission
				0,
				0,
				m_mem_size));

			if (m_data == nullptr) {
				std::cout << "Could not map view of file " << GetLastError() << std::endl;
				CloseHandle(m_hMapFile);
			}
		}
	}

	~IPC() {
		if (m_data != nullptr)
			UnmapViewOfFile(m_data);
		if (m_hMapFile != nullptr)
			CloseHandle(m_hMapFile);
	}

	void Write(T const& data) {
		*m_data = data;
	}

	void Read(T& data) const {
		data = *m_data;
	}
};
