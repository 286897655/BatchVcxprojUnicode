
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <boost/filesystem.hpp>

#include <iostream>

namespace pt=boost::property_tree;
namespace fs=boost::filesystem;
void updatevcxproj(const std::string& filename)
{
	pt::ptree vcxproj;
	try{
		pt::read_xml(filename, vcxproj, pt::xml_parser::trim_whitespace);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		throw std::runtime_error("read vcxproj error stop!");
	}

	for (pt::ptree::value_type& v : vcxproj.get_child("Project"))
	{
		if (v.first == "PropertyGroup")
		{
			boost::optional<pt::ptree&> charsetchild = v.second.get_child_optional("CharacterSet");
			if (!charsetchild)
				continue;
			std::cout << filename<<"CharacterSet:--" << charsetchild.value().data() << std::endl;
			charsetchild.value().put_value("Unicode");
		}
	}
	write_xml(filename, vcxproj, std::locale(), pt::xml_writer_make_settings< std::string >(' ', 4));
}

inline bool hassuffixstring(const std::string& str, const std::string& suffix)
{
	return str.size() >= suffix.size() &&
		str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::vector<std::string> filteredfilename(const std::string& path, const std::string& suffix)
{
	std::vector<std::string> result;

	fs::path rootpath(path);

	if (!fs::exists(rootpath))
		throw std::runtime_error("not exist path");

	fs::recursive_directory_iterator end;
	for (fs::recursive_directory_iterator pos(rootpath); pos != end; ++pos)
	{
		if (fs::is_directory(*pos))
		{
			std::cout << "Directory:" << *pos << std::endl;
			continue;
		}
		std::string filename = pos->path().string();

		if (hassuffixstring(filename, suffix))
		{
			result.push_back(filename);
		}
	}

	return result;
}

int main(int argc, char* argv[])
{

	std::string path = R"(F:\GitWork\OpenDDS\OpenDDS3.9)";

	std::vector<std::string> vcxproj = filteredfilename(path, R"(.vcxproj)");

	std::cout << "filtered file£ºCount" <<vcxproj.size()<< std::endl;
	for (const auto& var : vcxproj)
	{
		std::cout << var << '\t';
	}
	std::cout << std::endl;
	system("pause");

	for (const auto& var : vcxproj)
	{
		updatevcxproj(var);
	}
	std::cout << "process end" << std::endl;
	return 0;
}

