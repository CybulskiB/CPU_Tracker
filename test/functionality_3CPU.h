#ifndef FUNCTIONALITY_3CPU_H

#define FUNCTIONALITY_3CPU_H

#define TESTS_NO 10
#define CPU_NO 3
#define PRINTER_CPU_NO 4

#define CONTEXTS_SEPARATOR "&"


#define SP_TESTS_CONTEXTS_SIZE 1928

#define MAX_SP_TEST_CONTEXTS_SIZE 250

#define SP_TESTS_CONTEXTS  \
"cpu  88177 1804 18125 307230 18762 0 1157 0 0 0\n\
cpu0 29883 525 5980 103204 7609 0 330 0 0 0\n\
cpu1 29383 382 5782 103997 7310 0 323 0 0 0\n\
cpu2 28910 896 6363 100029 3841 0 503 0 0 0\n\
&\
cpu  128396 1804 23894 420709 19005 0 1608 0 0 0\n\
cpu0 43290 525 7842 141539 7714 0 526 0 0 0\n\
cpu1 42942 382 7570 142231 7392 0 494 0 0 0\n\
cpu2 42164 896 8481 136938 3898 0 587 0 0 0\n\
&\
cpu  111787 821 30062 1959106 12050 0 7133 0 0 0\n\
cpu0 38015 223 9709 659092 4191 0 557 0 0 0\n\
cpu1 38468 284 10274 642213 3855 0 5616 0 0 0\n\
cpu2 35304 313 10078 100029 3841 0 503 0 0 0\n\
&\
cpu  799817 1829 167272 3182774 24972 0 10818 0 0 0\n\
cpu0 269101 982 53432 1076821 9508 0 2794 0 0 0\n\
cpu1 269248 252 53480 1073556 9346 0 3422 0 0 0\n\
cpu2 261468 593 60360 1032396 6116 0 4601 0 0 0\n\
&\
cpu  804299 1836 167982 3190534 24997 0 10865 0 0 0\n\
cpu0 270570 988 53658 1079494 9514 0 2810 0 0 0\n\
cpu1 270732 253 53703 1076188 9350 0 3437 0 0 0\n\
cpu2 262996 594 60620 1034851 6132 0 4617 0 0 0\n\
&\
cpu  810501 1836 168993 3200751 25016 0 10931 0 0 0\n\
cpu0 272690 988 53952 1082951 9516 0 2835 0 0 0\n\
cpu1 272804 253 54040 1079632 9359 0 3461 0 0 0\n\
cpu2 265006 594 61001 1038167 6140 0 4634 0 0 0\n\
&\
cpu  817135 1836 170038 3214651 25066 0 10999 0 0 0\n\
cpu0 274917 988 54295 1087638 9534 0 2858 0 0 0\n\
cpu1 275037 253 54372 1084303 9376 0 3487 0 0 0\n\
cpu2 267180 594 61370 1042708 6156 0 4653 0 0 0\n\
&\
cpu  824111 1836 171272 3227108 25095 0 11078 0 0 0\n\
cpu0 277235 988 54686 1091882 9542 0 2883 0 0 0\n\
cpu1 277334 253 54771 1088515 9388 0 3518 0 0 0\n\
cpu2 269542 594 61815 1046710 6164 0 4676 0 0 0\n\
&\
cpu  827925 1836 171880 3232047 25104 0 11108 0 0 0\n\
cpu0 278526 988 54875 1093553 9546 0 2894 0 0 0\n\
cpu1 278586 253 54962 1090216 9390 0 3528 0 0 0\n\
cpu2 270812 594 62043 1048277 6168 0 4685 0 0 0\n\
&\
cpu  904526 1852 185921 3607241 25655 0 12116 0 0 0\n\
cpu0 304116 1001 59339 1220411 9750 0 3260 0 0 0\n\
cpu1 304569 254 59309 1216629 9548 0 3838 0 0 0\n\
cpu2 295841 597 67272 1170200 6356 0 5018 0 0 0\n\
"


#define EXPECTED_RESULTS \
{ \
{33.89,33.32,32.79},\
{33.72,33.44,32.84},\
{34.01,34.41,31.58},\
{33.65,33.66,32.69},\
{33.64,33.66,32.70},\
{33.64,33.66,32.70},\
{33.64,33.66,32.70},\
{33.64,33.65,32.71},\
{33.64,33.65,32.71},\
{33.62,33.67,32.71}\
}

#endif 