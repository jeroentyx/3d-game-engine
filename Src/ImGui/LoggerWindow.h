#pragma once

#include "..\pch.h"
#include "IImGuiWindow.h"
#include "imgui-1.84.2/imterm/terminal.hpp"
#include "imgui-1.84.2/imterm/terminal_helpers.hpp"


namespace Eos
{
	struct custom_command_struct {
		bool should_close = false;
	};

	class Terminal_Commands : public ImTerm::basic_spdlog_terminal_helper<Terminal_Commands, custom_command_struct, misc::no_mutex>
	{
	public:
		Terminal_Commands();

		static std::vector<std::string> no_completion(argument_type&) { return {}; }

		static void clear(argument_type&);
		static void configure_term(argument_type&);
		static std::vector<std::string> configure_term_autocomplete(argument_type&);

		static void exit(argument_type&);
		static void quit(argument_type&);
	};


	class LoggerWindow : public IImGuiWindow
	{
		custom_command_struct cmd_struct;
		ImTerm::terminal<Terminal_Commands> terminal_log;
	public:
		LoggerWindow();
		virtual void Update() override;
	};
}
