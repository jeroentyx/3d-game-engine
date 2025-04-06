/**********************************************************************************
* \file			Logger.h

* \brief		A wrapper to handle spdlog
*

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Logger.h"

namespace Eos
{
	std::shared_ptr<spdlog::logger> Logger::m_CoreLogger;

	Logger::Logger()
	{
		spdlog::set_pattern("%^[%Y-%m-%d %H:%M:%S:%e] %v%$");
		m_CoreLogger = spdlog::stdout_color_mt("CONSOLE");
		m_CoreLogger->set_level(spdlog::level::trace);
	}

	
}