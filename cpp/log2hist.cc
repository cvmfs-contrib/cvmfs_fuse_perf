/**
 * This file is part of the CernVM File System.
 *
 * Implements a log2 histogram data structure
 * which is used for internal computation and plotting.
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <cmath>
#include <mutex>
#include <cassert>
using namespace std; // NOLINT

#define BUFFSIZE 150

class Log2Hist
{
 private:
    mutex _mutex;
    uint32_t number_of_bins;
    uint32_t *bins;
    uint32_t *boundary_values;
    uint32_t count_digits(uint64_t n)
    {
        return (uint32_t)floor(log10(n) + 1);
    }
    string generate_stars(uint32_t n)
    {
        uint32_t i = 0;
        string stars = "";
        for (i = 0; i < n; i++)
        {
            stars += "*";
        }
        return stars;
    }

 public:
    explicit Log2Hist(uint32_t n)
    {
        assert(n != 0);
        this->number_of_bins = n;
        this->bins = new uint32_t[n + 1]; // +1 for overflow bin.
        this->boundary_values = new uint32_t[n + 1]; // +1 to avoid giant if statement
        memset(this->bins, 0, sizeof(uint32_t) * (n + 1));
        memset(this->boundary_values, 0, sizeof(uint32_t) * (n + 1));

        uint32_t i;
        for (i = 1; i <= n; i++)
        {
            this->boundary_values[i] = (1 << ((i - 1) + 1));
        }
    }
    ~Log2Hist()
    {
        delete[] bins;
        delete[] boundary_values;
    }
    void Add(float value)
    {
        uint32_t i;
        uint32_t flag = 1; 

        for (i = 1; i <= this->number_of_bins; i++)
        {
            if (value < this->boundary_values[i])
            {
                _mutex.lock();
                this->bins[i]++;
                flag = 0;
                _mutex.unlock();
                return;
            }
        }
        if (flag)
        {
            this->bins[0]++; // add to overflow bin.
        }
    }
    uint32_t *GetBins()
    {
        return this->bins;
    }
    std::string Print()
    {
        uint32_t i = 0;

        uint32_t max_left_boundary_count = 1;
        uint32_t max_right_boundary_count = 1;
        uint32_t max_value_count = 1;
        uint32_t max_stars = 0;
        uint32_t max_bins = 0;
        uint32_t total_stars = 38;
        uint32_t total_sum_of_bins = 0;

        for (i = 1; i <= number_of_bins; i++)
        {
            max_left_boundary_count = max(max_left_boundary_count,
                                        count_digits(boundary_values[i] / 2));
            max_right_boundary_count = max(max_right_boundary_count,
                                        count_digits(boundary_values[i] - 1));
            max_value_count = max(max_value_count, count_digits(bins[i]));
            max_bins = max(max_bins, bins[i]);
            total_sum_of_bins += bins[i];
        }

        max_bins = max(max_bins, bins[0]);
        total_sum_of_bins += bins[0];


        max_stars = max_bins * total_stars / total_sum_of_bins;

        string format = " %" + to_string(max_left_boundary_count) +
                        "d -> %" + to_string(max_right_boundary_count) +
                        "d :     %" + to_string(max_value_count) + "d | %" +
                        to_string(max_stars) + "s |\n";

        string title_format = " %" +
                        to_string(max_left_boundary_count +
                                    max_right_boundary_count +
                                    4) +
                        "s | %" + to_string(max_value_count + 4) +
                        "s | %" + to_string(max_stars) + "s |\n";

        string overflow_format = "%" +
                        to_string(max_left_boundary_count +
                                    max_right_boundary_count +
                                    5) +
                        "s : %" + to_string(max_value_count + 4) +
                        "d | %" + to_string(max_stars) + "s |\n";


        string result_string = "";

        char buffer[BUFFSIZE];
        memset(buffer, 0, sizeof(buffer));

        snprintf(buffer,
            BUFFSIZE,
            title_format.c_str(),
            "usec",
            "count",
            "distribution");
        result_string += buffer;
        memset(buffer, 0, sizeof(buffer));

        for (i = 1; i <= number_of_bins; i++)
        {
            uint32_t n_of_stars = bins[i] * total_stars / total_sum_of_bins;
            snprintf(buffer,
                    BUFFSIZE,
                    format.c_str(),
                    boundary_values[i - 1],
                    boundary_values[i] - 1,
                    bins[i],
                    generate_stars(n_of_stars).c_str());
            result_string += buffer;
            memset(buffer, 0, sizeof(buffer));
        }

        uint32_t n_of_stars = bins[0] * total_stars / total_sum_of_bins;
        snprintf(buffer,
                BUFFSIZE,
                overflow_format.c_str(),
                "overflow",
                bins[0],
                generate_stars(n_of_stars).c_str());
        result_string += buffer;
        memset(buffer, 0, sizeof(buffer));

        return result_string;
    }
    void PrintLog2Hist()
    {
        printf("%s", this->Print().c_str());
        fflush(stdout);
    }
};


int main(void)	
{	
    Log2Hist hist(20);	
    hist.Add(1);	
    hist.Add(2);	
    hist.Add(3);	
    hist.Add(4);	
    hist.Add(5);	
    hist.Add(8);
    for (int i = 0; i < 100; i++) {
        hist.Add(1048576);	
    }
    hist.PrintLog2Hist();	
}	
