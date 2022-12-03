#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void print_memory();
void get_in_buffer();
char Main_Memory[100][4];
char IR[4];
int SI = 3;
int IC = 0;
char R[4];
bool C;
char buffer[40];
int flag = 1;

ifstream MyReadFile;

fstream fio;

void get_in_buffer(){
    for (int i = 0; i < 40; i++)
    {
        buffer[i] = '#';
    }
    MyReadFile.getline(buffer, 40+1);
    for (int i = 0; i < 40; i++)
    {
        if (buffer[i] == '#')
        {
            buffer[i - 1] = '#';
            break;
        }
    }
    // fio << "buffer: ";
    //     for (int i = 0; i < 40; i++)
    //     {
    //         fio << buffer[i] << " ";
    //     }
    //     fio << endl;
}

void Read()
{
    IR[3] = 0;
    get_in_buffer();
    // fio << "buffer: ";
    //     for (int i = 0; i < 40; i++)
    //     {
    //         fio << buffer[i] << " ";
    //     }
    //     fio<<endl;
    int loc = (IR[2] - 48) * 10;
    for (int i = 0; i < 40; i++)
    {
        Main_Memory[loc + i / 4][i % 4] = buffer[i];
    }
}

void Write()
{
    // IR[3]=0;
    int loc = (IR[2] - 48) * 10 + (IR[3] - 48);
    int i = 0;
    while (Main_Memory[loc + i / 4][i % 4] != '#')
    {
        fio << Main_Memory[loc + i / 4][i % 4];
        i++;
    }
    fio << endl;
    fio << endl;
}

void Terminate()
{
    flag = 0;
}

void MOS()
{
    if (SI == 1)
    {
        // fio<<"Hello6"<<endl;
        Read();
        // fio<<"Hello8"<<endl;
        SI = 3;
    }
    else if (SI == 2)
    {
        Write();
        SI = 3;
    }
    else if (SI == 3)
    {
        Terminate();
    }
}

void ExecuteUserProgram()
{
    // fio<<"Hello3"<<endl;
    while (flag)
    {
        IR[0] = Main_Memory[IC][0];
        IR[1] = Main_Memory[IC][1];
        IR[2] = Main_Memory[IC][2];
        IR[3] = Main_Memory[IC][3];
        IC++;
        // fio<<IR[0]<<IR[1]<<IR[2]<<IR[3]<<endl;
        if (IR[0] == 'L' && IR[1] == 'R')
        {
            int loc = (IR[2] - 48) * 10 + (IR[3] - 48);
            R[0] = Main_Memory[loc][0];
            R[1] = Main_Memory[loc][1];
            R[2] = Main_Memory[loc][2];
            R[3] = Main_Memory[loc][3];
        }
        else if (IR[0] == 'S' && IR[1] == 'R')
        {
            int loc = (IR[2] - 48) * 10 + (IR[3] - 48);
            Main_Memory[loc][0] = R[0];
            Main_Memory[loc][1] = R[1];
            Main_Memory[loc][2] = R[2];
            Main_Memory[loc][3] = R[3];
        }
        else if (IR[0] == 'C' && IR[1] == 'R')
        {
            int loc = (IR[2] - 48) * 10 + (IR[3] - 48);
            int flag1 = 1;
            for (int i = 0; i < 4; i++)
            {
                if (Main_Memory[loc][i] != R[i])
                {
                    flag1 = 0;
                    break;
                }
            }
            if (flag1)
            {
                C = 1;
            }
            else
            {
                C = 0;
            }
        }
        else if (IR[0] == 'B' && IR[1] == 'T' && C == 1)
        {
            int loc = (IR[2] - 48) * 10 + (IR[3] - 48);
            IC = loc;
            IR[0] = Main_Memory[IC][0];
            IR[1] = Main_Memory[IC][1];
            IR[2] = Main_Memory[IC][2];
            IR[3] = Main_Memory[IC][3];
            continue;
        }
        else if (IR[0] == 'G' && IR[1] == 'D')
        {
            SI = 1;
            // fio<<"Hello5"<<endl;
            MOS();
        }
        else if (IR[0] == 'P' && IR[1] == 'D')
        {
            SI = 2;
            MOS();
        }
        else if (IR[0] == 'H')
        {
            SI = 3;
            MOS();
        }
    }
}

void startexecution()
{
    IC = 0;
    // fio<<"Hello2"<<endl;
    ExecuteUserProgram();
}

void init()
{
    for (int i = 0; i < 100; i++)
    {
        Main_Memory[i][0] = '#';
        Main_Memory[i][1] = '#';
        Main_Memory[i][2] = '#';
        Main_Memory[i][3] = '#';
    }
    for (int i = 0; i < 40; i++)
    {
        buffer[i] = '#';
    }
}

void load()
{
    int m = 0;
    // string buffer;
    MyReadFile.open("input.txt");
    fio.open("output.txt", ios::trunc | ios::out | ios::in);
    while (MyReadFile)
    {
        get_in_buffer();
        if (buffer[0] == '$')
        {
            if (buffer[1] == 'D' && buffer[2] == 'T' && buffer[3] == 'A')
            {
                // Start
                // fio<<"Hello1"<<endl;
                startexecution();
            }
            else if (buffer[1] == 'A' && buffer[2] == 'M' && buffer[3] == 'J')
            {
                continue;
            }
            else if (buffer[1] == 'E' && buffer[2] == 'N' && buffer[3] == 'D')
            {
                m = 0;
                print_memory();
                flag = 1;
                init();
                continue;
            }
            else
            {
                fio << "Wrong Instruction" << endl;
                break;
            }
        }
        else
        {
            if (m == 100)
            {
                fio << "Memory Limit Excedded" << endl;
                break;
            }
            else
            {
                int x = m;
                for (int i = 0; i < 40; i++)
                {
                    if (buffer[i] != 'H')
                    {
                        Main_Memory[x][0] = buffer[i];
                        Main_Memory[x][1] = buffer[i + 1];
                        Main_Memory[x][2] = buffer[i + 2];
                        Main_Memory[x][3] = buffer[i + 3];
                        x++;
                        i += 3;
                    }
                    else if (buffer[i] == 'H')
                    {
                        Main_Memory[x][0] = buffer[i];
                        Main_Memory[x][1] = '#';
                        Main_Memory[x][2] = '#';
                        Main_Memory[x][3] = '#';
                        x++;
                        // i+=3;
                    }
                }
                //print_memory();
                m += 10;
            }
        }
        for (int i = 0; i < 100; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (Main_Memory[i][j] == '\0')
                {
                    Main_Memory[i][j] = '#';
                }
            }
        }
    }
}

void print_memory()
{
    for (int i = 0; i < 100; i++)
    {
        fio << i << " ---> ";
        for (int j = 0; j < 4; j++)
        {
            fio << Main_Memory[i][j] << " ";
        }
        fio << endl;
    }
    fio << endl
         << endl
         << endl;
}

int main()
{
    init();
    load();
    return 0;
}