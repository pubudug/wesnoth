/*
   Copyright (C) 2014 by Ignacio R. Morelle <shadowm2006@gmail.com>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef GUI_DIALOGS_WML_ERROR_HPP_INCLUDED
#define GUI_DIALOGS_WML_ERROR_HPP_INCLUDED

#include "gui/dialogs/dialog.hpp"

namespace gui2 {

class twml_error : public tdialog
{
public:
    twml_error(const std::string& summary,
			   const std::vector<std::string>& files,
			   const std::string& details);

	/** The display function; see @ref tdialog for more information. */
	static void display(const std::string& summary,
						const std::vector<std::string>& files,
						const std::string& details,
						CVideo& video)
	{
		twml_error(summary, files, details).show(video);
	}

	/** The display function; see @ref tdialog for more information. */
	static void display(const std::string& summary,
						const std::string& details,
						CVideo& video)
	{
		display(summary, std::vector<std::string>(), details, video);
	}

private:
	bool have_files_;

	/** Inherited from tdialog, implemented by REGISTER_DIALOG. */
	virtual const std::string& window_id() const;

	/** Inherited from tdialog. */
	void pre_show(CVideo& video, twindow& window);
};

}  // end namespace gui2

#endif
