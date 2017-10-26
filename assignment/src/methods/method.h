#ifndef METHOD_H  // include guard
#define METHOD_H

#include "../variants/problem.h" // declare the problem structure


/**
*  A Method class to structure information used to solve the problem
*
* The Method class provides:
* \n-basic constructors for creating a Method object.
* \n-acessor methods to retrieve valuable information
* \n-mutator methods to change the problem grid system
*/
class Method {
protected:
	Problem problem; /**< Private Problem problem. Space step of the solution. */
	std::string name; /**< Private string name. Name of the method. */
public:
	// CONSTRUCTORS

	/**
	* Default constructor.  Intialize a Method object
	* @see Method(Problem problem)
	*/
	Method();

	/**
	* Alternate constructor. Initializes a Method with a given parabolic problem.
	* @see Method()
	*/
	Method(Problem problem);

	// PUBLIC ACCESSOR METHODS

	/**
	* Normal public get method.
	* get the method name
	* @return string. Method name.
	*/
	std::string get_name();

	/**
	* Normal public get method.
	* get the solution grid
	* @return Matrix. Computed solution grid.
	*/
	Matrix get_solution();

	// PUBLIC MUTATOR METHODS

	/**
	* A pure virtual member.
	* compute the solution following the rules of a given method.
	*/
	virtual void compute_solution() = 0;
};

#endif