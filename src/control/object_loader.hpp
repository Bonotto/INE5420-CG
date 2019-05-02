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

/* Local includes */
#include "../config/traits.hpp"

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

		std::vector<std::shared_ptr<model::Shape>> load();
	
	private:
		std::vector<std::string> split(std::string line, std::string separator);
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
	}

	std::vector<std::shared_ptr<model::Shape>> ObjectLoader::load(std::string path_name)
	{
		std::ifstream file(path_name);

		if (!file.is_open())
			std::cout << "Unable to open file" << std::endl;
		
		std::string line;
		std::string name;
		bool new_vectors = true;

		std::vector<model::Vector> vectors;
		std::vector<std::shared_ptr<model::Shape>> shapes;
		
		while (std::getline(file, line))
		{
			auto words = split(line, " ");

			switch (words[0][0])
			{
			case 'o':
				name = words[1] + shapes.size();
				break;

			case 'v': {
				if (new_vectors)
				{
					vectors.clear();
					new_vectors = false;
				}

				double x = std::stod(words[1]);
				double y = std::stod(words[2]);
				double z = std::stod(words[3]);
				
				vectors.emplace_back(x, y, z);

				break;
			}

			case 'p': {
				new_vectors = true;

				int p = std::stoi(split(words[1], "/")[0]) - 1;

				shapes.emplace_back(new model::Point(name, vectors[p]));

				break;
			}

			case 'l': {
				new_vectors = true;

				int p0 = std::stoi(split(words[1], "/")[0]) - 1;
				int p1 = std::stoi(split(words[2], "/")[0]) - 1;

				shapes.emplace_back(new model::Line(name, vectors[p0], vectors[p1]));

				break;
			}

			case 'f': {
				new_vectors = true;

				words.pop_front();
				std::vector<model::Vector> pvectors;

				for (const auto & v : words)
				{
					int idx = std::stoi(split(v, "/")[0]) - 1;

					pvectors.push_back(vectors[idx]);
				}

				shapes.emplace_back(new model::Polygon(name, pvectors));

				break;
			}

			default:
				break;
			}
		}

		file.close();

		return std::move(shapes);
	}

} //! namespace control

#endif  // CONTROL_OBJECT_LOADER_HPP
