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

#include "../model/point.hpp"
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
            add(_column_id);
            add(_column_name);
            add(_column_type);
        }

        Gtk::TreeModelColumn<int> _column_id;
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

        void insert_point(std::string name);
        void insert_polygon(std::string name);
        void insert_object(std::string name, std::string type);

        void add_entry(int id, std::string name, std::string type);

        //! OUR
        int _objects_control{0};
		control::Viewport * _viewport;
		model::Window * _window;
        
        /* Shapes */
        std::vector<std::shared_ptr<model::Shape>> _shapes;
        std::map<int, std::shared_ptr<model::Shape>> _shapes_map;


        //! GTK
        ModelColumns _tree_model;
        Gtk::TreeView * _tree;
        Glib::RefPtr<Gtk::Builder> _builder;
        Glib::RefPtr<Gtk::ListStore> _ref_tree_model;
        Glib::RefPtr<Gtk::TreeSelection> _tree_selection;
        // Gtk::Entry * _step;
        // Gtk::Row * _window_row;
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

        _tree->append_column("ID", _tree_model._column_id);
        _tree->append_column("Name", _tree_model._column_name);
        _tree->append_column("Type", _tree_model._column_type);

        _tree_selection = _tree->get_selection();
        _tree_selection->signal_changed().connect(sigc::mem_fun(*this, &MainControl::on_item_selected));

        Gtk::TreeModel::Row row         = *(_ref_tree_model->append());
        row[_tree_model._column_id]     = 0;
        row[_tree_model._column_name]   = "Window";
        row[_tree_model._column_type]   = "View";

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
		_builder->get_widget("entry_name", entry);

  		std::string name = std::string(entry->get_text());
    	
  		if (radio->get_active()) {
  			insert_point(name);
  		}
  		else
  		{
    		_builder->get_widget("radio_line", radio);

	    	if (radio->get_active()) {
	  			insert_object(name, "Line");
	    	}
	  		else
	  		{
	    		_builder->get_widget("radio_rectangle", radio);

		    	if (radio->get_active())
		  			insert_object(name, "Rectangle");
		  		else
		  			insert_polygon(name);
	  		}
  		}

		_viewport->update();
	}

	void MainControl::insert_point(std::string name)
	{
		Gtk::Entry* entry;
		double x, y, z;

		_builder->get_widget("entry_point_x", entry);
		x = atof(std::string(entry->get_text()).c_str());

		_builder->get_widget("entry_point_y", entry);
		y = atof(std::string(entry->get_text()).c_str());

		if (model::Vector::dimension == 4)
		{
			_builder->get_widget("entry_point_z", entry);
			z = atof(std::string(entry->get_text()).c_str());
		}
		else
			z = model::Vector::z;

		add_entry(++_objects_control, name, "Point");

		_shapes.emplace_back(new model::Point(name, x, y, z));

		_shapes_map[_objects_control] = _shapes.back();
	}

	void MainControl::insert_object(std::string name, std::string type)
	{
		Gtk::Entry* entry;
		double x1, y1, z1, x2, y2, z2;

		_builder->get_widget("entry_line_x1", entry);
		x1 = atof(std::string(entry->get_text()).c_str());

		_builder->get_widget("entry_line_y1", entry);
		y1 = atof(std::string(entry->get_text()).c_str());

		_builder->get_widget("entry_line_z1", entry);
		z1 = atof(std::string(entry->get_text()).c_str());

		_builder->get_widget("entry_line_x2", entry);
		x2 = atof(std::string(entry->get_text()).c_str());

		_builder->get_widget("entry_line_y2", entry);
		y2 = atof(std::string(entry->get_text()).c_str());

		if (model::Vector::dimension == 4)
		{
			_builder->get_widget("entry_line_z1", entry);
			z1 = atof(std::string(entry->get_text()).c_str());


			_builder->get_widget("entry_line_z2", entry);
			z2 = atof(std::string(entry->get_text()).c_str());
		}
		else
			z1 = z2 = model::Vector::z;

		add_entry(++_objects_control, name, type);

		if (!type.compare("Line"))
			_shapes.emplace_back(new model::Line(name, model::Vector(x1, y1, z1), model::Vector(x2, y2, z2)));
		else
			_shapes.emplace_back(new model::Rectangle(name, model::Vector(x1, y1, z1), model::Vector(x2, y2, z2)));

		_shapes_map[_objects_control] = _shapes.back();
	}

	void MainControl::insert_polygon(std::string name)
	{

	}

	void MainControl::add_entry(int id, std::string name, std::string type)
	{
	        Gtk::TreeModel::Row row = *(_ref_tree_model->append());
	        row[_tree_model._column_id]   = id;
	        row[_tree_model._column_name] = name;
	        row[_tree_model._column_type] = type;
	}

    void MainControl::up()
    {
        Gtk::SpinButton *spin;
        _builder->get_widget("spin_step", spin);

        double step = spin->get_value();
        int selected = _tree_selection->get_selected()->get_value(_tree_model._column_id);

        auto T = model::transformations::translation(model::Vector(0, step));
        
        if (!selected)
            _window->transformation(T);
        else
            _shapes_map[selected]->transformation(T);

        _viewport->update();
    }

    void MainControl::left()
    {
        Gtk::SpinButton *spin;
        _builder->get_widget("spin_step", spin);

        double step = spin->get_value();
        int selected = _tree_selection->get_selected()->get_value(_tree_model._column_id);

        auto T = model::transformations::translation(model::Vector(-step, 0));
        
        if (!selected)
            _window->transformation(T);
        else
            _shapes_map[selected]->transformation(T);

        _viewport->update();
    }

    void MainControl::right()
    {
        Gtk::SpinButton *spin;
        _builder->get_widget("spin_step", spin);

        double step = spin->get_value();
        int selected = _tree_selection->get_selected()->get_value(_tree_model._column_id);

        auto T = model::transformations::translation(model::Vector(step, 0));
        
        if (!selected)
            _window->transformation(T);
        else
            _shapes_map[selected]->transformation(T);

        _viewport->update();
    }

    void MainControl::down()
    {
        Gtk::SpinButton *spin;
        _builder->get_widget("spin_step", spin);

        double step = spin->get_value();
        int selected = _tree_selection->get_selected()->get_value(_tree_model._column_id);

        auto T = model::transformations::translation(model::Vector(0, -step));
        
        if (!selected)
            _window->transformation(T);
        else
            _shapes_map[selected]->transformation(T);

        _viewport->update();
    }

    void MainControl::zoom_in()
    {
        Gtk::SpinButton *spin;
        _builder->get_widget("spin_step", spin);

        double step = spin->get_value();
        int selected = _tree_selection->get_selected()->get_value(_tree_model._column_id);
        
        if (!selected)
        {
            auto mass_center = _window->mass_center();
            auto T = model::transformations::scheduling(step, mass_center);
            _window->transformation(T);
        }
        else
        {
            auto mass_center = _shapes_map[selected]->mass_center();
            auto T = model::transformations::scheduling(step, mass_center);
            _shapes_map[selected]->transformation(T);
        }

        _viewport->update();
    }

    void MainControl::zoom_out()
    {
        Gtk::SpinButton *spin;
        _builder->get_widget("spin_step", spin);

        double step = spin->get_value();
        int selected = _tree_selection->get_selected()->get_value(_tree_model._column_id);
        
        if (!selected)
        {
            auto mass_center = _window->mass_center();
            auto T = model::transformations::scheduling(step, mass_center);
            _window->transformation(T);
        }
        else
        {
            auto mass_center = _shapes_map[selected]->mass_center();
            auto T = model::transformations::scheduling(step, mass_center);
            _shapes_map[selected]->transformation(T);
        }

        _viewport->update();
    }


} //! namespace control

#endif  // CONTROL_MAIN_CONTROL_HPP
