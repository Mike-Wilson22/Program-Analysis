# Program-Analysis

I developed two different tools: one to detect uncalled functions, and one to detect statements generating a variable definition that was never used.


# Uncalled Functions

I set out to identify functions that were never called by the program. This makes use of the previous analysis done on PTS as well as the ICFG. Without the PTS analysis, the detection is much less accurate. When approaching this problem, however, there are a couple of design issues I ran into.

First, it is impossible to know the entry point of the program. Second, there could be multiple entry points to the program, and only the developer using the tool knows where the entry points are.
In order to resolve these issues, I designed my code to find splits in the ICFG. Instead of assuming an entry point, it finds all sections that contain no connections between them. This allows the developer to look through the output and utilize their knowledge of the entry points of the program in order to identify any functions that can never be called.

The code does this by starting at a random function, then spreading in all directions. It marks all nodes it can reach, then adds the set of functions it reached to a set. It iterates through the beginning of each function, checking to see if that's already been visited before doing analysis.
After it finishes, it has identified the each set of functions that can reach each other. It gives this to the user so they, knowing the entry points, can easily see functions that are unreachable.

There are several different tests I modified, and each one tests a different piece of functionality. The tests can be run from the pa_template directory using this command:
./build/myAnalysis -b benchmark/benchX/demo.bc -f dcd1
Replace X with one of the bench numbers identified below in order to see functionality

bench8:
Tests that the code can handle function pointers without identifying them as unreachable

bench9:
Tests that the code can handle only a single section

bench10:
Tests that the code can handle multiple entry points, similar to a library



# Unused Variable Declarations

I set out to identify definitions that are not used in the result of a function. This makes use of the GEN set as well as the IDFG graph (though no analysis is done across functions). The code also identifies definitions that are used only in a useless definition as dead code.

The code does this by iterating through a DFG of a function. It looks for nodes that have GEN sets--indicating that they generate definitions. It then looks for outgoing DFG edges. If it finds one, this means that the definition generated at the current node is used elsewhere, so it is not dead code. After adding all nodes that generate new definitions but have no outgoing DFG edges to a set, however, it does not finish. Instead, it checks to see whether the set of dead code has changed. If it has, it reruns the analysis, but also disqualifies nodes that have definitions with outgoing DFG edges if those edges only lead to nodes identified as dead code. This allows the program to identify chains of dead code if the final result is unused.

There are several different tests I modified, and each one tests a different piece of functionality. The tests can be run from the pa_template directory using this command:
./build/myAnalysis -b benchmark/benchX/demo.bc -f dcd2
Replace X with one of the bench numbers identified below in order to see functionality

bench2:
Tests that the code can identify unused parameters and unused variable definitions, along with their allocations. It also identifies the initialization of a used variable, though I'm not sure this default initialization can actually be removed from the IR, as it something the LLVM IR does by itself

bench3:
Tests that code has no false positives in a program with no unused variables. It does however, again detect the default initialization done by the LLVM IR as dead code, as the initialization is not used.

bench4:
Tests that the code can identify strings of variables that are using each other but not outputting anything. This required a modification--for some reason the DFG had a link backwards to a node that shouldn't have existed--I'm not sure if this was a mistake with my DFG or something else, but I added a check to tell the program that only DFG links to nodes after itself count as valid.

