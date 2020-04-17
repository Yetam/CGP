# CGP
Cartesian Genetic Porgraming Algorithm; C++ implementation
This program is realizing CGP algorithm on user-defined class.

## \(E)Sense of CGP
CGP evolves genotype of "operations" of given "object type" so that,
when input parameters are passed, the graph of operations process those
inputs into desired outputs.

CGP operates on acyclic graph of operations set over cartesian (square) grid.
Each node proceses two inputs into one output.

Fitness function describes similarity of output to desired output and creates
an offspring based on that fitness function

## Course of implementation
This implementation should provide a class to be inherit from to implement
user-defined type of object that CGP will manipulate and user-defined
operaations that CGP will perform on those objects

# Implementation
__**CGP_Algorithm**__
* Core class of CGP implementation
* Stores:
	* *Program* objects
	* *functions* performed on *Operational* objects
* Perform:
	* Cgp init
	* computes fitness algorithm of genotypes
	* mutations
	* recombinations

__**Program**__
* Class representing full genotype
* Stores:
	* *Block* objects
	* program size (number of rows and columns)
* Perform
	* access to each nodes


__**Block**__
* Class representing a node in CGP graph
* Stores:
	* *Operational* object
	* input *Block* pointers
	* its cruciality to current combination


# Should do:

* class **CGP_Algorithm**
- [x] WORKS!

* class **Block**
- [x] WORKS!
# User must define:
	* Fitness funtion of form:
		* double fitness(Program<bool> * p){
		* 	std::vector<T> v_input;		//vector of input values to program (initialized and filled by user)
    * 	std::vector<T*> *v_output;	//pointer to vector of input values to program (NOT initialized by user)
		* 	v_output = p->calcFitnessValue( &v_input );	//calculates outputs from inputs with Porgram p. May be run multiple times if needed
		* 	returns fitness; //based on input values and output values user must calculate a fitness of given program.
		* }

	* Formulas functions of form:
		* void XOR(std::vector<T*> * funcInputs, T *tuncOutput){
		*		//some code that calculates output value from input values vector. User must not define the memory of funcInputs or tuncOutput
		* }

# Setting up and running the algorithm
- CGP_Algorithm<bool> mainCGP( inputGenes, workGenes, outputGenes, fitnessFunction);	//one
	mainCGP.addFormula(formulaPointer,inputsRequired);
	mainCGP.doCGP("SETUP");
	mainCGP.doCGP("PROCEED",epochs);

	user must provide:
	* inputGenes - number of inputs to program (int)
	* workGenes - number of genes that run formulas
	* outputGenes - number of output genes (also running formulas) (int)
	* fitnessFunction - pointer to fitness function of form:  which measures the program
	*	formulaPointer - pointer to formula function
	* inputsRequired - number of inpiuts required by given formula function to work properly
	* epochs - number of epochs the CGP algorithm is supposed to run

# User may also use:
	* mainCGP.getBestOrganism(); //returns pointer to lowest fitness Organism
	* Program::programToString(); //returns std::string with all Program information
		* each program gene is in new line of form: "(int)ID (int)formula_id (int)input_1 ... (int)input_n"

# Meeting TODO:
- [x] Program has one dimensional Block graph
- [x] formulas accepts any number of input arguments as array
- [x] Block has an array of inputs
