/*
   Copyright (C) 2008 - 2016 by Mark de Wever <koraq@xs4all.nl>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#define GETTEXT_DOMAIN "wesnoth-lib"

#include "gui/widgets/vertical_scrollbar.hpp"
#include "gui/core/register_widget.hpp"
#include "gui/widgets/settings.hpp"
#include "wml_exception.hpp"

#include "utils/functional.hpp"

namespace gui2
{

// ------------ WIDGET -----------{

REGISTER_WIDGET(vertical_scrollbar)

unsigned tvertical_scrollbar::minimum_positioner_length() const
{
	boost::intrusive_ptr<const tvertical_scrollbar_definition::tresolution> conf
			= boost::dynamic_pointer_cast<const tvertical_scrollbar_definition::
												  tresolution>(config());
	assert(conf);
	return conf->minimum_positioner_length;
}

unsigned tvertical_scrollbar::maximum_positioner_length() const
{
	boost::intrusive_ptr<const tvertical_scrollbar_definition::tresolution> conf
			= boost::dynamic_pointer_cast<const tvertical_scrollbar_definition::
												  tresolution>(config());
	assert(conf);
	return conf->maximum_positioner_length;
}

unsigned tvertical_scrollbar::offset_before() const
{
	boost::intrusive_ptr<const tvertical_scrollbar_definition::tresolution> conf
			= boost::dynamic_pointer_cast<const tvertical_scrollbar_definition::
												  tresolution>(config());
	assert(conf);
	return conf->top_offset;
}

unsigned tvertical_scrollbar::offset_after() const
{
	boost::intrusive_ptr<const tvertical_scrollbar_definition::tresolution> conf
			= boost::dynamic_pointer_cast<const tvertical_scrollbar_definition::
												  tresolution>(config());
	assert(conf);
	return conf->bottom_offset;
}

bool tvertical_scrollbar::on_positioner(const tpoint& coordinate) const
{
	// Note we assume the positioner is over the entire width of the widget.
	return coordinate.y >= static_cast<int>(get_positioner_offset())
		   && coordinate.y < static_cast<int>(get_positioner_offset()
											  + get_positioner_length())
		   && coordinate.x > 0 && coordinate.x < static_cast<int>(get_width());
}

int tvertical_scrollbar::on_bar(const tpoint& coordinate) const
{
	// Not on the widget, leave.
	if(static_cast<size_t>(coordinate.x) > get_width()
	   || static_cast<size_t>(coordinate.y) > get_height()) {
		return 0;
	}

	// we also assume the bar is over the entire width of the widget.
	if(static_cast<size_t>(coordinate.y) < get_positioner_offset()) {
		return -1;
	} else if(static_cast<size_t>(coordinate.y) > get_positioner_offset()
												  + get_positioner_length()) {
		return 1;
	} else {
		return 0;
	}
}

bool tvertical_scrollbar::in_orthogonal_range(const tpoint& coordinate) const
{
	return static_cast<size_t>(coordinate.y) < get_height();
}

const std::string& tvertical_scrollbar::get_control_type() const
{
	static const std::string type = "vertical_scrollbar";
	return type;
}

// }---------- DEFINITION ---------{

tvertical_scrollbar_definition::tvertical_scrollbar_definition(
		const config& cfg)
	: tcontrol_definition(cfg)
{
	DBG_GUI_P << "Parsing vertical scrollbar " << id << '\n';

	load_resolutions<tresolution>(cfg);
}

/*WIKI
 * @page = GUIWidgetDefinitionWML
 * @order = 1_vertical_scrollbar
 *
 * == Vertical scrollbar ==
 *
 * The definition of a vertical scrollbar. This class is most of the time not
 * used directly. Instead it's used to build other items with scrollbars.
 *
 * @begin{parent}{name="gui/"}
 * @begin{tag}{name="vertical_scrollbar_definition"}{min=0}{max=-1}{super="generic/widget_definition"}
 * The resolution for a vertical scrollbar also contains the following keys:
 * @begin{tag}{name="resolution"}{min=0}{max=-1}{super=generic/widget_definition/resolution}
 * @begin{table}{config}
 *     minimum_positioner_length & unsigned & &
 *                                     The minimum size the positioner is
 *                                     allowed to be. The engine needs to know
 *                                     this in order to calculate the best size
 *                                     for the positioner. $
 *     maximum_positioner_length & unsigned & 0 &
 *                                     The maximum size the positioner is
 *                                     allowed to be. If minimum and maximum are
 *                                     the same value the positioner is fixed
 *                                     size. If the maximum is 0 (and the
 *                                     minimum not) there's no maximum. $
 *     top_offset & unsigned & 0 &     The number of pixels at the top which
 *                                     can't be used by the positioner. $
 *     bottom_offset & unsigned & 0 &  The number of pixels at the bottom which
 *                                     can't be used by the positioner. $
 * @end{table}
 * The following states exist:
 * * state_enabled, the vertical scrollbar is enabled.
 * * state_disabled, the vertical scrollbar is disabled.
 * * state_pressed, the left mouse button is down on the positioner of the
 *   vertical scrollbar.
 * * state_focused, the mouse is over the positioner of the vertical scrollbar.
 * @begin{tag}{name="state_enabled"}{min=0}{max=1}{super="generic/state"}
 * @end{tag}{name="state_enabled"}
 * @begin{tag}{name="state_disabled"}{min=0}{max=1}{super="generic/state"}
 * @end{tag}{name="state_disabled"}
 * @begin{tag}{name="state_pressed"}{min=0}{max=1}{super="generic/state"}
 * @end{tag}{name="state_pressed"}
 * @begin{tag}{name="state_focused"}{min=0}{max=1}{super="generic/state"}
 * @end{tag}{name="state_focused"}
 * @end{tag}{name="resolution"}
 * @end{tag}{name="vertical_scrollbar_definition"}
 * @end{parent}{name="gui/"}
 */
tvertical_scrollbar_definition::tresolution::tresolution(const config& cfg)
	: tresolution_definition_(cfg)
	, minimum_positioner_length(cfg["minimum_positioner_length"])
	, maximum_positioner_length(cfg["maximum_positioner_length"])
	, top_offset(cfg["top_offset"])
	, bottom_offset(cfg["bottom_offset"])
{
	VALIDATE(minimum_positioner_length,
			 missing_mandatory_wml_key("resolution",
									   "minimum_positioner_length"));

	// Note the order should be the same as the enum tstate in scrollbar.hpp.
	state.push_back(tstate_definition(cfg.child("state_enabled")));
	state.push_back(tstate_definition(cfg.child("state_disabled")));
	state.push_back(tstate_definition(cfg.child("state_pressed")));
	state.push_back(tstate_definition(cfg.child("state_focused")));
}

// }---------- BUILDER -----------{

/*WIKI
 * @page = GUIWidgetInstanceWML
 * @order = 2_vertical_scrollbar
 *
 * == Vertical scrollbar ==
 *
 *
 * @begin{parent}{name="gui/window/resolution/grid/row/column/"}
 * @begin{tag}{name="vertical_scrollbar"}{min=0}{max=1}{super="generic/widget_instance"}
 * @end{tag}{name="vertical_scrollbar"}
 * @end{parent}{name="gui/window/resolution/grid/row/column/"}
 */

namespace implementation
{

tbuilder_vertical_scrollbar::tbuilder_vertical_scrollbar(const config& cfg)
	: tbuilder_control(cfg)
{
}

twidget* tbuilder_vertical_scrollbar::build() const
{
	tvertical_scrollbar* widget = new tvertical_scrollbar();

	init_control(widget);

	DBG_GUI_G << "Window builder:"
			  << " placed vertical scrollbar '" << id << "' with definition '"
			  << definition << "'.\n";

	return widget;
}

} // namespace implementation

// }------------ END --------------

} // namespace gui2
