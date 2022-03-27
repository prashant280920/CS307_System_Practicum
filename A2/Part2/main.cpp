#include <bits/stdc++.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>

using namespace std;

// file discriptor for pipe created between child and parent
int pipe_discriptor1[2];
int pipe_discriptor3[2];

// For CPU utilization get_cpu_times functin created
// Reference - https://rosettacode.org/wiki/Linux_CPU_utilization#C
std::vector<size_t> get_cpu_times()
{
    std::ifstream proc_stat("/proc/stat");
    proc_stat.ignore(5, ' '); // Skip the 'cpu' prefix.
    std::vector<size_t> times;
    for (size_t time; proc_stat >> time; times.push_back(time))
        ;
    return times;
}

bool get_cpu_times(size_t &idle_time, size_t &total_time)
{
    const std::vector<size_t> cpu_times = get_cpu_times();
    if (cpu_times.size() < 4)
        return false;
    idle_time = cpu_times[3];
    total_time = std::accumulate(cpu_times.begin(), cpu_times.end(), 0);
    return true;
}

// thread function for reading cpu utilization through pipe
void cpu_time_util()
{
    float cpu_util_value;
    while (true)
    {
        // for parent process to read the cpu utilization using pipe that come from 4th Child process
        if (read(pipe_discriptor3[0], &cpu_util_value, sizeof(float)) != 0)
        {
            cout << cpu_util_value << '%' << "\n";
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        cout << "expected 4 arguments";
        return 0;
    }
    string mapping_filename = argv[1];
    string espeak_filename = argv[2];
    string copping_filename = argv[3];
    string new_copy_filename = argv[4];

    if (pipe(pipe_discriptor1) == -1)
    {
        perror("pipe failure");
        exit(0);
    }
    if (pipe(pipe_discriptor3) == -1)
    {
        perror("pipe failure");
        exit(0);
    }

    int child_1 = fork();

    if (child_1 == -1)
    {
        perror("Child_1 not created");
        cout << "\n";
        exit(0);
    }
    else if (child_1 == 0)
    {

        int child_2 = fork();

        if (child_2 == -1)
        {
            perror("Child_2 not created");
            cout << "\n";
            exit(0);
        }
        else if (child_2 == 0)
        {

            int child_3 = fork();

            if (child_3 == -1)
            {
                perror("Child_3 not created");
                cout << "\n";
                exit(0);
            }
            else if (child_3 == 0)
            {

                int child_4 = fork();
                if (child_4 == -1)
                {
                    perror("Child_4 not created");
                    cout << "\n";
                    exit(0);
                }
                else if (child_4 == 0)
                {
                    // C1 process
                    map<char, char> mapping_table;
                    ifstream myfile;
                    myfile.open(mapping_filename, ios::in);
                    string line;
                    if (myfile.is_open())
                    {
                        while (getline(myfile, line))
                        {
                            mapping_table[line[0]] = line[2];
                        }
                    }
                    else
                    {
                        perror("file opening error");
                        exit(0);
                    }

                    myfile.close();
                    char array[3];
                    char char_input;
                    while (1)
                    {
                        cin >> char_input;
                        array[0] = char_input;
                        array[1] = ' ';
                        array[2] = mapping_table[char_input];
                        write(pipe_discriptor1[1], array, 3);
                    }
                }
                else
                {
                    // C2 process
                    ifstream espeak_file_discriptor;
                    espeak_file_discriptor.open(espeak_filename, ios::in);
                    string line;
                    if (espeak_file_discriptor.is_open())
                    {
                        while (getline(espeak_file_discriptor, line))
                        {

                            string phrase = line;
                            string speak = "espeak \"" + phrase + "\"";
                            const char *espeak_command = speak.c_str();
                            system(espeak_command);
                        }
                    }
                    else
                    {
                        perror("file opening error");
                        exit(0);
                    }

                    espeak_file_discriptor.close();

                    int *status4;
                    wait(status4);
                    exit(0);
                }
            }
            else
            {
                // C3 process
                ifstream copy_temp_file;
                copy_temp_file.open(copping_filename, ios::in);

                ofstream output_file;
                output_file.open(new_copy_filename, ios::app);
                string line;
                while (getline(copy_temp_file, line))
                {
                    output_file << line << "\n";
                }

                copy_temp_file.close();
                output_file.close();
                int *status3;
                wait(status3);
                exit(0);
            }
        }
        else
        {
            // C2 process
            // this cpu utilization code for printing CPU util is taken from same refernce that we give below
            // Reference - https://rosettacode.org/wiki/Linux_CPU_utilization#C
            size_t previous_idle_time = 0, previous_total_time = 0;
            for (size_t idle_time, total_time; get_cpu_times(idle_time, total_time); sleep(2))
            {
                const float idle_time_delta = idle_time - previous_idle_time;
                const float total_time_delta = total_time - previous_total_time;
                const float utilization = 100.0 * (1.0 - idle_time_delta / total_time_delta);

                // using pipe for sending CPU utilization to parent process
                write(pipe_discriptor3[1], &utilization, sizeof(utilization));

                previous_idle_time = idle_time;
                previous_total_time = total_time;
            }

            int *status2;
            wait(status2);
            exit(0);
        }
    }

    else
    {
        char result[3];
        float util;
        // using thread to parrally read the message from child4 process using pipe
        thread cpu_util_thread = thread(cpu_time_util);
        while (1)
        {
            // using pipe to read message from child 1 process
            if (read(pipe_discriptor1[0], result, 3) == 0)
            {
                continue;
            }
            cout << result << "\n";
        }

        cpu_util_thread.join();
        int *status1;
        wait(status1);
    }

    return 0;
}