#pragma once

#include <memory>

namespace Eos
{
	template<typename TypeOfWindow>
	inline void GfxImGui::RegisterWindow()
	{
		m_layers.insert({ typeid(TypeOfWindow).name(),
			std::make_shared<TypeOfWindow>() });
	}
}