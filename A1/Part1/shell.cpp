// Our Hacker Shell
// System Practicum - A1 - Part1
// Team Details:
// Prashant Kumar  -  B19101
// Ravi Kumar      -  B19191
// Saloni Patidar  -  B19111
// Gaurav Sahitya  -  B19083
// Sagar Taffardar -  B19110
// Shubham Sourav  -  B19222

// Including all necessay library
#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <signal.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>

// library separate for windows and linux
#ifdef __unix__ /* __unix__ is usually defined by compilers targeting Unix systems */

#define OS_Windows 0
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#elif defined(_WIN32) || defined(WIN32) /* _Win32 is usually defined by compilers targeting 32 or   64 bit Windows systems */

#define OS_Windows 1
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#endif

using namespace std;

// storing environment Variables in key value pair
map<string, string> environmentVariables;

// list of all commands and their description
vector<pair<string, string>> commands_Table = {
    {"clr", "Clear the terminal screen"},
    {"pause", "Pause operations of the shell until 'Enter' is pressed."},
    {"help", "Display user manual"},
    {"quit", "Quit the shell"},
    {"history", "Display the list of previously executed commands, even on shell restart."},
    {"pwd", "GIve the current working directory"},
    {"cd", "change directory"},
    {"dir", "list content of directory"},
    {"echo", "display a line of text"},
    {"environ", "List all the environment strings of the current shell and the bash shell"}};

// Class contain execution of all commands
class ALLCommands
{
public:
    // 1. clr
    // function for clear the terminal screen
    void clear_screen()
    {
        printf("\e[H\e[2J\e[3J");
    }

    // 2. pause
    // function for Pausing operations of the shell until ‘Enter’ is pressed.
    void pause_cmd()
    {
        cout << "Execution is paused! :)\nPress enter to continue!\n";
        while (cin.get() != '\n');
    }

    // 3. help
    // function for dispaying user manual and all commands info
    void help()
    {
        for (auto x : commands_Table)
            cout << x.first << " - " << x.second << "\n";
    }

    // 4. quit
    // function that quit the shell
    void quit_shell()
    {
        exit(1); // exit() which cause process termination. Include in <stdlib.h>
    }

    // 5. history
    // Display the list of previously executed commands, even on shell restart
    void print_cmd_history()
    {
        ifstream read_history("history.txt");
        string line;
        int line_no = 0;
        if (read_history.is_open())
        {
            while (read_history)
            {
                getline(read_history, line);
                cout << line_no << " " << line << endl;
                line_no++;
            }
            read_history.close();
        }else perror("File open error");
    }

    // 6. pwd
    // give the present working directory
    string getPWD()
    {
        char tmp[1000];
        if (getcwd(tmp, sizeof(tmp)) == nullptr)
            perror("get directory path error");
        // give the absolute pathname of CWD. Include in <unistd.h>
        return string(tmp);
    }

    // 7. cd <directory>
    // Change the current default directory to <directory>. If the <directory> argument is
    // not present, report the current directory. If the directory doesn’t exist an
    // appropriate error should be reported.
    void changeDirectory(vector<string> arg)
    {
        int arg_length = arg.size() - 1;
        if (arg_length > 1)
            cout << "shell: cd: too many arguments\n";
        else if (arg_length == 1)
        {
            if (chdir(arg[1].c_str()) != 0){ // chdir include in <unistd.h>
                perror("Error");
            }   
            else
                environmentVariables["PWD"] = this->getPWD();
        }
    } 

    // 8. dir <directory>
    // list content of directory
    void listDirContent(vector<string> args)
    {
        int arg_len = args.size() - 1;
        if (arg_len == 0)
            this->printDirContent(".");
        else
        {
            for (int i = 1; i < arg_len + 1; i++)
            {
                cout << args[i] << "\n";
                this->printDirContent(args[i]);
                cout << "\n";
            }
        }
    }

    // function for listing content of particular path directory
    void printDirContent(string path)
    {
        DIR *dir;
        struct dirent *diread;

        if ((dir = opendir(path.c_str())) != nullptr)
        {
            while ((diread = readdir(dir)) != nullptr)
            {
                cout << diread->d_name << "\n";
                // cout << diread->d_type<<"\n";
            }
            closedir(dir);
        }
        else
        {
            string error = "cannot access " + path + ": ";
            perror(error.c_str());
            // return EXIT_FAILURE;
        }
    }

    // 9. environ
    // List all the environment strings of the current shell and the bash shell
    void printEnvVariables()
    {
        for (auto x : environmentVariables)
        {
            cout << x.first << "=" << x.second << "\n";
        }
    }

    // 10. echo <comment>
    // Display <comment> on the display followed by a new line. Multiple spaces/tabs
    // should be reduced to a single space.
    void echo(vector<string> args)
    {
        int i = 1, arg_len = args.size();
        for (; i < arg_len; i++)
            cout << args[i] << " ";
        cout << "\n";
    }
};

// to check that the command exist or not
bool command_Exist(string commad)
{
    for (auto x : commands_Table)
        if (commad == x.first)
            return true;
    return false;
}

// function for splitting commands
vector<string> splitString(string command)
{
    vector<string> wordList;
    string word = "";
    for (auto x : command)
    {
        if (x == ' ')
        {
            if (word != "")
                wordList.push_back(word);
            word = "";
        }
        else
            word = word + x;
    }
    if (word != "")
        wordList.push_back(word);
    return wordList;
}

ALLCommands func;
// calling particular function for respective commands
void run_command(vector<string> splitCommand)
{

    string cmd = splitCommand[0];
    if (cmd == "clr")
        func.clear_screen();
    else if (cmd == "pause")
        func.pause_cmd();
    else if (cmd == "help")
        func.help();
    else if (cmd == "quit")
        func.quit_shell();
    else if (cmd == "history")
        func.print_cmd_history();
    else if (cmd == "environ")
        func.printEnvVariables();
    else if (cmd == "pwd")
        cout << func.getPWD() << "\n";
    else if (cmd == "cd")
        func.changeDirectory(splitCommand);
    else if (cmd == "dir")
        func.listDirContent(splitCommand);
    else if (cmd == "echo")
        func.echo(splitCommand);
}


// func for storing all env in environmentVariables
void stroringAllEnvVariables(char **envp)
{
    for (char **env = envp; *env != 0; env++)
    {
        string key = "", value = "";
        char *thisEnv = *env;
        string str = string(thisEnv);
        int i = 0, n = str.length();
        for (i = 0; i < n; i++)
        {
            if (str[i] == '=')
                break;
        }
        key = str.substr(0, i);
        value = str.substr(i + 1, str.length() - i - 1);
        environmentVariables[key] = value;
    }
}

// setting SHELL env equal to our shell path 
void setting_shell_Environ()
{
    char buf[1000];
    if (readlink("/proc/self/exe", buf, sizeof(buf)) < 0)
        perror("readlink() error: not able to set shell env");
    else
        environmentVariables["SHELL"] = buf;
}


int main(int argc, char **argv, char **envp)
{
    // storing all env
    stroringAllEnvVariables(envp);
    // set SHELL env
    setting_shell_Environ();
    
    // for executing batchfile
    if (argc == 2)
    {
        if (freopen(argv[1], "r", stdin) == NULL)
        {
            perror("Error");
            return 0;
        }
    }

    // ignoring Ctrl+C intrupt signal 
    signal(SIGINT, SIG_IGN);
    string command;
    if (argc == 1)
        cout << "\033[1;31mHacker@root:>>\033[0m ";
    
    // looping shell
    while (getline(cin, command))
    {
        if(command[0]=='#' || command=="") continue;
        
        ofstream historyFile;
        // storing history of all command in history file
        historyFile.open("history.txt", ios::app);
        historyFile << command << "\n";
        historyFile.close();

        vector<string> splitCommand = splitString(command);

        if (command_Exist(splitCommand[0]))
            run_command(splitCommand);
        else
        {
            int status;
            pid_t pid = fork();
            if (pid == -1)
            {
                // pid == -1 means error occured
                printf("can't fork, error occured\n");
                exit(EXIT_FAILURE);
            }
            else if (pid == 0)
            {
                /* Child Process */

                /* Allow signal's default behaviour for process */
                signal(SIGINT, SIG_DFL);
                char *arg[] = {NULL};
                if (execv(splitCommand[0].c_str(), arg) == -1)
                {
                    cout << splitCommand[0] << ": command not found\n";
                };
                exit(0);
            }
            else
            {
                wait(&status);
            }
        }
        if (argc == 1)
            cout << "\033[1;31mHacker@root:>>\033[0m ";
    }
    return 0;
}