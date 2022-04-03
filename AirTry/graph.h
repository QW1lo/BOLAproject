#pragma once
#ifndef GRAPH1010
#define GRAPH1010
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "gnuplot-iostream.h"

#define GRAPH_ENABLED true

class plot {
public:
	FILE* gp;
	bool enabled, persist;
	plot(bool _persist = false, bool _enabled = GRAPH_ENABLED) {
		enabled = _enabled;
		persist = _persist;
		if (enabled) {
			if (persist)
				gp = _popen("gnuplot -persist", "w");
			else
				gp = _popen("gnuplot", "w");
		}
	}

	void plot_data(vector<double> x, const char* style = "points", const char* title = "Data") {
		if (!enabled)
			return;
		fprintf(gp, "set title '%s' \n", title);
		fprintf(gp, "plot '-' w %s \n", style);
		for (int k = 0;k < x.size();k++) {
			fprintf(gp, "%f\n", x[k]);
		}
		fprintf(gp, "e\n");
		fflush(gp);
	}

	void plot_data(vector<double> x, vector<double> y, const char* style = "points", const char* title = "Data") {
		if (!enabled)
			return;
		fprintf(gp, "set title '%s' \n", title);
		fprintf(gp, "plot '-' w %s \n", style);
		for (int k = 0;k < x.size();k++) {
			fprintf(gp, "%f %f \n", x[k], y[k]);
		}
		fprintf(gp, "e\n");
		fflush(gp);
	}

	~plot() {
		if (enabled)
			_pclose(gp);
	}

};

/*
int main(int argc,char **argv) {
 plot p;
 for(int a=0;a<100;a++) {
 vector<float> x,y;
 for(int k=a;k<a+200;k++) {
   x.push_back(k);
   y.push_back(k*k);
 }
  p.plot_data(x,y);
 }
 return 0;
}
*/

#endif