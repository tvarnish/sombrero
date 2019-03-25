#ifndef __SOMBRERO_H_INCLUDED__
#define __SOMBRERO_H_INCLUDED__

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <regex>
#include <map>

#include "lib/body.h"
#include "lib/output.h"
#include "lib/misc.h"
#include "lib/matrix.h"
#include "lib/position_vector.h"
#include "lib/linkedlist.h"
#include "lib/units.h"

using namespace std;

class Simulation {
  // Parameters initialised with defaults
  string name = "simulation";
  string output_folder = "";
  int body_count = 0;
  int width = 640;
  int height = 480;
  double scale = (AU / 100);
  double dt = DAY / 2;
  int framerate = 60;
  double grav_const = GR;

  List body_list;
  Body * body;
  Body * body_a;
  Body * body_b;

  string GetTimeUnits(double time);

  // Private Simulation Methods
  void HandleCollision(Body * body_a, Body * body_b, double t, double e = 1.0, bool combine = true);
  void CollideBodies(Body * body_a, Body * body_b, double t, double e);
  void CombineBodies(Body * body_a, Body * body_b, double t);

  public:
    // Constructors
  	Simulation();
  	Simulation(int _width, int _height, double _dt);
  	Simulation(string _name, int _width, int _height, double _dt);

  	// Set Methods
  	void SetTimestep(double _dt) { dt = _dt; };
  	void SetOutputDirectory(string _output_folder);
  	void SetSimulationName(string _name) { name = _name; };
  	void SetGravitationalConstant(double _grav_const) { grav_const = _grav_const; };

    // Get Methods
  	int GetNumberOfBodies() { return body_list.GetLength(); };

  	bool LoadBodiesFromFile(string _file_name);
	  void SaveOutputFile(string _file_name, int _step_number = -1, double _dt = -1,
                        double _time_elapsed = -1, int _body_count = -1);

    // Generate body setup commands
  	void GenerateRandomShell(int _body_count);
  	void GenerateRandomDistribution(int _body_count);
    void GenerateRandomDistribution(int _body_count, double _width, double _body_mass_min,
                                    double _body_mass_max, double _radius_min, double _radius_max);

    void AddBody(Body * body);
    void ClearBodyList();
    List GetBodyList() { return body_list; };

    // Simulation Methods
  	void Run(int starting_frame, int frames_to_simulate);
};

#endif