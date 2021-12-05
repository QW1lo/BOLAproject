
#include <iostream>
#include <string>
#include "kml/dom.h"
#include "kmlTransformer.h"

// libkml types are in the kmldom namespace
using kmldom::CoordinatesPtr;
using kmldom::KmlPtr;
using kmldom::KmlFactory;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;
using std::vector;


int main() {
	KML_Transformer kml_trns;
	double coord[3] = { 10,15,120 };
	double coord1[3] = { 11,14,120 };
	double coord2[3] = { 12,13,120 };
	double coord3[3] = {10,10,120};
	kml_trns.CreateKML("test");
	kml_trns.KMLNewValue(coord);
	kml_trns.KMLNewValue(coord1);
	kml_trns.KMLNewValue(coord2);
	kml_trns.KMLNewValue(coord3);

	double c[3] = { 10,30,120 };
	double c1[3] = { 30,10,120 };
	kml_trns.KML_ops_point(0, c);
	kml_trns.KML_ops_point(0, c1);
	
	vector<vector<double>> vec_coord;
	vec_coord = parser("pyt.kml");
	std::cout << vec_coord[1][0] << std::endl;
}
