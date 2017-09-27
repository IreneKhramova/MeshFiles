#ifndef MESHREADERSALOMEUNV_H
#define MESHREADERSALOMEUNV_H

#include <Mesh.h>
#include <vector>
#include <set>
#include <string>
#include <cstdlib>
#include <cstdio>

class MeshReaderSalomeUnv
{
    private:
        struct Point
        {
            double x;
            double y;
            double z;
            Point(double x1 = 0, double y1 = 0, double z1 = 0) : x(x1), y(y1), z(z1){}
        };

        char* fileName;
        vector<Point> points;
        vector<vector<int> > edges, faces , cells;

        void read_block(vector<string> *, ifstream&);
        void parse_block(vector<string>);
        void parse_block_164(vector<string>);
        void parse_block_2420(vector<string>);
        void parse_block_2411(vector<string>);
        void parse_block_2412(vector<string>);

        bool face_is_exist(set<int>);
        bool edge_is_exist(int, int);
        void createFace(int*, vector<int>);

    public:
        MeshReaderSalomeUnv(char* fName): fileName(fName){}
        ~MeshReaderSalomeUnv() { delete [] fileName; }

        void read(Mesh*);

};

#endif // MESHREADERSALOMEUNV_H
