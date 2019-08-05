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
#include <cmath>
#include <sstream>
#include <cassert>
#include "atomic.h"
using namespace std; // NOLINT

#define BUFFSIZE 150

class Log2Hist
{
 private:
    uint number_of_bins;
    atomic_int32 *bins;
    uint *boundary_values;
    uint count_digits(ulong n)
    {
        return (uint)floor(log10(n) + 1);
    }
    string generate_stars(uint n)
    {
        uint i = 0;
        string stars = "";
        for (i = 0; i < n; i++)
        {
            stars += "*";
        }
        return stars;
    }
    string to_string(uint n)
    {
        stringstream s;
        s << n;
        return s.str();
    }
 public:
    explicit Log2Hist(uint n)
    {
        assert(n != 0);
        this->number_of_bins = n;
        this->bins = new atomic_int32[n + 1]; // +1 for overflow bin.
        this->boundary_values = new uint[n + 1]; // +1 to avoid giant if statement
        memset(this->bins, 0, sizeof(atomic_int32) * (n + 1));
        memset(this->boundary_values, 0, sizeof(uint) * (n + 1));

        uint i;
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
        uint i;
        uint flag = 1; 

        for (i = 1; i <= this->number_of_bins; i++)
        {
            if (value < this->boundary_values[i])
            {
                atomic_inc32(&(this->bins[i]));
                flag = 0;
                return;
            }
        }
        if (flag)
        {
             atomic_inc32(&(this->bins[0])); // add to overflow bin.
        }
    }
    atomic_int32 *GetBins()
    {
        return this->bins;
    }
    std::string Print()
    {
        uint i = 0;

        uint max_left_boundary_count = 1;
        uint max_right_boundary_count = 1;
        uint max_value_count = 1;
        uint max_stars = 0;
        uint max_bins = 0;
        uint total_stars = 38;
        uint total_sum_of_bins = 0;

        for (i = 1; i <= number_of_bins; i++)
        {
            max_left_boundary_count = max(max_left_boundary_count,
                                        count_digits(boundary_values[i] / 2));
            max_right_boundary_count = max(max_right_boundary_count,
                                        count_digits(boundary_values[i] - 1));
            max_value_count = max(max_value_count, count_digits(bins[i]));
            max_bins = max(max_bins, (uint)atomic_read32(&bins[i]));
            total_sum_of_bins += (uint)atomic_read32(&bins[i]);
        }

        max_bins = max(max_bins, (uint)atomic_read32(&bins[0]));
        total_sum_of_bins += (uint)atomic_read32(&bins[0]);


        max_stars = max_bins * total_stars / total_sum_of_bins;

        string format = " %" + to_string(max_left_boundary_count < 2 ? 2 : max_left_boundary_count) +
                        "d -> %" + to_string(max_right_boundary_count) +
                        "d :     %" + to_string(max_value_count) + "d | %" +
                        to_string(max_stars) + "s |\n";

        string title_format = " %" +
                        to_string((max_left_boundary_count < 2 ? 2 : max_left_boundary_count) +
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
            uint n_of_stars = (uint)atomic_read32(&bins[i]) * total_stars / total_sum_of_bins;
            snprintf(buffer,
                    BUFFSIZE,
                    format.c_str(),
                    boundary_values[i - 1],
                    boundary_values[i] - 1,
                    (uint)atomic_read32(&bins[i]),
                    generate_stars(n_of_stars).c_str());
            result_string += buffer;
            memset(buffer, 0, sizeof(buffer));
        }

        uint n_of_stars = (uint)atomic_read32(&bins[0]) * total_stars / total_sum_of_bins;
        snprintf(buffer,
                BUFFSIZE,
                overflow_format.c_str(),
                "overflow",
                (uint)atomic_read32(&bins[0]),
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
