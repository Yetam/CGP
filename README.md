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

__**Operational**__
* Item class representing an object (its value/s)
* Is virtual class. User defining own object type must inherit from it

# Should do:

* class **CGP_Algorithm**
- [ ] Can instantiate an "organism" - a set of connected _Blocks_ with set functions
- [ ] Initializes organism with nRows and nCols of empty nodes
- [ ] Creates _Blocks_ in all nodes of organism and connect them in random manner
- [ ] Can acquire and store functions as pointers. Each function must operate on two _Operational_ objects (functions are static for all organisms)
- [ ] Can copy entire organism (Copying _Blocks_ from one organism pointer to another)
- [ ] Can calculate fitness functions of an organism
- [ ] Can mutate operation types of an organism with preffered probability
- [ ] Can mutate connections of an organism with preffered probability

* class **Block**
- [ ] Stores pointers to two input *Blocks*
- [ ] Stores info about used function
- [ ] Stores info if the block is inputBlock
- [ ] Stores info wether this block is crucial to algorithm (whether its output has impact on global output)
- [ ] Has ability to store value calculated by this _Block_ - exact value that will be passed to output
- [ ] Can return its value as pointer to it

* class **Operational**
- [ ] Must be inherited for user-defined object (value)

# Roadmap
- [x] Start implementing :)
- [x] Define architecture of program
- [x] Implement template-ness so that user inherit *Operational* can be imposed
- [x] Adding *formulas* to *CGP_Algorithm* so that user only adds by *addFormula()* method, and ends with *enoughFormulas()* method
- [ ] Blocks setup on grid and *fasion* they are arranged in
- [ ] Define architecture of running of CGP_Algorithm
- [ ] Define architecture of evaluating current set of Blocks into fitness
- [ ] Implement virtual methods in *Operational* reauired by running and evaluating architecture
- [ ] Define genetic strategies possible in this implementations
- [ ] Define architecture of offspring generation
- [ ] Define architecture of mutation
- [ ] Define architecture of crossing two algorithms

# Im at:
* defining copy functions for Program, Build, Block and Operational
* @ is being defined to be able to copy organism from parent to offspring
