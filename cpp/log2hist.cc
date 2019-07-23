#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <cmath>
using namespace std;

class Log2Hist
{
private:
    int number_of_bins;
    int *bins;
    int *boundary_values;
    int count_digits(long long n)
    {
        return (int)floor(log10(n) + 1);
    }
    string generate_stars(int n)
    {
        int i = 0;
        string stars = "";
        for (i = 0; i < n; i++)
        {
            stars += "*";
        }
        return stars;
    }

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

        int max_left_boundary_count = 1;
        int max_right_boundary_count = 1;
        int max_value_count = 1;
        int max_stars = 0;
        int max_bins = 0;
        int total_stars = 38;
        int total_sum_of_bins = 0;

        for (i = 0; i < number_of_bins; i++)
        {
            max_left_boundary_count = max(max_left_boundary_count, count_digits(boundary_values[i] / 2));
            max_right_boundary_count = max(max_right_boundary_count, count_digits(boundary_values[i] - 1));
            max_value_count = max(max_value_count, count_digits(bins[i]));
            max_bins = max(max_bins, bins[i]);
            total_sum_of_bins += bins[i];
        }

        max_stars = max_bins * total_stars / total_sum_of_bins;

        string format = "%-" + to_string(max_left_boundary_count) +
                        "d -> %-" + to_string(max_right_boundary_count) +
                        "d :     %-" + to_string(max_value_count) + "d | %-" + to_string(max_stars) + "s |\n";

        string title_format = "%-" + to_string(max_left_boundary_count + max_right_boundary_count +4) +
                        "s | %-" + to_string(max_value_count + 4) +
                        "s : %-" + to_string(max_stars) + "s\n";

        string result_string = "";

        char buffer[150];
        memset(buffer, 0, sizeof(buffer));

        sprintf(buffer, title_format.c_str(), "usec", "count", "distribution");
        result_string += buffer;
        memset(buffer, 0, sizeof(buffer));

        for (i = 0; i < number_of_bins; i++)
        {
            int n_of_stars = bins[i] * total_stars / total_sum_of_bins;
            if (i == 0)
            {
                sprintf(buffer, format.c_str(), 0, boundary_values[i] - 1, bins[i], generate_stars(n_of_stars).c_str());
                result_string += buffer;
                memset(buffer, 0, sizeof(buffer));
            }
            else
            {
                sprintf(buffer, format.c_str(), boundary_values[i] / 2, boundary_values[i] - 1, bins[i], generate_stars(n_of_stars).c_str());
                result_string += buffer;
                memset(buffer, 0, sizeof(buffer));
            }
        }

        return result_string;
    }
    void PrintLog2Hist()
    {
        printf(this->Print().c_str());
        fflush(stdout);
    }
};

int main(void)
{
    Log2Hist hist(10);
    hist.Add(1);
    hist.Add(2);
    hist.Add(3);
    hist.Add(4);
    hist.Add(5);
    hist.Add(6);
    hist.Add(7);
    hist.Add(8);
    hist.Add(45);
    for (int i = 0; i < 10; i++) hist.Add(45);
    hist.Add(77);
    hist.Add(90);
    hist.Add(223);
    hist.Add(655);
    hist.Add(440);
    hist.PrintLog2Hist();
}