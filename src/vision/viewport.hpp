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

#ifndef VISION_VIEWPORT_HPP
#define VISION_VIEWPORT_HPP

#include <gtkmm/drawingarea.h>

namespace Vision
{

	class Viewport
	{
	public:
		Viewport(Gtk::DrawingArea& draw_area) :
			_draw_area(draw_area)
		{
			_draw_area.signal_draw().connect(sigc::mem_fun(*this, &Viewport::on_draw));
		}

		~Viewport() = default;

		const bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	
	private:
		Gtk::DrawingArea &_draw_area;
	};

	const bool Viewport::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
	{
		cr->set_source_rgb(255, 1, 1); //! Test Paints background
		cr->paint();

		return true;
	}

} //! namespace Vision

#endif  // VISION_VIEWPORT_HPP
