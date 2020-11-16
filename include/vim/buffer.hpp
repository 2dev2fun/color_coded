#pragma once

#include <string>

#include "highlight.hpp"

namespace color_coded {
namespace vim {

struct buffer {
	std::string name;
	highlight_group group;
	std::size_t new_begin{}, new_end{};
	std::size_t old_begin{}, old_end{};
	std::size_t height{};
};

inline void apply(buffer &buf) {
	if (buf.group.empty()) { return; }

	vim::clear_matches();

	buf.old_begin = buf.new_begin;
	buf.old_end = buf.new_end;

	for (auto const &h : buf.group) {
		if (h.line >= buf.new_begin && h.line <= buf.new_end) {
			vim::matchaddpos(h.type, h.line, h.column, h.token.size());
		}
	}
}

inline void try_apply(buffer &buf) {
	if ((buf.new_begin != buf.old_begin) || (buf.new_end != buf.old_end)) { apply(buf); }
}

} // namespace vim
} // namespace color_coded
