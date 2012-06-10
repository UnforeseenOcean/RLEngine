#pragma once

class ConVar {
public:
	inline ConVar(const char* name, const char* description, int value) : m_name(name), m_description(description), m_value(value), m_min(INT32_MIN), m_max(INT32_MAX) {
		AddToList();
	}

	inline ConVar(const char* name, const char* description, const int value, const int min, const int max) :
	m_name(name), m_description(description), m_value(value), m_min(min), m_max(max) {
		RL_ASSERT(min <= max, "Minimum value must be greater than the maximum.");
		RL_ASSERT(value >= min && value <= max, "The value must be between the minimum and maximum values!");
		AddToList();
	}
	
	inline ConVar& operator=(int value) {
		if(value < m_min) {
			//TODO: Warn to console/file
			m_value = m_min;
		}
		if(value > m_max) {
			m_value = m_max;
		}
		m_value = value;
		return *this;
	}

	inline operator int(void) const {
		return m_value;
	}

private:
	inline void AddToList();

	ConVar* m_next;
	const char* m_name;
	const char* m_description;
	const int m_min;
	const int m_max;
	int m_value;
};

namespace ConVarns {
	static ConVar* head = nullptr;
	static ConVar* tail = nullptr;
}

void ConVar::AddToList() {
	if(ConVarns::head != nullptr) {
		ConVarns::tail->m_next = this;
		ConVarns::tail = this;
	}
	else {
		ConVarns::head = this;
		ConVarns::tail = this;
	}
}