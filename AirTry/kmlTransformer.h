#pragma once

#include "kml/dom.h"
#include "kml/engine.h"
#include "kml/base/file.h"

#include <fstream>
#include "Lin.h"
#include <vector>


using kmldom::CoordinatesPtr;
using kmldom::KmlPtr;
using kmldom::KmlFactory;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;
using kmldom::LineStringPtr;
using std::vector;

class KML_Transformer
{
	//// Get the factory singleton to create KML elements.
	protected: KmlFactory* factory = KmlFactory::GetFactory();

	// Create <kml> and give it <Placemark>.
	protected: KmlPtr kml = factory->CreateKml();
	protected: kmldom::DocumentPtr document = factory->CreateDocument();
	protected: CoordinatesPtr coordinates = factory->CreateCoordinates();
	protected: LineStringPtr line = factory->CreateLineString();
	protected: PlacemarkPtr placemark = factory->CreatePlacemark();


	protected: std::fstream write;
	protected: std::string filename;
	protected: std::string xml;

	public: void CreateKML(std::string filename_inp)
	{
		kml->set_feature(document);  // kml takes ownership.

		filename = filename_inp + ".kml";
		line->set_coordinates(coordinates);  // point takes ownership
		placemark->set_geometry(line);  // placemark takes ownership
		document->add_feature(placemark);
	};
public: void KMLNewValue( Lin::Vector input) 
	{
		
		coordinates->add_latlngalt(input[0], input[1], input[2]);

		xml = kmldom::SerializePretty(kml);

		write.open(filename.c_str(), std::ios::out);
		write.write(xml.c_str(), xml.size());
		write.close();
	};
	public: void KML_ops_point(int waypoint_num, Lin::Vector input)
	{
		CoordinatesPtr coordinata = factory->CreateCoordinates();
		PointPtr point = factory->CreatePoint();
		PlacemarkPtr placemark1 = factory->CreatePlacemark();

		coordinata->add_latlngalt(input[0], input[1], input[2]);

		point->set_coordinates(coordinata);  // point takes ownership
		placemark1->set_name("Waypoint" + std::to_string(waypoint_num));
		placemark1->set_geometry(point);  // placemark takes ownership
		document->add_feature(placemark1);

		xml = kmldom::SerializePretty(kml);

		write.open(filename.c_str(), std::ios::out);
		write.write(xml.c_str(), xml.size());
		write.close();
	};
};


std::vector<Lin::Vector> parser(std::string filename)
{
	std::string filestr;
	std::string errors;
	kmlbase::File::ReadFileToString(filename, &filestr);
	kmldom::ElementPtr element = kmldom::Parse(filestr,&errors);
	
	KmlPtr kml = kmldom::AsKml(element);
	kmlengine::ElementVector spisok;
	kmlengine::GetElementsById(kml, kmldom::Type_coordinates, &spisok);
	CoordinatesPtr coord;
	coord = kmldom::AsCoordinates( spisok[0]);
	coord->get_coordinates_array_at(0);
	kmlbase::Vec3 vec;

	Lin::Vector V(3);
	std::vector<Lin::Vector> vec_coord;
	for (int i = 0; i < coord->get_coordinates_array_size(); i++)
	{
		vec = coord->get_coordinates_array_at(i);
		V = { vec.get_latitude(), vec.get_longitude(), vec.get_altitude() };
		vec_coord.push_back(V);	
	}
	
	return vec_coord;
};
