#!/usr/bin/env python3

VERSION = "1.0.4"

##############################################################################
# DEFAULTS #
CSSE7030 = True
SCRIPT = "assign1"
TEST_DATA = None
TEST_DATA_RAW = 'from random import shuffle \nmeans = [\n    ([[1, 2, 3, 4, 5]], 3),\n    ([list(range(1,1001))],500.5),\n]\n\nfor i in range(len(means)):\n    shuffle(means[i][0][0])\n\nranges = [\n    ([[1, 2]], (1, 2)),\n    ([list(range(123))],(0,122)),\n]\n\nfor i in range(len(ranges)):\n    shuffle(ranges[i][0][0])\n\nmedians = [\n    ([[1, 2, 3]], 2),\n    ([[4]],4),\n    ([[2,3,4,5,6,7,8,9,10,11,12,13]],7.5),\n]\n\nfor i in range(len(medians)):\n    shuffle(medians[i][0][0])\n\nstddevs = [\n    ([list(range(100))],28.866070047722118),\n    ([list(range(50,100))],14.430869689661812),\n]\n\n\nloads = [\n    (["animal_heights.csv"],\n        [(\'Slow Loris\', [21.72, 29.3, 20.08, 29.98, 29.85, 26.22, 19.99, 27.78,\n            27.02, 22.44, 26.54, 26.46, 22.24, 27.72, 22.25, 28.52, 25.57,\n            22.44, 27.06, 24.24, 21.74, 26.72, 27.23, 21.48, 26.14, 21.09,\n            28.28, 29.05, 23.52, 24.74, 29.08, 21.57, 21.98, 26.66, 21.99,\n            20.91, 28.07, 26.06, 22.54, 25.9, 21.78, 25.96, 25.25, 28.08, \n            23.79, 29.99, 21.41, 21.67, 22.94, 29.68]), (\'Ocelot\', [57.51,\n            47.59, 55.89, 47.15, 46.71, 51.7, 46.68, 48.36, 52.15, 59.02, 45.1, \n            45.21, 51.13, 45.27, 57.24, 57.17, 49.47, 46.32, 42.47, 41.14,\n            46.86, 40.1, 58.67, 48.91, 48.21, 43.63, 43.11, 46.97, 52.07, 47.71,\n            53.27, 57.54, 56.19, 47.79, 45.32, 50.56, 51.23, 40.6, 48.03, 51.95,\n            54.54]), (\'Tiger\', [75.0, 82.43, 112.11, 89.93, 103.19, 80.6,\n            113.44, 105.44, 95.54, 121.98, 114.25, 109.84, 90.48, 105.84, 82.89,\n            113.64, 102.73, 104.57, 100.83, 75.59, 79.86, 91.11, 94.75, 109.89,\n            117.39, 74.71, 71.04, 92.97, 88.87, 92.95, 86.67, 101.46, 92.4, 85.2,\n            107.19, 117.81, 90.95, 82.02, 87.31, 106.53, 86.28, 106.62, 107.57,\n            89.38, 105.88, 74.45, 90.03, 107.96, 77.42, 98.9, 109.81, 102.51,\n            116.71, 82.92, 81.78, 74.42, 76.27, 73.84, 75.55, 102.29, 108.1,\n            98.84, 101.48, 77.75, 98.57, 70.31, 78.28, 80.18])]\n    ),\n]\n\nsummaries = [\n    ([loads[0][1]],\n        [(\'Slow Loris\', 50, 25.0544, 25.735, 19.99, 29.99, 3.0183440228045577),\n        (\'Ocelot\', 41, 49.42780487804878, 48.21, 40.1, 59.02, 5.117594000325868),\n        (\'Tiger\', 68, 94.19852941176471, 92.96000000000001, 70.31, 121.98, 13.908963177063187)]\n     ),\n]\n\nsets = [\n    (([[(\'A\', 1, 2, 3, 4, 5, 6), (\'B\', 6, 5, 4, 3, 2, 1)]], None), ("",\n"""Set Summaries\n\n               A              B              \nCount:         1              6              \nMean:          2              5              \nMedian:        3              4              \nMinimum:       4              3              \nMaximum:       5              2              \nStd Dev:       6              1              \n\n"""))\n]\n\n\ninteractions = [\n    ("animal_heights.csv\\nsets 0\\nq\\n",\n"""Welcome to the Statistic Summariser\n\nPlease enter the data source file: \nCommand: Set Summaries\n\n               Slow Loris     \nCount:         50             \nMean:          25.05          \nMedian:        25.73          \nMinimum:       19.99          \nMaximum:       29.99          \nStd Dev:       21.34          \n\n\nCommand: """),\n    ("animal_heights.csv\\nbad\\nq\\n",\n"""Welcome to the Statistic Summariser\n\nPlease enter the data source file: \nCommand: Unknown command: bad\n\nCommand: """),\n]\n\ninteractions7030 = [("animal_heights.csv\\ncomp 0 1\\nq\\n","""Welcome to the Statistic Summariser\n\nPlease enter the data source file: \nCommand: Comparison between Slow Loris and Ocelot\n\n               Mean           Std Dev        \nShared (S)     36.04          12.8           \nSlow Loris     S-10.98        3.02           \nOcelot         S+13.39        5.12           \n\nCommand: """)]\n\ninteractions = [(([], None),x) for x in interactions]\ninteractions7030 = [(([], None),x) for x in interactions7030]\n\ncomparisons = [\n    ([loads[0][1],0,1], ((\'Slow Loris\', \'Ocelot\'), (36.03582417582418, -10.98142417582418, 13.3919807022246), (12.8011140540201, 3.0183440228045577, 5.117594000325868))),\n]\n\ncomp_io = [([comparisons[0][1]],"""Comparison between Slow Loris and Ocelot\n\n               Mean           Std Dev        \nShared (S)     36.04          12.8           \nSlow Loris     S-10.98        3.02           \nOcelot         S+13.39        5.12           \n"""),\n]\n\ncomp_io = [((i[0],None),("",i[1]))   for i in comp_io]'
# END DEFAULTS #
##############################################################################

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("script",
    help="The script you want to run the tests against.",
    nargs="?",
    default=SCRIPT)
parser.add_argument("test_data",
    help="The file containing test data to use.",
    nargs="?",
    default=TEST_DATA)
parser.add_argument("-m", "--masters",
    help="Whether or not to utilize master's tests.",
    action='store_true',
    default=CSSE7030)
parser.add_argument('unittest_args', nargs='*')

args = parser.parse_args()

from unittest import main, TestCase

if args.test_data:
    data = __import__(args.test_data)
else:
    import imp
    data = imp.new_module("data")
    exec(TEST_DATA_RAW, data.__dict__)

assign1 = __import__(args.script)

from io import StringIO
import sys

class GetFnTest(TestCase):
    pass
            
def addGetTestCases(fnname,dataname, f=lambda x: x):
    for i, item in enumerate(eval("data.{}".format(dataname))):
        def fn(self):
            self.assertEqual(f(eval("assign1.{}".format(fnname))(*item[0])),f(item[1]))
        setattr(GetFnTest, "test_{}_{}".format(fnname,i),fn)

loads_sorting = lambda x: [(k,list(sorted(map(lambda x:float(round(x,10)),v)))) for k,v in x]

def fix_floats(data):
    return [(x,) +tuple(map(lambda x:float(round(x,10)),y)) for x,*y in data]

addGetTestCases("get_ranges","ranges")
addGetTestCases("get_mean","means")
addGetTestCases("get_median","medians")
addGetTestCases("get_std_dev","stddevs",lambda x:float(round(x,10)))
addGetTestCases("load_data","loads",loads_sorting)
addGetTestCases("data_summary","summaries", fix_floats)

class stdInOutTest(TestCase):
    maxDiff = None
    def setUp(self):
        self._in = sys.stdin
        self._out = sys.stdout
        sys.stdin = StringIO()
        sys.stdout = StringIO()
    
    def tearDown(self):
        sys.stdin = self._in
        sys.stdout = self._out

def addIOTestCases(fnname, dataname, fout = lambda x: x):
    d = eval("data.{}".format(dataname))
    for i, item in enumerate(d):
        def fn(self):
            sys.stdin.write(item[1][0])
            sys.stdin.seek(0)
            self.assertEqual(eval("assign1."+fnname)(*item[0][0]),item[0][1])
            sys.stdout.seek(0)
            self.assertEqual(fout(sys.stdout.read()), fout(item[1][1]))
        setattr(stdInOutTest, "test_{}_{}".format(fnname,i),fn)



            
addIOTestCases("display_set_summaries","sets",str.strip)
addIOTestCases("interact","interactions",str.strip)

if args.masters:
    addGetTestCases("data_comparison","comparisons")
    addIOTestCases("display_comparison","comp_io",str.strip)
    addIOTestCases("interact","interactions7030",str.strip)

if __name__=="__main__":
    print("Version {}".format(VERSION))
    sys.argv[1:] = args.unittest_args
    main(verbosity=2)
