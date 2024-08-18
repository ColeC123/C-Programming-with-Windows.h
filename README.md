#Snake Game in C
Snake Game Project using C and windows library functions. This unfortunately means this program will only run on windows machines, and I have included the exe file for easy downloading.

##Download instructs for MinGW-w64 GCC
If that fails you can compile the program yourself using GCC. [MinGW-w64 includes GCC and other langauge compilers](https://github.com/niXman/mingw-builds-binaries/releases/tag/13.2.0-rt_v11-rev1).
Specifically, I recommend downloding the "x86_64-13.2.0-release-posix-seh-ucrt-rt_v11-rev1.7z", which is the version of MinGW-w64 that I use on my machine.

  1. Once downloaded, extract the contents of the file, and move the folder inside of it named "mingw64" to the directory of your choosing. For instance, you could move it to the C:\ root directory.
     This would give an example file path of "C:\mingw64". I would also just check now that there is a folder inside of mingw64 named "bin".

  2. Once this has been completed, go to the search bar and type in "Edit the System Environment Variables". In the popup window produced, look at the bottom for a button titled
     "Environment Variables..." and click it.

  3. Next, look for where the new popup window says "System variables". Underneath "Ststem Variables" look for "Path" on the left side. Click on "Path" and select edit.

  4. Then select "New" on the right side of the new popup, and type in the full directory path to mingw64\bin. Using the earlier example it would be "C:\mingw64\bin".

  5. Now press the button "Ok" at the bottom of each popup. To see if the installation worked, open command prompt or Windows Powershell and type the command "gcc --version".

  6. If the command doesn't throw an error and returns the version of gcc installed, then everything has been installed correctly.

##Compilation Instructions For Program Files Using GCC
To compile the project using gcc, first download all of the ".c" and ".h" files, and move them into a folder of your choice.

  1. Once you have all of the downloaded files in one folder, open command prompt or windows power shell and type in cd + "path to folder containing files". Example: cd "C:\Users\Bob\Program Files"
     This command allows you to access the files contained in the folder you cd (change directory) into.

  2. Now that you have access to the program files, type in the command "gcc test.c -L C:\Windows\System32 -l winmm -o test". This will output an executable file that can be run. Enjoy the program!
     
     The above commands tells gcc to compile the test.c program "gcc test.c", include the path to System32 for library linking purposes "-L C:\Windows\System32", link the DLL (Dynamic Link Library)
     named winmm from inside C:\Windows\System32 to the code written in test.c "-l winmm", and output an executable file named test "-o test".

     The reason I need to link to winmm.dll is because it contains the definitions for the functions declared in "timeapi.h", which means that without winmm.dll my program doesn't know what the
     functions declared in "timeapi.h" do. 
