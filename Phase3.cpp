#include <bits/stdc++.h>
#define CH1_TS 5
#define CH2_TS 5
#define CH3_TS 2

using namespace std;

struct state
{
    char IC[4], R[4], IR[4], error[100];
    bool T;
    int SI, TI, PI;
};
int Global_Timer = 0;
int IOI = 1;
class PCB
{

public:
    int Job_id;
    int TTL, TTC, LLC, TLL, Ptr;
    int datacount, program_count;
    vector<int> CodePtr, DataPtr, OutputPtr;
    state curr_state;
    char flag;
    PCB()
    {
    }
    PCB(string card)
    {
        cout << "in PCB\n"
             << card << "\n";
        Job_id = atoi(card.substr(4, 4).c_str());
        TTL = atoi(card.substr(8, 4).c_str());
        TLL = atoi(card.substr(12, 4).c_str());
        TTC = 0;
        LLC = 0;
        datacount = 0;
        program_count = 0;
        // TLL = 0;
        CodePtr.clear();
        DataPtr.clear();
        cout << "-------------------------------------------\n";
        cout << "                    PCB\n";
        // cout << "\n-----------------------------------------" << endl;
        // cout << "\n_______________" << endl;
        cout << "\t---------------------\n";
        cout << "\t| job_id\t|  " << Job_id << "\t|" << endl;
        cout << "\t---------------------\n";
        cout << "\t| llc\t\t|  " << LLC << "\t|" << endl;
        cout << "\t---------------------\n";
        cout << "\t| ttc\t\t|  " << TTC << "\t|" << endl;
        cout << "\t---------------------\n";
        cout << "\t| ttl\t\t|  " << TTL << "\t|" << endl;
        cout << "\t---------------------\n";
        cout << "\t| tll\t\t|  " << TLL << "\t|" << endl;
        cout << "\t---------------------\n";
        cout << "\t| Data_count\t|  " << datacount << "\t|" << endl;
        cout << "\t---------------------\n";
        cout << "\t| Program_count\t|  " << program_count << "\t|" << endl;
        cout << "\t---------------------\n";
        cout << "-------------------------------------------\n";
    }
};

PCB temp, *ptr;
class SuperVisiorMem
{

public:
    queue<int> EmptyBuffers;
    queue<int> InputFullBuffers;
    queue<int> OutputFullBuffers;
    char buffer[10][40];

    SuperVisiorMem()
    {
        buffer[0][0] = 0;
        for (int i = 0; i < 10; i++)
            EmptyBuffers.push(i);
    }

    int allotEmptyBuffer()
    {
        int t;
        if (!EmptyBuffers.empty())
        {
            t = EmptyBuffers.front();
            EmptyBuffers.pop();
            return t;
        }
        else
            return -1;
    }

    void addInputBuffer(int buff_no)
    {
        InputFullBuffers.push(buff_no);
    }

    void addOutputBuffer(int buff_no)
    {
        OutputFullBuffers.push(buff_no);
    }
    void addEmptyBuffer(int buff_no)
    {
        EmptyBuffers.push(buff_no);
    }

    int returnIFB()
    {
        if (!InputFullBuffers.empty())
        {
            int t;
            t = InputFullBuffers.front();
            InputFullBuffers.pop();
            return t;
        }
        return -1;
    }
};

class DrumMem
{

public:
    char drum[1000][41];
    queue<int> EmptyTracks;
    DrumMem()
    {
        drum[0][0] = 0;
        for (int i = 0; i < 100; i++)
            EmptyTracks.push(i);
    }

    int allocateTrack()
    {
        int t;
        t = EmptyTracks.front();
        EmptyTracks.pop();
        return t;
    }

    void FreeTrack(int tno)
    {
        EmptyTracks.push(tno);
    }
};

struct CPU
{
    char IR[4];
    char R[4];
    bool T;
    int SI, PI, TI;
    char IC[2];
    char Mem[300][4];
    SuperVisiorMem sm;
    DrumMem dm;
    char error[100];
    ifstream inputCard;
    deque<PCB *> loadQ, readyQ, IOQ, terminateQ;
    vector<int> v;
    string TASK;
} c;

int no_of_pages = 0;

void clearbuffer(char buffer[][40], int n);

void loadInMain(PCB *pcb)
{
    cout << "\n---------------------------------\n";
    cout << "------------Load in Main----------\n";
    no_of_pages = 0;
    int n;
    bool code;
    char temp[40], ch;
    int b = 0;
    int k = -1, i = 0, j, currP = 0, x = 0, l, uptr, m, currentPage;
    bool new_block = true, flag = false;
    cout << "Creating a new Block" << endl;
    pcb->Ptr = -1;
    code = !pcb->CodePtr.empty();
    while (!pcb->CodePtr.empty())
    {
        cout << "while (!pcb->CodePtr.empty())" << endl;
        for (int q = 0; q < 40; ++q)
            temp[q] = '\0';

        if (!pcb->CodePtr.empty())
        {
            cout << "CodePtr is not empty" << endl;
            cout << "Adding process to drum" << endl;
            strcpy(temp, c.dm.drum[pcb->CodePtr[0]]);
            pcb->CodePtr.erase(pcb->CodePtr.begin());
        }

        if (pcb->Ptr == -1)
        {
            cout << "If memory no allocated" << endl;
            cout << "Allocating memory" << endl;
            pcb->Ptr = rand() % 30;
            if (pcb->Ptr == -1)
            {
                exit(0);
            }
            uptr = pcb->Ptr * 10;
            for (int n = 0; n < 10; n++)
                for (int o = 0; o < 4; o++)
                    c.Mem[uptr + n][o] = '#';
        }
        b = -1;
        k = -1;
        i = 0;
        while (b != 40)
        {

            b++;
            ch = temp[b];

            if (isprint(ch))
            {
                if (i % 4 == 0)
                {
                    k++;
                    if (k % 10 == 0)
                        flag = true;
                }
                if (flag)
                {
                    flag = false;
                    k = 0;
                    while (1)
                    {
                        currentPage = rand() % 30;
                        for (j = 0; j < c.v.size(); j++)
                        {
                            if (c.v[j] == currentPage)
                                break;
                        }
                        if (j >= c.v.size())
                            break;
                    }
                    no_of_pages++;
                    c.v.push_back(currentPage);
                }

                c.Mem[pcb->Ptr * 10 + no_of_pages - 1][0] = '0' + currentPage / 10;
                c.Mem[pcb->Ptr * 10 + no_of_pages - 1][1] = '0' + currentPage % 10;

                c.Mem[currentPage * 10 + k][i % 4] = ch;
                i++;
            }
            else
                break;
        }
    }
    cout << "\n---------------------------------\n";
}

void save_state(PCB *pcb)
{
    strcpy(pcb->curr_state.IC, c.IC);
    strcpy(pcb->curr_state.IR, c.IR);
    strcpy(pcb->curr_state.R, c.R);
    strcpy(pcb->curr_state.error, c.error);
    pcb->curr_state.T = c.T;
    pcb->curr_state.SI = c.SI;
    pcb->curr_state.PI = c.PI;
    pcb->curr_state.TI = c.TI;
}

void recover(PCB *pcb)
{
    strcpy(c.IC, pcb->curr_state.IC);
    strcpy(c.IR, pcb->curr_state.IR);
    strcpy(c.R, pcb->curr_state.R);
    strcpy(c.error, pcb->curr_state.error);
    c.T = pcb->curr_state.T;
    c.SI = pcb->curr_state.SI;
    c.PI = pcb->curr_state.PI;
    c.TI = pcb->curr_state.TI;
}

int PD_function(int block_no, PCB *pcb)
{
    char ch;
    int t = c.dm.allocateTrack();
    pcb->OutputPtr.push_back(t);
    pcb->LLC++;
    if (pcb->LLC > pcb->TLL)
    {
        return 2;
    }

    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 4; j++)
        {
            ch = c.Mem[block_no + i][j];
            c.dm.drum[t][i * 10 + j] = ch;
        }
    return 0;
}

int GD_function(int block_no, PCB *pcb)
{

    int k = 0;
    char line[41];
    if (pcb->datacount >= pcb->DataPtr.size())
    {
        return 1;
    }
    strcpy(line, c.dm.drum[pcb->DataPtr[pcb->datacount]]);
    pcb->datacount++;
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 4; j++)
        {
            if (k >= strlen(line))
                break;
            c.Mem[block_no + i][j] = line[k++];
        }
    return 0;
}
void printmsg2(string str)
{
    ofstream linePrinter;
    char ch;
    linePrinter.open("LinePrinter.txt", ios::app);
    linePrinter.put('\n');
    // for (int i = 0; i < str.length(); i++)
    //     linePrinter.put(str[i]);
    // linePrinter << endl;
    linePrinter.close();
    cout << str;
}

void printmsg(string str)
{
    ofstream linePrinter;
    char ch;
    linePrinter.open("LinePrinter.txt", ios::app);
    // linePrinter << "Hello" << endl;
    linePrinter << "NO ERROR:" << endl;
    for (int i = 0; i < str.length(); i++)
        linePrinter.put(str[i]);
    linePrinter << endl;
    linePrinter.close();
}
int AddressMap(char VA[], PCB *pcb)
{
    int RA;
    if (isdigit(VA[0]) && isdigit(VA[1]))
    {
        if (c.Mem[pcb->Ptr * 10 + (VA[0] - '0')][0] == '#')
        {
            c.PI = 3;
        }
        else
        {
            RA = (c.Mem[pcb->Ptr * 10 + (VA[0] - '0')][0] - '0') * 10 + (c.Mem[pcb->Ptr * 10 + (VA[0] - '0')][1] - '0');
            RA = RA * 10 + (VA[1] - '0');
        }
    }
    else
        c.PI = 2;

    return RA;
}
void Terminate(int EM)
{

    switch (EM)
    {

    case 5:
        printmsg2("Error:  NO ERROR");
        break;
    case 1:
        printmsg2("Error:  OUT OF DATA");
        break;
    case 2:
        printmsg2("Error:  LINE LIMIT EXCEEDED");
        break;
    case 3:
        printmsg2("Error:  TIME LIMIT EXCEEDED");
        break;
    case 4:
        printmsg2("Error:  OPERATION CODE ERROR");
        break;
    case 0:
        printmsg2("Error:  OPERAND ERROR");
        break;
    case 6:
        printmsg2("Error:  INVALID PAGE FAULT");
        break;
    case 7:
        printmsg2("Error:  TIME LIMIT EXCEEDED  And OPERATION CODE ERROR");
        break;
    case 8:
        printmsg2("Error:  TIME LIMIT EXCEEDED And OPERAND ERROR");
        break;
    }
    // pcb.TTL = pcb.TLL = pcb.TTC = pcb.LLC = 0;
}

int master_mode(PCB *cpcb)
{
    int block_no;
    int EM;
    char op[2];
    op[0] = c.IR[2];
    op[1] = '0';
    block_no = AddressMap(op, cpcb);
    cout << "block no:" << block_no << endl;
    if (c.TI == 0 && c.SI == 1)
    {
        EM = GD_function(block_no, cpcb);
        c.SI = 0;
        if (EM >= 0)
        {
            Terminate(EM);
            return -1;
        }
    }
    else if (c.TI == 0 && c.SI == 2)
    {
        EM = PD_function(block_no, cpcb);
        c.SI = 0;
        if (EM >= 0)
        {
            Terminate(EM);
            return -1;
        }
    }
    else if (c.TI == 0 && c.SI == 3)
    {
        c.SI = 0;
        Terminate(0);
        return -1;
    }
    else if (c.TI == 2 && c.SI == 1 && c.PI == 0)
    {
        Terminate(3);
        return -1;
    }
    else if (c.TI == 2 && c.SI == 2)
    {
        c.SI = 0;
        PD_function(block_no, cpcb);
        Terminate(3);
        return -1;
    }
    else if (c.TI == 2 && c.SI == 3)
    {
        Terminate(0);
        return -1;
    }
    else if (c.TI == 0 && c.PI == 1)
    {
        Terminate(4);
        return -1;
    }
    else if (c.TI == 0 && c.PI == 2)
    {
        Terminate(5);
        return -1;
    }

    else if (c.TI == 0 && c.PI == 3)
    {
        int currentPage, t;
        if (c.IR[0] == 'G' && c.IR[1] == 'D' || (c.IR[0] == 'S' && c.IR[1] == 'R'))
        {
            while (1)
            {
                currentPage = rand() % 30;
                for (t = 0; t < c.v.size(); t++)
                {
                    if (c.v[t] == currentPage)
                        break;
                }
                if (t >= c.v.size())
                    break;
            }
            c.v.push_back(currentPage);
            c.Mem[cpcb->Ptr * 10 + (c.IR[2] - '0')][0] = '0' + currentPage / 10;
            c.Mem[cpcb->Ptr * 10 + (c.IR[2] - '0')][1] = '0' + currentPage % 10;
            no_of_pages++;
            c.PI = 0;
            if (c.IC[1] == '0')
            {
                c.IC[1] = '9';
                c.IC[0]--;
            }
            else
                c.IC[1]--;
            cpcb->TTC--;
            cpcb->TTC++;
            if (cpcb->TTC > cpcb->TTL)
            {
                c.TI = 2;
            }
        }
        else
        {
            Terminate(6);
            return -1;
        }
    }
    return 0;
    cout << "mastermode_here\n";
}
void IOExecute()
{
    cout << "\n---------------Executing I/O----------------\n";
    int a1, a2;
    int block_no, EM, x;
    if (!c.IOQ.empty())
    {
        PCB *pcb = c.IOQ.front();
        c.IOQ.pop_front();
        cout << "Saving State of PCB" << endl;
        save_state(pcb);

        if (pcb->curr_state.TI == 0 && pcb->curr_state.SI == 1)
        {
            EM = GD_function(block_no, pcb);
            cout << "Performing GD operation" << endl;
            pcb->curr_state.SI = 0;
            if (EM >= 0)
            {
                Terminate(EM);
                x = -1;
            }
        }

        else if (pcb->curr_state.TI == 0 && pcb->curr_state.SI == 2)
        {
            EM = PD_function(block_no, pcb);
            cout << "Performing PD operation" << endl;
            pcb->curr_state.SI = 0;
            if (EM >= 0)
            {
                Terminate(EM);
                x = -1;
            }
        }
        save_state(pcb);
        if (x == -1)
        {
            cout << "Adding PCB to Terminate Queue" << endl;
            c.terminateQ.push_back(pcb);
        }
        else
        {
            cout << "I/O operation executed Successfully\nSending PCB to Ready Queue" << endl;
            c.readyQ.push_back(pcb);
        }
    }
}

void copy(char A[], char B[])
{
    for (int i = 0; i < 4; i++)
        A[i] = B[i];
}
void clearMem()
{
    for (int i = 0; i < 300; i++)
        for (int j = 0; j < 4; j++)
        {
            c.Mem[i][j] = '\0';
        }
    cout << "Clearing Memory and Queues" << endl;
    c.SI = 0;
    c.T = false;
    c.v.clear();
    c.loadQ.clear();
    c.readyQ.clear();
    c.SI = c.PI = c.TI = 0;
    // pcb.TTC = pcb.LLC = pcb.TLL = pcb.TTL = 0;
    no_of_pages = 0;
}
bool compare(char A[], char B[])
{
    bool result = true;
    for (int i = 0; i < 4; i++)
        if (A[i] != B[i])
        {
            result = false;
            break;
        }
    return result;
}

int execute(PCB *pcb)
{
    cout << "\n---------------Execution----------------\n";
    bool endprogram = false;
    int x = 0, a1, a2;
    c.IC[0] = c.IC[1] = '0';
    strcpy(c.error, "\0");
    char op[2];
    cout << endl
         << "Page Table Pointer" << pcb->Ptr;

    int RA = AddressMap(c.IC, pcb);

    cout << "mem2 " << c.Mem[RA][3];
    for (int i = 0; i < 4; ++i)
    {
        c.IR[i] = c.Mem[RA][i];
    }
    cout << "\nInstruction : ";
    for (int i = 0; i < 4; i++)
        cout << (char)c.IR[i];
    a1 = c.IR[2] - '0';
    a2 = c.IR[3] - '0';
    if (c.IC[1] == '9')
    {
        c.IC[0]++;
        c.IC[1] = '0';
    }
    else
        c.IC[1]++;

    cout << endl
         << "Real Address:"
         << " " << abs(RA) / 10 << endl;
    // if (c.IR[0] != 'H')
    // {
    //     op[0] = c.IR[2];
    //     op[1] = c.IR[3];
    //     x = AddressMap(op, pcb);
    // }
    // cout << "\nOperand:" << x;
    if (c.PI == 0) // if no page fault: START EXAMINE LOOP
    {
        if (c.IR[0] == 'H')
        {
            c.SI = 3;
            c.TI = 0;
        }

        else if (c.IR[0] == 'G' && c.IR[1] == 'D')
        {
            c.SI = 1;
            c.TI = 0;
            return 1;
        }

        else if (c.IR[0] == 'P' && c.IR[1] == 'D')
        {
            c.SI = 2;
            c.TI = 0;
            return 1;
        }

        else if (c.IR[0] == 'L' && c.IR[1] == 'R')
        {
            c.SI = 0;
            copy(c.R, c.Mem[x]);
            cout << "\nRegister: ";
            for (int i = 0; i < 4; i++)
                cout << c.R[i];
            cout << endl;
        }

        else if (c.IR[0] == 'S' && c.IR[1] == 'R')
            copy(c.Mem[x], c.R);

        else if (c.IR[0] == 'C' && c.IR[1] == 'R')
        {
            for (int i = 0; i < 4; i++)
                cout << c.Mem[x][i];
            cout << endl;
            for (int i = 0; i < 4; i++)
                cout << c.R[i];
            cout << endl;
            c.T = compare(c.R, c.Mem[x]);
        }

        else if (c.IR[0] == 'B' && c.IR[1] == 'T')
        {
            cout << "next IC :" << x << endl;
            cout << "Toggle : " << c.T << endl;
            if (c.T)
            {

                c.IC[1] = x / 10 + '0';
                c.IC[0] = x % 10 + '0';
            }
        }

        else
            c.PI = 1;

        pcb->TTC++;
        if (pcb->TTC > pcb->TTL)
            c.TI = 2;
    }
    if (c.IR[0] != 'H')
    {
        op[0] = c.IR[2];
        op[1] = c.IR[3];
        x = AddressMap(op, pcb);
    }
    if (a1 > 10 || a2 > 10)
    {
        c.PI = 2;
    }

    if (c.SI != 0 || c.PI != 0 || c.TI != 0)
        if (master_mode(pcb) == -1)
        {

            return -1;
        }

    if (strlen(c.error) > 1)
        return -1;
    cout << "\n---------------Execution ended------------------\n";
    
    cout << "\nOperand:" << x;
    return 0;
}

void channel1IR()
{

    cout << "\n-------------------------------------------\n";
    cout << "-----------------Channel 1-----------------" << endl;
    // for (int i = 0; i <= CH1_TS; i+=5)
    // {
    // cout << "Channel 1 Timer = " <<  << endl;

    if (c.inputCard.eof())
    {
        cout << "Reached End Of Line\nExiting Channel 1" << endl;
        return;
    }
    int buff_no = c.sm.allotEmptyBuffer();
    cout << "Buffer no = " << buff_no << endl;
    clearbuffer(c.sm.buffer, buff_no);
    if (buff_no == -1)
    {
        cout << "Buffers ended\nExiting Channel 1" << endl;
        return;
    }
    cout << "Reading input card" << endl;
    c.inputCard.getline(c.sm.buffer[buff_no], 41);
    cout << "Buffer value = " << c.sm.buffer[buff_no] << endl;
    cout << "Adding Inputful Buffer" << endl;
    c.sm.addInputBuffer(buff_no);
    IOI = 1;
    cout << "\n---------------Channel 1 ended ------------------\n";
    cout << "IOI = " << IOI << endl;
}

void clearbuffer(char buffer[][40], int n)
{
    // cout << "In Clear Buffer" << endl;
    for (int i = 0; i < strlen(buffer[n]); ++i)
    {
        buffer[n][i] = '\0';
    }
}
void channel2IR()
{
    cout << "\n---------------------------------\n";
    cout << "--------------Channel 2-------------" << endl;
    int temp;

    // if (c.sm.OutputFullBuffers.empty())
    // {
    cout << "OutputFullBuffer not empty" << endl;
    temp = c.sm.OutputFullBuffers.front();
    cout << "Printing Message" << endl;
    for (int i = 1; i < c.loadQ.front()->DataPtr.size(); i++)
    {
        printmsg(c.dm.drum[c.loadQ.front()->DataPtr[i]]);
        cout << c.dm.drum[c.loadQ.front()->DataPtr[i]] << endl;
    }
    // printmsg(string(c.sm.buffer));
    cout << "Clearing OutputfullBuffer" << endl;
    // clearbuffer(c.sm.buffer, temp);
    // c.sm.OutputFullBuffers.pop();
    // c.sm.EmptyBuffers.push(temp);
    // }
    IOI = 2;
    cout << "\n----------------Channel 2 ended-----------------\n";
    cout << "IOI = " << IOI << endl;
}

int channel3IR()
{

    bool code;
    PCB *ptr = new PCB;
    int n;
    IOI = 4;
    int check = 0;
    // for (int i = 0; i < CH3_TS; i++)
    // {
        // cout<<"here";
        if (c.TASK.compare("IS") == 0)
        {
            cout << " if(c.TASK.compare(IS) == 0)" << endl;
            cout << "c.TASK = " << c.TASK << endl;

            if (!c.sm.InputFullBuffers.empty())
            {
                cout << "InputFullBuffers not empty" << endl;
                string card;
                int k;
                char buffer[41];
                int bno = c.sm.returnIFB();
                cout << "Buffer no = " << bno << endl;
                for (k = 0; c.sm.buffer[bno][k] && k < 40; k++)
                {
                    buffer[k] = c.sm.buffer[bno][k];
                    cout << "buffer[k] = " << buffer[k] << endl;
                }
                buffer[k] = '\0';
                card = string(buffer);
                cout << "Card = " << card << endl;
                cout << "Adding empty buffer" << endl;
                c.sm.addEmptyBuffer(bno);
                if (card.find("$AMJ") != -1)
                {
                    cout << "Found $AMJ" << endl;
                    cout << "Initialized PCB" << endl;
                    temp = PCB(card);
                    IOI = 1;
                    // Global_Timer = 0;
                    ptr = &temp;
                    code = true;
                    c.loadQ.push_back(ptr);
                }
                else if (card.find("$DTA") != -1)
                {
                    c.loadQ.front()->flag = 'D';
                    cout << "Found $DTA" << endl;
                    code = false;
                }
                else if (card.find("$END") != -1)
                {
                    cout << "Found $END" << endl;
                    ptr = &temp;
                    // c.TASK = string("IS");
                    cout << "Adding Process to Load Queue" << endl;
                    // IOI += 4;
                    // cout << "IOI = " << IOI << endl;
                    cout << "Adding Process to Ready Queue" << endl;
                    loadInMain(ptr);
                    c.readyQ.push_back(ptr);
                    cout << "Executing the process" << execute(ptr) << endl;
                    // return 1;
                    check = 1;
                }
                else
                {
                    cout << "\n--------------------------------------\n";
                    cout << "-------------- Channel 3 --------------" << endl;
                    PCB *pcb = c.loadQ.front();

                    // c.loadQ.pop_front();
                    if (card.find("GD") != -1)
                    {
                        pcb->flag = 'P';
                        pcb->program_count++;
                        cout << "Program Count = " << pcb->program_count << endl;
                        cout << "Flag = " << pcb->flag << endl;
                        code = true;
                    }
                    else
                    {
                        pcb->datacount++;
                        cout << "Data count = " << pcb->datacount << endl;
                    }
                    int t = c.dm.allocateTrack();
                    Global_Timer++;
                    cout << "Global Timer = " << Global_Timer << endl;
                    cout << "Channel 3 Timer = " << 1 << endl;
                    strcpy(c.dm.drum[t], card.c_str());
                    if (code)
                        pcb->CodePtr.push_back(t);
                    else
                        pcb->DataPtr.push_back(t);
                    // c.loadQ.push_front(pcb);
                    Global_Timer++;
                    cout << "Global Timer = " << Global_Timer << endl;
                    cout << "Channel 3 Timer = " << 2 << endl;
                    // IOI -= 4;
                    cout << "\n-----------------Channel 3 ended---------------------\n";
                    cout << "IOI = " << IOI << endl;
                }
            }
        }
        if (!c.IOQ.empty())
        {
            IOExecute();
        }
        if (!c.readyQ.empty())
        {
            ptr = c.readyQ.front();
            n = execute(ptr);
            save_state(ptr);
            c.readyQ.pop_front();
            if (n == 0)
                c.readyQ.push_back(ptr);
            if (n == 1)
                c.IOQ.push_back(ptr);
            if (n == -1)
                c.terminateQ.push_back(ptr);
        }
    return check;
}
void simulate()
{
    int ch1_t, ch2_t, ch3_t;
    int flag = 1;
    ofstream o;
    o.open("Mem.txt", ios::out);
    for (int i = 0; i < 10; ++i) // Because we have 10 buffers in supervisory storage
    {
        clearbuffer(c.sm.buffer, i);
    }
    c.inputCard.open("ip.txt");
    while (flag)
    {
        ch1_t = ch2_t = ch3_t = 0;
        for (int j = 0; j < 5; j++)
        {
            Global_Timer++;
            cout << "Global Timer = " << Global_Timer << endl;
            ch1_t++;
            cout << "Channel 1 Timer = " << ch1_t << endl;

            if (Global_Timer % 5 == 0)
            {
                // cout << "IOI = " << IOI << endl;
                IOI++;
                channel1IR();
            }
        }
        // Global_Timer++;
        // cout << "Global Timer = " << Global_Timer << endl;
        c.TASK = string("IS");
        IOI += 4;
        // cout << "IOI = " << IOI << endl;
        int check = channel3IR();
        // Global_Timer++;
        // cout << "Global Timer = " << Global_Timer << endl;
        // channel2IR();
        if (c.inputCard.eof())
        {
            cout << "Reached End Of Line" << endl;
            flag = 0;
        }
        if (check)
        {
            cout << "size:" << c.loadQ.front()->DataPtr.size() << endl;
            for (int j = 0; j < 5; j++)
            {
                Global_Timer++;
                cout << "Global Timer = " << Global_Timer << endl;
                ch2_t++;
                cout << "Channel 2 Timer = " << ch2_t << endl;
            }
            IOI += 2;
            // cout << "IOI = " << IOI << endl;
            channel2IR();
            // cout << "For loop ended" << endl;
        }
    }
    for (int i = 0; i < 300; i++)
    {
        for (int j = 0; j < 4; j++)
            o << c.Mem[i][j] << "";
        o << endl;
    }
}
int main()
{
    simulate();
    return 0;
}