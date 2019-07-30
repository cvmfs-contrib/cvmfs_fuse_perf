#!/usr/bin/python

from __future__ import print_function
from bcc import BPF
from time import sleep
from sys import argv
import csv
from datetime import datetime
def usage():
    print("USAGE: %s [time]" % argv[0])
    exit()

interval = 99999999
if len(argv) > 1:
    try:
        interval = int(argv[1])
        if interval == 0:
            raise
    except:  # also catches -h, --help
        usage()
# load BPF program
b = BPF(text="""
#include <uapi/linux/ptrace.h>

struct key_t {
    u64 ip;
};

BPF_HASH(counts, struct key_t, u64, 256);

int do_count(struct pt_regs *ctx) {
    struct key_t key = {};
    key.ip = PT_REGS_IP(ctx);
    counts.increment(key);
    return 0;
}
""")
b.attach_kprobe(event_re=".*fuse.*", fn_name="do_count")

# header
print("Tracing... Ctrl-C to end.")

# output
try:
    sleep(interval)
except KeyboardInterrupt:
    pass
    #exit()

csv_data = [["ADDR", "FUNC", "COUNT"]]

counts = b.get_table("counts")
for k, v in sorted(counts.items(), key=lambda counts: counts[1].value):
    csv_data.append([k.ip, b.ksym(k.ip), v.value])
    
current_datetime = datetime.now() 
datetime_str = current_datetime.strftime("%Y%m%d-%H%M%S")
csv_file_name = "fuse_calls_count_" +datetime_str

with open(csv_file_name, "w") as csv_file:
    writer = csv.writer(csv_file)
    writer.writerows(csv_data)

