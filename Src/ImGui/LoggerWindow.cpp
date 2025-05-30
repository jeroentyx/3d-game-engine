#pragma once

#include "LoggerWindow.h"
#include <array>



namespace Eos
{
	constexpr std::array local_command_list{
			Terminal_Commands::command_type{"clear", "clears the terminal screen", Terminal_Commands::clear, Terminal_Commands::no_completion},
			Terminal_Commands::command_type{"exit", "closes this terminal", Terminal_Commands::exit, Terminal_Commands::no_completion},
			Terminal_Commands::command_type{"quit", "closes this application", Terminal_Commands::quit, Terminal_Commands::no_completion},
	};

	namespace cfg_term {
		namespace cmds {
			enum cmds {
				completion,
				cpl_disable,
				cpl_down,
				cpl_up,
				colors,
				col_begin,
				col_get_value = col_begin,
				col_list_themes,
				col_reset_theme,
				col_set_theme,
				col_set_value,
				col_end,
				csv_begin = col_end,
				csv_auto_complete_non_selected = csv_begin,
				csv_auto_complete_selected,
				csv_auto_complete_separator,
				csv_border,
				csv_border_shadow,
				csv_button,
				csv_button_active,
				csv_button_hovered,
				csv_cmd_backlog,
				csv_cmd_history_completed,
				csv_check_mark,
				csv_filter,
				csv_filter_match,
				csv_frame_bg,
				csv_frame_bg_active,
				csv_frame_bg_hovered,
				csv_log_level_drop_down_bg,
				csv_log_level_active,
				csv_log_level_hovered,
				csv_log_level_selected,
				csv_l_trace,
				csv_l_debug,
				csv_l_info,
				csv_l_warning,
				csv_l_error,
				csv_l_critical,
				csv_message_panel,
				csv_scrollbar_bg,
				csv_scrollbar_grab,
				csv_scrollbar_grab_active,
				csv_scrollbar_grab_hovered,
				csv_text,
				csv_text_selected_bg,
				csv_title_bg,
				csv_title_bg_active,
				csv_title_bg_collapsed,
				csv_window_bg,
				csv_end,
				set_text = csv_end,
				st_begin,
				st_autoscroll = st_begin,
				st_autowrap,
				st_clear,
				st_log_level,
				st_filter,
				st_optional_end,
				st_logs = st_optional_end,
				st_end,
				count = st_end,
			};
		}

		constexpr std::array<const char* const, cmds::count> strings{
				"completion",
				"disable",
				"to-bottom",
				"to-top",
				"colors",
				"get-value",
				"list-themes",
				"reset-theme",
				"set-theme",
				"set-value",
				"auto complete non selected",
				"auto complete selected",
				"auto complete separator",
				"border",
				"border shadow",
				"button",
				"button active",
				"button hovered",
				"cmd backlog",
				"cmd history completed",
				"check mark",
				"filter hint",
				"filter match",
				"frame bg",
				"frame bg active",
				"frame bg hovered",
				"log level drop down bg",
				"log level active",
				"log level hovered",
				"log level selected",
				"log trace",
				"log debug",
				"log info",
				"log warning",
				"log error",
				"log critical",
				"message panel",
				"scrollbar bg",
				"scrollbar grab",
				"scrollbar grab active",
				"scrollbar grab hovered",
				"text",
				"text selected bg",
				"title bg",
				"title bg active",
				"title bg collapsed",
				"window bg",
				"set-text",
				"autoscroll",
				"autowrap",
				"clear",
				"log level",
				"filter hint",
				"logs",
		};
	}


	Terminal_Commands::Terminal_Commands()
	{
		for (const command_type& cmd : local_command_list) {
			add_command_(cmd);
		}
	}

	void Terminal_Commands::clear(argument_type& arg)
	{
		arg.term.clear();
	}

	void Terminal_Commands::configure_term(argument_type& arg)
	{
		using namespace cfg_term;

		std::vector<std::string>& cl = arg.command_line;
		if (cl.size() < 3) {
			arg.term.add_text_err("Not enough arguments");
		}
		if (cl[1] == strings[cmds::completion] && cl.size() == 3) {
			if (cl[2] == strings[cmds::cpl_up]) {
				arg.term.set_autocomplete_pos(ImTerm::position::up);
			}
			else if (cl[2] == strings[cmds::cpl_down]) {
				arg.term.set_autocomplete_pos(ImTerm::position::down);
			}
			else if (cl[2] == strings[cmds::cpl_disable]) {
				arg.term.set_autocomplete_pos(ImTerm::position::nowhere);
			}
			else {
				PE_CORE_ERROR("Unknown completion parameter: {}", cl[2]);
			}
		}
		else if (cl[1] == strings[cmds::colors]) {
			if (cl.size() == 3 && cl[2] == strings[cmds::col_list_themes]) {
				unsigned long max_size = 0;
				for (const ImTerm::theme& theme : ImTerm::themes::list) {
					max_size = std::max(max_size, static_cast<unsigned long>(theme.name.size()));
				}

				arg.term.add_text("Available styles: ");
				for (const ImTerm::theme& theme : ImTerm::themes::list) {
					std::string str("      ");
					str += theme.name;
					arg.term.add_text(std::move(str));
				}


			}
			else if (cl.size() == 3 && cl[2] == strings[cmds::col_reset_theme]) {
				arg.term.reset_colors();

			}
			else if (cl.size() == 4 && cl[2] == strings[cmds::col_set_theme]) {

				for (const ImTerm::theme& theme : ImTerm::themes::list) {
					if (theme.name == cl[3]) {
						arg.term.theme() = theme;
						return;
					}
				}
				PE_CORE_ERROR("Unknown theme: {}", cl[3]);

			}
			else if (((cl.size() == 8 || cl.size() == 7 || cl.size() == 4) && cl[2] == strings[cmds::col_set_value])
				|| ((cl.size() == 4) && cl[2] == strings[cmds::col_get_value])) {
				auto it = misc::find_first_prefixed(cl[3], strings.begin() + cmds::csv_begin, strings.begin() + cmds::csv_end, [](auto&&) {return false; });

				if (it == strings.begin() + cmds::csv_end) {
					PE_CORE_ERROR("Unknown item: {}", cl[3]);
					return;
				}

				std::optional<ImTerm::theme::constexpr_color>* theme_color = nullptr;
				auto enum_v = static_cast<cmds::cmds>(it - strings.begin());
				switch (enum_v) {
				case cmds::csv_text:                       theme_color = &arg.term.theme().text;                        break;
				case cmds::csv_window_bg:                  theme_color = &arg.term.theme().window_bg;                   break;
				case cmds::csv_border:                     theme_color = &arg.term.theme().border;                      break;
				case cmds::csv_border_shadow:              theme_color = &arg.term.theme().border_shadow;               break;
				case cmds::csv_button:                     theme_color = &arg.term.theme().button;                      break;
				case cmds::csv_button_hovered:             theme_color = &arg.term.theme().button_hovered;              break;
				case cmds::csv_button_active:              theme_color = &arg.term.theme().button_active;               break;
				case cmds::csv_filter:                     theme_color = &arg.term.theme().filter_hint;                 break;
				case cmds::csv_filter_match:               theme_color = &arg.term.theme().matching_text;               break;
				case cmds::csv_frame_bg:                   theme_color = &arg.term.theme().frame_bg;                    break;
				case cmds::csv_frame_bg_hovered:           theme_color = &arg.term.theme().frame_bg_hovered;            break;
				case cmds::csv_frame_bg_active:            theme_color = &arg.term.theme().frame_bg_active;             break;
				case cmds::csv_text_selected_bg:           theme_color = &arg.term.theme().text_selected_bg;            break;
				case cmds::csv_check_mark:                 theme_color = &arg.term.theme().check_mark;                  break;
				case cmds::csv_title_bg:                   theme_color = &arg.term.theme().title_bg;                    break;
				case cmds::csv_title_bg_active:            theme_color = &arg.term.theme().title_bg_active;             break;
				case cmds::csv_title_bg_collapsed:         theme_color = &arg.term.theme().title_bg_collapsed;          break;
				case cmds::csv_message_panel:              theme_color = &arg.term.theme().message_panel;               break;
				case cmds::csv_auto_complete_selected:     theme_color = &arg.term.theme().auto_complete_selected;      break;
				case cmds::csv_auto_complete_non_selected: theme_color = &arg.term.theme().auto_complete_non_selected;  break;
				case cmds::csv_auto_complete_separator:    theme_color = &arg.term.theme().auto_complete_separator;     break;
				case cmds::csv_cmd_backlog:                theme_color = &arg.term.theme().cmd_backlog;                 break;
				case cmds::csv_cmd_history_completed:      theme_color = &arg.term.theme().cmd_history_completed;       break;
				case cmds::csv_log_level_drop_down_bg:     theme_color = &arg.term.theme().log_level_drop_down_list_bg; break;
				case cmds::csv_log_level_active:           theme_color = &arg.term.theme().log_level_active;            break;
				case cmds::csv_log_level_hovered:          theme_color = &arg.term.theme().log_level_hovered;           break;
				case cmds::csv_log_level_selected:         theme_color = &arg.term.theme().log_level_selected;          break;
				case cmds::csv_scrollbar_bg:               theme_color = &arg.term.theme().scrollbar_bg;                break;
				case cmds::csv_scrollbar_grab:             theme_color = &arg.term.theme().scrollbar_grab;              break;
				case cmds::csv_scrollbar_grab_active:      theme_color = &arg.term.theme().scrollbar_grab_active;       break;
				case cmds::csv_scrollbar_grab_hovered:     theme_color = &arg.term.theme().scrollbar_grab_hovered;      break;
				case cmds::csv_l_trace: [[fallthrough]];
				case cmds::csv_l_debug: [[fallthrough]];
				case cmds::csv_l_info: [[fallthrough]];
				case cmds::csv_l_warning: [[fallthrough]];
				case cmds::csv_l_error: [[fallthrough]];
				case cmds::csv_l_critical:                 theme_color = &arg.term.theme().log_level_colors[enum_v - cmds::csv_l_trace];    break;
				default:
					arg.term.add_text_err("Internal error.");
					return;
				}

				if (cl[2] == strings[cmds::col_set_value]) {
					if (cl.size() == 4) {
						theme_color->reset();
						return;
					}
					auto try_parse = [](std::string_view str, auto& value) {
						auto res = std::from_chars(&str[0], &str[str.size() - 1] + 1, value, 10);
						return misc::success(res.ec) && res.ptr == (&str[str.size() - 1] + 1);
					};
					std::uint8_t r{}, g{}, b{}, a{ 255 };
					if (!try_parse(cl[4], r) || !try_parse(cl[5], g) || !try_parse(cl[6], b) || (cl.size() == 8 && !try_parse(cl[7], a))) {
						arg.term.add_text_err("Bad color argument(s)");
					}

					*theme_color = { static_cast<float>(r) / 255.f,static_cast<float>(g) / 255.f,static_cast<float>(b) / 255.f,static_cast<float>(a) / 255.f };
				}
				else {
					if (*theme_color) {
						auto to_255 = [](float v) {
							return static_cast<int>(v * 255.f + 0.5f);
						};;
						PE_CORE_ERROR("Current value for {}: [R: {}] [G: {}] [B: {}] [A: {}]", cl[3], to_255((**theme_color).r)
							, to_255((**theme_color).g), to_255((**theme_color).b), to_255((**theme_color).a));
					}
					else {
						PE_CORE_ERROR("Current value for {}: unset", cl[3]);
					}
				}
			}

		}
		else if ((cl.size() == 3 || cl.size() == 4 || cl.size() == 10) && cl[1] == strings[cmds::set_text]) {
			auto it = misc::find_first_prefixed(cl[2], strings.begin() + cmds::st_begin, strings.begin() + cmds::st_optional_end, [](auto&&) {return false; });
			if (it == strings.begin() + cmds::st_optional_end) {
				it = misc::find_first_prefixed(cl[2], strings.begin() + cmds::st_optional_end, strings.begin() + cmds::st_end, [](auto&&) {return false; });
				if (it == strings.begin() + cmds::st_end) {
					PE_CORE_ERROR("Unknown text field: {}", cl[2]);
				}
				else if (cl.size() != 10) {
					arg.term.add_text_err("Not enough/Too much arguments !");
					arg.term.add_text_err("You should specify, in order: trace text, debug text, info text, warning text, error text, critical text, none text");
				}
				else {
					arg.term.set_level_list_text(cl[3], cl[4], cl[5], cl[6], cl[7], cl[8], cl[9]);
				}
			}
			else {
				std::optional<std::string>* str = nullptr;
				auto enum_v = static_cast<cmds::cmds>(it - strings.begin());
				switch (enum_v) {
				case cmds::st_autoscroll: str = &arg.term.autoscroll_text(); break;
				case cmds::st_autowrap:   str = &arg.term.autowrap_text();   break;
				case cmds::st_clear:      str = &arg.term.clear_text();      break;
				case cmds::st_log_level:  str = &arg.term.log_level_text();  break;
				case cmds::st_filter:     str = &arg.term.filter_hint();     break;
				default:
					arg.term.add_text_err("Internal error.");
					return;
				}
				if (cl.size() == 4) {
					*str = cl[3];
				}
				else {
					str->reset();
				}
			}

		}
		else {
			PE_CORE_ERROR("Unknown parameter: {}", cl[1]);
		}
	}

	std::vector<std::string> Terminal_Commands::configure_term_autocomplete(argument_type& all_args)
	{
		using namespace cfg_term;
		auto& args = all_args.command_line;

		std::vector<std::string> ans;
		unsigned int current_subpart = 0u;

		auto try_match_str = [&ans, &args, &current_subpart](std::string_view vs) {
			std::string& str = args[current_subpart];

			if (str.size() > vs.size()) {
				return;
			}

			bool is_prefix = std::equal(str.begin(), str.end(), vs.begin(), [](char c1, char c2) {
				auto to_upper = [](char c) -> char { return c < 'a' ? c + ('a' - 'A') : c; }; // don't care about the locale here
				return to_upper(c1) == to_upper(c2);
			});
			if (is_prefix) {
				ans.emplace_back(vs.data(), vs.size());
			}
		};
		auto try_match = [&try_match_str](cmds::cmds cmd) {
			try_match_str(strings[cmd]);
		};
		auto try_match_range = [&try_match](cmds::cmds begin, cmds::cmds end) {
			for (int i = begin; i < end; ++i) {
				try_match(static_cast<cmds::cmds>(i));
			}
		};

		if (args.size() == 2) {
			current_subpart = 1;
			try_match(cmds::completion);
			try_match(cmds::colors);
			try_match(cmds::set_text);

		}
		else if (args.size() == 3) {
			current_subpart = 2;
			if (args[1] == strings[cmds::completion]) {
				if (all_args.term.get_autocomplete_pos() != ImTerm::position::nowhere) {
					try_match(cmds::cpl_disable);
				}
				if (all_args.term.get_autocomplete_pos() != ImTerm::position::down) {
					try_match(cmds::cpl_down);
				}
				if (all_args.term.get_autocomplete_pos() != ImTerm::position::up) {
					try_match(cmds::cpl_up);
				}

			}
			else if (args[1] == strings[cmds::colors]) {
				try_match_range(cmds::col_begin, cmds::col_end);

			}
			else if (args[1] == strings[cmds::set_text]) {
				try_match_range(cmds::st_begin, cmds::st_end);
			}

		}
		else if (args.size() == 4) {
			if (args[1] == strings[cmds::colors]) {
				current_subpart = 3;
				if (args[2] == strings[cmds::col_set_theme]) {
					for (const ImTerm::theme& theme : ImTerm::themes::list) {
						try_match_str(theme.name);
					}
				}
				else if (args[2] == strings[cmds::col_set_value] || args[2] == strings[cmds::col_get_value]) {
					try_match_range(cmds::csv_begin, cmds::csv_end);
				}
			}
		}
		std::sort(ans.begin(), ans.end());
		return ans;
	}



	void Terminal_Commands::exit(argument_type& arg)
	{
		arg.term.set_should_close();
	}

	void Terminal_Commands::quit(argument_type& arg)
	{
		arg.val.should_close = true;
	}


	LoggerWindow::LoggerWindow(): terminal_log(cmd_struct)
	{
		terminal_log.set_min_log_level(ImTerm::message::severity::info);
		Logger::GetInstance().GetCoreLogger()->sinks().push_back(terminal_log.get_terminal_helper());
	}

	void LoggerWindow::Update()
	{
		terminal_log.show();

		if (cmd_struct.should_close) {
			active = false;
		}
	}
	
}
