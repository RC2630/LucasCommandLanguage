# LucasCommandLanguage

This is the interpreter for the LCL (Lucas Command Language) programming language. To get started with the language, follow these steps:
1. Download a copy of this project from GitHub and extract it, and open it in any C++ IDE of your choice (or anywhere else that can run C++ code).
2. Note the location of the file called `"input.txt"`. This is currently in the GitHub repository at the relative path `"LucasCommandLanguage/LucasCommandLanguage/inputFile/input.txt"`. It is important for you to see where this file is, relative to the entire project's location, and keep note of this path.
3. Open the file `"LucasCommandLanguage/LucasCommandLanguage/Interpreter.cpp"` (where the `main()` function is). Near the top, there is a constant called `INPUT_FILE`. Currently, this constant is declared as:
<br>`const string INPUT_FILE = "../LucasCommandLanguage/inputFile/input.txt";`
4. Change the value of this constant to be the relative path that you kept note of in step 2, such that when you run the program in your specific environment, this filename can be understood and the corresponding file can be successfully opened and read from.
5. Compile and run the program. It should run successfully, and print out some green text telling you that the source code is empty.
6. Follow the instructions from the message that was printed from step 5. Add the single command `/help` to `"input.txt"`. Then save the file and re-run the program. Now you should see a help directory. Go from there to learn how to create programs in LCL.
7. Have fun learning and using LCL! Remember to save `"input.txt"` every time before you run the interpreter!

----------------------------------------------------------------------------------------------------------------------------

Special notes:
1. To save your file somewhere else, simply copy and paste `"input.txt"` to a different location (do NOT cut and paste or drag the file there). Make sure a copy of `"input.txt"` still exists inside the interpreter's relative path. Then, in your new location, feel free to rename the file to whatever makes the most sense.
2. To run files other than the current `"input.txt"` inside the interpreter's relative path, first clear everything currently in `"input.txt"`, then simply copy the contents of the other file into `"input.txt"` and run it normally.
3. The interpreter reads commands from `"input.txt"` line by line. Use maximum one command per line in the source code (`"input.txt"`) UNLESS you use semicolons to separate multiple commands on the same line.

That's about it. Have fun using LCL! 😀

----------------------------------------------------------------------------------------------------------------------------

## Version History

v3.2 - Added support for nested structs and objects. No longer is the case where structs/objects can only contain primitive fields! Full support for object copying, struct/object deletion, inheritance, etc. are available for nested structs/objects, and there is guaranteed full backwards compatibility with old LCL programs. A new command (`/debug`) is also added, as well as more example files.

v3.1 - Added default constructors, copy constructors, copy assignment, inheritance, and the ability to get the datatype (these are all utility operations for objects or structs). More example files are included, as well.

v3.0 - BIG UPDATE! Added structs and objects, and commands related to them. Structs can only hold fields of primitive types at the moment, but that may change somewhere down the line! Example files for structs are provided in the `examples` folder, as usual.

v2.4 - It's been a while since the last update. This one is not huge, but should increase code quality and ease of code writing. Additions include support for indentation, inline comments, and multiple commands on a single line. Also, there are new commands `/space` and `/tab` for displaying special characters, which can be useful in particular situations. New example file added in the `examples` folder, as usual.

v2.3.1 - Tiny update. Attempted to improve efficiency (especially that of loops) by making variable and block finding/searching go from back to front (instead of from front to back). This is because more recently-created variables and blocks tend to be used more than older ones, and also because "temporary" blocks are always created at the end of the list of blocks maintained by the interpreter. This is supposed to ensure a lot faster finding/searching of these temporary blocks, especially when there is a long nested loop situation. However, the improvements (empirically) don't seem that great, so I will keep working on it.

v2.3 - Added `/gettype`, various assertion commands, and a simplified way of calling blocks (in addition to some small tweaks to existing features)

v2.2 - Added "memory management", which allows the programmer to explicitly delete variables and blocks, as well as clean up temporary blocks created by the program

v2.1 - Added internal name processing for `/while` loops, which fixed a bug that made different blocks share the same block name

v2.0 - BIG UPDATE! Added blocks (functions without arguments), control flow (`/if`, `/ifvar`, `/loop`, `/for`, `/while`), and a ton of example files that you can copy to `"input.txt"` and run! You can find the example files in the `examples` folder. Also, there is a practice problem (`"reverseStringProblem.txt"`) in the `examples` folder, so if you feel confident in LCL already, you can try that out 😀. The concise solution (without comments) and explanatory solution (with comments) are both in the `examples` folder as well - they are `"reverseStringSolution.txt"` and `"reverseStringExplanation.txt"`, respectively.

v1.2 - Added escaped output and console-based user input

v1.1 - Added relational, logical, casting, and string-related commands

v1.0 - Initial version (includes variables and simple arithmetic)
