#include "Core.hpp"

namespace _ConVar_Internal {
	static ConVar* head = nullptr;
	static ConVar* tail = nullptr;
}

void ConVar::AddToList() {
	if(_ConVar_Internal::head != nullptr) {
		_ConVar_Internal::tail->m_next = this;
		_ConVar_Internal::tail = this;
	}
	else {
		_ConVar_Internal::head = this;
		_ConVar_Internal::tail = this;
	}
}

ConVar* ConVar::GetHead() {
	return _ConVar_Internal::head;
}