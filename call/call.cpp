
// Copyright (c) 2020 Paul Raffer.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)

#if __cplusplus >= 201703L

#include "call.hpp"


#include "raffer/system/system.hpp"

#include <limits>


namespace raffer
{

template <typename Char>
auto call(
	function_body<Char> const & this_,
	ordered_functions<Char> const & functions,
	basic_smatch<Char> const & arg)
	-> std::basic_string<Char>
{
	if (std::holds_alternative<function_body_ptr<Char>>(this_))
		return std::get<function_body_ptr<Char>>(this_)(arg);
	else
	{
		auto ret = std::get<std::basic_string<Char>>(this_);
	
		for (auto fg = cbegin(functions); fg != cend(functions); ++fg)
			for (auto func_found = true; is_up(::gpl::key::esc) && func_found; )
			{
				auto func = function<Char>{};
				auto next_match = basic_smatch<Char>{};
	
				func_found = false;
				{
				auto next_match_pos = std::numeric_limits<typename basic_smatch<Char>::difference_type>::max();
				for (auto const & f : *fg)
					if (regex_search(ret, next_match,
						regex_namespace::basic_regex<Char>(f.first),
						regex_namespace::regex_constants::format_first_only) &&
						next_match.position() < next_match_pos)
					{
						func = f;
						func_found = true;
						if ((next_match_pos = next_match.position()) == 0)
							break;
					}
				}
	
				if(func_found)
				{
					ret = regex_replace(ret,
						regex_namespace::basic_regex<Char>(func.first),
						call<Char>(func.second, functions, next_match),
						regex_namespace::regex_constants::format_first_only);
	
					fg = begin(functions);
				}
			}
	
		return ret;
	}
}

template auto call<char>(
	function_body<char> const & this_,
	ordered_functions<char> const & functions,
	basic_smatch<char> const & arg)
	-> std::basic_string<char>;

template auto call<wchar_t>(
	function_body<wchar_t> const & this_,
	ordered_functions<wchar_t> const & functions,
	basic_smatch<wchar_t> const & arg)
	-> std::basic_string<wchar_t>;

} // namespace raffer


#endif // __cplusplus >= 201703L
