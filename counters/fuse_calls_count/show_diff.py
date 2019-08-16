import csv
import sys
import math


def usage():
    print("USAGE: %s [file1] [file2]" % sys.argv[0])
    exit()


def count_digits(n):
    return len(str(n))


class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


def main():
    if len(sys.argv) != 3:
        usage()

    file1 = sys.argv[1]
    file2 = sys.argv[2]

    result_mapping = {}

    with open(file1, "r") as csv_file1:
        reader = csv.reader(csv_file1)
        next(reader)  # skip headers

        for row in reader:
            result_mapping[row[1]] = [row[2]]

    with open(file2, "r") as csv_file2:
        reader = csv.reader(csv_file2)
        next(reader)  # skip headers

        for row in reader:
            # If the key exists in the result_mapping,
            # add the metric to the value array.
            # Otherwise, add the key and metric as a
            # second element in value array.
            if row[1] in result_mapping:
                result_mapping[row[1]].append(row[2])
            else:
                result_mapping[row[1]] = [0, row[2]]

        # If there are keys with the only element in
        # value array, then add 0 to the value array.
        for k in result_mapping.keys():
            if len(result_mapping[k]) == 1:
                result_mapping[k].append(0)

    max_metric_name_len = 0
    max_file1_value_len = 0
    max_file2_value_len = 0

    for k, v in result_mapping.items():
        max_metric_name_len = max(max_metric_name_len, len(k))
        max_file1_value_len = max(max_file1_value_len, count_digits(v[0]))
        max_file2_value_len = max(max_file2_value_len, count_digits(v[1]))

    format_str = "{} | {} | {}"

    for k, v in result_mapping.items():
        func_call = k.rjust(max_metric_name_len)
        file1_cnt = str(v[0]).rjust(max_file1_value_len)
        file2_cnt = str(v[1]).rjust(max_file2_value_len)

        diff = abs(int(v[0]) - int(v[1]))

        result_str = format_str.format(func_call, file1_cnt, file2_cnt)

        if diff <= 100:
            print(bcolors.OKGREEN + result_str + bcolors.ENDC)
        elif diff <= 500:
            print(bcolors.OKBLUE + result_str + bcolors.ENDC)
        else:
            print(bcolors.WARNING + result_str + bcolors.ENDC)


if __name__ == "__main__":
    main()
