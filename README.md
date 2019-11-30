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
	* *Block* objects
	* *functions* performed on *Operational* objects
* Perform:
	* Cgp init 
	* computes fitness algorithm of genotypes
	* mutations
	* recombinations

__**Block**__
* Class representing a node in CGP graph
* Stores:
	* *Operational* object
	* input *Block* pointers
	* its cruciality to current combination
	
__**Operational**__
* Item class representing an object (its value/s) 
* Is virtual class. User defining own object type must inherit from it

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

