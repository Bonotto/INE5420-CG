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

#ifndef CONTROL_MAIN_CONTROL_HPP
#define CONTROL_MAIN_CONTROL_HPP

#include <vector>
#include <iostream>
#include <gtkmm.h>

#include "../model/shape.hpp"
#include "../model/window.hpp"
#include "viewport.hpp"

namespace control
{

	class MainControl
	{
	public:
		MainControl(Glib::RefPtr<Gtk::Builder>& builder)
		{
            build_window(builder);
            build_viewport(builder);
            build_connection(builder);
		}

		~MainControl()
        {
            delete _window;
            delete _viewport;
        }

        void up();
        void left();
        void right();
        void down();
        void zoom_in();
        void zoom_out();
	
	private:
		void build_window(Glib::RefPtr<Gtk::Builder>& builder);
        void build_viewport(Glib::RefPtr<Gtk::Builder>& builder);
        void build_connection(Glib::RefPtr<Gtk::Builder>& builder);

        //! OUR
		control::Viewport * _viewport;
		model::Window * _window;
        std::vector<model::Shape> _shapes;

        //! GTK
        Gtk::Entry * _step;
        Gtk::Row * _window_row;
        std::map<Gtk::Row *, model::Shape*> _tree_map;
	};

    void MainControl::build_window(Glib::RefPtr<Gtk::Builder>& builder)
    {
        _window = new model::Window(/* draw area points */);
    }

    void MainControl::build_viewport(Glib::RefPtr<Gtk::Builder>& builder)
    {
        Gtk::DrawingArea *draw;
        refBuilder->get_widget("area_draw", draw);
        _viewport = new control::Viewport(*_window, _shapes, *draw);
    }

    void MainControl::build_connection(Glib::RefPtr<Gtk::Builder>& builder)
    {

    }

    void MainControl::up()
    {
        if (_select == _window_row)
        {
            //! Build T
            _window_row->transformation(T);
        }
        else
        {
            //! Build T
            _tree_map[_select]->transformation(T);
        }
    }

    void MainControl::left()
    {
        if (_select == _window_row)
        {
            //! Build T
            _window_row->transformation(T);
        }
        else
        {
            //! Build T
            _tree_map[_select]->transformation(T);
        }
    }

    void MainControl::right()
    {
        if (_select == _window_row)
        {
            //! Build T
            _window_row->transformation(T);
        }
        else
        {
            //! Build T
            _tree_map[_select]->transformation(T);
        }
    }

    void MainControl::down()
    {
        if (_select == _window_row)
        {
            //! Build T
            _window_row->transformation(T);
        }
        else
        {
            //! Build T
            _tree_map[_select]->transformation(T);
        }
    }

    void MainControl::zoom_in()
    {
        if (_select == _window_row)
        {
            //! Build T
            _window_row->transformation(T);
        }
        else
        {
            //! Build T
            _tree_map[_select]->transformation(T);
        }
    }

    void MainControl::zoom_out()
    {
        if (_select == _window_row)
        {
            //! Build T
            _window_row->transformation(T);
        }
        else
        {
            //! Build T
            _tree_map[_select]->transformation(T);
        }
    }


} //! namespace control

#endif  // CONTROL_MAIN_CONTROL_HPP
