#./compile.sh
#./build/main/program ./basic_tc/basic_data.igraph ./basic_tc/basic_query.igraph ./basic_tc/basic_cs.cs

echo "LCC_HPRD_N1"
./build/main/program ./data/lcc_hprd.igraph ./query/lcc_hprd_n1.igraph ./candidate_set/lcc_hprd_n1.cs 
echo ""

echo "LCC_HPRD_N5"
./build/main/program ./data/lcc_hprd.igraph ./query/lcc_hprd_n5.igraph ./candidate_set/lcc_hprd_n5.cs 
echo ""


