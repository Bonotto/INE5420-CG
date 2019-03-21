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

#include <iostream>
#include <gtkmm/drawingarea.h>

#include "../model/geometry.hpp"
#include "../model/shape.hpp"
#include "../model/point.hpp"
#include "../model/line.hpp"
#include "../model/rectangle.hpp"
#include "../model/window.hpp"

namespace control
{

	class Viewport
	{
	public:
		Viewport(Window & window, std::vector<Shape> & shapes, Gtk::DrawingArea& draw_area) :
			_window(window),
			_shapes(shapes),
			_draw_area(draw_area)
		{
			_draw_area.signal_draw().connect(sigc::mem_fun(*this, &Viewport::on_draw));
		}

		~Viewport() = default;

		const bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
		void update();
	
	private:
		Window & _window;
		std::vector<Shape> _shapes;
		Gtk::DrawingArea &_draw_area;
	};

	const bool Viewport::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
	{
		std::cout << "control::Viewport::on_draw()" << std::endl;

		cr->set_source_rgb(255, 1, 1); //! Test Paints background
		cr->paint();

		cr->set_line_cap(Cairo::LINE_CAP_ROUND); //! Line config
		cr->set_source_rgb(0, 0, 0);             //! Line color

		auto T = _window.transformation();

		for (auto & shape : _shapes)
			shape.draw(cr, T);

		cr->stroke();

		return true;
	}

	void Viewport::update()
	{
		_draw_area.queue_draw();
	}

} //! namespace control

#endif  // CONTROL_VIEWPORT_HPP
