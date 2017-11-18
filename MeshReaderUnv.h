#ifndef MESHREADERUNV_H
#define MESHREADERUNV_H

#include "Mesh.cpp"
#include <vector>
#include <set>
#include <cstdlib>
#include <fstream>
#include <cstring>

using namespace std;

class MeshReaderUnv
{
    private:

        string fileName;
        vector<Point> points;
        vector<vector<int> > edges, faces, cells;
        vector<int> type_faces, type_cells;

        void read_block(vector<string> *, ifstream&);
        void parse_block(vector<string>&);
        void parse_block_164(vector<string>);
        void parse_block_2420(vector<string>);
        void parse_block_2411(vector<string>&);
        void parse_block_2412(vector<string>&);

        bool face_is_exist(set<int>);
        bool edge_is_exist(int, int);

    public:
        MeshReaderUnv(string fName) { fileName = fName; }
        ~MeshReaderUnv() { }

        void read(Mesh**);

};

#endif // MESHREADERUNV_H
