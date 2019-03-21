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

#include "../model/geometry.hpp"
#include "../model/shape.hpp"
#include "../model/window.hpp"
#include "viewport.hpp"

namespace control
{

    class ModelColumns : public Gtk::TreeModel::ColumnRecord
    {
        public:

        ModelColumns()
        {
            add(_column_name);
            add(_column_type);
        }

        Gtk::TreeModelColumn<Glib::ustring> _column_name;
        Gtk::TreeModelColumn<Glib::ustring> _column_type;
    };


	class MainControl
	{
	public:

		MainControl(Glib::RefPtr<Gtk::Builder>& builder) :
			_builder(builder)
		{
            build_movements();
            build_window(builder);
            build_viewport(builder);
            build_tree_view(builder);
            build_connection(builder);
            build_new_objects(builder);
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

        void on_item_selected();
        void on_radio_clicked();
        void on_new_object_clicked();
        void on_dialog_exit_clicked();
        void on_dialog_ok_clicked();
	
	private:
		void build_window(Glib::RefPtr<Gtk::Builder>& builder);
        void build_viewport(Glib::RefPtr<Gtk::Builder>& builder);
        void build_tree_view(Glib::RefPtr<Gtk::Builder>& builder);
        void build_connection(Glib::RefPtr<Gtk::Builder>& builder);        
        void build_new_objects(Glib::RefPtr<Gtk::Builder>& builder);
        void build_movements();

        //! OUR
		control::Viewport * _viewport;
		model::Window * _window;
        
        /* Shapes */
        std::vector<std::shared_ptr<model::Shape>> _shapes;
        std::map<std::string, std::shared_ptr<model::Shape>> _shapes_map;

        ModelColumns _tree_model;

        //! GTK
        Gtk::TreeView * _tree;
        Glib::RefPtr<Gtk::Builder> _builder;
        Glib::RefPtr<Gtk::ListStore> _ref_tree_model;
        Glib::RefPtr<Gtk::TreeSelection> _tree_selection;

        Glib::RefPtr<Gtk::Builder>& _builder;
	};

    void MainControl::build_window(Glib::RefPtr<Gtk::Builder>& builder)
    {
        Gtk::DrawingArea *draw;
        Gtk::Allocation alloc;
		double width, height;

		builder->get_widget("area_draw", draw);

        draw->set_hexpand(true);
        alloc = draw->get_allocation();
		width = alloc.get_width();
		height = alloc.get_height();

        _window = new model::Window(model::Vector(0, 0), model::Vector(width, height), *draw);
    }

    void MainControl::build_viewport(Glib::RefPtr<Gtk::Builder>& builder)
    {
        Gtk::DrawingArea *draw;
        builder->get_widget("area_draw", draw);
        _viewport = new control::Viewport(*_window, _shapes, *draw);
    }

    void MainControl::build_tree_view(Glib::RefPtr<Gtk::Builder>& builder)
    {
        builder->get_widget("tree_objects", _tree);

        _ref_tree_model = Gtk::ListStore::create(_tree_model);
        _tree->set_model(_ref_tree_model);

        _tree->append_column("Name", _tree_model._column_name);
        _tree->append_column("Type", _tree_model._column_type);

        _tree_selection = _tree->get_selection();
        _tree_selection->signal_changed().connect(sigc::mem_fun(*this, &MainControl::on_item_selected));

        Gtk::TreeModel::Row row         = *(_ref_tree_model->append());
        row[_tree_model._column_name]   = "Window";
        row[_tree_model._column_type]   = "View";

/* Line TEST */
        _shapes.emplace_back(new model::Line("L1", model::Vector(0, 0), model::Vector(100, 100)));
        std::cout << "Size=" << _shapes.size() << std::endl;

        row = *(_ref_tree_model->append());
        row[_tree_model._column_name]   = "L1";
        row[_tree_model._column_type]   = "Line";

        _shapes_map["L1"] = _shapes.back();
/* End Line TEST */

        std::cout << "## " << row.get_value(_tree_model._column_name) << std::endl;
    }

    void MainControl::build_connection(Glib::RefPtr<Gtk::Builder>& builder)
    {
    }

    void MainControl::build_new_objects(Glib::RefPtr<Gtk::Builder>& builder)
    {
    	Gtk::Button* button;
    	Gtk::RadioButton* radio1;
    	Gtk::RadioButton* radio2;

    	builder->get_widget("button_new_object", button);
    	button->signal_clicked().connect(sigc::mem_fun(*this, &MainControl::on_new_object_clicked));

    	builder->get_widget("dialog_button_cancel", button);
    	button->signal_clicked().connect(sigc::mem_fun(*this, &MainControl::on_dialog_exit_clicked));

    	builder->get_widget("dialog_button_ok", button);
    	button->signal_clicked().connect(sigc::mem_fun(*this, &MainControl::on_dialog_ok_clicked));

    	builder->get_widget("radio_point", radio1);
    	radio1->signal_clicked().connect(sigc::mem_fun(*this, &MainControl::on_radio_clicked));

    	builder->get_widget("radio_line", radio2);
    	radio2->signal_clicked().connect(sigc::mem_fun(*this, &MainControl::on_radio_clicked));
    	radio2->join_group(*radio1);

    	builder->get_widget("radio_rectangle", radio2);
    	radio2->signal_clicked().connect(sigc::mem_fun(*this, &MainControl::on_radio_clicked));
    	radio2->join_group(*radio1);

    	builder->get_widget("radio_polygon", radio2);
    	radio2->signal_clicked().connect(sigc::mem_fun(*this, &MainControl::on_radio_clicked));
    	radio2->join_group(*radio1);
    }

    void MainControl::on_item_selected()
    {
        std::cout << "UI " << _tree_selection->get_selected()->get_value(_tree_model._column_name) << std::endl;
    }

<<<<<<< 42093e524922c0d8695981df1544b2d2f7d8a4e8
    void MainControl::build_movements()
    {
        Gtk::SpinButton *spin;
        
        _builder->get_widget("spin_step", spin);

        spin->set_digits(2);
        spin->set_range(0, 9999999);
        spin->set_increments(1, 100);
        spin->set_value(100);

        Gtk::Button *btn;
        _builder->get_widget("button_up", btn);
        btn->signal_clicked().connect(sigc::mem_fun(*this, &MainControl::up));

		_builder->get_widget("button_left", btn);
        btn->signal_clicked().connect(sigc::mem_fun(*this, &MainControl::left));
		
        _builder->get_widget("button_right", btn);
        btn->signal_clicked().connect(sigc::mem_fun(*this, &MainControl::right));
		
        _builder->get_widget("button_down", btn);
        btn->signal_clicked().connect(sigc::mem_fun(*this, &MainControl::down));

        _builder->get_widget("button_in", btn);
        btn->signal_clicked().connect(sigc::mem_fun(*this, &MainControl::zoom_in));

        _builder->get_widget("button_out", btn);
        btn->signal_clicked().connect(sigc::mem_fun(*this, &MainControl::zoom_out));
    }

    void MainControl::on_new_object_clicked()
    {
    	Gtk::Dialog* dialog;
    	_builder->get_widget("window_dialog", dialog);
  		dialog->run();
    }

    void MainControl::on_radio_clicked()
    {
    	// std::cout << "AI" << std::endl;
    }

    void MainControl::on_dialog_exit_clicked()
    {
    	Gtk::Dialog* dialog;
    	_builder->get_widget("window_dialog", dialog);
  		dialog->close();
    }

    void MainControl::on_dialog_ok_clicked()
    {
    	Gtk::RadioButton* radio;
    	Gtk::Entry* entry;

    	_builder->get_widget("radio_point", radio);
  		
  		if (radio->activate()) {
  			double x, y, z;

  			_builder->get_widget("entry_point_x", entry);
  			x = atof(((std::string) entry->get_text()).c_str());

  			_builder->get_widget("entry_point_y", entry);
  			y = atof(((std::string) entry->get_text()).c_str());

  			_builder->get_widget("entry_point_z", entry);
  			z = atof(((std::string) entry->get_text()).c_str());

  			/*
  				CRIAR UM MÉTODO PARA INSERIR O OBJETO NA TREEVIEW
  				CRIAR UM MÉTODO PARA CRIAR O OBJETO, INSERIR NA LISTA E DESENHÁ-LO NA WINDOW
  			*/

  		}
    }

    void MainControl::up()
    {
        Gtk::SpinButton *spin;
        _builder->get_widget("spin_step", spin);
        std::cout << "Up step = " << spin->get_value() << std::endl;

        double step = spin->get_value();
        auto T = model::transformations::translation(model::Vector(0, step, 1, 1));

		std::cout << "Transformation" << std::endl;
		for (int i = 0; i < T.dimension; i++) {
			for (int j = 0; j < T.dimension; j++)
				std::cout << T[i][j] << ", ";
            std::cout << std::endl;
        }

		std::cout << "AAAAAAA" << std::endl;
        model::Vector a(0,0,1,1);
        a = a * T;
        for (int i = 0; i < T.dimension; i++) {
            std::cout << a[i]<< ", ";
        }
            std::cout << std::endl;

        std::string selected(_tree_selection->get_selected()->get_value(_tree_model._column_name));
        
        if (selected == "Window")
            _window->transformation(T);
        else if (selected != "")
            _shapes_map[selected]->transformation(T);
        
        std::cout << "UPDATE" << std::endl;
        _viewport->update();
    }

    void MainControl::left()
    {
        Gtk::SpinButton *spin;
        _builder->get_widget("spin_step", spin);
        std::cout << "Left step = " << spin->get_value() << std::endl;

        // if (_select == _window_row)
        // {
        //     //! Build T
        //     _window_row->transformation(T);
        // }
        // else
        // {
        //     //! Build T
        //     _tree_map[_select]->transformation(T);
        // }
    }

    void MainControl::right()
    {
        Gtk::SpinButton *spin;
        _builder->get_widget("spin_step", spin);
        std::cout << "Right step = " << spin->get_value() << std::endl;

        // if (_select == _window_row)
        // {
        //     //! Build T
        //     _window_row->transformation(T);
        // }
        // else
        // {
        //     //! Build T
        //     _tree_map[_select]->transformation(T);
        // }
    }

    void MainControl::down()
    {
        Gtk::SpinButton *spin;
        _builder->get_widget("spin_step", spin);
        std::cout << "Down step = " << spin->get_value() << std::endl;
        // if (_select == _window_row)
        // {
        //     //! Build T
        //     _window_row->transformation(T);
        // }
        // else
        // {
        //     //! Build T
        //     _tree_map[_select]->transformation(T);
        // }
    }

    void MainControl::zoom_in()
    {
        Gtk::SpinButton *spin;
        _builder->get_widget("spin_step", spin);
        std::cout << "Zoom in step = " << spin->get_value() << std::endl;
        // if (_select == _window_row)
        // {
        //     //! Build T
        //     _window_row->transformation(T);
        // }
        // else
        // {
        //     //! Build T
        //     _tree_map[_select]->transformation(T);
        // }
    }

    void MainControl::zoom_out()
    {
        Gtk::SpinButton *spin;
        _builder->get_widget("spin_step", spin);
        std::cout << "Zoom out step = " << spin->get_value() << std::endl;
        // if (_select == _window_row)
        // {
        //     //! Build T
        //     _window_row->transformation(T);
        // }
        // else
        // {
        //     //! Build T
        //     _tree_map[_select]->transformation(T);
        // }
    }


} //! namespace control

#endif  // CONTROL_MAIN_CONTROL_HPP
