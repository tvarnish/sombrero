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

#include "sombrero.h"

using namespace std;

int main(int argc, char *argv[]) {
	Simulation sim = Simulation();

	string body_file;
	double timestep;
	int step_count;

	int required_flag_count = 3;
	int valid_flag_count = 0;

	if (argc > 1) {
		// Parse command line arguments
		string current_flag = "";
    
		for (int argument_index = 1; argument_index < argc; argument_index++) {
			string current_argument = argv[argument_index];

			if (current_argument[0] == '-') {
				current_flag = current_argument.substr(1);
        
        // Show usage message
        if (current_flag == "h") {
          cout << "Usage: ./sombrero -i init_filename.csv -s step_count -dt time_step" << endl;
          return 0;
        }
			}
			else if (current_flag != "") { 
				if (current_flag == "i") {         
					body_file = current_argument;
					valid_flag_count++;
				}
				else if (current_flag == "s") {
					try {
						step_count = stoi(current_argument);
						valid_flag_count++;
					}
					catch (...) {
						cout << "ERROR: Simulation step count value is invalid." << endl;
						return 1;
					}
				}
				else if (current_flag == "dt") {
					try {
						timestep = stod(current_argument);
						valid_flag_count++;
					}
					catch (...) {
						cout << "ERROR: Simulation time step value (dt) is invalid." << endl;
						return 1;
					}
				}
			}
		}
	}

	// Check all required flags set
	if (valid_flag_count != required_flag_count) {
		cout << "ERROR: Not all required flags have been set." << endl;
    cout << "Usage: ./sombrero -i init_filename.csv -s step_count -dt time_step" << endl;
		return 1;
	}
	
	if (sim.LoadBodiesFromFile(body_file) == false) {
		cout << "ERROR: Bodies count not be loaded from supplied filepath." << endl;
		return 1;
	}

	sim.SetTimestep(timestep);
	sim.Run(0, step_count);

	return 0;
}

Simulation::Simulation() {
	// Constructor for Simulation object with default parameters
	body_list = List();
}

Simulation::Simulation(int _width, int _height, double _dt) {
	// Constructor for Simulation object - key parameters defined
	body_list = List();

	width = _width;
	height = _height;
	dt = _dt;
}

Simulation::Simulation(string _name, int _width, int _height, double _dt) {
	// Constructor for Simulation object - key parameters defined (except simulation name)
	body_list = List();

	name = _name;
	width = _width;
	height = _height;
	dt = _dt;
}

bool Simulation::LoadBodiesFromFile(string _file_name) {
	// Load a simulation set-up from a body .csv file, into the body_list
	regex valid_parameter_line("\\s*[0-9]*,(\\s*([+\\-]?[0-9]+(.[0-9]+)?([eE][+\\-]?[0-9]+)?)\\s*,){2}\\s*[0-9]*\r?");
	regex valid_line("(\\s*([+\\-]?[0-9]+(.[0-9]+)?([eE][+\\-]?[0-9]+)?)\\s*,){7}\\s*[+\\-]?[0-9]+(.[0-9]+)?([eE][+\\-]?[0-9]+)?,?([0-9a-zA-Z]*)?\\s*(//(\\s*\\S*)*)?\r?");
	regex valid_comment_line("\\s*//(\\s*\\S*)*\r?");

	ifstream input_file(_file_name);
	string file_line, parameter;
	int i;

	// Return false if the file does not exist (raise error)
	if (!FileExists(_file_name)) {
		return false;
	}

	bool body_line_reached = false;

	// Iterate through lines in the file
	while (getline(input_file, file_line))
	{
		if (regex_match(file_line, valid_comment_line)) {
			// Ignore line if it is a comment line
			continue;
		}
		else if (regex_match(file_line, valid_parameter_line)) {
			// Parameters must be specified before any bodies
			if (body_line_reached) {
				return false;
			}
			else {
				stringstream parameters(file_line);
				string parameter_array [4];
				i = 0;

				while (getline(parameters, parameter, ','))
				{
					parameter_array[i] = parameter;
					i++;
				}


				// TODO: Set simulation parameters from entered parameters
				// int last_frame = stoi(parameter_array[0]);
				// double timestep = stod(parameter_array[1]);

			}
		}
		else if (!regex_match(file_line, valid_line)) {
			// Return false if the line is not valid (raise error)
			return false;
		}
		else {
			body_line_reached = true;
			stringstream body_details(file_line);
			string detail_array [9];
			i = 0;

			// Iterate through each comma separated value, and store each in the 
			// detail_array (for easy access later)
			while (getline(body_details, parameter, ','))
			{
				detail_array[i] = parameter;
				i++;
			}

			double x = stod(detail_array[0]);
			double y = stod(detail_array[1]);
			double z = stod(detail_array[2]);
			double mass = stod(detail_array[3]);
			double radius = stod(detail_array[4]);
			double x_velocity = stod(detail_array[5]);
			double y_velocity = stod(detail_array[6]);
			double z_velocity = stod(detail_array[7]);
			string body_name = detail_array[8];
			// string body_name = "";

			body_list.Append(new Body(x, y, z, mass, radius, x_velocity, y_velocity, z_velocity, body_name));
			body_count += 1;
		}
	}

	return true;
}

void Simulation::GenerateRandomShell(int _body_count) {
	// Generate a set up of bodies, in a randomly spaced shell (equidistant to the centre) 
	// around a central heavy body.
	srand(time(NULL));
	body_list = List();

	for (int i = 0; i < _body_count - 1; i++) {
		// Generate the body's location with spherical coordinates
		double r = 0.5e11;
		double theta = Random(0, 2 * PI);
		double phi = Random(0, 2 * PI);

		// Convert spherical coordinates to cartesian coordinates
		double x = r * cos(theta) * cos(phi);
		double y = r * sin(theta);
		double z = r * cos(theta) * sin(phi);

		double mass = Random(1e23, 1e24);

		body_list.Append(new Body(x, y, z, mass, Random(1e6, 9e6), 0, 0, 0));
	}

	// Add the central heavy body to the body list
	body_list.Append(new Body(0.0, 0.0, 0.0, 1e30, 1e8, 0.0, 0.0, 0.0));

	// Save bodies to file
	SaveOutputFile(output_folder + name + ".csv");
}

void Simulation::GenerateRandomDistribution(int _body_count) {
	srand(time(NULL));
	body_list = List();

	for (int i = 0; i < _body_count; i++) {
		double x = Random(-3e11, 3e11);
		double y = Random(-3e11, 3e11);
		double z = Random(-3e11, 3e11);

		double mass = Random(1e26, 1e27);
		double radius = Random(1e6, 1e8);

		body_list.Append(new Body(x, y, z, mass, radius, 0, 0, 0));
	}

	// Save bodies to file
	SaveOutputFile(output_folder + name + ".csv");
}

void Simulation::GenerateRandomDistribution(int _body_count, double _width, double _body_mass_min, 
											double _body_mass_max, double _radius_min, 
											double _radius_max) {
	srand(time(NULL));
	body_list = List();

	for (int i = 0; i < _body_count; i++) {
		double x = Random(-_width / 2.0, _width / 2.0);
		double y = Random(-_width / 2.0, _width / 2.0);
		double z = Random(-_width / 2.0, _width / 2.0);

		double mass = Random(_body_mass_min, _body_mass_max);
		double radius = Random(_radius_min, _radius_max);

		body_list.Append(new Body(x, y, z, mass, radius, 0, 0, 0));
	}

	// Save bodies to file
	SaveOutputFile(output_folder + name + ".csv");
}

void Simulation::Run(int starting_frame, int frames_to_simulate) {
	// TODO: Enable starting from a different frame number
	// int current_frames = 0;

	double elapsed_time = starting_frame * dt;

	// Simulate the next frames
	// <=, as it simulates 0->1, 1->2 therefore needs to simulate up to n.
	for (int f = starting_frame; f <= frames_to_simulate + starting_frame; f++) {
		// Reset force counter on each body
		// (as force is used to count the net force on the body for that frame)
		body = body_list.GetHead();
		while (body != NULL) {
			body->ResetForce();
			body = body->GetNext();
		}

		// n-body algorithm (optimised)
		// i.e. (1: 2,3,4,5; 2: 3,4,5; etc)
		body_a = body_list.GetHead();
		body_b = NULL;

		while (body_a != NULL) {
			body_b = body_a->GetNext();

			while (body_b != NULL) {
				// Calculate distance between objects
				Vector separation_vector = body_a->GetPosition() - body_b->GetPosition();
				double total_distance = separation_vector.Magnitude();
				separation_vector = separation_vector / total_distance;

				// Calculate force
				double force_magnitude = (-1.0 * grav_const * ((body_a->GetMass() 
										  * body_b->GetMass()) / (pow(total_distance, 2))));

				// Add forces to totals
				body_a->AddForce(separation_vector * force_magnitude);
				body_b->AddForce(separation_vector * -force_magnitude);

				// Advance pointer
				body_b = body_b->GetNext();
			}

			body_a = body_a->GetNext();
		}

		// Calculate next position for each body
		body = body_list.GetHead();
		while (body != NULL) {
			body->Update(dt);

			body = body->GetNext();
		}

		// Collision Physics
		body_a = body_list.GetHead();
		body_b = NULL;

		while (body_a != NULL) {
			body_b = body_a->GetNext();

			while (body_b != NULL) {
				// Check for collision
				Vector body_a_position = body_a->GetPosition();
				Vector body_b_position = body_b->GetPosition();
				Vector body_a_next_position = body_a->GetNextPosition();
				Vector body_b_next_position = body_b->GetNextPosition();

				Vector lambda = body_b_position - body_a_position;
				Vector mu = body_b_next_position - body_a_next_position 
							+ body_a_position - body_b_position;

				// Quadratic coefficients => at^2 + bt + c = 0
				double a = pow(mu.Magnitude(), 2);
				double b = 2 * lambda.DotProduct(mu);
				double c = pow(lambda.Magnitude(), 2)
						   - pow(body_a->GetRadius() + body_b->GetRadius(), 2);

				double t1 = ((-1 * b) + (sqrt(pow(b, 2) - (4 * a * c))))/(2 * a);
				double t2 = ((-1 * b) - (sqrt(pow(b, 2) - (4 * a * c))))/(2 * a);

				bool t1_valid = (t1 >= 0 && t1 <= 1);
				bool t2_valid = (t2 >= 0 && t2 <= 1);

				double separation = (body_b_position - body_a_position).Magnitude();
				double radii_sum = body_a->GetRadius() + body_b->GetRadius();
				
				// Select the first valid (0 <= t <= 1) collision time (smallest t)
				if (separation <= radii_sum) {
					// TODO: Determine if bodies should combine or bounce.
					//		 Currently, default: combine (true), (e = 1.0)
					HandleCollision(body_a, body_b, 0);

					// Update the body pointers 
					// 		(as both body_a and body_b have been deleted 
					//		from the list of bodies)
					body_a = body_list.GetHead();
					body_b = body_a->GetNext();
				}
				else if ((t1 <= t2 && t1_valid)) {
					// TODO: Determine if bodies should combine or bounce.
					//		 Currently, default: combine (true), (e = 1.0)
					HandleCollision(body_a, body_b, t1);

					// Update the body pointers
					body_a = body_list.GetHead();
					body_b = body_a->GetNext();
				}
				else if (t2_valid) {
					// TODO: Determine if bodies should combine or bounce.
					//		 Currently, default: combine (true), (e = 1.0)
					HandleCollision(body_a, body_b, t2);
					
					// Update the body pointers
					body_a = body_list.GetHead();
					body_b = body_a->GetNext();
				}
				else {
					body_b = body_b->GetNext();
				}
			}

			body_a = body_a->GetNext();
		}

		/*
		// Format elapsed time (for image output)
		string elapsed_timeString;
		string time_units = GetTimeUnits(elapsed_time);

		if (time_units == "YEARS") {
			elapsed_timeString = RemoveTrailingZeroes(to_string(elapsed_time / (double)YR));
		}
		else if (time_units == "DAYS ") {
			elapsed_timeString = RemoveTrailingZeroes(to_string(elapsed_time / (double)DAY));
		}
		else if (time_units == "HOURS") {
			elapsed_timeString = RemoveTrailingZeroes(to_string(elapsed_time / (double)HOUR));
		}
		else if (time_units == "MINS ") {
			elapsed_timeString = RemoveTrailingZeroes(to_string(elapsed_time / (double)MINUTE));
		}
		else if (time_units == "SECS ") {
			elapsed_timeString = RemoveTrailingZeroes(to_string(elapsed_time));
		}
		*/

		// Create _output.csv
		// TODO: This only creates an output file if the folder structure exists!
		string data_file_name = output_folder + "data/bodyData_"
							    + PadWithZeroes(f - starting_frame, frames_to_simulate) + ".csv";
		SaveOutputFile(data_file_name, f, dt, f * dt, body_count);

		// Move each body to their new positions
		body = body_list.GetHead();
		while (body != NULL) {
			body->Step();

			body = body->GetNext();
		}

		// Update the elapsed time
		elapsed_time += dt;
	}
}

void Simulation::SaveOutputFile(string _file_name, int _step_number, double _dt,
								double _time_elapsed, int _body_count) {
	// Save bodies to output.csv
	string output_file_name = output_folder + name + "_output.csv";

	if (_file_name != "") {
		output_file_name = _file_name;
	}

	Output output(output_file_name);
	output.AddParameters(_step_number, _dt, _time_elapsed, _body_count);
	output.AddAllBodies(body_list);
	output.Save();
}

void Simulation::AddBody(Body * body) {
	body_list.Append(body);
}

void Simulation::ClearBodyList() {
	body_list = List();
}

void Simulation::SetOutputDirectory(string _output_folder) {
	output_folder = _output_folder;
	string command = "mkdir -p " + output_folder;
	system(command.c_str());

	// Deal with final slash
	if (output_folder[output_folder.length() - 1] != '/') {
		output_folder += '/';
	}
}

string Simulation::GetTimeUnits(double time) {
	// Determine, and return, the time units (Seconds, Days, Years) for the given time.
	regex valid_integer("[1-9][0-9]*");
	string time_units;

	// Note: space included for "DAYS " purely for spacing when output
	if (time < DAY) {
			if (regex_match(to_string(1 / (time / DAY)), valid_integer)) {
				// Check if the time is in the form DAY / n, where n is an integer
				time_units = "DAYS ";
			}
			else if (time >= HOUR) {
				time_units = "HOURS";
			}
			else if (time >= 2 * MINUTE) {
				time_units = "MINS ";
			}
			else {
				time_units = "SECS ";
			}
	}
	else if (time > DAY) {
		if (time < 2 * YR) {
			time_units = "DAYS ";
		}
		else {
			time_units = "YEARS";
		}
	}
	else if (time == DAY) {
		time_units = "DAYS ";
	}

	return time_units;
}

void Simulation::HandleCollision(Body * body_a, Body * body_b, double t, double e, bool combine) {
	// TODO: Improve this function to include a dynamic combine/collide 
	//		 functionality (depending on certain conditions).

	// Currently decides between combining (2 -> 1) or colliding 
	// ("bouncing" off one another) bodies, depending on arguments.
	
	if (combine == true) {
		CombineBodies(body_a, body_b, t);
	} else {
		CollideBodies(body_a, body_b, t, e);
	}
}

void Simulation::CombineBodies(Body * body_a, Body * body_b, double t) {
	// Calculate new Mass (sum of original bodies)
	double new_mass = body_a->GetMass() + body_b->GetMass();

	// Conservation of linear momentum (assuming bodies will combine)
	Vector a_momentum = body_a->GetMomentum();
	Vector b_momentum = body_b->GetMomentum();
	Vector new_velocity = (a_momentum + b_momentum) / new_mass;

	// Calculate position of new body
	// Average the centres of the bodies at their collision positions
	body_a->Update(t * dt);
	body_b->Update(t * dt);

	Vector body_a_position = body_a->GetNextPosition();
	Vector body_b_position = body_b->GetNextPosition();

	Vector new_position = (body_a_position + body_b_position) / 2.0;

	// Calculate new radius - use volumes of the two materials
	double a_volume = (4.0/3.0) * PI * pow(body_a->GetRadius(), 3);
	double b_volume = (4.0/3.0) * PI * pow(body_b->GetRadius(), 3);
	double new_volume = a_volume + b_volume;

	double new_radius = pow(new_volume / ((4.0/3.0) * PI), (1.0/3.0));

	// Create a new (combined) body, and remove A and B
	Body * new_body = new Body(new_position, new_mass, new_radius, new_velocity);
	new_body->Update((1 - t) * dt);

	body_list.Append(new_body);
	
	body_list.Remove(body_a->GetID());
	body_list.Remove(body_b->GetID());

	// Update body count
	body_count -= 1;
}

void Simulation::CollideBodies(Body * body_a, Body * body_b, double t, double e) {
	// Move bodies to point of collision
	body_a->Update(t * dt);
	body_b->Update(t * dt);

	double m_a = body_a->GetMass();
	double m_b = body_b->GetMass();
	double m = m_a + m_b;

	// Calculate direction vector connecting centers of the bodies
	Vector center_line_ab = body_b->GetPosition() - body_a->GetPosition();
	center_line_ab = center_line_ab / center_line_ab.Magnitude();

	// Calculate velocities along center line (problem is 1D)
	double u_a = body_a->GetVelocity().DotProduct(center_line_ab);
	double u_b = body_b->GetVelocity().DotProduct(center_line_ab);

	double total_momentum = (m_a * u_a) + (m_b * u_b);
	double v_a = (total_momentum + (m_b * e * (u_b - u_a))) / m_b;
	double v_b = (total_momentum + (m_a * e * (u_a - u_b))) / m;
	
	body_a->AddVelocity(center_line_ab * (v_a - u_a));
	body_b->AddVelocity(center_line_ab * (v_b - u_b));

	// Move the bodies to the end of the time-step
	body_a->Update((1 - t) * dt);
	body_b->Update((1 - t) * dt);
}