#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
using namespace std;

class Log2Hist
{
private:
    int number_of_bins;
    int *bins;
    int *boundary_values;

public:
    Log2Hist(int n)
    {
        this->number_of_bins = n;
        this->bins = new int[n];
        this->boundary_values = new int[n];
        memset(this->bins, 0, sizeof(int) * n);
        memset(this->boundary_values, 0, sizeof(int) * n);

        int i;
        for (i = 0; i < n; i++)
        {
            this->boundary_values[i] = (1 << (i + 1));
        }
    }
    ~Log2Hist()
    {
        delete[] bins;
        delete[] boundary_values;
    }
    void Add(float value)
    {
        int i;
        for (i = 0; i < this->number_of_bins; i++)
        {
            if (value < this->boundary_values[i])
            {
                this->bins[i]++;
                return;
            }
        }
    }
    std::string Print()
    {
        int i = 0;
        for (i = 0; i < number_of_bins; i++)
        {
            cout << boundary_values[i] << endl;
        }
        cout << "dsadsadad" << endl;

        for (i = 0; i < number_of_bins; i++)
        {
            if (i == 0)
            {
                cout << 0 << "\t->\t" << boundary_values[i] - 1 << "\t\t" << bins[i] << endl;
            }
            else
            {
                cout << boundary_values[i] / 2 << "\t->\t" << boundary_values[i] - 1 << "\t\t" << bins[i] << endl;
            }

            //cout << bins[i] << endl;
        }

        return "";
    }
};

int main(void)
{
    Log2Hist hist(5);
    hist.Add(1);
    hist.Add(2);
    hist.Add(3);
    hist.Add(4);
    hist.Add(5);
    hist.Add(6);
    hist.Add(7);
    hist.Add(8);

    hist.Print();
}