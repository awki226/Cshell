# Cshell
--------------------------------
# <h1>    TABLE OF CONTENTS
# <h3> 1.Purpose of application
# <h3> 2.Files required for compilaiton
# <h3> 3.How to use novsh
# <h3> 3.bugs/limitaitons
# <h3> 4.Refrences
--------------------------------
# <h3> 1.Purpose of novsh

novsh is used as a linux shell reads 
commands typed by the user and then 
executes them. It also allows users 
to set variables stored in the shell 
that can be used in subsequent commands.
---------------------------------
# <h3> 2.Compilation
Use the makefile given with the file:
novsh.cpp


    '''bash
    make
    ./novsh
    '''



--------------------------------
# <h3> 3.How to use novsh
*Note this was given from Dr. Finkel
*for function description

    $! anyText
The ! command introduces a comment. novsh ignores all text after the ! to the end of the line. The ! must occur as the first token on the line.
    
    $variable = value
Here, variable is any reasonable variable name (starting with a letter, continuing with letters or numbers).
    
    $newprompt prompt
Set the shell prompt to any text. The default prompt (including the space at the end) is novsh > .
    
    $dir directoryName
This command changes novsh's current directory to directoryName
    
    $listprocs
This built-in command requests that novsh list all processes running in the background.
    
    $bye
novsh exits with exit status 0.
    
    $run cmd param* [<bg>]
The user submits a run command to execute a program. 
The keyword run must appear as the first token on the command line, followed by the command to be executed. 
Here, cmd is a token that specifies the filename of the program the user wants to execute. 
It is followed by 0 or more tokens specifying parameters. 
The optional <bg> at the end specifies that the program should run in the background, 
that is, novsh should immediately prompt for and accept the next command. 
If <bg> is not present, novsh should wait for the program it starts to finish before it prompts for and accepts the next command.

    $assignto variable cmd param*
The assignto command executes the program cmd along with its parameters,
The assignto command does not allow a <bg> option.
    
----------------------------------
# <h3> 4.bugs/limitations

assignto only works with one parameters and fails
if parameters are greater than 1.


----------------------------------
# <h3> 5.References
reference: https://en.cppreference.com/w/cpp/utility/program/getenv

