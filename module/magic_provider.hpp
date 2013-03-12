// elliptics-fastcgi - FastCGI-module component for Elliptics file storage
// Copyright (C) 2011 Leonid A. Movsesjan <lmovsesjan@yandex-team.ru>

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

#ifndef _MAGIC_PROVIDER_HPP_INCLUDED_
#define _MAGIC_PROVIDER_HPP_INCLUDED_

#include <boost/noncopyable.hpp>

#include <magic.h>

namespace elliptics {

class MagicProvider : private boost::noncopyable {

public:
	MagicProvider() :
		inited_(false) {
		magic_ = magic_open(MAGIC_MIME_TYPE);
		if (NULL != magic_) {
			if (magic_load(magic_, 0) != -1) {
				inited_ = true;
			}
		}
	}

	~MagicProvider() {
		if (inited_) {
			magic_close(magic_);
		}
	}

public:
	std::string type(const std::string &content) {
		const char *result(magic_buffer(magic_, content.data(), content.size()));

		if (result) {
			return result;
		}

		return "";
	}

private:
	magic_t magic_;
	bool inited_;

};

} // namespace elliptics

#endif // _MAGIC_PROVIDER_HPP_INCLUDED_
