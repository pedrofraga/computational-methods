#include <iostream>
#include "matrix.h"

const double XSIZE = 100;
const double DELTAX = 80 / XSIZE;
const double DELTAT = 0.1;
const double TSIZE = 20 / DELTAT;
const double A = 1;

double sign(double value) {
	if (value < 0) {
		return -1.0;
	} else if (value == 0) {
		return 0.0;
	} else {
		return 1.0;
	}
}

void upwind_scheme(Matrix & f) {
	for (int n = 1; n < TSIZE; n++) {
		for (int i = 1; i < XSIZE; i++) {
			f[n][i] = f[n - 1][i] - A * DELTAT * (f[n - 1][i] - f[n - 1][i - 1]) / DELTAX;
		}
	}
}

void central_difference_scheme(Matrix & f) {
	for (int n = 0; n < TSIZE; n++) {
		for (int i = 1; i < XSIZE; i++) {
			f[n + 1][i] = f[n][i] - A * DELTAT * (f[n][i + 1] - f[n][i - 1]) / (2 * DELTAX);
		}
	}
}

void lax_scheme(Matrix & f) {
	for (int n = 0; n < TSIZE; n++) {
		for (int i = 1; i < XSIZE; i++) {
			f[n + 1][i] = 0.5 * (f[n][i + 1] + f[n][i - 1]) - A * DELTAT / (2 * DELTAX) * (f[n][i + 1] - f[n][i - 1]);
		}
	}
}

void leapfrog_scheme(Matrix & f) {
	for (int i = 1; i < XSIZE; i++) {
		f[1][i] = f[0][i] - A * DELTAT / DELTAX * (f[0][i + 1] - f[0][i - 1]);
	}
	for (int n = 1; n < TSIZE; n++) {
		for (int i = 1; i < XSIZE; i++) {
			f[n + 1][i] = f[n - 1][i] - A * DELTAT / DELTAX * (f[n][i + 1] - f[n][i - 1]);
		}
	}
}

void first_analitical_solution(Matrix & f, Vector & x, Vector & t) {
	for (int n = 0; n <= TSIZE; n++) {
		for (int i = 0; i <= XSIZE; i++) {
			f[n][i] = 0.5 * (sign(x[i] - t[n]) + 1);
		}
	}
}

void second_analitical_solution(Matrix & f, Vector & x, Vector & t) {
	for (int n = 0; n <= TSIZE; n++) {
		for (int i = 0; i <= XSIZE; i++) {
			f[n][i] = 0.5 * exp(-pow((x[i] - t[n]), 2));
		}
	}
}

void compute_errors(Matrix & a, Matrix & b, Matrix & e) {
	for (int n = 0; n <= TSIZE; n++) {
		for (int i = 0; i <= XSIZE; i++) {
			e[n][i] = a[n][i] - b[n][i];
		}
	}
}

void print_csv(const char* name, Matrix matrix, Vector xvalues, Vector tvalues) {
	std::ofstream out;
	out.open(name);
	for (unsigned int j = 0; j < xvalues.getSize(); j++) {
		out << xvalues[j] << ',';
	}
	out << '\n';
	for (unsigned int i = 0; i < matrix.getNrows(); i++) {
		if (tvalues[i] == 5 || tvalues[i] == 10 || tvalues[i] == 20) {
		  for (unsigned int j = 0; j < matrix.getNcols(); j++)
		    out << matrix[i][j] << ',';
		  out << '\n';
		}
	}
	out.close();
}

void print_norms(std::string scheme_name, Matrix analitical_matrix, Matrix matrix, Vector tvalues) {
	Matrix error_matrix(TSIZE + 1, XSIZE + 1);
	compute_errors(matrix, analitical_matrix, error_matrix);

	std::cout << scheme_name << std::endl;
	for (double t = 1; t <= TSIZE; t++) {
		if (tvalues[t] == 5 || tvalues[t] == 10 || tvalues[t] == 20) {
			Vector vector(error_matrix[t]);
			std::cout << "1st norm at " << tvalues[t] << ": " << vector.one_norm() / XSIZE << std::endl;
			std::cout << "2nd norm at  " << tvalues[t] << ": " << vector.two_norm() / XSIZE << std::endl;
			std::cout << "uniform norm at " << tvalues[t] << ": " << vector.uniform_norm() / XSIZE << std::endl << std::endl;
		}
	}
}

void solve_first_case() {
	Matrix matrix(TSIZE + 1, XSIZE + 1);
	Matrix analitical_matrix(TSIZE + 1, XSIZE + 1);

	Vector xvalues(XSIZE + 1);
	Vector tvalues(TSIZE + 1);

	for (double i = 0; i <= XSIZE; i++) {
		xvalues[i] = -40 + (i * DELTAX);
		matrix[0][i] = 0.5 * (sign(xvalues[i]) + 1);
	}
	for (double t = 0; t <= TSIZE; t++) {
		matrix[t][0] = 0;
		matrix[t][XSIZE] = 1;
		tvalues[t] = DELTAT * t;
	}

	Matrix upwind_matrix = matrix, 
	central_difference_matrix = matrix, 
	lax_matrix = matrix,
	leapfrog_matrix = matrix;

	upwind_scheme(upwind_matrix);
	central_difference_scheme(central_difference_matrix);
	lax_scheme(lax_matrix);
	leapfrog_scheme(leapfrog_matrix);
	first_analitical_solution(analitical_matrix, xvalues, tvalues);
	
	std::cout << std::endl << "DELTAX = " << DELTAX << std::endl;
	std::cout << "DELTAT = " << DELTAT << std::endl << std::endl;
	
	print_norms("Upwind scheme", analitical_matrix, upwind_matrix, tvalues);
	print_norms("Central Difference scheme", analitical_matrix, central_difference_matrix, tvalues);
	print_norms("Lax scheme", analitical_matrix, lax_matrix, tvalues);
	print_norms("Leapfrog scheme", analitical_matrix, leapfrog_matrix, tvalues);

	print_csv("./matrices/1stcase/analitical.csv", analitical_matrix, xvalues, tvalues);
	print_csv("./matrices/1stcase/upwind.csv", upwind_matrix, xvalues, tvalues);
	print_csv("./matrices/1stcase/centraldifference.csv", central_difference_matrix, xvalues, tvalues);
	print_csv("./matrices/1stcase/lax.csv", lax_matrix, xvalues, tvalues);
	print_csv("./matrices/1stcase/leapfrog.csv", leapfrog_matrix, xvalues, tvalues);
}

void solve_second_case() {
	Matrix matrix(TSIZE + 1, XSIZE + 1);
	Matrix analitical_matrix(TSIZE + 1, XSIZE + 1);

	Vector xvalues(XSIZE + 1);
	Vector tvalues(TSIZE + 1);

	for (double i = 0; i <= XSIZE; i++) {
		xvalues[i] = -40 + (i * DELTAX);
		matrix[0][i] = 0.5 * exp(-pow(xvalues[i], 2));
	}
	for (double t = 0; t <= TSIZE; t++) {
		matrix[t][0] = 0;
		matrix[t][XSIZE] = 0;
		tvalues[t] = DELTAT * t;
	}

	Matrix upwind_matrix = matrix, 
	central_difference_matrix = matrix, 
	lax_matrix = matrix,
	leapfrog_matrix = matrix;

	upwind_scheme(upwind_matrix);
	central_difference_scheme(central_difference_matrix);
	lax_scheme(lax_matrix);
	leapfrog_scheme(leapfrog_matrix);
	second_analitical_solution(analitical_matrix, xvalues, tvalues);
	
	std::cout << std::endl << "DELTAX = " << DELTAX << std::endl;
	std::cout << "DELTAT = " << DELTAT << std::endl << std::endl;
	
	print_norms("Upwind scheme", analitical_matrix, upwind_matrix, tvalues);
	print_norms("Central Difference scheme", analitical_matrix, central_difference_matrix, tvalues);
	print_norms("Lax scheme", analitical_matrix, lax_matrix, tvalues);
	print_norms("Leapfrog scheme", analitical_matrix, leapfrog_matrix, tvalues);

	print_csv("./matrices/2ndcase/analitical.csv", analitical_matrix, xvalues, tvalues);
	print_csv("./matrices/2ndcase/upwind.csv", upwind_matrix, xvalues, tvalues);
	print_csv("./matrices/2ndcase/centraldifference.csv", central_difference_matrix, xvalues, tvalues);
	print_csv("./matrices/2ndcase/lax.csv", lax_matrix, xvalues, tvalues);
	print_csv("./matrices/2ndcase/leapfrog.csv", leapfrog_matrix, xvalues, tvalues);
}

int main() {
	solve_first_case();
	//solve_second_case();

	return 0;
}