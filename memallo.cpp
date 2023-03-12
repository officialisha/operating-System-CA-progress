#include <bits/stdc++.h>
using namespace std;

void firstFit(vector<int> &blockSize,vector<int> &processSize)
{

    vector<int> allocation(processSize.size(), -1);

    for (int i = 0; i < processSize.size(); i++)
    {
        for (int j = 0; j < blockSize.size(); j++)
        {
            if (blockSize[j] >= processSize[i])
            {

                allocation[i] = j;

                blockSize[j] -= processSize[i];

                break;
            }
        }
    }

    cout << "\nProcess No.\tProcess Size\tBlock no.\n";
    for (int i = 0; i < processSize.size(); i++)
    {
        cout << " " << i + 1 << "\t\t"
             << processSize[i] << "\t\t";
        if (allocation[i] != -1)
            cout << allocation[i] + 1;
        else
            cout << "Not Allocated";
        cout << endl;
    }
}

int main()
{
    vector<int> blockSize = {100, 500, 200, 300, 600};
    vector<int> processSize = {212, 417, 112, 426};

    firstFit(blockSize, processSize);

    return 0;
}
