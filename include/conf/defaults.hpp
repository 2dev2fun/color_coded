#pragma once

#include <vector>
#include <string>

#include "env/environment.hpp"

namespace color_coded {
namespace conf {

using args_t = std::vector<std::string>;

inline auto constexpr clang_include      = "-isystem/opt/llvm/include";
inline auto constexpr clang_include_cpp  = "-isystem/opt/llvm/include/c++/v1";
inline auto constexpr clang_include_lib  = "-isystem/opt/llvm/lib/clang/10.0.0/include";
inline auto constexpr clang_resource_dir = "-resource-dir=/opt/llvm/lib/clang/10.0.0";

/* Prefixed onto every set of args to make life easier. */
inline args_t pre_constants(std::string const &filetype) {
	/* These C++ include paths must always precede /usr/include and alike. */
	args_t cpp_includes = {
		/* Local clang+llvm */
		clang_include_cpp,
		clang_include,
		//environment<env::tag>::clang_include_cpp,
		//environment<env::tag>::clang_include,
	};

	if (filetype == "c") {
		return { "-x", "c", };
	} else if (filetype == "objective-c") {
		return { "-x", "objective-c", };
	} else if(filetype == "objective-c++") {
		args_t args = { "-x", "objective-c++", };
		std::move(cpp_includes.begin(), cpp_includes.end(), std::back_inserter(args));
		return args;
	} else {
		args_t args = { "-x", "c++", "-std=c++17", };
		std::move(cpp_includes.begin(), cpp_includes.end(), std::back_inserter(args));
		return args;
	}
}

inline args_t post_constants() {
	return {
		"-isystem/usr/local/include",
		"-isystem/opt/local/include",
		clang_include_lib,
		clang_resource_dir,
		//environment<env::tag>::clang_include_lib,
		//environment<env::tag>::clang_resource_dir, // internal libraries and intrinsics
		"-isystem/usr/include",
		"-w",
		"-fcolor-diagnostics" // See https://github.com/jeaye/color_coded/issues/104
	};

}

/*
 * Add system defaults to user specified arguments. Needed because libclang
 * often fails to find system search paths.
 */
inline args_t add_defaults_to_args(std::string const& filetype, args_t&& args) {
	auto const pre_additions(pre_constants(filetype));
	static auto const post_additions(post_constants());

	args.insert(args.begin(), pre_additions.begin(), pre_additions.end());
	std::copy(std::begin(post_additions), std::end(post_additions), std::back_inserter(args));

	return std::move(args);
}

inline args_t defaults(std::string const &filetype) {
	args_t args{ "-I.", "-Iinclude" };
	return add_defaults_to_args(filetype, std::move(args));
}

} // namespace conf
} // namespace color_coded
