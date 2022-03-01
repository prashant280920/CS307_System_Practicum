#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <signal.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>


using namespace std;

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
#define DIV 1048576
#define WIDTH 7

#endif

map<string, string> environmentVariables;

vector<pair<string, string>> commands_Table = {
    {"clr", "Clear the screen"},
    {"pause", "pause screen"},
    {"help", "help"},
    {"quit", "Quit the shell"},
    {"history", "Display the list of previously executed commands, even on shell restart."},
    {"environ", "List all the environment strings of the current shell and the bash shell"},
    {"pwd", "present working directory"},
    {"cd", "change directory"},
    {"dir", "list content of directory"},
    {"echo", "display a line of text"}};

vector<string> splitString(string command)
{
    vector<string> wordList;
    string word = "";
    for (auto x : command)
    {
        if (x == ' ')
        {
            wordList.push_back(word);
            word = "";
        }
        else
            word = word + x;
    }
    wordList.push_back(word);
    return wordList;
}

class ALLCommands
{
public:
    void clear_screen()
    {
        if (!OS_Windows)
            system("clear");
        else
            system("cls");
    }

    void pause_cmd()
    {
        cout << "Execution is paused! :)\nPress enter to continue!\n";
        while (cin.get() != '\n')
            ;
    }
    void help()
    {

        for (auto x : commands_Table)
        {
            cout << x.first << " - " << x.second << "\n";
        }
    }

    void quit_shell()
    {
        exit(1);
    }

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
        }
    }

    string getPWD()
    {
        char tmp[256];
        getcwd(tmp, 256);
        return string(tmp);
    }

    void printEnvVariables()
    {
        for (auto x : environmentVariables)
        {
            cout << x.first << "=" << x.second << "\n";
        }
    }

    void changeDirectory(vector<string> arg)
    {

        int arg_length = arg.size() - 1;
        if (arg_length > 1)
            cout << "shell: cd: too many arguments";
        else if (arg_length == 1)
        {
            if (chdir(arg[1].c_str()) != 0)
                perror("Error: ");
            else
                environmentVariables["PWD"] = getPWD();
        }
    }

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

    void printDirContent(string path)
    {
        DIR *dir;
        struct dirent *diread;

        if ((dir = opendir(path.c_str())) != nullptr)
        {
            while ((diread = readdir(dir)) != nullptr)
            {
                cout << diread->d_name << "\n";
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

    void echo(vector<string> args)
    {
        int i = 1, arg_len = args.size();
        for (; i < arg_len; i++)
            cout << args[i] << " ";
        cout << "\n";
    }
};

bool command_Exist(string commad)
{
    for (auto x : commands_Table)
        if (commad == x.first)
            return true;
    return false;
}
ALLCommands func;
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

int main(int argc, char **argv, char **envp)
{
    stroringAllEnvVariables(envp);
    if (argc == 2)
    {
        if (freopen(argv[1], "r", stdin) == NULL)
        {
            perror("error: ");
            return 0;
        }
    }
    // char *argo[]={NULL};
    // execv("./try", argo);
    signal(SIGINT, SIG_IGN);
    string command;
    if (argc == 1)
        cout << "\033[1;92mHacker@root:>>\033[0m ";
    while (getline(cin, command))
    {
        ofstream historyFile;

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
                if (execv(splitCommand[0].c_str(), arg)==-1){
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
            cout << "\033[1;92mHacker@root:>>\033[0m ";
    }
    return 0;
}