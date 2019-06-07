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

#ifndef CONTROL_OBJECT_LOADER_HPP
#define CONTROL_OBJECT_LOADER_HPP

/* External includes */
#include <fstream>
#include <sstream>

/* Local includes */
#include "../config/traits.hpp"
#include "../model/complex_shape.hpp"
#include "../model/window.hpp"

namespace control
{

/*================================================================================*/
/*                                   Definitions                                  */
/*================================================================================*/

	class ObjectLoader
	{
	public:
		ObjectLoader()  = default;
		~ObjectLoader() = default;

		std::vector<std::shared_ptr<model::Shape>> load(std::string path_name, const model::Vector& min, const model::Vector& max);
	
	private:
		std::vector<std::string> split(std::string s, const std::string & delimiter);
	};

/*================================================================================*/
/*                                 Implementaions                                 */
/*================================================================================*/


	std::vector<std::string> ObjectLoader::split(std::string s, const std::string & delimiter)
	{
		size_t pos = 0;
		std::string token;
		std::vector<std::string> tokens;
		
		while ((pos = s.find(delimiter)) != std::string::npos)
		{
			token = s.substr(0, pos);
			s.erase(0, pos + delimiter.length());

			tokens.push_back(token);
		}

		if(s.size())
			tokens.push_back(s);

		return tokens;
	}

	std::vector<std::shared_ptr<model::Shape>> ObjectLoader::load(std::string path_name, const model::Vector& min, const model::Vector& max)
	{
		db<ObjectLoader>(INF) << "ObjectLoader::load() => " << path_name << std::endl;

		std::ifstream file(path_name);

		if (!file.is_open())
		{
			db<ObjectLoader>(ERR) << "Unable to open file" << std::endl;
			return {};
		}

		auto map_x = [=](double x)
		{
			return x;
			// return (x - model::Window::fixed_min[0])
			// 	* (max[0] - min[0])
			// 	/ (model::Window::fixed_max[0] - model::Window::fixed_min[0])
			// 	+ min[0];
		};

		auto map_y = [=](double y)
		{
			return y;
			// return (y - model::Window::fixed_min[1])
			// 	* (max[1] - min[1])
			// 	/ (model::Window::fixed_max[1] - model::Window::fixed_min[1])
			// 	+ min[1];
		};

		auto map_z = [=](double z)
		{
			return z;
			// return (z - model::Window::fixed_min[2])
			// 	* (max[2] - min[2])
			// 	/ (model::Window::fixed_max[2] - model::Window::fixed_min[2])
			// 	+ min[2];
		};

		int count = 0;
		std::string line;
		std::string name;

		std::vector<model::Vector> vectors;
		std::vector<std::shared_ptr<model::Shape>> shapes;
		std::vector<std::shared_ptr<model::Shape>> complex_shapes;
		
		while (std::getline(file, line))
		{
			std::vector<std::string> words = split(line, " ");

			if (words.empty())
				continue;

			switch (words[0][0])
			{
			case 'o':
				if (!shapes.empty())
				{
					complex_shapes.emplace_back(new model::ComplexShape(name, shapes));
					shapes.clear();
				}

				count = 0;
				name = words[1];
				break;

			case 'v':
				if (words[0] == "v")
				{
					double x, y, z;

					std::stringstream string_x(words[1]);
					std::stringstream string_y(words[2]);
					std::stringstream string_z(words[3]);

					string_x >> x;
					string_y >> y;
					string_z >> z;

					if (model::Vector::dimension == 3)
						z = 1;
					
					vectors.emplace_back(
						map_x(x),
						map_y(y),
						map_z(z)
					);
				}

				break;

			case 'p':
			{
				int p = std::stoi(split(words[1], "/")[0]) - 1;

				shapes.emplace_back(
					new model::Point(name + std::to_string(count++), vectors[p])
				);

				break;
			}

			case 'l':
			{
				int p0 = std::stoi(split(words[1], "/")[0]) - 1;
				int p1 = std::stoi(split(words[2], "/")[0]) - 1;

				shapes.emplace_back(
					new model::Line(name + std::to_string(count++), vectors[p0], vectors[p1])
				);

				break;
			}

			case 'f':
			{
				words.erase(words.begin());
				std::vector<model::Vector> pvectors;

				for (const auto & v : words)
				{
					int idx = std::stoi(split(v, "/")[0]) - 1;
					pvectors.push_back(vectors[idx]);
				}

				shapes.emplace_back(
					new model::Polygon(name + std::to_string(count++), pvectors)
				);

				break;
			}

			default:
				break;
			}
		}

		file.close();

		return std::move(complex_shapes);
	}

} //! namespace control

#endif  // CONTROL_OBJECT_LOADER_HPP
