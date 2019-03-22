/* The MIT License
 *
 * Copyright (c) 2019 João Vicente Souto and Bruno Izaias Bonotto
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef CONTROL_VIEWPORT_HPP
#define CONTROL_VIEWPORT_HPP

/* External includes */
#include <iostream>
#include <gtkmm/drawingarea.h>

/* Local includes */
#include "../model/geometry.hpp"
#include "../model/shape.hpp"
#include "../model/point.hpp"
#include "../model/line.hpp"
#include "../model/rectangle.hpp"
#include "../model/window.hpp"

namespace control
{

/*================================================================================*/
/*                                   Definitions                                  */
/*================================================================================*/

	class Viewport
	{
	public:
		Viewport(model::Window & window, std::vector<std::shared_ptr<model::Shape>> & shapes, Gtk::DrawingArea& draw_area) :
			_window(window),
			_shapes(shapes),
			_draw_area(draw_area)
		{
			_draw_area.signal_draw().connect(sigc::mem_fun(*this, &Viewport::on_draw));
		}

		~Viewport() = default;

		void update();
		const bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

	private:
		model::Window & _window;
		std::vector<std::shared_ptr<model::Shape>>& _shapes;
		Gtk::DrawingArea &_draw_area;
	};

/*================================================================================*/
/*                                  Implementaion                                 */
/*================================================================================*/

	void Viewport::update()
	{
		_draw_area.queue_draw();
	}

	const bool Viewport::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
	{
		std::cout << "control::Viewport::on_draw()" << std::endl;

		/* Test Paints background (Values range [0.0-1.0]). */
		cr->set_source_rgb(1, 1, 1);
		cr->paint();

		/* Line configuration */
		cr->set_line_cap(Cairo::LINE_CAP_ROUND);
		cr->set_source_rgb(0, 0, 0);

		/* Build transformation matrix. */
		auto alloc = _draw_area.get_allocation();
		model::Vector vp_min(0, 0);
		model::Vector vp_max(alloc.get_width(), alloc.get_height());
		model::Vector win_min = _window.min();
		model::Vector win_max = _window.max();

		auto T = _window.transformation();
		auto P = model::transformations::viewport_transformation(vp_min, vp_max, win_min, win_max);

		/* Draw all shapes. */
		for (auto & shape : _shapes)
			shape->draw(cr, T * P);

		/* Commit a drawing. */
		cr->stroke();

		return true;
	}

} //! namespace control

#endif  // CONTROL_VIEWPORT_HPP
