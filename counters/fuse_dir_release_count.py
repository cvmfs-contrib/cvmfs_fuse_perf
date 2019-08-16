#!/usr/bin/python

from __future__ import print_function
from bcc import BPF
from time import sleep
from sys import argv
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
b.attach_kprobe(event_re="^fuse_dir_release$", fn_name="do_count")

# header
print("Tracing... Ctrl-C to end.")

# output
try:
    sleep(interval)
except KeyboardInterrupt:
    pass
    #exit()

print("\n%-16s %-26s %8s" % ("ADDR", "FUNC", "COUNT"))
counts = b.get_table("counts")
for k, v in sorted(counts.items(), key=lambda counts: counts[1].value):
    print("%-16x %-26s %8d" % (k.ip, b.ksym(k.ip), v.value))
