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
3. The interpreter reads commands from `"input.txt"` line by line. Use maximum one command per line in the source code (`"input.txt"`). Do not place a comment and a non-comment command in the same line.

That's about it. Have fun using LCL! 😀
